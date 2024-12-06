
from scripts.base import *

################################################################

# SCRIPT_PATH = "./examples/help.ug"
SCRIPT_PATH = "./examples/test.ug"
# SCRIPT_PATH = "./examples/externals/yuguresh.ug"
# SCRIPT_PATH = "./examples/features/objective.ug"

SCRIPT_DIR, SCRIPT_FILE, SCRIPT_EXT, SCRIPT_NAME = tools.tools.parse_path(SCRIPT_PATH)
DST_ALIAS = DIR_BUILD + "uyghur"
DST_SCRIPT = DST_ALIAS + "." + SCRIPT_EXT

################################################################

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
    if command == "PROJECT_NAME":
        return code.format(PROJECT_NAME)
    elif command == "VERSION_CODE":
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

isUseRaysan = False
isUseRiley = False
ugStageLib = None
if isUseRaysan:
    ugStageLib = "raylib"
if isUseRiley:
    ugStageLib = "RSGL"

_ugLibs = [
    "incbin",
    "utf8",
    "tinyfiledialogs",
]
if ugStageLib:
    _ugLibs.insert(0, ugStageLib)
    

task = builder.c()
task.setName("UYGHUR")
task.setDebug(True)
task.setRelease(False)
task.setInput('./main.c')
task.setLibs(_ugLibs)
task.addLinks([
    "gdi32",
    "winmm",
    "opengl32",
])
task.setOutput(DST_ALIAS)
task.setIcon('./resources/icon.ico')
# 
# task.addLib("c_std", {
#     "URL": "git@github.com:KaisenAmin/c_std.git",
#     "BRANCH": "main",
#     "DIR_I": "./",
# })
task.addWarnings(False, [
    "unused-result",
    "discarded-qualifiers",
    "attributes",
    "incompatible-pointer-types"
])
#
if isUseRaysan:
    task.addFlags(["-D BUILDER_USE_RAYSAN"])
elif isUseRiley:
    task.addFlags(["-D BUILDER_USE_RILEY"])
else:
    # task.addFlags(["-D BUILDER_NO_BACKEND"])
    task.addFlags(["-D BUILDER_USE_TIGR"])

#
task.addFlags(["-I ../c-pure-tools/"])
task.addFlags(["-I ../c-extra-tools/"])
task.start()
task.run()
