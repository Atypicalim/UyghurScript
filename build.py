
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
cConfLine = """{{&{0}, "{1}"}},"""
cConfDefine = """char* {0} = \"\";"""
cConfBody = """
#define YAML_SIZE_{0}_{1} {2}
static const YAML_PAIRS YAML_MAP_{0}_{1}[YAML_SIZE_{0}_{1}] = {{
{3}
}};
"""
cConfContent = """
#ifndef YAML_MAP_{0}
#define YAML_MAP_{0}
//
{1}
//
{2}
//
#endif // YAML_MAP_{0}
"""

# dump yaml
def dumpYaml(name, fromPath, toPath):
    _configs = tools.files.read(fromPath, 'utf-8')
    configs = yaml.safe_load(_configs)
    defines = []
    bodies = []
    _map = {}
    # parse
    count = 1000
    for alias, pairs in configs.items():
        count = count + 1
        define = cConfDefine.format(alias, count)
        defines.append(define)
        # english
        lang = 'en'
        if lang not in _map:
            _map[lang] = {}
        _map[lang][alias] = alias.lower().replace('_', " ")
        # languages
        for lang, value in pairs.items():
            if lang not in _map:
                _map[lang] = {}
            _map[lang][alias] = value
    # body
    for lang, pairs in _map.items():
        lang = lang.upper()
        lines = []
        for alias, value in pairs.items():
            line = cConfLine.format(alias, value)
            lines.append(line)
        _length = len(lines)
        _lines = "\n    ".join(lines)
        body = cConfBody.format(name, lang, _length, "    " + _lines)
        bodies.append(body)
    # content
    _defines = "\n".join(defines)
    _bodies = "\n".join(bodies)
    _content = cConfContent.format(name, _defines, _bodies)
    tools.files.write(toPath, _content, "utf-8")

dumpYaml("LANGUAGES", "./uyghur/others/languages.yml", DST_DIR + "languages.yaml.h")
# dumpYaml("TOKENS", "./uyghur/others/tokens.yml", DST_DIR + "tokens.yaml.h")

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
task.setDebug(False)
task.setInput('./main.c')
task.setLibs("incbin", "raylib", "raygui")
task.setOutput(DST_ALIAS)
# task.setIcon('./resources/icon.ico')
task.start()
task.run()
