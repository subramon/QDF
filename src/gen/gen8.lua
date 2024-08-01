-- generates the following
-- vs${operator}_${qtype}.c
-- e.g., vsadd_I4.c 

local plfile = require 'pl.file'
local plpath = require 'pl.path'
local qtype_to_ctype = require 'qtype_to_ctype'
local gen_code       = require 'RSUTILS/lua/gen_code'
local QDF_SRC_ROOT = assert(os.getenv("QDF_SRC_ROOT"))
local prefix = QDF_SRC_ROOT .. "/src/gen/"
local cat_src_file = prefix .. "src/gen8.c"
local cat_inc_file = prefix .. "inc/gen8.h"
local lua_inc_file = prefix .. "inc/gen8_lua.h"
print(cat_src_file)
print(cat_inc_file)
plfile.delete(cat_src_file)
plfile.delete(cat_inc_file)

local sfp = io.open(cat_src_file, "w")
local ifp = io.open(cat_inc_file, "w")
local subs = {}
subs.tmpl   = prefix .. "tmpl8.c.lua"
subs.incdir = prefix .. "inc/"
subs.srcdir = prefix .. "src/"
local S = {}
local I = {}
local qtypes = { "I1", "I2", "I4", "I8", "F4", "F8", }
for i, qtype1 in ipairs(qtypes) do 
  for j, qtype2 in ipairs(qtypes) do 
    subs.fn = "mk_sort_idx_" .. qtype1 .. "_" .. qtype2
    subs.qtype1 = qtype1
    subs.qtype2 = qtype2
    subs.ctype1 = assert(qtype_to_ctype(subs.qtype1))
    subs.ctype2 = assert(qtype_to_ctype(subs.qtype2))
    local inc_file = gen_code.doth(subs, subs.incdir)
    local src_file = gen_code.dotc(subs, subs.srcdir)
    I[#I+1] = incfile
    S[#S+1] = srcfile
    --===============================================
    local inc_str = plfile.read(inc_file)
    local src_str = plfile.read(src_file)
    ifp:write(inc_str)
    sfp:write(src_str)
    -- IMPORTANT plfile.delete(inc_file)
    plfile.delete(src_file)
    --===============================================
  end
end
ifp:close()
sfp:close()
-- create an include file for cdef 
local ifp = io.open(cat_inc_file, "r")
local ofp = io.open(lua_inc_file, "w")
local write = false
while true do 
  local str = ifp:read()
  if ( not str ) then break end 
  -- start writing when you see an a line starting with extern 
  local m, n = string.find(str, "extern") 
  if ( m == 1 ) then 
    write = true
  end
  -- stopt writing when you see an a line starting with extern 
  if ( write == true ) then 
    ofp:write(str); ofp:write("\n")
    local m, n = string.find(str, ");") 
    if ( m ) then 
      write = false
    end
  end
end
