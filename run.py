
import os
import sys
import subprocess

################################################################

__arg1 = sys.argv[1] if len(sys.argv) > 1 else ""
__arg2 = sys.argv[2] if len(sys.argv) > 2 else ""
RunCmd = __arg1
RunExt = __arg2
__workingDir = os.path.dirname(os.path.abspath(__file__))

RunNeedUpdate = RunCmd == "--update" or RunCmd == "-u"
RunNeedGenerate = RunCmd == "--generate" or RunCmd == "-g"
RunNeedCompile = RunCmd == "--compile" or RunCmd == "-c"
RunNeedExecute = RunCmd == "--execute" or RunCmd == "-e"
RunNeedRelease = RunCmd == "--release" or RunCmd == "-r"

################################################################

try:
    import yaml
except ModuleNotFoundError:
    os.system("pip3 install PyYAML")

################################################################

def downloadGitTo(url, path):
    print("\ndownload:", url)
    if os.path.isdir(path):
        if RunNeedUpdate:
            os.chdir(path)
            subprocess.run(['git', 'pull'], check=True)
        else:
            print("exist!")
    else:
        try:
            subprocess.run(['git', 'clone', url, path], check=True)
            print("Git clone completed successfully.")
        except subprocess.CalledProcessError as e:
            print(f"An error occurred while cloning: {e}")
        pass

################################################################

downloadGitTo("git@github.com:Atypicalim/my-build-tools.git", "../my-build-tools")
downloadGitTo("git@github.com:Atypicalim/pure-js-tools.git", "../pure-js-tools")
downloadGitTo("git@github.com:Atypicalim/c-pure-tools.git", "../c-pure-tools")
downloadGitTo("git@github.com:Atypicalim/c-xtra-tools.git", "../c-xtra-tools")
downloadGitTo("git@github.com:Atypicalim/replay.git", "../c-replay-library")
downloadGitTo("git@github.com:Atypicalim/replot.git", "../c-replot-library")

################################################################

os.chdir(__workingDir)
if RunNeedUpdate:
    print("\nupdateing self:")
    subprocess.run(['git', 'pull'], check=True)
    print("updated!\n")
    sys.exit(0)

################################################################

from scripts.base import *
import scripts.bind

Run.workingDir = __workingDir
Run.scriptPath = None
Run.runTarget = None
Run.arg1 = __arg1
Run.arg2 = __arg2
Run.isRelease = RunNeedRelease

################################################################

if RunNeedGenerate:
    import scripts.converter
    import scripts.converting
    import scripts.extension
    import scripts.readme
    import scripts.document
    print("generated!\n")
    sys.exit(0)

################################################################

SCRIPT_PATH = "./examples/help.ug"
SCRIPT_PATH = "./examples/test.ug"
# SCRIPT_PATH = "./examples/features/objective.en"
# SCRIPT_PATH = "./examples/internals/number.ug"
# SCRIPT_PATH = "./examples/externals/yuguresh.ug"
Run.scriptPath = RunExt if RunExt != "" else SCRIPT_PATH

import scripts.interpreter

################################################################

task = Run.runTarget
dir, name, runnable = task.getRunnable()

runPath = RunExt if RunExt != "" else Run.scriptPath
runCmd = None
if RunNeedCompile:
    runCmd = "-c"
    pass
elif RunNeedExecute:
    runCmd = "-e"
    pass

if runCmd != None:
    isOk, output = tools.tools_execute(f"{runnable} {runCmd} {runPath}")
    print(output)
else:
    isOk, extra = tools.tools_spawn(runnable, [], cwd=dir)
    print(f"RUNNED:{isOk}", "" if isOk else extra)

################################################################
