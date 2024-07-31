-- TODO ADD a test for old_tcin_to_cluster
-- TODO ADD a test for new_tcin_to_cluster
local plpath = require 'pl.path'
local plfile = require 'pl.file'
local ffi    = require 'ffi'
-- Make .so file 
local rootdir = assert(os.getenv("LIGHTR_ROOT"))
assert(plpath.isdir(rootdir))
-- This cmd is terrible. Assumes rclient/ checked out at same level as RBC
local cmd = [[
cd %s/src/;
gcc -g -std=gnu99 -fPIC -shared ../utils/src/split_str.c extract_name_value.c aux_R_online.c -I../../rclient/rincs/ -I../../rclient/inc/ -I../inc/ -I../utils/inc/ -o libaux_R_online.so;
mv libaux_R_online.so %s/test/lua/;
cd -
]]
cmd = string.format(cmd, rootdir, rootdir)
print(cmd)
os.execute(cmd)
assert(plpath.isfile("libaux_R_online.so"))
--===================================
-- Set up for Lua to call C 
local ccode = ffi.load("aux_R_online.so")
-- TODO Unfortunately, this cdef is brittle. 
ffi.cdef([[
extern int 
get_tcin(
    const char * const spec,
    uint32_t *ptr_tcin
    );
extern int 
get_tcins(
    const char * const spec,
    uint32_t **ptr_tcins,
    uint32_t *ptr_n_tcins
    );
]]
)

--===================================
local tests = {}
tests.get_tcin = function() 
  print("Test get_tcin starting")
  -- positive test 
  local cfn = assert(ccode["get_tcin"])
  local spec = "TCIN=12345678"
  local tcin = ffi.new("uint32_t[?]", 1)
  local status = cfn(spec, tcin)
  assert(status == 0)
  assert(tcin[0] == 12345678)
  print("Test get_tcin positive case passed")
  -- negative test 
  local spec = "tcins=12345678"
  local tcin = ffi.new("uint32_t[?]", 1)
  local status = cfn(spec, tcin)
  assert(status ~= 0)
  print("Test get_tcin negative case passed")
  print("Test get_tcin completed successfully")
end
tests.get_tcins = function() 
  print("Test get_tcins starting")
  -- positive test 
  local cfn = assert(ccode["get_tcins"])
  local spec = "Similar=1234:5678:9012"
  local tcins = ffi.new("uint32_t *[?]", 1)
  local n_tcins = ffi.new("uint32_t[?]", 1)
  local status = cfn(spec, tcins, n_tcins)
  assert(status == 0)
  assert(n_tcins[0] == 3)
  assert(tcins[0][0] == 1234)
  assert(tcins[0][1] == 5678)
  assert(tcins[0][2] == 9012)
  print("Test get_tcins completed successfully")
end
tests.get_change_col_val = function() 
  print("Test get_tcins starting")
  -- positive test 
  local cfn = assert(ccode["get_change_col_val"])
  local spec = "Change=Baseprice:12345"
  local change_col = ffi.new("char *[?]", 1)
  local change_val = ffi.new("char *[?]", 1)
  local status = cfn(spec, change_col, change_val)
  assert(status == 0)
  assert(ffi.string(change_col[0]) == "Baseprice")
  assert(ffi.string(change_val[0]) == "12345")
  print("Test get_change_col_val completed successfully")
end
tests.get_tcin()
tests.get_tcins()
tests.get_change_col_val()
-- return tests

