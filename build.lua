
-- pcall(os.execute, "git clone git@github.com:kompasim/my-build-tools.git ./my-build-tools")
package.path = package.path .. ";./../my-build-tools/?.lua"
local Builder = require("c_builder")

local DST_DIR = "./build/"
local DST_NAME = "uyghur"
local DST_PROGRAM = DST_DIR .. DST_NAME
local TEMP_PATH = "./build/script.ug"
local HEAD_PATH = "./build/script.h"
local SCRIPT_PATH = "./examples/sinaq.ug"
local IS_INTEGRATE = true

files.mk_folder(DST_DIR)
files.delete(TEMP_PATH)
files.delete(HEAD_PATH)
if IS_INTEGRATE then
    assert(files.is_file(SCRIPT_PATH), 'script not found!')
    files.copy(SCRIPT_PATH, TEMP_PATH)
else
    local help = " # yardem \n"
    local help = help .. "ikrangha [merhaba, uyghur script qa xosh kepsiz ...] yezilsun\n"
    files.write(TEMP_PATH, help)
end
os.execute(string.format('echo # uyghurche >> %s', TEMP_PATH))
os.execute(string.format('xxd -i %s > %s', TEMP_PATH, HEAD_PATH))
os.execute(string.format('echo #define UG_SCRIPT_CODE >> %s', HEAD_PATH))
assert(files.is_file(HEAD_PATH), 'header not found!')

local builder = Builder(false)
builder:setInput('./main.c')
builder:setLibs("raylib")
builder:setLibs("raygui")
builder:setOutput(DST_PROGRAM)
builder:setIcon('./resources/icon.ico')
builder:start(false)

if tools.is_windows() then
    os.execute(string.format([[cd %s & uyghur.exe]], DST_DIR, DST_NAME))
else
    builder:run()
end
