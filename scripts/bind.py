
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

tplBindRegisterModule = '''    Bridge_bindNative(bridge, "ALIAS_{module}_{func}", native_{module}_{func});'''
tplBindRegisterGlobal = '''    Bridge_bindNative(bridge, "ALIAS_{func}", native_{func});'''

tplBindDescriptionless = ''''''

################################################################ generate

def _isToGlobal(module):
    return module == 'global'

def _tryGenerateAutoBind(module, functions):
    #
    _functions = ""
    _registers = ""
    for function in functions:
        func = function[0]
        desc = function[1]
        rtrn = function[2]
        args = function[3]
        #
        rcyv = ""
        send = ""
        for arg in args:
            ctyp = arg[0]
            name = arg[1]
            vtyp = arg[2]
            _rcyv = tplBindReceive.format(typ=ctyp, nam=name, val=vtyp)
            _send = tplBindSending.format(typ=ctyp, nam=name)
            rcyv = _rcyv if rcyv == "" else rcyv + "\n    " + _rcyv
            send = _send if send == "" else send + ", " + _send
        # 
        rslt = ""
        exct = ""
        if rtrn[1] == 'void':
            exct = f"bind_{module}_{func}({send});"
            rslt = tplBindResultEmpty
        else:
            exct = f"{rtrn[1]} rrr = bind_{module}_{func}({send});"
            rslt = tplBindResultValue.format(rtrn=rtrn[0])
            if rtrn[2] != '':
                rslt = rslt + "\n    " + rtrn[2]

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
        TEMPLATE = tplBindRegisterGlobal if _isToGlobal(module) else tplBindRegisterModule
        _register = TEMPLATE.format(module=module, func=func)
        _registers = _register if _registers == "" else _registers + "\n" + _register 
    # 
    return _functions, _registers

def _tryGenerateManualBind(module, functions):
    _registers = ""
    for function in functions:
        func = function[0]
        TEMPLATE = tplBindRegisterGlobal if _isToGlobal(module) else tplBindRegisterModule
        _register = TEMPLATE.format(module=module, func=func)
        _registers = _register if _registers == "" else _registers + "\n" + _register 
    #
    return _registers

################################################################

def _tryWalkBindModule(module, fromPath):
    functionsBindable = []
    functionsBinded = []
    def _onLine(index, lines):
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

