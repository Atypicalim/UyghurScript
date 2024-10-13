
from scripts.base import *

################################################################

DIR_BIND = tools.tools.append_path(DIR_BUILD, "bind")
tools.files.delete(DIR_BIND)
tools.files.mk_folder(DIR_BIND)

################################################################

tplBindReceive = '''{typ} {nam} = Bridge_receive{val}(bridge);'''

tplBindSending = '''{nam}'''

tplBindResult = '''Bridge_return{rtrn}(bridge, rrr);'''

tplBindFunction = '''// {desc}
void native_{module}_{func}(Bridge *bridge) {{
    {rcyv}
    {rtrn} rrr = auto_{module}_{func}({send});
    {rslt}
}}'''

tplBindRegister = '''    Bridge_bindNative(bridge, "ALIAS_{module}_{func}", native_{module}_{func});'''

tplBindDescriptionless = ''''''

################################################################ regex

BINDABLE_FUNC_PATTERN = re.compile('(.*)auto_([^(]*)\(([^)]*)\)')
BINDED_FUNC_PATTERN = re.compile('(.*)native_([^(]*)\(([^)]*)\)')

# ctype -> bridge type, return append
BINDING_MAP = {
    'int': ["Number", ""],
    'long': ["Number", ""],
    'float': ["Number", ""],
    'double': ["Number", ""],
    'bool': ["Boolean", ""],
    'CString': ["String", "pct_free(rrr);"],
}

################################################################ find


def _tryConvertArguments(text):
    args = []
    if text == "":
        return args
    for _arg in text.split(","):
        assert "*" not in _arg, "invalid auto bind arg"
        _arg = _arg.strip()
        [typ, nam] = _arg.split(" ")
        args.append([typ, nam])
    return args

def _tryConvertReturn(text):
    rtrn = text.strip()
    assert "*" not in rtrn, "invalid auto bind arg"
    return rtrn

def _tryConvertFunction(module, text):
    func = text.replace(module + "_", "").strip()
    return func

def _tryFindDescription(line):
    line = line.strip()
    if not line.startswith("//"):
        return
    return line[2:]

################################################################ detect

def _tryDetectBindableFunction(path, module, index, lines):
    #
    line = lines[index]
    match = BINDABLE_FUNC_PATTERN.search(line)
    if not match:
        return
    #
    rtrn = match.group(1).strip()
    func = match.group(2).strip()
    args = match.group(3).strip()
    # 
    desc = _tryFindDescription(lines[index - 1])
    func = _tryConvertFunction(module, func)
    rtrn = _tryConvertReturn(rtrn)
    args = _tryConvertArguments(args)
    #
    return [func, desc, rtrn, args]

def _tryDetectBindedFunction(path, module, index, lines):
    #
    line = lines[index]
    match = BINDED_FUNC_PATTERN.search(line)
    if not match:
        return
    #
    func = match.group(2).strip()
    func = func.replace(module + "_", "").strip()
    #
    return [func]

################################################################ generate

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
            typ = arg[0]
            nam = arg[1]
            info = BINDING_MAP.get(typ)
            _rcyv = tplBindReceive.format(typ=typ, nam=nam, val=info[0])
            _send = tplBindSending.format(typ=typ, nam=nam)
            rcyv = _rcyv if rcyv == "" else rcyv + "\n    " + _rcyv
            send = _send if send == "" else send + ", " + _send
        # 
        info = BINDING_MAP.get(rtrn)
        rslt = tplBindResult.format(rtrn=info[0])
        if info[1] != '':
            rslt = rslt + "\n    " + info[1]
        # 
        if not desc:
            desc = tplBindDescriptionless.format(module=module, func=func)
        # 
        _function = tplBindFunction.format(
            module=module,
            func=func,
            desc=desc,
            rcyv=rcyv,
            send=send,
            rtrn=rtrn,
            rslt=rslt
        )
        _functions = _function if _functions == "" else _functions + "\n\n" + _function
        #
        _register = tplBindRegister.format(module=module, func=func)
        _registers = _register if _registers == "" else _registers + "\n" + _register 
    # 
    return _functions, _registers

def _tryGenerateManualBind(module, functions):
    _registers = ""
    for function in functions:
        func = function[0]
        _register = tplBindRegister.format(module=module, func=func)
        _registers = _register if _registers == "" else _registers + "\n" + _register 
    #
    return _registers

################################################################

def _tryForEachLine(module, fromPath, func):
    if not tools.files.is_file(fromPath):
        return
    _lines = readLines(fromPath)
    lineIndex = -1
    for _line in _lines:
        lineIndex = lineIndex + 1
        func(lineIndex, _lines)
    pass

def _tryWalkBindModule(module, fromPath):
    functionsBindable = []
    functionsBinded = []
    def _onLine(index, lines):
        bindableInfo = _tryDetectBindableFunction(fromPath, module, index, lines)
        if bindableInfo:
            # print("-->", bindableInfo)
            functionsBindable.append(bindableInfo)
            return
        bindendInfo = _tryDetectBindedFunction(fromPath, module, index, lines)
        if bindendInfo:
            # print("-->", bindendInfo)
            functionsBinded.append(bindendInfo)
            return
    _tryForEachLine(module, fromPath, _onLine)
    functions, registers = _tryGenerateAutoBind(module, functionsBindable)
    _registers = _tryGenerateManualBind(module, functionsBinded)
    if _registers != '':
        registers = registers + '\n\n' + _registers
    return functions, registers


    return _registers

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

