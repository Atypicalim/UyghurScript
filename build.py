
import os
import sys
import json
import subprocess
import shutil
import yaml
sys.path.append('../my-build-tools/')

from builder import builder
tools = builder.tools

###############################################################################

PROJECT_NAME ="UyghurScript"
PROJECT_REPO ="https://github.com/kompasim/UyghurScript"
VERSION_CODE = 0.5
EXTENSION_VERSION = "1.0.5"
EXAMPLE_LANG = 'ug'
SUPPORT_LANG = set()
SCRIPT_PATH = "./examples/sinaq.ug"
SCRIPT_DIR, SCRIPT_FILE, SCRIPT_EXT, SCRIPT_NAME = tools.tools.parse_path(SCRIPT_PATH)

###############################################################################

DIR_BUILD = "./build/"
tools.files.mk_folder(DIR_BUILD)

DIR_SNIPPETS = tools.tools.append_path(DIR_BUILD, "snippets")
DIR_FEATURES = tools.tools.append_path(DIR_BUILD, "features")

DIR_SNIPPET_TRANS = tools.tools.append_path(DIR_SNIPPETS, "trans")
DIR_SNIPPET_COLOR = tools.tools.append_path(DIR_SNIPPETS, "color")
DIR_FEATURE_TRANS = tools.tools.append_path(DIR_FEATURES, "trans")
DIR_FEATURE_COLOR = tools.tools.append_path(DIR_FEATURES, "color")
tools.files.delete(DIR_SNIPPET_TRANS)
tools.files.delete(DIR_SNIPPET_COLOR)
tools.files.delete(DIR_FEATURE_TRANS)
tools.files.delete(DIR_FEATURE_COLOR)
tools.files.mk_folder(DIR_SNIPPET_TRANS)
tools.files.mk_folder(DIR_SNIPPET_COLOR)
tools.files.mk_folder(DIR_FEATURE_TRANS)
tools.files.mk_folder(DIR_FEATURE_COLOR)

DST_ALIAS = DIR_BUILD + "uyghur"
DST_SCRIPT = DST_ALIAS + "." + SCRIPT_EXT

###############################################################################

# c conf
tplYamlDeclare = """char* {0} = \"{0}\";"""
tplYamlDefine = """#define {0} \"{0}\""""
#
tplYamlLineReference = """{{&{0}, "{1}"}},"""
tplYamlLineNormal = """{{"{0}", "{1}"}},"""
#
tmplYamlNameSize = "YAML_SIZE_{0}_{1}"
tmplYamlNameConf = "YAML_CONF_{0}_{1}"
# 
tplYamlFilter = """    if (pct_cstr_ends_with(tp, "{0}") == 0) {{return {1};}}"""
#
tplYamlConfig = """#define {0} {2}
static const PAIR_{3} {1}[{0}] = {{
{4}
}};
"""

############################################################################### utils

def readYaml(fromPath):
    _configs = tools.files.read(fromPath, 'utf-8')
    configs = yaml.safe_load(_configs)
    return configs

def readLanguages(fileName):
    fromName = fileName.lower()
    fromPath = f"./uyghur/multilangs/{fromName}.yml"
    configs = readYaml(fromPath)
    langsArr = []
    namesArr = []
    mapLang2Name = {}
    mapName2Lang = {}
    for alias, pairs in configs.items():
        # names
        namesArr.append(alias)
        # 
        for lang, value in pairs.items():
            # langs
            if lang not in langsArr:
                langsArr.append(lang)
            #
            if lang not in mapLang2Name:
                mapLang2Name[lang] = {}
            mapLang2Name[lang][alias] = value
            #
            if alias not in mapName2Lang:
                mapName2Lang[alias] = {}
            mapName2Lang[alias][lang] = value
            #
    return langsArr, namesArr, mapLang2Name, mapName2Lang

############################################################################### yamls

langsArrLetters, namesArrLetters, mapLang2NameLetters, mapName2LangLetters = readLanguages("LETTERS")
langsArrLanguages, namesArrLanguages, mapLang2NameLanguages, mapName2LangLanguages = readLanguages("LANGUAGES")
langsArrAliases, namesArrAliases, mapLang2NameAliases, mapName2LangAliases = readLanguages("ALIASES")
langsArrConfigs, namesArrConfigs, mapLang2NameConfigs, mapName2LangConfigs = readLanguages("CONFIGS")

