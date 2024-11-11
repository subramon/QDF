-- generates C code for the following
-- vv${op}_${qtype.c
-- where op in  { "add", "sub", "mul", "div", }
-- where qtype in { "I1", "I2", "I4", "I8", "F4", "F8" }

local cutils = require 'libcutils'
local qtype_to_ctype = require 'qtype_to_ctype'

local gen_code = require 'RSUTILS/lua/gen_code'
local QDF_SRC_ROOT = assert(os.getenv("QDF_SRC_ROOT"))
local prefix = QDF_SRC_ROOT .. "/src/gen/"
local ops = { "add", "sub", "mul", "div", }
--==================================================
local cat_src_file = prefix .. "src/gen1d.c"
local cat_inc_file = prefix .. "src/gen1d.h"
cutils.delete(cat_src_file)
cutils.delete(cat_inc_file)
local sfp = io.open(cat_src_file, "w")
--==================================================

local operator = {}
operator.add = " + "
operator.sub = " - "
operator.mul = " * "
operator.div = " / "

local qtypes = { "I1", "I2", "I4", "I8", "F4", "F8" }
for i, op in ipairs(ops) do 
  for j, qtype in ipairs(qtypes) do 
    local subs = {}
    subs.ctype = qtype_to_ctype(qtype) 
    subs.tmpl = prefix .. "tmpl1d.c.lua"
    subs.operator = operator[op]
    subs.fn = "vv" .. op .. "_" .. qtype
    subs.srcdir = prefix .. "src/"
    subs.incdir = prefix .. "inc/"
    local src_file = gen_code.dotc(subs, subs.srcdir)
    local inc_file = gen_code.doth(subs, subs.incdir)
    print("Created " .. inc_file)
    --===============================================
    local src_str = cutils.file_as_str(src_file)
    sfp:write(src_str)
    -- IMOPRTANT cutils.delete(inc_file)
    cutils.delete(src_file)
    --===============================================
  end
end
sfp:close()
