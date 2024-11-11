-- generates the following
-- vvadd.c
-- vvsub.c
-- vvmul.c
-- vvdiv.c

local cutils = require 'libcutils'

local gen_code = require 'RSUTILS/lua/gen_code'
local QDF_SRC_ROOT = assert(os.getenv("QDF_SRC_ROOT"))
local prefix = QDF_SRC_ROOT .. "/src/gen/"
local ops = { "add", "sub", "mul", "div", }
--==================================================
local cat_src_file = prefix .. "src/gen1.c"
local cat_inc_file = prefix .. "inc/gen1.h"
cutils.delete(cat_src_file)
cutils.delete(cat_inc_file)
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
  local inc_str = cutils.file_as_str(inc_file)
  local src_str = cutils.file_as_str(src_file)
  ifp:write(inc_str)
  sfp:write(src_str)
  -- IMOPRTANT cutils.delete(inc_file)
  cutils.delete(src_file)
  --===============================================
end
ifp:close()
sfp:close()
