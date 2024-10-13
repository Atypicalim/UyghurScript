
from scripts.base import *

################################################################

DIR_DOCUMENT = tools.tools.append_path("./", "documents")
tools.files.delete(DIR_DOCUMENT)
tools.files.mk_folder(DIR_DOCUMENT)

################################################################

tplDocumentArgument = '''* {name}  `{type}`'''
tplDocumentArgumentless = ''' none'''

tplDocDescriptionless = ''' {func} func of {module} module'''

tplDocFunction = '''
### {index}. {func}

> 📝: {desc}

> 🔙: {rtrn}

> 🛒: {args}

'''

tplDocBody = '''
## {project} {module}

> source code [{module}.c]({source})
---
{functions}
---
> document info [document.md](../README.md)
'''

################################################################ regex

BRIDGE_FUNC_PATTERN = re.compile('(.*) native_(.*)\(Bridge \*bridge\)')
BRIDGE_RTRN_PATTERN = re.compile('Bridge_return([^(]*)\(')
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

def _tryDetectFunction(path, module, index, lines):
    line = lines[index]
    func = _tryFindFunction(module, line)
    if not func:
        return
    # 
    desc = _tryFindDescription(lines[index - 1])
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

def _tryGenerateDocument(module, source, functions):
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
            _arg = tplDocumentArgument.format(type=typ, name=nam)
            _args = _args + "\n" + _arg
        if _args == "":
            _args = tplDocumentArgumentless
        # 
        if not desc:
            desc = tplDocDescriptionless.format(module=module, func=func)
        # 
        _index = _index + 1
        _function = tplDocFunction.format(index=_index, func=func, desc=desc, rtrn=rtrn, args=_args)
        _functions = _functions + "" + _function
    # 
    text = tplDocBody.format(project=PROJECT_NAME, module=module, source=source, functions=_functions)
    return text

################################################################ walk

def _tryWalkDocumentModule(module, fromPath, toPath):
    _lines = readLines(fromPath)
    #
    functions = []
    lineIndex = -1
    for _line in _lines:
        lineIndex = lineIndex + 1
        info = _tryDetectFunction(fromPath, module, lineIndex, _lines)
        if info:
            functions.append(info)
    #
    source = "../../" + fromPath.replace("\\", "/")
    _document = _tryGenerateDocument(module, source, functions)
    tools.files.write(toPath, _document, 'utf-8')

def _tryWalkDocumentLibrary(libName, modNames, libPath):
    # 
    outPath = tools.tools.append_path(DIR_DOCUMENT, libName)
    tools.files.mk_folder(outPath)
    # 
    _sources = []
    _targets = []
    for name in modNames:
        source = tools.tools.append_path(libPath, name) + ".c"
        target = tools.tools.append_path(outPath, name) + ".md"
        _sources.append(source)
        _targets.append(target)
        pass
    #
    for index, _module in enumerate(modNames):
        _source = _sources[index]
        _target = _targets[index]
        print("documenting:", _module)
        _tryWalkDocumentModule(_module, _source, _target)
    pass

################################################################ work

print("DOCUMENT:")
_tryWalkDocumentLibrary(NAME_INTERNALS, INTERNAL_MODULES, DIR_INTERNALS)
_tryWalkDocumentLibrary(NAME_EXTERNALS, EXTERNAL_MODULES, DIR_EXTERNALS)
print("DOCUMENTED!")


################################################################ readme

tplDocBodyLine = '''- [{name}]({path})'''

def _buildDocument(document, modules):
    # 
    mdLibrariesText = ""
    for index, name in enumerate(modules):
        path = document + "/" + name + ".md"
        internalText = tplDocBodyLine.format(index=index, name=name, path=path)
        mdLibrariesText = mdLibrariesText + "\n" + internalText
    #
    def _onMacro(code, command, argument = None):
        if command == "MD_LIBRARIES":
            return mdLibrariesText
        elif command == "DOCUMENT_NAME":
            return code.format(document=document)
    #
    bldr = builder.code()
    bldr.setName("DOCUMENT_" + document)
    bldr.setInput("./others/document.tpl.md")
    bldr.setComment("//", False)
    bldr.setOutput("./documents/" + document + "/README.md")
    bldr.onMacro(_onMacro)
    bldr.onLine(lambda line: line)
    bldr.start()
    #
    return mdLibrariesText

mdInternalsText = _buildDocument(NAME_INTERNALS, INTERNAL_MODULES)
mdExternalsText = _buildDocument(NAME_EXTERNALS, EXTERNAL_MODULES)

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

