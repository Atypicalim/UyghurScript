
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

################################################################ generate

def _tryGenerateFunctionDoc(module, function, index):
    func = function[0]
    desc = function[1]
    rtrn = function[2]
    args = function[3]
    #
    _args = ""
    for arg in args:
        ctyp = arg[0]
        name = arg[1]
        vtyp = arg[2]
        _arg = tplDocumentArgument.format(type=vtyp, name=name)
        _args = _args + "\n" + _arg
    if _args == "":
        _args = tplDocumentArgumentless
    # 
    if not desc:
        desc = tplDocDescriptionless.format(module=module, func=func)
    # 
    print(rtrn)
    _return = rtrn[0] if rtrn[0] != None else rtrn[1]
    _function = tplDocFunction.format(index=index, func=func, desc=desc, rtrn=_return, args=_args)
    return _function

def _tryGenerateDocument(module, source, functions):
    functions = "".join(functions)
    text = tplDocBody.format(project=PROJECT_NAME, module=module, source=source, functions=functions)
    return text

################################################################ walk

def _tryWalkDocumentModule(module, fromPath):
    functionDocs = []
    def _onLine(index, lines):
        autoFunc = tryDetectBindAutoFunction(fromPath, module, index, lines)
        if autoFunc:
            # print("-->", autoFunc)
            _funcDoc = _tryGenerateFunctionDoc(module, autoFunc, len(functionDocs) + 1)
            functionDocs.append(_funcDoc)
            return
        manuFunc = tryDetectBindManuFunction(fromPath, module, index, lines)
        if manuFunc:
            # print("-->", manuFunc)
            _funcDoc = _tryGenerateFunctionDoc(module, manuFunc, len(functionDocs) + 1)
            functionDocs.append(_funcDoc)
            return
    tryForEachLine(module, fromPath, _onLine)
    return functionDocs

def _tryWalkDocumentLibrary(libName, modNames, libPath):
    # 
    outPath = tools.tools.append_path(DIR_DOCUMENT, libName)
    tools.files.mk_folder(outPath)
    # 
    bindDir = tools.tools.append_path(DIR_BUILD, "bind")
    bindDir = tools.tools.append_path(bindDir, libName)
    codeDir = tools.tools.append_path(DIR_SOURCE, libName)
    # 
    for index, module in enumerate(modNames):
        source = tools.tools.append_path(codeDir, module) + ".c"
        target = tools.tools.append_path(outPath, module) + ".md"
        print("documenting:", module, source)
        functions = _tryWalkDocumentModule(module, source)
        # 
        _source = "../../" + source.replace("\\", "/")
        _document = _tryGenerateDocument(module, _source, functions)
        # 
        tools.files.write(target, _document, 'utf-8')
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

