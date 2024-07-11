
import sys
import yaml
sys.path.append('../my-build-tools/')

from builder import builder

tools = builder.tools

SCRIPT_PATH = "./examples/yuguresh.ug"

DST_DIR = "./build/"
DST_NAME = "uyghur"
DST_ALIAS = DST_DIR + DST_NAME
DST_SCRIPT = DST_ALIAS + ".ug"

###############################################################################

# c conf
cConfDeclare = """char* {0} = \"{0}\";"""
cConfDefine = """#define {0} \"{0}\""""
#
cConfLanguageLine = """{{&{0}, "{1}"}},"""
cConfTokenLine = """{{"{0}", "{1}"}},"""
# 
cConfBody = """
#define YAML_SIZE_{0}_{1} {3}
static const {2} YAML_MAP_{0}_{1}[YAML_SIZE_{0}_{1}] = {{
{4}
}};
"""

def readYaml(fromPath):
    _configs = tools.files.read(fromPath, 'utf-8')
    configs = yaml.safe_load(_configs)
    aliasesArr = []
    languagesMap = {}
    for alias, pairs in configs.items():
        # aliases
        aliasesArr.append(alias)
        # languages
        for lang, value in pairs.items():
            if lang not in languagesMap:
                languagesMap[lang] = {}
            languagesMap[lang][alias] = value
    return aliasesArr, languagesMap

def formatAliases(template, aliases):
    variables = []
    for alias in aliases:
        variable = template.format(alias)
        variables.append(variable)
    return "\n".join(variables)

def formatLanguages(template, name, laguages, typ):
    bodies = []
    for lang, pairs in laguages.items():
        lang = lang.upper()
        lines = []
        for alias, value in pairs.items():
            line = template.format(alias, value)
            lines.append(line)
        _length = len(lines)
        _lines = "\n    ".join(lines)
        body = cConfBody.format(name, lang, typ, _length, "    " + _lines)
        bodies.append(body)
    return "\n".join(bodies)

# languages
name = "LANGUAGES"
aliases, laguages = readYaml("./uyghur/others/languages.yml")
langaugeVariables = formatAliases(cConfDeclare, aliases)
languageBodies = formatLanguages(cConfLanguageLine, name, laguages, "LANGUAGE_PAIRS")

# # tokens
name = "TOKENS"
aliases, laguages = readYaml("./uyghur/others/tokens.yml")
tokenVariables = formatAliases(cConfDefine, aliases)
tokenBodies = formatLanguages(cConfTokenLine, name, laguages, "UG_PAIRS")

###############################################################################

# languages
bldr = builder.code()
bldr.setInput("./uyghur/others/languages.yaml.h")
bldr.setComment("//")
bldr.setOutput(DST_DIR + "languages.yaml.h")
bldr.onMacro(lambda code, command, argument = None: langaugeVariables if command == "VARIABLES" else languageBodies)
bldr.onLine(lambda line: line)
bldr.start()

# tokens
bldr = builder.code()
bldr.setInput("./uyghur/others/tokens.yaml.h")
bldr.setComment("//")
bldr.setOutput(DST_DIR + "tokens.yaml.h")
bldr.onMacro(lambda code, command, argument = None: tokenVariables if command == "VARIABLES" else tokenBodies)
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
    "string",
    "raylib",
    "raygui",
])
task.setOutput(DST_ALIAS)
# task.setIcon('./resources/icon.ico')
task.start()
task.run()
