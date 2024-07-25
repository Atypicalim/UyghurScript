
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
VERSION_CODE = 0.2
EXTENSION_VERSION = "1.0.2"
SUPPORT_LANG = set()
SCRIPT_PATH = "./examples/sinaq.kz"
SCRIPT_DIR, SCRIPT_FILE, SCRIPT_EXT, SCRIPT_NAME = tools.tools.parse_path(SCRIPT_PATH)

###############################################################################

DST_DIR = "./build/"
DST_ALIAS = DST_DIR + "script"
DST_SCRIPT = DST_ALIAS + "." + SCRIPT_EXT
tools.files.mk_folder(DST_DIR)

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
tplYamlConfig = """
#define {0} {2}
static const PAIR_{3} {1}[{0}] = {{
{4}
}};
"""

def readYaml(fromPath):
    _configs = tools.files.read(fromPath, 'utf-8')
    configs = yaml.safe_load(_configs)
    return configs

def readColors():
    configs = readYaml(f"./others/colors.yml")
    color2nameMap = {}
    name2colorMap = {}
    for color, names in configs.items():
        for name in names:
            if name not in name2colorMap:
                name2colorMap[name] = {}
            if color not in color2nameMap:
                color2nameMap[color] = []
            name2colorMap[name] = color
            if name not in color2nameMap[color]:
                color2nameMap[color].append(name)
    return color2nameMap, name2colorMap

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


langsArrLetters, namesArrLetters, mapLang2NameLetters, mapName2LangLetters = readLanguages("LETTERS")
langsArrLanguages, namesArrLanguages, mapLang2NameLanguages, mapName2LangLanguages = readLanguages("LANGUAGES")
langsArrAliases, namesArrAliases, mapLang2NameAliases, mapName2LangAliases = readLanguages("ALIASES")
langsArrOthers, namesArrOthers, mapLang2NameOthers, mapName2LangOthers = readLanguages("OTHERS")

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

###############################################################################

def _Yaml2Template(name, varTpl, lineTpl):
    _name = name.lower()
    langsArr, namesArr, mapLang2Name, mapName2Lang = readLanguages(name)
    #
    variables = formatVariables(varTpl, namesArr)
    bodies = formatBodies(lineTpl, name, mapLang2Name)
    footers = formatFooters(tplYamlLineNormal, name, mapName2Lang)
    [filterSizeByLang, filterConfByLang] = formatFilters(name, mapLang2Name)
    [filterSizeByName, filterConfByName] = formatFilters(name, mapName2Lang)
    #
    for lang in mapLang2Name:
        SUPPORT_LANG.add(lang)
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
    #
    bldr = builder.code()
    bldr.setInput(f"./uyghur/templates/{_name}.tpl.h")
    bldr.setComment("//")
    bldr.setOutput(f"{DST_DIR}{_name}.h")
    bldr.onMacro(_onMacro)
    bldr.start()
    pass

###############################################################################

_Yaml2Template("LANGUAGES", tplYamlDeclare, tplYamlLineReference)
_Yaml2Template("LETTERS", tplYamlDefine, tplYamlLineNormal)
_Yaml2Template("ALIASES", tplYamlDefine, tplYamlLineNormal)

###############################################################################

def _onMacro():
    def onMacro(code, command, argument = None):
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
    return onMacro

# configs
bldr = builder.code()
bldr.setInput("./uyghur/templates/configs.tpl.h")
bldr.setComment("//")
bldr.setOutput(DST_DIR + "configs.h")
bldr.onMacro(_onMacro())
bldr.onLine(lambda line: line)
bldr.start()

###############################################################################

# letters common
langsArr, namesArr, mapLang2Name, mapName2Lang = readLanguages("LETTERS")

###############################################################################

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

langNames = mapName2LangOthers['LANG_LANGUAGE_FULLNAME']
langTrans = mapName2LangOthers['LANG_LANGUAGE_TRANSLATION']
grammars = readYaml(f"./others/grammars.yml")

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

for lang in langsArr:
    # if lang == 'en':
    #     continue
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
    #
    def _onMacro(code, command, argument = None):
        if command == "FORMAT_ARGS":
            return code.format(lang=lang, name=name, tran=tran)
        else:
            translates = translateNames(lang, grammars[command])
            return code.format(translates)
    #
    bldr = builder.code()
    bldr.setInput("./extension/tmLanguage.tpl.json")
    bldr.setComment("//", False)
    bldr.setOutput(f"./extension/syntaxes/{lang}.tmLanguage.json")
    bldr.onMacro(_onMacro)
    bldr.onLine(lambda line: line)
    bldr.start()

