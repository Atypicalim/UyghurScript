
from scripts.base import *

###############################################################################

DIR_TEMPORARY = tools.tools.append_path(DIR_BUILD, "temporary")
tools.files.delete(DIR_TEMPORARY)
tools.files.mk_folder(DIR_TEMPORARY)

############################################################################### convert

print("CONVERT_SNIPPETS:")
for name in snippets:
    fromPath = tools.tools.append_path(DIR_SNIPPETS, name) + "." + EXAMPLE_LANG
    result = subprocess.run([
        "node", "./others/convert.js", fromPath, DIR_SNIPPET_TRANS, DIR_SNIPPET_COLOR
    ], capture_output=True, text=True)
    print("translate:", name, "OK" if result.returncode == 0 else result.stderr)
print("CONVERTED!\n")

print("CONVERT_FEATURES:")
for name in features:
    fromPath = tools.tools.append_path(DIR_FEATURES, name) + "." + EXAMPLE_LANG
    toPath = tools.tools.append_path(DIR_FEATURES, name)
    tools.files.delete(toPath)
    tools.files.mk_folder(toPath)
    result = subprocess.run([
        "node", "./others/convert.js", fromPath, toPath, DIR_TEMPORARY
    ], capture_output=True, text=True)
    print("translate:", name, "OK" if result.returncode == 0 else result.stderr)
    #
    for lang in langsArrConfigs:
        fPath = tools.tools.append_path(toPath, name) + "." + lang
        tPath = tools.tools.append_path(DIR_LANGUAGE, lang)
        tools.files.mk_folder(tPath)
        shutil.copy(fPath, tPath)
    print("movement:", name)
    
print("CONVERTED!\n")
