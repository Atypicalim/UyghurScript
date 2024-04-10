
-- pcall(os.execute, "git clone git@github.com:kompasim/my-build-tools.git ./my-build-tools")
package.path = package.path .. ";./../my-build-tools/?.lua"
local builder = require("builder")

local SCRIPT_PATH = "./examples/yuguresh.ug"

local DST_DIR = "./build/"
local DST_NAME = "uyghur"
local DST_ALIAS = DST_DIR .. DST_NAME
local DST_SCRIPT = DST_ALIAS .. ".ug"

files.mk_folder(DST_DIR)
files.delete(DST_SCRIPT)
if SCRIPT_PATH ~= nil and files.is_file(SCRIPT_PATH) then
    files.copy(SCRIPT_PATH, DST_SCRIPT)
else
    local help = " # yardem \n"
    local help = help .. "ikrangha [merhaba, uyghur script qa xosh kepsiz ...] yezilsun\n"
    files.write(DST_SCRIPT, help)
end

local builder = builder.c {}
builder:setDebug(false)
builder:setInput('./main.c')
builder:setLibs("incbin", "raylib", "raygui")
builder:setOutput(DST_ALIAS)
builder:setIcon('./resources/icon.ico')
builder:start(false)
builder:run()
