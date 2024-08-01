-- generates the following
-- vvrem.c
-- vvbitwise_and.c
-- vvbitwise_or.c
-- vvbitwise_xor.c

local plfile = require 'pl.file'
local plpath = require 'pl.path'
local gen_code = require 'RSUTILS/lua/gen_code'
local QDF_SRC_ROOT = assert(os.getenv("QDF_SRC_ROOT"))
local prefix = QDF_SRC_ROOT .. "/src/gen/"
--==================================================
local cat_src_file = prefix .. "src/gen1a.c"
local cat_inc_file = prefix .. "inc/gen1a.h"
print(cat_src_file)
print(cat_inc_file)
plfile.delete(cat_src_file)
plfile.delete(cat_inc_file)
local sfp = io.open(cat_src_file, "w")
local ifp = io.open(cat_inc_file, "w")
--==================================================
local ops = { "rem", "bitwise_and", "bitwise_or", "bitwise_xor", }
local operator = {}
operator.rem         = " % "
operator.bitwise_and = " & "
operator.bitwise_or  = " | "
operator.bitwise_xor = " ^ "

for i, op in ipairs(ops) do 
  local subs = {}
  subs.tmpl = prefix .. "tmpl1a.c.lua"
  subs.operator = operator[op]
  subs.fn = "vv" .. op
  subs.incdir = prefix .. "inc/"
  subs.srcdir = prefix .. "src/"
  local inc_file = gen_code.doth(subs, subs.incdir)
  local src_file = gen_code.dotc(subs, subs.srcdir)
  --===============================================
  local inc_str = plfile.read(inc_file)
  local src_str = plfile.read(src_file)
  ifp:write(inc_str)
  sfp:write(src_str)
  -- IMPORTANT: plfile.delete(inc_file)
  plfile.delete(src_file)
  --===============================================
end
ifp:close()
sfp:close()
