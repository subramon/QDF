-- generates the following
-- vvadd.c
-- vvsub.c
-- vvmul.c
-- vvdiv.c


local plfile = require 'pl.file'
local plpath = require 'pl.path'

local gen_code = require 'Q/UTILS/lua/gen_code'
local QDF_ROOT = assert(os.getenv("QDF_ROOT"))
local prefix = QDF_ROOT .. "/src/gen/"
local ops = { "add", "sub", "mul", "div", }
--==================================================
local cat_src_file = prefix .. "src/gen1.c"
local cat_inc_file = prefix .. "inc/gen1.h"
plfile.delete(cat_src_file)
plfile.delete(cat_inc_file)
local sfp = io.open(cat_src_file, "w")
local ifp = io.open(cat_inc_file, "w")
--==================================================

local operator = {}
operator.add = " + "
operator.sub = " - "
operator.mul = " * "
operator.div = " / "

for i, op in ipairs(ops) do 
  local subs = {}
  subs.tmpl = prefix .. "tmpl1.c.lua"
  subs.operator = operator[op]
  subs.fn = "vv" .. op
  subs.ispcIfn = "ispc_" .. "vv" .. op
  subs.ispcFfn = "ispc_" .. "vv" .. op
  subs.exclude_I = ""
  subs.exclude_F = ""
  if ( op == "div" ) then 
    subs.ispcIfn = subs.fn
    subs.exclude_I = "// "
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
  -- IMOPRTANT plfile.delete(inc_file)
  plfile.delete(src_file)
  --===============================================
end
ifp:close()
sfp:close()
