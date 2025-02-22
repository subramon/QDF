-- generates the following
-- seq_I1.c, incr_on_by_I1.c 
-- seq_I2.c, incr_on_by_I2.c 
-- seq_I4.c, incr_on_by_I4.c 
-- seq_I8.c, incr_on_by_I8.c 
-- seq_F4.c, incr_on_by_F4.c 
-- seq_F8.c, incr_on_by_F8.c 

local cutils = require 'libcutils'
local qtype_to_ctype = require 'qtype_to_ctype'
local gen_code = require 'RSUTILS/lua/gen_code'
local QDF_SRC_ROOT = assert(os.getenv("QDF_SRC_ROOT"))
local prefix = QDF_SRC_ROOT .. "/src/gen/"
--==================================================
local cat_src_file = prefix .. "src/gen4.c"
local cat_inc_file = prefix .. "inc/gen4.h"
print(cat_src_file)
print(cat_inc_file)
cutils.delete(cat_src_file)
cutils.delete(cat_inc_file)
local sfp = io.open(cat_src_file, "w")
local ifp = io.open(cat_inc_file, "w")
--==================================================
local operators = { "seq", "incr_on_by",  "is_sawtooth", }
local qtypes = { "I1", "I2", "I4", "I8", "F4", "F8", }

for _, operator in ipairs(operators) do 
  for i, qtype in ipairs(qtypes) do 
    local subs = {}
    subs.tmpl = prefix .. "tmpl4_" .. operator .. ".c.lua"
    assert(cutils.isfile(subs.tmpl))
    subs.fn = operator .. "_" .. qtype
    subs.qtype = qtype
    subs.ctype = assert(qtype_to_ctype(qtype))
    subs.incdir = prefix .. "inc/"
    subs.srcdir = prefix .. "src/"
    local inc_file = gen_code.doth(subs, subs.incdir)
    local src_file = gen_code.dotc(subs, subs.srcdir)
      --===============================================
    local inc_str = cutils.file_as_str(inc_file)
    local src_str = cutils.file_as_str(src_file)
    ifp:write(inc_str)
    sfp:write(src_str)
    -- IMPORTANT cutils.delete(inc_file)
    cutils.delete(src_file)
    --===============================================
  end
end
ifp:close()
sfp:close()
