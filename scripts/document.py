
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

def _tryGenerateFunctionDoc(module, function: BindInfo, index):
    func = function.func
    desc = function.desc
    rtrn = function.rtrn
    args = function.args
    #
    _args = ""
    for arg in args:
        _arg = tplDocumentArgument.format(type=arg.vType, name=arg.name)
        _args = _args + "\n" + _arg
    if _args == "":
        _args = tplDocumentArgumentless
    # 
    if not desc:
        desc = tplDocDescriptionless.format(module=module, func=func)
    # 
    _return = rtrn.vType if rtrn.vType != None else rtrn.cType
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
    mdLibrariesTextOut = ""
    mdLibrariesTextIn = ""
    for index, name in enumerate(modules):
        pathOut = document + "/" + name + ".md"
        pathIn = name + ".md"
        textOut = tplDocBodyLine.format(index=index, name=name, path=pathOut)
        textIn = tplDocBodyLine.format(index=index, name=name, path=pathIn)
        mdLibrariesTextOut = mdLibrariesTextOut + "\n" + textOut
        mdLibrariesTextIn = mdLibrariesTextIn + "\n" + textIn
    #
    def _onMacro(code, command, argument = None):
        if command == "MD_LIBRARIES":
            return mdLibrariesTextIn
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
    return mdLibrariesTextOut

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

