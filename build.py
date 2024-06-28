
import sys
sys.path.append('../my-build-tools/')

from builder import builder

tools = builder.tools

SCRIPT_PATH = "./examples/yuguresh.ug"

DST_DIR = "./build/"
DST_NAME = "uyghur"
DST_ALIAS = DST_DIR + DST_NAME
DST_SCRIPT = DST_ALIAS + ".ug"

tools.files.mk_folder(DST_DIR)
tools.files.delete(DST_SCRIPT)
if SCRIPT_PATH is not None and tools.files.is_file(SCRIPT_PATH):
    tools.files.copy(SCRIPT_PATH, DST_SCRIPT)
else:
    help_text = " # yardem \n ikrangha [merhaba, uyghur script qa xosh kepsiz ...] yezilsun\n"
    tools.files.write(DST_SCRIPT, help_text, 'utf-8')

task = builder.c()
task.setDebug(True)
task.setInput('./main.c')
task.setLibs("incbin", "raylib", "raygui")
task.setOutput(DST_ALIAS)
task.setIcon('./resources/icon.ico')
task.start()
task.run()
