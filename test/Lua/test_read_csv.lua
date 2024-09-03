G = {}
local plpath = require 'pl.path'
require 'strict' -- put this after penlight because of problem in penlight
local ffi    = require 'ffi'
local lQDF   = require 'lQDF'
local cutils = require 'libcutils'
local mk_c_qtypes = require 'mk_c_qtypes'
local alloc_csv_out = require 'alloc_csv_out'
local rsutils = ffi.load("librsutils.so")

local tests = {}
tests.read_csv_1 = function ()
  local infile = "in1.csv"
  assert(plpath.isfile(infile))
  local is_hdr = true
  local nrows = cutils.num_lines(infile)
  assert(nrows >= 1)
  local ncols = cutils.num_cols(infile)
  assert(ncols >= 1)
  -- get qtypes and set up for C 
  local qtypes = require 'qtypes1'
  assert(type(qtypes) == "table")
  assert(#qtypes == ncols)
  if ( is_hdr ) then nrows = nrows - 1 end
  local sz = ncols * ffi.sizeof("void *")
  local c_qtypes = ffi.gc(ffi.C.malloc(sz), ffi.C.free)
  c_qtypes = ffi.cast("char **", c_qtypes)
  c_qtypes[0] = ffi.NULL
  for i, v in ipairs(qtypes) do
    assert(type(qtypes[i]) == "string")
    local len = #qtypes[i] + 1
    local x = ffi.gc(ffi.C.malloc(len), ffi.C.free)
    ffi.fill(x, len)
    ffi.copy(x, qtypes[i], len)
    c_qtypes[i-1] = x
  end
  c_qtypes = ffi.cast("char ** const", c_qtypes)
  --=========================
  -- allocated space for output
  local sz = ncols * ffi.sizeof("void *")
  local out = ffi.gc(ffi.C.malloc(sz), ffi.C.free)
  ffi.fill(out, sz)
  out = ffi.cast("char ** const", out)
  for i = 1, ncols do 
    local width
    if ( qtypes[i] == "I1" ) then
      width = 1
    elseif ( qtypes[i] == "I2" ) then
      width = 2
    elseif ( qtypes[i] == "I4" ) then
      width = 4
    elseif ( qtypes[i] == "I8" ) then
      width = 8
    elseif ( qtypes[i] == "F4" ) then
      width = 4
    elseif ( qtypes[i] == "F8" ) then
      width = 8
    else 
      error("bad qtype " .. qtypes[i])
    end
    local sz = nrows * width
    out[i-1] = ffi.gc(ffi.C.malloc(sz), ffi.C.free)
  end
  
  local status = rsutils.read_csv(infile, ffi.NULL, 0, c_qtypes, out, ffi.NULL,
    nrows, ncols, ",", '"', "\n", is_hdr)
  assert(status == 0)
  -- spot checking the output
  local iptr = ffi.cast("int *", out[0])
  assert(iptr[0] == 23)
  assert(iptr[nrows-1] == 5)

  local fptr = ffi.cast("float *", out[2])
  assert(fptr[0] >= -1.705088734627)
  assert(fptr[nrows-1] >= -1.705088734627)

  local i2ptr = ffi.cast("int16_t *", out[1])
  assert(i2ptr[0] == 54)
  assert(i2ptr[nrows-1] == 3313)
  -------------------------------
  print("Test read_csv_1 completed successfully\n");
end
tests.read_csv_2 = function ()
  local infile = "../data/in2.csv"
  assert(plpath.isfile(infile))
  local is_hdr = true
  local nrows = cutils.num_lines(infile)
  if ( is_hdr ) then nrows = nrows - 1 end
  assert(nrows >= 1)
  local ncols = cutils.num_cols(infile)
  assert(ncols >= 1)
  -- get qtypes and set up for C 
  local qtypes = require 'qtypes2'
  local c_qtypes = assert(mk_c_qtypes(qtypes))
  c_qtypes = ffi.cast("char ** const ", c_qtypes)
  -- allocate space for output
  local out = assert(alloc_csv_out(qtypes, nrows, ncols))
  out = ffi.cast("char ** const ", out)
  -- read in file  
  local status = rsutils.read_csv(infile, ffi.NULL, 0, c_qtypes, out, ffi.NULL, 
    nrows, ncols, ",", '"', "\n", is_hdr)
  assert(status == 0)
  -- spot checking the output
  local tmptr = ffi.cast("tm_t *",    out[0])
  assert((1900 + tmptr[0].tm_year) == 2020)
  assert(tmptr[0].tm_yday == 365)
  assert((1900 + tmptr[1].tm_year) == 2021)
  assert(tmptr[1].tm_yday == 0)

  local i2ptr = ffi.cast("int16_t *", out[1])
  for i = 1, nrows do 
    assert(i2ptr[i-1] == i-1)
  end
  -------------------------------
  print("Test read_csv_2 completed successfully\n");
end
tests.read_csv_1()
tests.read_csv_2()
-- return tests
