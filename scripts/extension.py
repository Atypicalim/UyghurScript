
from scripts.base import *



############################################################################### base

langNames = mapName2LangConfigs['LANG_LANGUAGE_FULLNAME']
langTrans = mapName2LangConfigs['LANG_LANGUAGE_TRANSLATION']

############################################################################### script

def _modulesForLang(lang):
    resultList = []
    for moduleName in TOTAL_MODULES:
        langMap = tryFindModuleName(moduleName)
        langText = langMap[lang]
        resultList.append(langText)
    return resultList

def _methodsForLang(module, lang):
    resultList = []
    functionsInfos = moduleFunctionsMap[module]
    for info in functionsInfos:
        langMap = tryFindMethodName(module, info.func)
        langText = langMap[lang]
        resultList.append(langText)
    return resultList

def _onScriptMacro(code, command, argument = None):
    if command == "PROJECT_REPO":
        return code.format(PROJECT_REPO)
    elif command == "PROJECT_NAME":
        return code.format(PROJECT_NAME)
    elif command == "LANG_VERSION":
        return code.format(LANGUAGE_VERSION)
    elif command == "EXT_VERSION":
        return code.format(EXTENSION_VERSION)
    elif command == "RUN_ACTIVATE":
        code = ""
        for lang in langsArrConfigs:
            code = code + "\n" + f"{lang}Module.run_activate(context);"
            pass
        return code
    elif command == "IMPORT_LANG":
        code = ""
        for lang in langsArrConfigs:
            code = code + "\n" + f"var {lang}Module = require('./{lang}.language');"
            pass
        return code
    else:
        return code

for lang in langsArrConfigs:
    def _onLandMacro(code, command, argument = None):
        if command == "TARGET_LANG":
            return code.format(lang)
        elif command == "COMPLETE_BOXES":
            modules = _modulesForLang(lang)
            _modules = list(map(lambda x: f'    "{x}"', modules))
            completes = ",\n".join(_modules)
            return f"    return [\n{completes}\n    ];"
        elif command == "MODULE_METHODS":
            cases = []
            for module in TOTAL_MODULES:
                _module = tryFindModuleName(module)[lang]
                internal = module in INTERNAL_MODULES
                methods = _methodsForLang(module, lang)
                _methods = list(map(lambda x: f'        "{x}"', methods))
                completes = ",\n".join(_methods)
                folder =  f"{NAME_INTERNALS}" if internal else f"{NAME_EXTERNALS}"
                cases.append(f'    "{_module}": ["{folder}", "{module}", [\n{completes}\n    ]],')
            body = "\n".join(cases)
            return body
        else:
            return _onScriptMacro(code, command, argument)
    name = langNames[lang]
    tran = langTrans[lang]
    bldr = builder.code()
    bldr.setName(f"EXT·SCRIPT.{lang}")
    bldr.setInput("./extension/src/language.tpl.js")
    bldr.setComment("//", False)
    bldr.setOutput(f"./extension/src/{lang}.language.js")
    bldr.onMacro(_onLandMacro)
    bldr.onLine(lambda line: line)
    bldr.start()

bldr = builder.code()
bldr.setName("EXT·SCRIPT.MAIN")
bldr.setInput("./extension/src/extension.tpl.js")
bldr.setComment("//", False)
bldr.setOutput(f"./extension/src/extension.js")
bldr.onMacro(_onScriptMacro)
bldr.onLine(lambda line: line)
bldr.start()

############################################################################### syntax

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
        filePath = tools.tools.append_path(DIR_SNIPPET_TRANS, name) + "." + lang
        _file = open(filePath, "r", encoding="utf-8")
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

langArray = []
gramArray = []
keywordsArray = []
snippetsArray = []

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
        if command == "LANG_VERSION":
            return code.format(LANGUAGE_VERSION)
        elif command == "EXT_VERSION":
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

