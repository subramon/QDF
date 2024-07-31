-- generates the following
-- vs${operator}_${qtype}.c
-- e.g., vsadd_I4.c 

local plfile = require 'pl.file'
local plpath = require 'pl.path'
local qtype_to_ctype = require 'qtype_to_ctype'
local unsigned_qtype = require 'unsigned_qtype'
local is_in          = require 'RSUTILS/lua/is_in'
local gen_code       = require 'RSUTILS/lua/gen_code'
local QDF_ROOT = assert(os.getenv("QDF_ROOT"))
local prefix = QDF_ROOT .. "/src/gen/"
--==================================================
local cat_src_file = prefix .. "src/gen7.c"
local cat_inc_file = prefix .. "inc/gen7.h"
print(cat_src_file)
print(cat_inc_file)
plfile.delete(cat_src_file)
plfile.delete(cat_inc_file)
local sfp = io.open(cat_src_file, "w")
local ifp = io.open(cat_inc_file, "w")
--==================================================

local qtypes = { "I1", "I2", "I4", "I8", "F4", "F8", }
local operators = { 
  "+", "-", "*", "/", 
}
local operator_names  = { 
  "add", "sub", "mul", "div", 
}
assert(#operators == #operator_names)

local subs = {}
subs.tmpl = prefix .. "tmpl7.c.lua"
subs.incdir = prefix .. "inc/"
subs.srcdir = prefix .. "src/"
for j, op in ipairs(operator_names) do 
  for i, qtype in ipairs(qtypes) do 
    name          = operator_names[j]
    subs.operator = operators[j]
    subs.fn = "vs" .. name .. "_" .. qtype
    subs.xqtype = qtype
    subs.yqtype = subs.xqtype
    subs.yqtype = subs.xqtype
    --=================================
    subs.xctype = assert(qtype_to_ctype(subs.xqtype))
    subs.yctype = assert(qtype_to_ctype(subs.yqtype))
    --=================================
    -- print(op, name, qtype, subs.xqtype, subs.yqtype)
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
end
