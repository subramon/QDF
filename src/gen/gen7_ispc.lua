-- generates the following
-- vs${operator}_${qtype}.c
-- e.g., vsadd_I4.c 

local cutils = require 'libcutils'
local qtype_to_ispctype = require 'qtype_to_ispctype'
local qtype_to_ctype = require 'qtype_to_ctype'
local gen_code       = require 'RSUTILS/lua/gen_code'
local QDF_SRC_ROOT = assert(os.getenv("QDF_SRC_ROOT"))
local prefix = QDF_SRC_ROOT .. "/src/gen/"
--==================================================
local cat_src_file = prefix .. "ispc/gen7.ispc"
print(cat_src_file)
cutils.delete(cat_src_file)
local sfp = io.open(cat_src_file, "w")
--==================================================

local qtypes = { "I1", "I2", "I4", "I8", "F4", "F8", }
local operators = { 
  "+", "-", "*", "/", 
}
local operator_names  = { 
  "add", "sub", "mul", "div", 
}
assert(#operators == #operator_names)

for j, op in ipairs(operator_names) do 
  local qtypes
  if ( op == "div" ) then 
    qtypes = { "F4", "F8", }
  else
    qtypes = { "I1", "I2", "I4", "I8", "F4", "F8", }
  end
  for i, qtype in ipairs(qtypes) do 
    local subs = {}
    subs.tmpl = prefix .. "tmpl7.ispc.lua"
    subs.srcdir = prefix .. "ispc/"
    subs.incdir = prefix .. "ispc/"
    name          = operator_names[j]
    subs.operator = operators[j]
    subs.fn = "ispc_" .. "vs" .. name .. "_" .. qtype
    subs.qtype = qtype
    --=================================
    subs.ctype = assert(qtype_to_ctype(subs.qtype))
    subs.ispctype = assert(qtype_to_ispctype(subs.qtype))
    --=================================
    -- print(op, name, qtype, subs.xqtype, subs.yqtype)
    local src_file = gen_code.dotc(subs, subs.srcdir)
    local inc_file = gen_code.doth(subs, subs.incdir)
    --===============================================
    local src_str = cutils.file_as_str(src_file)
    sfp:write(src_str)
    cutils.delete(src_file)
    --===============================================
  end
end