grammars = readYaml(f"./others/grammars.yml")
snippets = readYaml(f"./others/snippets.yml")
features = readYaml(f"./others/features.yml")

############################################################################### formatting

langArray = []
for lang in langsArrConfigs:
    SUPPORT_LANG.add(lang)

langNames = mapName2LangConfigs['LANG_LANGUAGE_FULLNAME']
langTrans = mapName2LangConfigs['LANG_LANGUAGE_TRANSLATION']

############################################################################### formatting

def formatVariables(template, names):
    variables = []
    for name in names:
        variable = template.format(name)
        variables.append(variable)
    return "\n".join(variables)

def formatBodies(template, name, mapLang2Name):
    bodies = []
    for lang, infos in mapLang2Name.items():
        lines = []
        for alias, value in infos.items():
            line = template.format(alias, value)
            lines.append(line)
        _length = len(lines)
        _lines = "\n    ".join(lines)
        #
        txtYamlNameSize = tmplYamlNameSize.format(name, lang) 
        txtYamlNameConf = tmplYamlNameConf.format(name, lang) 
        #
        body = tplYamlConfig.format(txtYamlNameSize, txtYamlNameConf, _length, name, "    " + _lines)
        bodies.append(body)
    _bodies = "\n".join(bodies)
    return _bodies

def formatFooters(template, name, mapName2Lang):
    footers = []
    for alias, infos in mapName2Lang.items():
        lines = []
        for lang, value in infos.items():
            line = template.format(lang, value)
            lines.append(line)
        _length = len(lines)
        _lines = "\n    ".join(lines)
        #
        txtYamlNameSize = tmplYamlNameSize.format(name, alias) 
        txtYamlNameConf = tmplYamlNameConf.format(name, alias) 
        #
        body = tplYamlConfig.format(txtYamlNameSize, txtYamlNameConf, _length, name, "    " + _lines)
        footers.append(body)
    _footers = "\n".join(footers)
    return _footers

def formatFilters(name, mapping):
    filterSizes = []
    filterConfs = []
    for key, infos in mapping.items():
        #
        txtYamlNameSize = tmplYamlNameSize.format(name, key) 
        txtYamlNameConf = tmplYamlNameConf.format(name, key) 
        #
        filterSize = tplYamlFilter.format(key, txtYamlNameSize)
        filterConf = tplYamlFilter.format(key, txtYamlNameConf)
        filterSizes.append(filterSize)
        filterConfs.append(filterConf)
    _filterSizes = "\n".join(filterSizes)
    _filterConfs = "\n".join(filterConfs)
    return [_filterSizes, _filterConfs]

############################################################################### configs

def _configsMacro(code, command, argument = None):
    if command == "VERSION_CODE":
        return code.format(VERSION_CODE)
    elif command == "VERSION_NAME":
        return code.format(VERSION_CODE)
    elif command == "SCRIPT_PATH":
        return code.format(DST_SCRIPT)
    elif command == "SCRIPT_NAME":
        return code.format("*." + SCRIPT_EXT)
    elif command == "LANGUAGE_COUNT":
        return code.format(len(SUPPORT_LANG))
    elif command == "LANGUAGE_ARRAY":
        return code.format('", "'.join(SUPPORT_LANG))
    else:
        return code

############################################################################### multilang

