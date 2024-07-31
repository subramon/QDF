G = {} -- to circumvent strict 
G.debug= true
require 'strict'
require 'strict'
local compress_modes = require 'compress_modes'
local get_rbc_rec    = require 'get_rbc_rec'
local lRBC = require 'lRBC'
local cmem = require 'libcmem'
local ffi  = require 'ffi'

local tests = {}

tests.compress = function ()
  local col_names =  { "x", "y", }
  local qtypes = { "F4", "F4", }
  local infile = "data_compress.csv"
  local optargs = { is_hdr = true }
  local df = assert(lRBC.read_csv(col_names, qtypes, infile, optargs))
  assert(type(df) == "lRBC")
  --====== Check for first kind of compression
  local x = df:get("x")
  local outx1, outx2, compress_mode = x:compress()
  outx1:check()
  outx2:check()
  assert(compress_mode == compress_modes.distinct_vals)
  assert(outx1:qtype() == "I1")
  assert(outx1:num_elements() == x:num_elements())

  assert(outx2:qtype() == "F4")
  assert(x:num_unique() == outx2:num_elements())
  assert(x:min() == outx2:min())
  assert(x:max() == outx2:max())
  --====== Check for second kind of compression
  local y = df:get("y")
  local outy1, outy2, compress_mode = lRBC.compress(y)

  outy2:check()
  assert(outy2:jtype() == "j_nil")

  outy1:check()
  assert(outy1:qtype() == "F2")
  assert(outy1:num_elements() == y:num_elements())

  assert(compress_mode == compress_modes.F4_to_F2)
  --=================

  local z = lRBC.decompress(outy1, outy2, "F4_to_F2")
  assert(type(z) == "lRBC")
  assert(z:qtype() == "F4")
  assert(z:num_elements() == y:num_elements())
  assert(z:min() == y:min())
  assert(z:max() == y:max())

  local w = lRBC.decompress(outx1, outx2, "distinct_vals")
  assert(type(w) == "lRBC")
  assert(w:qtype() == "F4")
  assert(w:num_elements() == x:num_elements())
  assert(w:min() == x:min())
  assert(w:max() == x:max())

  print("Test compress completed successfully")
end
tests.compress_I1 = function ()
  local col_names =  { "x", "y", }
  local qtypes = { "I1", "I1", }
  local infile = "data_compress.csv"
  local optargs = { is_hdr = true }
  local df = assert(lRBC.read_csv(col_names, qtypes, infile, optargs))
  assert(type(df) == "lRBC")
  --====== Check for first kind of compression
  local x = df:get("x")
  local outx1, outx2, compress_mode = x:compress()
  outx1:check()
  outx2:check()
  assert(outx2:jtype() == "j_nil")
  assert(compress_mode == compress_modes.none)
  assert(outx1:qtype() == "I1")
  assert(outx1:num_elements() == x:num_elements())
  assert(outx1:min() == x:min())
  assert(outx1:max() == x:max())

  print("Test compress_I1 completed successfully")
end
tests.compress_I2_I4 = function ()
  local col_names =  { "x", "y", }
  local infile = "data_compress.csv"
  local optargs = { is_hdr = true }
  local choices = { "I2", "I4" } 
  for _, choice in ipairs(choices) do 
    -- print("choice = ", choice)
    local qtypes = { choice, choice }
    local df = assert(lRBC.read_csv(col_names, qtypes, infile, optargs))
    assert(type(df) == "lRBC")
    --====== Check for first kind of compression
    local x = df:get("x")
    local outx1, outx2, compress_mode = x:compress()
    outx1:check()
    assert(outx2:qtype() == choice)
    assert(compress_mode == compress_modes.distinct_vals)
    assert(outx1:qtype() == "I1")
    assert(outx1:num_elements() == x:num_elements())
    -- test decompression for distinct values
    local w = lRBC.decompress(outx1, outx2, ffi.NULL, "distinct_vals")
    assert(type(w) == "lRBC")
    assert(w:qtype() == choice)
    assert(w:num_elements() == x:num_elements())
    assert(w:min() == x:min())
    assert(w:max() == x:max())

    --====== Check for second kind of compression
    local y = df:get("y")
    local outy1, outy2, compress_mode = lRBC.compress(y)
  
    outy2:check()
    assert(outy2:jtype() == "j_nil")
  
    outy1:check()
    assert(outy1:qtype() == choice)
    assert(outy1:num_elements() == y:num_elements())
  
    assert(compress_mode == compress_modes.none)
  end
  print("Test compress_I2_I4 completed successfully")
end
tests.compress_date = function ()
  local col_names =  { "x",  }
  local qtypes = { "TM:%Y-%m-%d", }
  local infile = "data_compress_date.csv"
  local optargs = { is_hdr = true }
  local df = assert(lRBC.read_csv(col_names, qtypes, infile, optargs))
  assert(type(df) == "lRBC")
  --====== Check for first kind of compression
  local x = df:get("x")
  local outx1, outx2, compress_mode = x:compress()
  outx1:check()
  assert(compress_mode == compress_modes.week_to_offset)
  assert( ( outx1:qtype() == "UI1") or ( outx1:qtype() == "UI2") )
  assert(outx1:num_elements() == x:num_elements())
  assert(outx1:min() == 0)

  outx2:check()
  assert(outx2:jtype() == "j_date")
  print(outx2)

  -- START: Create H for decompress
  local week_file = "../../test/week_start_date_to_hols.txt"
  local format = "%Y-%m-%d"
  local H = ffi.new("date_to_hol_t[?]", 1)
  H = ffi.cast("date_to_hol_t *", H)
  local mtmgcv = ffi.load("mtmgcv.so")
  local status = mtmgcv.read_date_to_hol(week_file, format, H)
  assert(status == 0)
  local nH = H[0].n
  H = H[0].date_hol

  -- STOP : Create H for decompress
  local crbc = cmem.empty(false,  "decompress")
  local w = mtmgcv.decompress_week_to_offset(
    get_rbc_rec(outx1), get_rbc_rec(outx2), H, nH, crbc)
  local w = setmetatable({}, lRBC)
  w._cmem = crbc
  assert(type(w) == "lRBC")
  assert(w:jtype() == "j_array")
  assert(w:qtype() == "TM")
  assert(w:num_elements() == x:num_elements())
  print(w)

  print("Test compress completed successfully")

  print("Test compress_date completed successfully")
end
tests.compress()
tests.compress_I1()
tests.compress_I2_I4()
-- TODO tests.compress_date()
-- return tests
