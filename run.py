
import os
import sys
import subprocess

################################################################

__arg1 = sys.argv[1] if len(sys.argv) > 1 else ""
__arg2 = sys.argv[2] if len(sys.argv) > 2 else ""
__workingDir = os.path.dirname(os.path.abspath(__file__))

class Run:
    pass
Run.workingDir = __workingDir
Run.arg1 = __arg1
Run.arg2 = __arg2
Run.needUpdate = __arg1 == "--update" or __arg1 == "-u"
Run.needGenerate = __arg1 == "--generate" or __arg1 == "-g"
Run.isRelease = __arg1 == "--release" or __arg1 == "-r"

################################################################

try:
    import yaml
except ModuleNotFoundError:
    os.system("pip3 install PyYAML")

################################################################

def downloadGitTo(url, path):
    print("\ndownload:", url)
    if os.path.isdir(path):
        if Run.needUpdate:
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
if Run.needUpdate:
    print("\nupdateing self:")
    subprocess.run(['git', 'pull'], check=True)
    print("updated!\n")
    sys.exit(0)

################################################################

from scripts.base import *
import scripts.bind
if Run.needGenerate:
    import scripts.converter
    import scripts.converting
    import scripts.extension
    import scripts.readme
    import scripts.document
else:
    import scripts.interpreter