def _Yaml2Template(name, varTpl, lineTpl):
    _name = name.lower()
    _, namesArr, mapLang2Name, mapName2Lang = readLanguages(name)
    #
    variables = formatVariables(varTpl, namesArr)
    bodies = formatBodies(lineTpl, name, mapLang2Name)
    footers = formatFooters(tplYamlLineNormal, name, mapName2Lang)
    [filterSizeByLang, filterConfByLang] = formatFilters(name, mapLang2Name)
    [filterSizeByName, filterConfByName] = formatFilters(name, mapName2Lang)
    #
    def _onMacro(code, command, argument = None):
        if command == "VARIABLES":
            return variables
        elif command == "BODIES":
            return bodies
        elif command == "FOOTERS":
            return footers
        elif command == "FILTER_SIZE_BY_LANG":
            return filterSizeByLang
        elif command == "FILTER_CONF_BY_LANG":
            return filterConfByLang
        elif command == "FILTER_SIZE_BY_NAME":
            return filterSizeByName
        elif command == "FILTER_CONF_BY_NAME":
            return filterConfByName
        else:
            return _configsMacro(code, command, argument)
    #
    bldr = builder.code()
    bldr.setName(name)
    bldr.setInput(f"./uyghur/templates/{_name}.tpl.h")
    bldr.setComment("//")
    bldr.setOutput(f"{DIR_BUILD}{_name}.h")
    bldr.onMacro(_onMacro)
    bldr.start()
    pass

_Yaml2Template("LANGUAGES", tplYamlDeclare, tplYamlLineReference)
_Yaml2Template("LETTERS", tplYamlDefine, tplYamlLineNormal)
_Yaml2Template("ALIASES", tplYamlDefine, tplYamlLineNormal)
_Yaml2Template("CONFIGS", tplYamlDefine, tplYamlLineNormal)

############################################################################### translate

letterArray = []
for name, infos in mapName2LangLetters.items():
    lines = ""
    for lang, value in infos.items():
        lines = lines + "{}:\"{}\",".format(lang, value)
    letterArray.append(name + ":{"+ lines + "},")
letterText = "\n".join(letterArray)
#
langArray = []
for lang in langsArrConfigs:
    langArray.append("'" + lang + "'")
langText = ", ".join(langArray)

# translate
def _onMacro(code, command, argument = None):
    if command == "PROJECT_REPO":
        return code.format(PROJECT_REPO)
    elif command == "LETTERS_MAP":
        return letterText
    elif command == "LANGS_ARR":
        return langText
bldr = builder.code()
bldr.setName("TRANSLATE")
bldr.setInput("./others/translate.tpl.js")
bldr.setComment("//")
bldr.setOutput(DIR_BUILD + "translate.js")
bldr.onMacro(_onMacro)
bldr.onLine(lambda line: line)
bldr.start()

############################################################################### colorize

colorMap = grammars["GRAMMAR_COLORS"]

def getColor(name):
    for typ in grammars:
        names = grammars[typ]
        if name in names:
            return colorMap[typ]
    return "black"
#
colorArray = []
for name, infos in mapName2LangLetters.items():
    lines = ""
    for lang, value in infos.items():
        color = getColor(name)
        line = "\"{}\":\"{}\",".format(value, color)
        colorArray.append(line)
colorText = "\n".join(colorArray)
#
langArray = []
for lang in langsArrConfigs:
    langArray.append("'" + lang + "'")
langText = ", ".join(langArray)

# colorize
def _onMacro(code, command, argument = None):
    if command == "PROJECT_REPO":
        return code.format(PROJECT_REPO)
    elif command == "COLORS_MAP":
        return colorText
    elif command == "LANGS_ARR":
        return langText
bldr = builder.code()
bldr.setName("COLORIZE")
bldr.setInput("./others/colorize.tpl.js")
bldr.setComment("//", False)
bldr.setOutput(DIR_BUILD + "colorize.js")
bldr.onMacro(_onMacro)
bldr.onLine(lambda line: line)
bldr.start()

############################################################################### converter

bldr = builder.html()
bldr.setName("CONVERTER")
bldr.setInput("./others/converter.html")
bldr.containScript()
bldr.containStyle()
bldr.containImage()
bldr.setOutput(DIR_BUILD + "converter.html")
bldr.start()

############################################################################### syntax

langNames = mapName2LangConfigs['LANG_LANGUAGE_FULLNAME']
langTrans = mapName2LangConfigs['LANG_LANGUAGE_TRANSLATION']

langArray = []
gramArray = []
keywordsArray = []
snippetsArray = []
def translateNames(lang, names):
    translates = []
    for name in names:
        langs = mapName2LangLetters[name]
        translate = langs[lang]
        translates.append(translate)
    return "|".join(translates)

