-- generates ISPC code for the following
-- vvadd.c
-- vvsub.c
-- vvmul.c
-- vvdiv.c


local plfile = require 'pl.file'
local plpath = require 'pl.path'
local qtype_to_ispctype = require 'qtype_to_ispctype'
local qtype_to_ctype = require 'qtype_to_ctype'

local gen_code = require 'RSUTILS/lua/gen_code'
local QDF_SRC_ROOT = assert(os.getenv("QDF_SRC_ROOT"))
local prefix = QDF_SRC_ROOT .. "/src/gen/"
local ops = { "add", "sub", "mul", "div", }
--==================================================
local cat_src_file = prefix .. "ispc/gen1d.ispc"
local cat_inc_file = prefix .. "ispc/gen1d.isph"
plfile.delete(cat_src_file)
plfile.delete(cat_inc_file)
local sfp = io.open(cat_src_file, "w")
--==================================================

local operator = {}
operator.add = " + "
operator.sub = " - "
operator.mul = " * "
operator.div = " / "

for i, op in ipairs(ops) do 
  local qtypes 
  if ( op == "div" ) then 
    qtypes = { "F4", "F8" }
  else
    qtypes = { "I1", "I2", "I4", "I8", "F4", "F8" }
  end
  for j, qtype in ipairs(qtypes) do 
    local subs = {}
    subs.ispctype = qtype_to_ispctype(qtype) 
    subs.ctype = qtype_to_ctype(qtype) 
    subs.tmpl = prefix .. "tmpl1d.ispc.lua"
    subs.operator = operator[op]
    subs.fn = "ispc_" .. "vv" .. op .. "_" .. qtype
    subs.srcdir = prefix .. "ispc/"
    subs.incdir = prefix .. "ispc/"
    local src_file = gen_code.dotc(subs, subs.srcdir)
    local inc_file = gen_code.doth(subs, subs.incdir)
    print("Created " .. inc_file)
    --===============================================
    local src_str = plfile.read(src_file)
    sfp:write(src_str)
    -- IMOPRTANT plfile.delete(inc_file)
    plfile.delete(src_file)
    --===============================================
  end
end
sfp:close()
