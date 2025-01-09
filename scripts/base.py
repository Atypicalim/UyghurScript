

import os
import re
import sys
import json
import subprocess
import shutil
import yaml
from typing import List
sys.path.append('../my-build-tools/')

from builder import builder
tools = builder.tools

###############################################################################

PROJECT_NAME ="UyghurScript"
PROJECT_REPO ="https://github.com/Atypicalim/UyghurScript"
VERSION_CODE = 0.11
EXTENSION_VERSION = "1.0.11"
EXAMPLE_LANG = 'en'
SUPPORT_LANG = set()


###############################################################################

print("\nBASE_INITIALIZE:")

DIR_SOURCE = "./uyghur/"
DIR_EXAMPLE = "./examples/"
DIR_BUILD = "./build/"
tools.files.mk_folder(DIR_BUILD)

# source
NAME_INTERNALS = "internals"
NAME_EXTERNALS = "externals"
DIR_INTERNALS = tools.tools.append_path(DIR_SOURCE, NAME_INTERNALS)
DIR_EXTERNALS = tools.tools.append_path(DIR_SOURCE, NAME_EXTERNALS)

# example
DIR_SNIPPETS = tools.tools.append_path(DIR_EXAMPLE, "snippets")
DIR_FEATURES = tools.tools.append_path(DIR_EXAMPLE, "features")
DIR_LANGUAGE = tools.tools.append_path(DIR_EXAMPLE, "language")

DIR_SNIPPET_TRANS = tools.tools.append_path(DIR_BUILD, "snippets_trans")
DIR_SNIPPET_COLOR = tools.tools.append_path(DIR_BUILD, "snippets_color")
tools.files.delete(DIR_SNIPPET_TRANS)
tools.files.delete(DIR_SNIPPET_COLOR)
tools.files.mk_folder(DIR_SNIPPET_TRANS)
tools.files.mk_folder(DIR_SNIPPET_COLOR)

###############################################################################

# c conf
tplYamlDeclare = """char* {0} = \"{0}\";"""
tplYamlDefine = """#define {0} \"{0}\""""
#
tplYamlLineReference = """{{&{0}, "{1}"}},"""
tplYamlLineNormal = """{{"{0}", "{1}"}},"""
#
tmplYamlNameSize = "YAML_SIZE_{0}_{1}"
tmplYamlNameConf = "YAML_CONF_{0}_{1}"
# 
tplYamlFilter = """    if (pct_cstr_ends_with(tp, "{0}") == 0) {{return {1};}}"""
#
tplYamlConfig = """#define {0} {2}
static const PAIR_{3} {1}[{0}] = {{
{4}
}};
"""

############################################################################### utils

def readLines(filePath):
    _file = open(filePath, "r", encoding="utf-8")
    _content = _file.read().replace("\r\n", "\n")
    _lines = _content.split("\n")
    return _lines

def readYaml(fromPath):
    _configs = tools.files.read(fromPath, 'utf-8')
    configs = yaml.safe_load(_configs)
    return configs

def readLanguages(fileName):
    fromName = fileName.lower()
    fromPath = f"./uyghur/multilangs/{fromName}.yml"
    configs = readYaml(fromPath)
    langsArr = []
    namesArr = []
    mapLang2Name = {}
    mapName2Lang = {}
    for alias, pairs in configs.items():
        # names
        namesArr.append(alias)
        # 
        for lang, value in pairs.items():
            # langs
            if lang not in langsArr:
                langsArr.append(lang)
            #
            if lang not in mapLang2Name:
                mapLang2Name[lang] = {}
            mapLang2Name[lang][alias] = value
            #
            if alias not in mapName2Lang:
                mapName2Lang[alias] = {}
            mapName2Lang[alias][lang] = value
            #
    return langsArr, namesArr, mapLang2Name, mapName2Lang

############################################################################### yamls

langsArrLetters, namesArrLetters, mapLang2NameLetters, mapName2LangLetters = readLanguages("LETTERS")
langsArrLanguages, namesArrLanguages, mapLang2NameLanguages, mapName2LangLanguages = readLanguages("LANGUAGES")
langsArrAliases, namesArrAliases, mapLang2NameAliases, mapName2LangAliases = readLanguages("ALIASES")
langsArrConfigs, namesArrConfigs, mapLang2NameConfigs, mapName2LangConfigs = readLanguages("CONFIGS")

letterNamesMap = {}
aliasNamesMap = {}
for k in namesArrLetters:
    assert k not in letterNamesMap
    letterNamesMap[k] = True
for k in namesArrAliases:
    assert k not in aliasNamesMap
    aliasNamesMap[k] = True

grammars = readYaml(f"./others/grammars.yml")
snippets = readYaml(f"./others/snippets.yml")
features = readYaml(f"./others/features.yml")

langArray = []
for lang in langsArrConfigs:
    print("language:", lang)
    SUPPORT_LANG.add(lang)

langNames = mapName2LangConfigs['LANG_LANGUAGE_FULLNAME']
langTrans = mapName2LangConfigs['LANG_LANGUAGE_TRANSLATION']

############################################################################### source

BRIDGE_RGSTR_PATTERN = re.compile('lib_(.*)_register')
def _tryWalkLibraryHeader(libPath):
    headerPath = tools.tools.append_path(libPath, "header.h")
    _names = []
    _lines = readLines(headerPath)
    for _line in _lines:
        _line = _line.strip()
        if _line.startswith("//"):
            continue;
        match = BRIDGE_RGSTR_PATTERN.search(_line)
        if match:
            name = match.group(1).strip()
            _names.append(name)
        pass
    return _names

