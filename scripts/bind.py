
from scripts.base import *

################################################################

DIR_BIND = tools.tools.append_path(DIR_BUILD, "bind")
tools.files.delete(DIR_BIND)
tools.files.mk_folder(DIR_BIND)

################################################################

tplBindReceive = '''{typ} {nam} = Bridge_receive{val}(bridge);'''

tplBindSending = '''{nam}'''

tplBindResultValue = '''Bridge_return{rtrn}(bridge, rrr);'''
tplBindResultEmpty = '''Bridge_returnEmpty(bridge);'''

tplBindFunction = '''// {desc}
void native_{module}_{func}(Bridge *bridge) {{
    {rcyv}
    {exct}
    {rslt}
}}'''

tplBindRegister = '''    Bridge_bindNativeExt(bridge, "{alias}", native_{func}, "{file}", {line});'''

tplBindDescriptionless = ''''''

################################################################ generate

def _isToGlobal(module):
    return module == 'global'

def _tryGenerateAutoBind(module, functions: list[BindInfo]):
    #
    _functions = ""
    _registers = ""
    for function in functions:
        func = function.func
        desc = function.desc
        rtrn = function.rtrn
        args = function.args
        #
        rcyv = ""
        send = ""
        for arg in args:
            _rcyv = tplBindReceive.format(typ=arg.cType, nam=arg.name, val=arg.vType)
            _send = tplBindSending.format(typ=arg.cType, nam=arg.name)
            rcyv = _rcyv if rcyv == "" else rcyv + "\n    " + _rcyv
            send = _send if send == "" else send + ", " + _send
        # 
        rslt = ""
        exct = ""
        if rtrn.cType == 'void':
            exct = f"bind_{module}_{func}({send});"
            rslt = tplBindResultEmpty
        else:
            exct = f"{rtrn.cType} rrr = bind_{module}_{func}({send});"
            rslt = tplBindResultValue.format(rtrn=rtrn.vType)
            if rtrn.fCode != '':
                rslt = rslt + "\n    " + rtrn.fCode

        # 
        if not desc:
            desc = tplBindDescriptionless.format(module=module, func=func)
        # 
        _function = tplBindFunction.format(
            module=module,
            func=func,
            desc=desc,
            rcyv=rcyv,
            exct=exct,
            rslt=rslt
        )
        _functions = _function if _functions == "" else _functions + "\n\n" + _function
        #
        _register = _tryGenerateBindCode(module, function)
        _registers = _register if _registers == "" else _registers + "\n" + _register 
    # 
    return _functions, _registers

def _tryGenerateManualBind(module, functions: list[BindInfo]):
    _registers = ""
    for function in functions:
        _register = _tryGenerateBindCode(module, function)
        _registers = _register if _registers == "" else _registers + "\n" + _register 
    #
    return _registers

def _tryGenerateBindCode(module, function):
    func = function.func
    alias = function.func
    if not _isToGlobal(module):
        func = module + "_" + func
        if "ALIAS_" + alias not in aliasNamesMap:
            alias = module + "_" + alias
        pass
    __code = tplBindRegister.format(module=module, alias=alias, func=func, file=function.file, line=function.line)
    return __code

################################################################

def _tryWalkBindModule(module, fromPath):
    functionsBindable: List[BindInfo] = []
    functionsBinded: List[BindInfo] = []
    def _onLine(index, lines):
        line = lines[index]
        if line.startswith("//"):
            return
        bindableInfo = tryDetectBindAutoFunction(fromPath, module, index, lines)
        if bindableInfo:
            # print("-->", bindableInfo)
            functionsBindable.append(bindableInfo)
            return
        bindendInfo = tryDetectBindManuFunction(fromPath, module, index, lines)
        if bindendInfo:
            # print("-->", bindendInfo)
            functionsBinded.append(bindendInfo)
            return
    tryForEachLine(module, fromPath, _onLine)
    functions, registers = _tryGenerateAutoBind(module, functionsBindable)
    _registers = _tryGenerateManualBind(module, functionsBinded)
    if _registers != '':
        registers = registers + '\n\n' + _registers
    return functions, registers

################################################################


tplBindLine = '''- [{name}]({path})'''

def _buildBinding(library, module, libPath):
    # 
    bldr = builder.code()
    bldr.setName("BIND_" + module)
    # 
    outPath = tools.tools.append_path(DIR_BIND, library)
    tools.files.mk_folder(outPath)
    #
    source = tools.tools.append_path(libPath, module) + ".c"
    target = tools.tools.append_path(outPath, module) + ".c"
    functions, registers = _tryWalkBindModule(module, source)
    if not registers:
        bldr._print("skipped empty register!")
        return
    #
    def _onMacro(code, command, argument = None):
        if command == "MODULE_FORMAT":
            return code.format(library=library,module=module)
        elif command == "MODULE_FUNSTIONS":
            return functions
        elif command == "MODULE_REGISTERS":
            return registers
        elif command == "MODULE_TARGET":
            target = 'NULL' if _isToGlobal(module) else 'ALIAS_' + module
            return code.format(target=target)
    #
    bldr.setInput("./uyghur/templates/module.tpl.c")
    bldr.setComment("//", False)
    bldr.setOutput(target)
    bldr.onMacro(_onMacro)
    bldr.onLine(lambda line: line)
    bldr.start()
    #

def _tryWalkBindLibrary(libray, modules, libPath):
    for _, module in enumerate(modules):
        _buildBinding(libray, module, libPath)

################################################################ library

print("BIND:")
_tryWalkBindLibrary(NAME_INTERNALS, INTERNAL_MODULES, DIR_INTERNALS)
_tryWalkBindLibrary(NAME_EXTERNALS, EXTERNAL_MODULES, DIR_EXTERNALS)
print("BINDED!")

