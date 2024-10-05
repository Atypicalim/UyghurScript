
from scripts.base import *

################################################################

NAME_INTERNALS = "internals"
NAME_EXTERNALS = "externals"

DIR_DOCUMENT = tools.tools.append_path("./", "documents")
tools.files.delete(DIR_DOCUMENT)
tools.files.mk_folder(DIR_DOCUMENT)

################################################################

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

> source code [{name}]({source})
---
{functions}
---
> document info [document.md](../README.md)
'''

################################################################ regex

BRIDGE_RGSTR_PATTERN = re.compile('lib_(.*)_register')

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

def _tryGenerateDocument(module, name, source, functions):
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
    text = tplMdDocument.format(project=PROJECT_NAME, module=module, name=name, source=source, functions=_functions)
    return text

################################################################ walk

def _tryWalkBridgeFile(isInternal, fromPath, toPath):
    _lines = readLines(fromPath)
    #
    functions = []
    dir, module, ext, name = tools.tools.parse_path(fromPath)
    lineIndex = -1
    for _line in _lines:
        lineIndex = lineIndex + 1
        info = _tryDetectFunction(fromPath, module, lineIndex, _line, _lines)
        if info:
            functions.append(info)
    #
    source = "../../" + fromPath.replace("\\", "/")
    _document = _tryGenerateDocument(module, name, source, functions)
    tools.files.write(toPath, _document, 'utf-8')

def _tryWalkRegisteredModules(isInternal, _modules, _sources, _targets):
    for index, _module in enumerate(_modules):
        _source = _sources[index]
        _target = _targets[index]
        print("documenting:", _module)
        _tryWalkBridgeFile(isInternal, _source, _target)
    pass

def _tryWalkBridgeRegister(isInternal, libPath):
    # 
    outName = NAME_INTERNALS if isInternal else NAME_EXTERNALS
    outPath = tools.tools.append_path(DIR_DOCUMENT, outName)
    tools.files.mk_folder(outPath)
    # 
    headerPath = tools.tools.append_path(libPath, "header.h")
    _names = []
    _sources = []
    _targets = []
    _lines = readLines(headerPath)
    for _line in _lines:
        match = BRIDGE_RGSTR_PATTERN.search(_line)
        if match:
            name = match.group(1).strip()
            source = tools.tools.append_path(libPath, name) + ".c"
            target = tools.tools.append_path(outPath, name) + ".md"
            _names.append(name)
            _sources.append(source)
            _targets.append(target)
        pass
    _tryWalkRegisteredModules(isInternal, _names, _sources, _targets)
    return [_names, _sources, _targets]

################################################################ work

print("DOCUMENT:")
[internalModules, internalSources, internalTargets] = _tryWalkBridgeRegister(True, DIR_INTERNALS)
[externalModules, externalSources, externalTargets] = _tryWalkBridgeRegister(False, DIR_EXTERNALS)
print("DOCUMENTED!")


################################################################ readme

tplMdDocuments = '''- [{name}]({path})'''

mdInternalsText = ""
mdExternalsText = ""

for index, name in enumerate(internalModules):
    path = NAME_INTERNALS + "/" + name + ".md"
    internalText = tplMdDocuments.format(index=index, name=name, path=path)
    mdInternalsText = mdInternalsText + "\n" + internalText

for index, name in enumerate(externalModules):
    path = NAME_EXTERNALS + "/" + name + ".md"
    internalText = tplMdDocuments.format(index=index, name=name, path=path)
    mdExternalsText = mdExternalsText + "\n" + internalText

def _onMacro(code, command, argument = None):
    if command == "MD_INTERNALS":
        return mdInternalsText
    elif command == "MD_EXTERNALS":
        return mdExternalsText
    elif command == "MD_PROJECT":
        return code.format(project=PROJECT_NAME)
bldr = builder.code()
bldr.setName("DOCUMENTS")
bldr.setInput("./others/documents.tpl.md")
bldr.setComment("//", False)
bldr.setOutput("./documents/README.md")
bldr.onMacro(_onMacro)
bldr.onLine(lambda line: line)
bldr.start()

