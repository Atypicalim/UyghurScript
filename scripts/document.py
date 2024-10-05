
from scripts.base import *


print("DOCUMENT:")

DIR_INTERNALS = tools.tools.append_path(DIR_SOURCE, "internals")
DIR_EXTERNALS = tools.tools.append_path(DIR_SOURCE, "externals")

tplMdArgument = '''* {name}  `{type}`'''
tplMdArgumentless = ''' none'''

tplMdDescriptionless = ''' {func} func of {module} module'''

tplMdFunction = '''
### {index}. {func}

> 📝: {desc}

> 🔙: {rtrn}

> 🛒: {args}
'''

tplMdDocument = '''
## {project} {module}

> source code [{name}]({path})
---
{functions}
---
> document info [document.md](../document.md)
'''

################################################################ regex

BRIDGE_FUNC_PATTERN = re.compile('(.*) native_(.*)\(Bridge \*bridge\)')
BRIDGE_RTRN_PATTERN = re.compile('Bridge_return(.*)\(')
BRIDGE_ARGS_PATTERN = re.compile('(.*) = Bridge_receive.*\(')

BRIDGE_ARGS_CUSTOM_PATTENR = re.compile('(.*) = .*_from_bridge')

################################################################ find


def _tryFindArgument(line):
    match = BRIDGE_ARGS_PATTERN.search(line)
    if match:
        arg = match.group(1).strip()
        return arg
    match = BRIDGE_ARGS_CUSTOM_PATTENR.search(line)
    if match:
        arg = match.group(1).strip()
        return arg
    

def _tryFindReturn(line):
    match = BRIDGE_RTRN_PATTERN.search(line)
    if not match:
        return
    pass
    rtn = match.group(1).strip()
    return rtn

def _tryFindFunction(module, line):
    match = BRIDGE_FUNC_PATTERN.search(line)
    if not match:
        return
    fun = match.group(2).strip().replace(module + "_", "")
    return fun

def _tryFindDescription(line):
    line = line.strip()
    if not line.startswith("//"):
        return
    return line[2:]

################################################################ detect

def _tryDetectFunction(path, module, index, line, lines):
    func = _tryFindFunction(module, line)
    if not func:
        return
    # 
    desc = _tryFindDescription(lines[index - 1])
    #
    args = []
    rtrn = None
    for num in range(1,50):
        _index = index + num
        _line = lines[_index]
        _return = _tryFindReturn(_line)
        if _return:
            rtrn = _return
            break
        _argument = _tryFindArgument(_line)
        if _argument:
            args.append(_argument)
        pass
    #
    return [func, desc, rtrn, args]


################################################################ generate

def _tryGenerateDocument(module, name, path, functions):
    #
    _index = 0
    _functions = ""
    for function in functions:
        func = function[0]
        desc = function[1]
        rtrn = function[2]
        args = function[3]
        #
        _args = ""
        for arg in args:
            arg = arg.replace(" *", " ").split(" ")
            typ = arg[0]
            nam = arg[1]
            _arg = tplMdArgument.format(type=typ, name=nam)
            _args = _args + "\n" + _arg
        if _args == "":
            _args = tplMdArgumentless
        # 
        if not desc:
            desc = tplMdDescriptionless.format(module=module, func=func)
        # 
        _index = _index + 1
        _function = tplMdFunction.format(index=_index, func=func, desc=desc, rtrn=rtrn, args=_args)
        _functions = _functions + "" + _function
    # 
    mdPath = tools.tools.append_path(DIR_DOCUMENT, module) + ".md"
    text = tplMdDocument.format(project=PROJECT_NAME, module=module, name=name, path=path, functions=_functions)
    tools.files.write(mdPath, text, 'utf-8')
    pass

################################################################ walk

def _tryWalkBridgeFile(filePath):
    _file = open(filePath, "r", encoding="utf-8")
    _content = _file.read().replace("\r\n", "\n")
    _lines = _content.split("\n")
    #
    functions = []
    dir, module, ext, name = tools.tools.parse_path(filePath)
    lineIndex = -1
    for _line in _lines:
        lineIndex = lineIndex + 1
        info = _tryDetectFunction(filePath, module, lineIndex, _line, _lines)
        if info:
            functions.append(info)
    #
    _tryGenerateDocument(module, name, filePath, functions)
    pass

def tryWalkBridgeDirectory(myPath):
    files = []
    for fileName in os.listdir(myPath):
        fullPath = tools.tools.append_path(myPath, fileName)
        if u"header" not in fileName:
            files.append(fullPath)
    for path in files:
        _tryWalkBridgeFile(path)
        # break

tryWalkBridgeDirectory(DIR_INTERNALS)
tryWalkBridgeDirectory(DIR_EXTERNALS)
print("DOCUMENTED!\n")
