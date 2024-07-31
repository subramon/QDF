G = {} --- to overcome strict 
local plfile = require 'pl.file'
local plpath = require 'pl.path'
-- TODO P1 require 'strict'
local lQDF = require 'lQDF'
assert(arg)
local infile  = assert(arg[1])
-- local outfile = assert(arg[2])
-- assert(infile ~= outfile)
--=========================================
local x = lQDF.binld(infile)
assert(x:check())
print(x)
-- local str = tostring(x)
-- plfile.write(str, outfile)
