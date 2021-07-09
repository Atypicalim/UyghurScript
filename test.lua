--[[
    test
]]

-- execute script
-- os.execute("lua uyghur.lua example.ug false")

-- compile script
-- os.execute("lua uyghur.lua example.ug true")

-- run module
-- local uyghur = require("uyghur")
-- uyghur.register("chongniTepish", "math.max")
-- uyghur.execute("example.ug")

-- test
local uyghur = require("uyghur")
uyghur.execute("example.ug")