for lang in langsArrConfigs:
    name = langNames[lang]
    tran = langTrans[lang]
    def _onMacro(code, command, argument = None):
        if command == "FORMAT_ARGS":
            return code.format(lang=lang, name=name, tran=tran)
        else:
            translates = translateNames(lang, grammars[command])
            return code.format(translates)
    bldr = builder.code()
    bldr.setName("EXT·SYNTAX")
    bldr.setInput("./extension/syntaxes/tmLanguage.tpl.json")
    bldr.setComment("//", False)
    bldr.setOutput(f"./extension/syntaxes/{lang}.tmLanguage.json")
    bldr.onMacro(_onMacro)
    bldr.onLine(lambda line: line)
    bldr.start()

############################################################################### package

tplExtKeyword = '''        "{name}", "{tran}"'''

tplExtLanguage = '''            {{
                "id": "{lang}",
                "aliases": [
                    "{tran}",
                    "{name}",
                    "{lang}"
                ],
                "extensions": [
                    ".{lang}"
                ],
                "configuration": "./language-configuration.json"
            }}'''

tplExtGrammar = '''             {{
				"language": "{lang}",
				"scopeName": "text.language.{name}",
				"path": "./syntaxes/{lang}.tmLanguage.json"
			}}'''

tplExtSnippet = '''             {{
                "language": "{lang}",
                "path": "./snippets/{lang}.language-snippets.json"
			}}
'''

for lang in langsArrConfigs:
    name = langNames[lang]
    tran = langTrans[lang]
    _lang = tplExtLanguage.format(lang=lang, name=name, tran=tran)
    langArray.append(_lang)
    _lang = tplExtGrammar.format(lang=lang, name=name, tran=tran)
    gramArray.append(_lang)
    _keyword = tplExtKeyword.format(lang=lang, name=name, tran=tran)
    keywordsArray.append(_keyword)
    _snippets = tplExtSnippet.format(lang=lang, name=name, tran=tran)
    snippetsArray.append(_snippets)

langText = ",\n".join(langArray)
gramText = ",\n".join(gramArray)
keywordsText = ",\n".join(keywordsArray)
snippetsText = ",\n".join(snippetsArray)

def _onMacro():
    def onMacro(code, command, argument = None):
        if command == "EXT_VERSION":
            return code.format(EXTENSION_VERSION)
        elif command == "EXT_KEYWORDS":
            return keywordsText
        elif command == "EXT_LANGUAGES":
            return langText
        elif command == "EXT_GRAMMARS":
            return gramText
        elif command == "EXT_SNIPPETS":
            return snippetsText
    return onMacro
bldr = builder.code()
bldr.setName("EXT·PACKAE")
bldr.setInput("./extension/package.tpl.json")
bldr.setComment("//", False)
bldr.setOutput("./extension/package.json")
bldr.onMacro(_onMacro())
bldr.onLine(lambda line: line)
bldr.start()

############################################################################### convert

print("CONVERT_SNIPPETS:")
for name in snippets:
    fromPath = tools.tools.append_path("./examples/snippets/", name) + "." + EXAMPLE_LANG
    result = subprocess.run([
        "node", "./others/convert.js", fromPath, DIR_SNIPPET_TRANS, DIR_SNIPPET_COLOR
    ], capture_output=True, text=True)
    print("translate:", name, "OK" if result.returncode == 0 else result.stderr)
print("CONVERTED!\n")

print("CONVERT_FEATURES:")
for name in features:
    fromPath = tools.tools.append_path("./examples/features/", name) + "." + EXAMPLE_LANG
    result = subprocess.run([
        "node", "./others/convert.js", fromPath, DIR_FEATURE_TRANS, DIR_FEATURE_COLOR
    ], capture_output=True, text=True)
    print("translate:", name, "OK" if result.returncode == 0 else result.stderr)
print("CONVERTED!\n")

############################################################################### snippets