langText = ",\n".join(langArray)
gramText = ",\n".join(gramArray)
keywordsText = ",\n".join(keywordsArray)
snippetsText = ",\n".join(snippetsArray)

# extension
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
bldr.setInput("./extension/package.tpl.json")
bldr.setComment("//", False)
bldr.setOutput("./extension/package.json")
bldr.onMacro(_onMacro())
bldr.onLine(lambda line: line)
bldr.start()

# snippets

print("SNIPPETS:")
snippets = readYaml(f"./others/snippets.yml")
extensionsDir = "./extension/"
snippetsBldDir = tools.tools.append_path(DST_DIR, "snippets")
snippetsExtDir = tools.tools.append_path(extensionsDir, "snippets")
tools.files.delete(snippetsBldDir)
tools.files.delete(snippetsExtDir)
tools.files.mk_folder(snippetsBldDir)
tools.files.mk_folder(snippetsExtDir)
snippetLang = "ug"
snippetsMap = {}
for name in snippets:
    print("\nsnippet:", name)
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

    # translate
    fineName = name + "." + snippetLang
    fromPath = tools.tools.append_path("./examples/snippets/", fineName)
    toPath = tools.tools.append_path(snippetsBldDir, fineName)
    shutil.copy(fromPath, toPath)
    result = subprocess.run(["node", "./others/convert.js", toPath], capture_output=True, text=True)
    print("translate:", name, result if isinstance(result, str) else "OK")
    # generate
    for lang in langsArr:
        snippetPath = toPath
        if lang != snippetLang:
            snippetPath = snippetPath + '.' + lang
            # continue
        # 
        _file = open(snippetPath, "r", encoding="utf-8")
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
for lang in langsArr:
    confPath = tools.tools.append_path(snippetsExtDir, f"{lang}.language-snippets.json")
    with open(confPath, "w", encoding="utf-8") as f:
        json.dump(snippetsMap[lang], f, ensure_ascii=False, indent=4)

###############################################################################

# 
letterArray = []
for name, infos in mapName2Lang.items():
    lines = ""
    for lang, value in infos.items():
        lines = lines + "{}:'{}',".format(lang, value)
    letterArray.append(name + ":{"+ lines + "},")
letterText = "\n".join(letterArray)
#
langArray = []
for lang in langsArr:
    langArray.append("'" + lang + "'")
langText = ", ".join(langArray)

# translate
def _onMacro():
    def onMacro(code, command, argument = None):
        if command == "LETTERS_MAP":
            return letterText
        elif command == "LANGS_ARR":
            return langText
    return onMacro
bldr = builder.code()
bldr.setInput("./others/translate.tpl.js")
bldr.setComment("//")
bldr.setOutput(DST_DIR + "translate.js")
bldr.onMacro(_onMacro())
bldr.onLine(lambda line: line)
bldr.start()


###############################################################################

#
color2nameMap, name2colorMap = readColors()
#
colorArray = []
for name, infos in mapName2Lang.items():
    lines = ""
    for lang, value in infos.items():
        color = name2colorMap[name]
        line = "'{}':'{}',".format(value, color)
        colorArray.append(line)
colorText = "\n".join(colorArray)
#
langArray = []
for lang in langsArr:
    langArray.append("'" + lang + "'")
langText = ", ".join(langArray)

# colorize
def _onMacro():
    def onMacro(code, command, argument = None):
        if command == "COLORS_MAP":
            return colorText
        elif command == "LANGS_ARR":
            return langText
    return onMacro
bldr = builder.code()
bldr.setInput("./others/colorize.tpl.js")
bldr.setComment("//", False)
bldr.setOutput(DST_DIR + "colorize.js")
bldr.onMacro(_onMacro())
bldr.onLine(lambda line: line)
bldr.start()

###############################################################################

# script
tools.files.mk_folder(DST_DIR)
tools.files.delete(DST_SCRIPT)
if SCRIPT_PATH is not None and tools.files.is_file(SCRIPT_PATH):
    tools.files.copy(SCRIPT_PATH, DST_SCRIPT)
else:
    help_text = " # yardem \n ikrangha [merhaba, uyghur script qa xosh kepsiz ...] yezilsun\n"
    tools.files.write(DST_SCRIPT, help_text, 'utf-8')

###############################################################################

# task
task = builder.c()
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
# task.addFlags(["-I ../pure-c-tools/"])
# task.start()
# task.run()