INTERNAL_MODULES = _tryWalkLibraryHeader(DIR_INTERNALS)
EXTERNAL_MODULES = _tryWalkLibraryHeader(DIR_EXTERNALS)

###############################################################################

def tryForEachLine(module, fromPath, func):
    if not tools.files.is_file(fromPath):
        return
    _lines = readLines(fromPath)
    lineIndex = -1
    for _line in _lines:
        lineIndex = lineIndex + 1
        func(lineIndex, _lines)
    pass

###############################################################################

# bind

BIND_AUTO_FUNC_PATTERN = re.compile('(.*)bind_([^(]*)\(([^)]*)\)')
BIND_MANU_FUNC_PATTERN = re.compile('(.*)native_([^(]*)\(([^)]*)\)')


BRIDGE_RTRN_PATTERN = re.compile('Bridge_return([^(]*)\(')
BRIDGE_ARGS_PATTERN = re.compile('(.*) = Bridge_receive.*\(')
BRIDGE_ARGS_CUSTOM_PATTENR = re.compile('(.*) = .*_from_bridge')

# ctype -> bridge type, return append
BINDING_MAP = {
    'void': ["Empty", ""],
    'int': ["Number", ""],
    'long': ["Number", ""],
    'float': ["Number", ""],
    'double': ["Number", ""],
    'bool': ["Boolean", ""],
    'CString': ["String", "pct_free(rrr);"],
}

class BindArgument:
    def __init__(self, vType, cType, name):
        self.vType: str = vType
        self.cType: str = cType
        self.name: str = name

class BindReturn:
    def __init__(self, vType, cType, fCode):
        self.vType: str = vType
        self.cType: str = cType
        self.fCode: str = fCode

class BindInfo:
    def __init__(self, func, desc, rtrn, args):
        self.func: str = func
        self.desc: str = desc
        self.rtrn: BindReturn = rtrn
        self.args: list[BindArgument] = args
        self.file: str = None
        self.line: int = None
        pass

def tryFindBindDescription(line):
    line = line.strip()
    if not line.startswith("//"):
        return
    return line[2:]

def _tryConvertBindFunction(module, text):
    text = text.strip()
    func = text.replace(module + "_", "").strip()
    return func

###############################################################################

# auto

def _tryConvertBindAutoArguments(text):
    text = text.strip()
    args = []
    if text == "":
        return args
    for _arg in text.split(","):
        assert "*" not in _arg, "invalid auto bind arg"
        _arg = _arg.strip()
        [typ, nam] = _arg.split(" ")
        info = BINDING_MAP.get(typ)
        assert info != None, "invalid auto bind type:" + text
        _typ = info[0]
        args.append(BindArgument(_typ, typ, nam))
    return args

def _tryConvertBindAutoReturn(text):
    rtrn = text.strip()
    assert "*" not in rtrn, "invalid auto bind arg"
    info = BINDING_MAP.get(rtrn)
    assert info != None, "invalid auto bind type:" + text
    _rtrn = info[0]
    return BindReturn(_rtrn, rtrn, info[1])

def tryDetectBindAutoFunction(path, module, index, lines):
    #
    line = lines[index]
    match = BIND_AUTO_FUNC_PATTERN.search(line)
    if not match:
        return
    desc = tryFindBindDescription(lines[index - 1])
    func = _tryConvertBindFunction(module, match.group(2))
    rtrn = _tryConvertBindAutoReturn(match.group(1))
    args = _tryConvertBindAutoArguments(match.group(3))
    info = BindInfo(func, desc, rtrn, args)
    info.file = module + ".c"
    info.line = index + 1
    return info

###############################################################################

# manu

def _tryConvertBindManuArgument(text):
    text = text.strip()
    if text == "":
        return None
    match = BRIDGE_ARGS_PATTERN.search(text)
    if not match:
        match = BRIDGE_ARGS_CUSTOM_PATTENR.search(text)
    
    if not match:
        return None
    arg = match.group(1).replace(" *", " ").strip()
    [typ, nam] = arg.split(" ")
    return BindArgument(typ, None, nam)
    

def _tryConvertBindManuReturn(line):
    match = BRIDGE_RTRN_PATTERN.search(line)
    if not match:
        return
    pass
    rtrn = match.group(1).strip()
    return BindReturn(rtrn, None, '')


def tryDetectBindManuFunction(path, module, index, lines):
    line = lines[index]
    match = BIND_MANU_FUNC_PATTERN.search(line)
    if not match:
        return
    # 
    desc = tryFindBindDescription(lines[index - 1])
    func = _tryConvertBindFunction(module, match.group(2))
    args = []
    rtrn = None
    for num in range(1,50):
        _index = index + num
        _line = lines[_index]
        if _line.strip().startswith("//"):
            continue
        _return = _tryConvertBindManuReturn(_line)
        if _return:
            rtrn = _return
            break
        _argument = _tryConvertBindManuArgument(_line)
        if _argument:
            args.append(_argument)
        pass
    #
    info = BindInfo(func, desc, rtrn, args)
    info.file = module + ".c"
    info.line = index + 1
    return info

###############################################################################

print("BASE_INITIALIZED!\n")