print("SNIPPETS:")
extensionsDir = "./extension/"
snippetsDir = tools.tools.append_path(extensionsDir, "snippets")
tools.files.delete(snippetsDir)
tools.files.mk_folder(snippetsDir)
snippetLang = "ug"
snippetsMap = {}
for name in snippets:
    print("snippet:", name)
    snippet = snippets[name]
    # initialize
    _prefix = snippet['prefix']
    _prefixes = None
    if isinstance(_prefix, str) and not _prefixes and _prefix in mapName2LangLetters:
        _prefixes = mapName2LangLetters[_prefix]
    if isinstance(_prefix, str) and not _prefixes and _prefix in mapName2LangAliases:
        _prefixes = mapName2LangAliases[_prefix]
    if _prefixes:
        snippet['prefix'] = _prefixes
    # generate
    for lang in langsArrConfigs:
        transPath = tools.tools.append_path(DIR_SNIPPET_TRANS, name) + "." + lang
        _file = open(transPath, "r", encoding="utf-8")
        _content = _file.read().replace("\r\n", "\r")
        body = _content.split("\n")
        # 
        prefix = snippet['prefix'][lang]
        message = snippet['message'][lang]
        # 
        print("generate:", lang)
        if lang not in snippetsMap:
            snippetsMap[lang] = {}
        snippetsMap[lang][message] = {
            "prefix": prefix,
            "describe": "",
            "body": body,
        }
# 
for lang in langsArrConfigs:
    confPath = tools.tools.append_path(snippetsDir, f"{lang}.language-snippets.json")
    with open(confPath, "w", encoding="utf-8") as f:
        json.dump(snippetsMap[lang], f, ensure_ascii=False, indent=4)

############################################################################### readme

tplMdLangauge = '''
* {alpha}. {tran}

![](./resources/languages/hello.{lang}.png)
'''

tplMdFeature = '''* {name}

```powershell
{code}
```
'''

print("README:")

mdLangaugesArray = []
i = 0
for lang in langTrans:
    tran = langTrans[lang]
    i = i + 1
    alpha = chr(ord('a') + i - 1)
    text = tplMdLangauge.format(alpha=alpha, lang=lang, tran=tran)
    mdLangaugesArray.append(text)
mdLanguagesText = "\n".join(mdLangaugesArray)

mdfeaturesArray = []
for name in features:
    if name == "hello":
        continue
    path = tools.tools.append_path("./examples/features/", name) + ".ug"
    code = tools.files.read(path, 'utf-8').strip()
    text = tplMdFeature.format(name=name, code=code)
    mdfeaturesArray.append(text)
mdFeaturesText = "\n".join(mdfeaturesArray)

def _onMacro(code, command, argument = None):
    if command == "MD_LANGUAGES":
        return mdLanguagesText
    elif command == "MD_REATURES":
        return mdFeaturesText
bldr = builder.code()
bldr.setName("README")
bldr.setInput("./readme.tpl.md")
bldr.setComment("//", False)
bldr.setOutput("./readme.md")
bldr.onMacro(_onMacro)
bldr.onLine(lambda line: line)
bldr.start()

############################################################################### script

assert tools.files.is_file(SCRIPT_PATH), 'script file not found' + SCRIPT_PATH
tools.files.delete(DST_SCRIPT)

def _onMacro(code, command, argument = None):
    if command == "PROJECT_REPO":
        return code.format(PROJECT_REPO)
    pass
bldr = builder.code()
bldr.setName("SCRIPT")
bldr.setInput(SCRIPT_PATH)
bldr.setComment("#", False)
bldr.setOutput(DST_SCRIPT)
bldr.onMacro(_onMacro)
bldr.onLine(lambda line: line)
bldr.start()

############################################################################### task

task = builder.c()
task.setName("UYGHUR")
task.setDebug(True)
task.setInput('./main.c')
task.setLibs([
    "incbin",
    # "string",
    "utf8",
    "raylib",
    "raygui",
])
task.addLib("utf8iter", {
    "URL": "git@github.com:adrianwk94/utf8-iterator.git",
    "BRANCH": "master",
    "DIR_I": "./source/",
})
task.setOutput(DST_ALIAS)
# task.setIcon('./resources/icon.ico')
task.addWarnings(False, [
    "unused-result",
    "discarded-qualifiers",
    "attributes",
    "incompatible-pointer-types"
])
task.addFlags(["-I ../pure-c-tools/"])
task.start()
task.run()
