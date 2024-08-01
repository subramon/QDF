-- generates the following
-- vsadd.c
-- vssub.c
-- vsmul.c
-- vsdiv.c

local plfile = require 'pl.file'
local plpath = require 'pl.path'
local gen_code = require 'RSUTILS/lua/gen_code'
local QDF_SRC_ROOT = assert(os.getenv("QDF_SRC_ROOT"))
local prefix = QDF_SRC_ROOT .. "/src/gen/"
--==================================================
local cat_src_file = prefix .. "src/gen6.c"
local cat_inc_file = prefix .. "inc/gen6.h"
print(cat_src_file)
print(cat_inc_file)
plfile.delete(cat_src_file)
plfile.delete(cat_inc_file)
local sfp = io.open(cat_src_file, "w")
local ifp = io.open(cat_inc_file, "w")
--==================================================
local ops = { "add", "sub", "mul", "div", }

for i, op in ipairs(ops) do 
  local subs = {}
  subs.tmpl = prefix .. "tmpl6.c.lua"
  subs.fn = "vs" .. op
  subs.ispcfn = "ispc_" .. "vs" .. op
  -- Integer division is bad for ISPC. We are being overly restrictive below
  -- We should allow ISPC for qtypes = { "F4", "F8", }, op = {"div"}
  if ( op == "div" ) then 
    subs.ispcfn = subs.fn
  end
  subs.incdir = prefix .. "inc/"
  subs.srcdir = prefix .. "src/"
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
