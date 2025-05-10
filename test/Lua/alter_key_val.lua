-- luajit alter_key_val.lua regular_price x.qdf z.qdf
local lQDF = require 'lQDF'
local cutils = require 'libcutils'

assert(type(arg) == "table")
local key = assert(arg[1])
local qdf_file = assert(arg[2])
-- make a copy of qdf_File
local bak_qdf_file = cutils.mkstemp("/tmp/qdf_XXXXXX")
cutils.copyfile(qdf_file, bak_qdf_file)
local new_data_file = assert(arg[3])

assert(type(key) == "string")
assert(cutils.isfile(qdf_file))
assert(cutils.isfile(new_data_file))

local x = lQDF.binld(qdf_file, true)
assert(x:check())
assert(x:is_data_frame())
local K = x:keys()
local found = false
for k, v in pairs(K) do 
  if ( v == key ) then found = true; break end
end
assert(found, "Key not found in dataframe " .. key)
--
-- START: load a vector from CSV file 
local M = {}
M[#M+1] = { name =  "regular_price", qtype = "F4", 
    is_load = true, has_nulls = false, }
local infile    = new_data_file
local optargs   = { is_hdr = true }

local tmp = assert(lQDF.read_csv(M, infile, optargs))
assert(type(tmp) == "lQDF")
assert(tmp:jtype() == "j_object")
local new_vec = tmp:get("regular_price")
assert(new_vec:jtype() == "j_array")
assert(new_vec:qtype() == "F4")
assert(new_vec:num_elements() == x:obj_arr_len())
-- STOP : load a vector from CSV file 

new_vec:pr_csv()
-- over-write QDF with new data 
x:place_in_data_frame(key, new_vec)
assert(x:check())
local y = x:get(key)
-- y:pr_csv()
-- restore data 
-- cutils.copyfile(bak_qdf_file, qdf_file)
-- cutils.unlink(bak_qdf_file)

print("Completed")
