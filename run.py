
import os
import sys
import subprocess

################################################################

RunCmd = sys.argv[1] if len(sys.argv) > 1 else None
RunExts = sys.argv[2:]
RunFolder = os.path.dirname(os.path.abspath(__file__))

RunNeedUpdate = RunCmd == "--update" or RunCmd == "-u"
RunNeedGenerate = RunCmd == "--generate" or RunCmd == "-g"
RunNeedRelease = RunCmd == "--release" or RunCmd == "-r"
RunIsConsumed = RunNeedUpdate or RunNeedGenerate or RunNeedRelease
RunIsDeliver = RunCmd != None and not RunIsConsumed

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

os.chdir(RunFolder)
if RunNeedUpdate:
    print("\nupdateing self:")
    subprocess.run(['git', 'pull'], check=True)
    print("updated!\n")
    sys.exit(0)

################################################################

from scripts.base import *
import scripts.bind

Run.workingDir = RunFolder
Run.scriptPath = None
Run.runTarget = None
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

scriptPath = "./examples/help.ug"
scriptPath = "./examples/test.ug"
# scriptPath = "./examples/features/objective.en"
# scriptPath = "./examples/internals/number.ug"
# scriptPath = "./examples/externals/yuguresh.ug"

Run.scriptPath = scriptPath
# assert tools.files.is_file("." + Run.scriptPath)
import scripts.interpreter

################################################################

task = Run.runTarget
dir, name, runnable = task.getRunnable()
if Run.isRelease:
    tools.files.copy(runnable, "release/" + name)

runArgs = [RunCmd] + RunExts if RunIsDeliver else []
isOk, extra = tools.tools_spawn(runnable, runArgs, cwd=dir)
print(f"RUNNED:{isOk}", "" if isOk else extra)

################################################################
