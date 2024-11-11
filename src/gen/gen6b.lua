-- generates the following
-- vseq.c
-- vsneq.c
-- vslt.c
-- vsleq.c
-- vsgt.c
-- vsgeq.c

local cutils = require 'libcutils'
local gen_code = require 'RSUTILS/lua/gen_code'
local QDF_SRC_ROOT = assert(os.getenv("QDF_SRC_ROOT"))
local prefix = QDF_SRC_ROOT .. "/src/gen/"
--==================================================
local cat_src_file = prefix .. "src/gen6b.c"
local cat_inc_file = prefix .. "inc/gen6b.h"
print(cat_src_file)
print(cat_inc_file)
cutils.delete(cat_src_file)
cutils.delete(cat_inc_file)
local sfp = io.open(cat_src_file, "w")
local ifp = io.open(cat_inc_file, "w")
--==================================================
local ops = { "eq", "neq", "lt", "leq", "gt", "geq", }

for i, op in ipairs(ops) do 
  local subs = {}
  subs.tmpl = prefix .. "tmpl6b.c.lua"
  subs.fn = "vs" .. op
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
ifp:close()
sfp:close()
