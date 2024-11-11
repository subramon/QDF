-- generates the following
-- vs${operator}_${qtype}.c
-- e.g., vsadd_I4.c 

local plfile = require 'pl.file'
local plpath = require 'pl.path'
local qtype_to_ctype = require 'qtype_to_ctype'
local unsigned_qtype = require 'unsigned_qtype'
local is_in          = require 'RSUTILS/lua/is_in'
local gen_code       = require 'RSUTILS/lua/gen_code'
local QDF_SRC_ROOT = assert(os.getenv("QDF_SRC_ROOT"))
local prefix = QDF_SRC_ROOT .. "/src/gen/"
--==================================================
local cat_src_file = prefix .. "src/gen7b.c"
local cat_inc_file = prefix .. "inc/gen7b.h"
print(cat_src_file)
print(cat_inc_file)
cutils.delete(cat_src_file)
cutils.delete(cat_inc_file)
local sfp = io.open(cat_src_file, "w")
local ifp = io.open(cat_inc_file, "w")
--==================================================

local operators = { 
  "==", "!=", "<", "<=", ">", ">=", 
  "%", 
  "&", "|", "^", 
  "&&", "||", 
  "<<", ">>", 
}
local operator_names  = { 
  "eq", "neq", "lt", "leq", "gt", "geq", 
  "rem", 
  "bitwise_and", "bitwise_or", "bitwise_xor", 
  "logical_and", "logical_or", 
  "shift_left", "shift_right", 
}
assert(#operators == #operator_names)

local logical_ops = { "eq", "neq", "lt", "leq", "gt", "geq", }
local integer_ops = { 
  "rem", 
  "bitwise_and", "bitwise_or", "bitwise_xor", 
  "logical_and", "logical_or", 
  "shift_left", "shift_right", 
}
local shift_ops = { 
  "shift_left", "shift_right", 
}
local bitwise_ops = {
  "bitwise_and", "bitwise_or", "bitwise_xor", 
  "logical_and", "logical_or", 
  "shift_left", "shift_right", 
}
local subs = {}
subs.tmpl = prefix .. "tmpl7b.c.lua"
subs.incdir = prefix .. "inc/"
subs.srcdir = prefix .. "src/"
for j, op in ipairs(operator_names) do 
  local qtypes
  if ( is_in(op, integer_ops) ) then 
    qtypes = { "I1", "I2", "I4", "I8", "UI1", "UI2", "UI4",  "UI8", }
  else
    qtypes = { "I1", "I2", "I4", "I8", "UI1", "UI2", "UI4",  "UI8", "F4", "F8", }
  end
  if ( ( op == "eq" ) or ( op == "neq" ) ) then 
    qtypes = { "I1", "I2", "I4", "I8", "UI1", "UI2", "UI4",  "UI8", "F4", "F8", }
  end
  for i, qtype in ipairs(qtypes) do 
    name          = operator_names[j]
    subs.operator = operators[j]
    subs.fn = "vs" .. name .. "_" .. qtype
    print(subs.fn)
    --=================================
    if ( is_in(name, bitwise_ops ) ) then 
      subs.xqtype = unsigned_qtype(qtype)
    else
      subs.xqtype = qtype
    end
    --=================================
    if ( is_in(name, logical_ops ) ) then 
      subs.yqtype = "I1"
    else
      subs.yqtype = subs.xqtype
    end
    --=================================
    subs.xctype = assert(qtype_to_ctype(subs.xqtype))
    subs.yctype = assert(qtype_to_ctype(subs.yqtype))
    -- print(op, name, qtype, subs.xqtype, subs.yqtype)
    local inc_file = gen_code.doth(subs, subs.incdir)
    local src_file = gen_code.dotc(subs, subs.srcdir)
    --===============================================
    local inc_str = cutils.file_as_str(inc_file)
    local src_str = cutils.file_as_str(src_file)
    ifp:write(inc_str)
    sfp:write(src_str)
    -- IMPORTANT: cutils.delete(inc_file)
    cutils.delete(src_file)
    --===============================================
  end
end
