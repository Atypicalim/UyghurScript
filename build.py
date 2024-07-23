
import sys
import yaml
sys.path.append('../my-build-tools/')

from builder import builder
tools = builder.tools

###############################################################################

VERSION_CODE = 0.2
SUPPORT_LANG = set()
SCRIPT_PATH = "./examples/sinaq.uay"
SCRIPT_DIR, SCRIPT_FILE, SCRIPT_EXT, SCRIPT_NAME = tools.tools.parse_path(SCRIPT_PATH)

###############################################################################

DST_DIR = "./build/"
DST_ALIAS = DST_DIR + "script"
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
tplYamlConfig = """
#define {0} {2}
static const PAIR_{3} {1}[{0}] = {{
{4}
}};
"""

def readYaml(fromPath):
    _configs = tools.files.read(fromPath, 'utf-8')
    configs = yaml.safe_load(_configs)
    aliasesArr = []
    mapLang2Name = {}
    mapName2Lang = {}
    for alias, pairs in configs.items():
        # aliases
        aliasesArr.append(alias)
        # languages
        for lang, value in pairs.items():
            #
            if lang not in mapLang2Name:
                mapLang2Name[lang] = {}
            mapLang2Name[lang][alias] = value
            #
            if alias not in mapName2Lang:
                mapName2Lang[alias] = {}
            mapName2Lang[alias][lang] = value
            #
    return aliasesArr, mapLang2Name, mapName2Lang

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
    aliasesArr, mapLang2Name, mapName2Lang = readYaml(f"./uyghur/others/multilangs/{_name}.yml")
    #
    variables = formatVariables(varTpl, aliasesArr)
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
    bldr.setInput(f"./uyghur/others/templates/{_name}.tpl.h")
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
bldr.setInput("./uyghur/others/templates/configs.tpl.h")
bldr.setComment("//")
bldr.setOutput(DST_DIR + "configs.h")
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
task.start()
task.run()
