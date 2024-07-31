require 'strict'
local lRBC = require 'lRBC'
local ffi  = require 'ffi'
local cRBC = ffi.load("rbc.so")
local cmem = require 'libcmem'
local tbl_of_str_to_C_array = require 'tbl_of_str_to_C_array'
local df_compress           = require 'df_compress'
local df_decompress         = require 'df_decompress'
local mk_week_start_date    = require 'mk_week_start_date'
--===================
local cols = { "co_loc_i", "sls_unit_q", "week_start_date"}
local qtypes = { "I2",  "F4", "TM:%Y-%m-%d"}
local optargs = { is_hdr = true }

-- TODO P2 This function is also in lRBC. Bad idea to duplicate code
local function get_rbc_rec(w)
  assert(w)
  local x = assert(w._cmem)
  local y = ffi.cast("RBC_REC_TYPE *", x)
  assert(y[0].data ~= ffi.NULL)
  assert(y[0].size > 0 )
  return y
end
local tests = {}
tests.t1= function(incsvfile, inrbcfile, outrbcfile)
  local optargs = { is_hdr = true }
  local df = lRBC.read_csv(cols, qtypes, incsvfile, optargs)
  local is_df, sn = lRBC.is_data_frame(df)
  assert(is_df == true)
  assert(df:bindmp(inrbcfile))
  local K = assert(df:keys())
  assert(type(K) == "table")
  for _, k in pairs(K) do 
    assert(type(k) == "string")
    local v = df:get(k)
    assert(type(v) == "lRBC")
    assert(v:jtype() == "j_array")
  end
  --===================
  local D = {} -- contains compressed data 
  local M = {} -- contains meta data about compression
  local C = {} -- contains compress modes
               -- K contains keys 
  local cM = ffi.new("RBC_REC_TYPE[?]", #K)
  cM = ffi.cast("RBC_REC_TYPE *", cM)
  local cD = ffi.new("RBC_REC_TYPE[?]", #K)
  cD = ffi.cast("RBC_REC_TYPE *", cD)

  for i, k in pairs(K) do 
    local v = df:get(k)
    local d, m, c = v:compress()
    assert(type(d) == "lRBC")
    assert(type(m) == "lRBC")
    assert(type(c) == "number")
    D[#D+1] = d
    M[#M+1] = m
    C[#C+1] = c

    local xm = get_rbc_rec(m)
    ffi.copy(cM[i-1], xm, ffi.sizeof("RBC_REC_TYPE"))

    local xd = get_rbc_rec(d)
    ffi.copy(cD[i-1], xd, ffi.sizeof("RBC_REC_TYPE"))
  end
  -- make an RBC out of K 
  local df_K = lRBC(K)
  assert(type(df_K) == "lRBC")
  assert(df_K:jtype() == "j_array")
  assert(df_K:qtype() == "SC")
  df_K:check()
  -- make an RBC out of C 
  local df_C = lRBC(C)
  assert(type(df_C) == "lRBC")
  assert(df_C:jtype() == "j_array")
  assert(df_C:qtype() == "F8")
  df_C:check()
  -- make an RBC that is an array that contains each m in M 
  local crbc = cmem.empty(false)
  local status = cRBC.make_mixed_array_or_object(cM, ffi.NULL, #K, crbc)
  assert(status == 0)
  local df_M = setmetatable({}, lRBC)
  df_M._cmem = crbc
  assert(type(df_M) == "lRBC")
  df_M:check()
  --=====================================================
  -- make an RBC that is an array that contains each d in D 
  local crbc = cmem.empty(false)
  local status = cRBC.make_mixed_array_or_object(cD, ffi.NULL, #K, crbc)
  assert(status == 0)
  local df_D = setmetatable({}, lRBC)
  df_D._cmem = crbc
  assert(type(df_D) == "lRBC")
  df_D:check()
  --=====================================================
  -- make an RBC that is an object that contains D, M and C 
  local crbc = cmem.empty(false)
  local OV = {df_K, df_C, df_D, df_M}
  local cOV = ffi.new("RBC_REC_TYPE[?]", #OV)
  cOV = ffi.cast("RBC_REC_TYPE *", cOV)
  for i, v in pairs(OV) do
    ffi.copy(cOV[i-1], get_rbc_rec(v), ffi.sizeof("RBC_REC_TYPE"))
  end

  local OK = { "keys", "compress_modes", "data", "metadata" }
  local cOK = tbl_of_str_to_C_array(OK)

  local status = cRBC.make_mixed_array_or_object(cOV, cOK, #OV, crbc)
  assert(status == 0)
  local df_O = setmetatable({}, lRBC)
  df_O._cmem = crbc
  assert(type(df_O) == "lRBC")
  df_O:check()
  --===============================================
  df_O:bindmp(outrbcfile)
  for _, k in pairs(OK) do
    assert(df_O:is_key(k))
  end
  local x = tostring(df_O)
  -- this is what we need to return 
  --=====================================================
  print("test t1 executed successfully")
  return true
end
tests.t2 = function(inrbcfile, outrbcfile)
  assert(df_compress(inrbcfile, outrbcfile))
  print("test t2 executed successfully")
end
tests.t3 = function(inrbcfile, outrbcfile)
  -- make H, nH  
  local week_file = "../../test/week_start_date_to_hols.txt"
  local H, nH = mk_week_start_date(week_file)
  assert(df_decompress(inrbcfile, nil, H, nH, outrbcfile))
  print("test t3 executed successfully")
end

local incsvfile = "data_df_compress.csv"
local inrbcfile = "_data_df_compress.rbc"
local outrbcfile1 = "/tmp/_compressed1.rbc"
local outrbcfile2 = "/tmp/_compressed2.rbc"
local inrbcfile2 = "_data_df_compress2.rbc"

tests.t1(incsvfile, inrbcfile, outrbcfile1)
tests.t2(inrbcfile, outrbcfile2)
tests.t3(outrbcfile2, inrbcfile2)
