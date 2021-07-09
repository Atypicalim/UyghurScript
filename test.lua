--[[
    test
]]

-- execute script
-- os.execute("lua uyghur.lua example.ug false")

-- compile script
-- os.execute("lua uyghur.lua example.ug true")

-- run module
-- local uyghur = require("uyghur")
-- uyghur.import("chongniTepish", "math.max")
-- uyghur.export("sanQushush", "addNumber")
-- uyghur.execute("example.ug")

-- test
local uyghur = require("uyghur")
uyghur.execute("example.ug")
