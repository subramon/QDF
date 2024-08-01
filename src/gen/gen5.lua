-- generates the following
-- const_I1.c
-- const_I2.c
-- const_I4.c
-- const_I8.c
-- const_F4.c
-- const_F8.c

local plfile = require 'pl.file'
local plpath = require 'pl.path'
local qtype_to_ctype = require 'qtype_to_ctype'
local gen_code = require 'RSUTILS/lua/gen_code'
local QDF_SRC_ROOT = assert(os.getenv("QDF_SRC_ROOT"))
local prefix = QDF_SRC_ROOT .. "/src/gen/"
--==================================================
local cat_src_file = prefix .. "src/gen5.c"
local cat_inc_file = prefix .. "inc/gen5.h"
print(cat_src_file)
print(cat_inc_file)
plfile.delete(cat_src_file)
plfile.delete(cat_inc_file)
local sfp = io.open(cat_src_file, "w")
local ifp = io.open(cat_inc_file, "w")
--==================================================
local qtypes = { "I1", "I2", "I4", "I8", "F4", "F8", }

for i, qtype in ipairs(qtypes) do 
  local subs = {}
  subs.tmpl = prefix .. "tmpl5.c.lua"
  subs.fn = "const_" .. qtype
  subs.qtype = qtype
  subs.ctype = assert(qtype_to_ctype(qtype))
  subs.incdir = prefix .. "inc/"
  subs.srcdir = prefix .. "src/"
  local inc_file = gen_code.doth(subs, subs.incdir)
  local src_file = gen_code.dotc(subs, subs.srcdir)
  --===============================================
  local inc_str = plfile.read(inc_file)
  local src_str = plfile.read(src_file)
  ifp:write(inc_str)
  sfp:write(src_str)
  -- IMPORTANT plfile.delete(inc_file)
  plfile.delete(src_file)
  --===============================================
end
ifp:close()
sfp:close()
