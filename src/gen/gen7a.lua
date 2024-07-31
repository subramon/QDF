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
local cat_src_file = prefix .. "src/gen7a.c"
local cat_inc_file = prefix .. "inc/gen7a.h"
print(cat_src_file)
print(cat_inc_file)
plfile.delete(cat_src_file)
plfile.delete(cat_inc_file)
local sfp = io.open(cat_src_file, "w")
local ifp = io.open(cat_inc_file, "w")
--==================================================

local comparators     = { ">", "<", }
local operator_names  = { "min", "max", }
assert(#comparators == #operator_names)
local qtypes = { "I1", "I2", "I4", "I8", "F4", "F8", }
local subs = {}
subs.tmpl = prefix .. "tmpl7a.c.lua"
subs.incdir = prefix .. "inc/"
subs.srcdir = prefix .. "src/"
for j, op in ipairs(operator_names) do 
  for i, qtype in ipairs(qtypes) do 
    name            = operator_names[j]
    subs.comparator = comparators[j]
    subs.qtype = qtype
    subs.fn = "vs" .. name .. "_" .. subs.qtype
    --=================================
    subs.ctype = assert(qtype_to_ctype(subs.qtype))
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
