-- generates the following
-- vsbitwise_and.c
-- vsbitwise_or.c
-- vsbitwise_xor.c
-- vslogical_and.c
-- vslogical_or.c
-- vsshift_left.c
-- vsshift_right.c

local plfile = require 'pl.file'
local plpath = require 'pl.path'
local gen_code = require 'RSUTILS/lua/gen_code'
local QDF_SRC_ROOT = assert(os.getenv("QDF_SRC_ROOT"))
local prefix = QDF_SRC_ROOT .. "/src/gen/"
--==================================================
local cat_src_file = prefix .. "src/gen6a.c"
local cat_inc_file = prefix .. "inc/gen6a.h"
print(cat_src_file)
print(cat_inc_file)
plfile.delete(cat_src_file)
plfile.delete(cat_inc_file)
local sfp = io.open(cat_src_file, "w")
local ifp = io.open(cat_inc_file, "w")
--==================================================
local ops = { 
"bitwise_and", 
"bitwise_or", 
"bitwise_xor", 
"logical_and", 
"logical_or", 
"shift_left", 
"shift_right", 
}

for i, op in ipairs(ops) do 
  local subs = {}
  subs.tmpl = prefix .. "tmpl6a.c.lua"
  subs.fn = "vs" .. op
  subs.incdir = prefix .. "inc/"
  subs.srcdir = prefix .. "src/"
  subs.U = "u"
  local inc_file = gen_code.doth(subs, subs.incdir)
  local src_file = gen_code.dotc(subs, subs.srcdir)
  --===============================================
  local inc_str = plfile.read(inc_file)
  local src_str = plfile.read(src_file)
  ifp:write(inc_str)
  sfp:write(src_str)
  -- IMPORTANT plfile.delete(inc_file)
  plfile.delete(src_file)
  --===============================================
end
ifp:close()
sfp:close()
