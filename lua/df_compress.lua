-- TODO P1 require 'strict' Uncomment for testing 
local lqdf = require 'lqdf'
local ffi  = require 'ffi'
local get_qdf_rec = require 'get_qdf_rec'
local cqdf = ffi.load("qdf.so")
local cmem = require 'libcmem'
local tbl_of_str_to_C_array = require 'tbl_of_str_to_C_array'
--===================
local function df_compress(infile, outfile)
  assert(type(infile) == "string")
  assert(type(outfile) == "string")
  assert(infile ~= outfile)
  --===============================
  local indf = lqdf.binld(infile)
  if ( G and G.debug ) then 
    assert(type(indf) == "lqdf")
    indf:check()
  end
  local K = indf:keys()
  --===================
  local D = {} -- contains compressed data 
  local M = {} -- contains meta data about compression
  local C = {} -- contains compress modes
               -- K contains keys 
  local cM = ffi.new("qdf_REC_TYPE[?]", #K)
  cM = ffi.cast("qdf_REC_TYPE *", cM)
  local cD = ffi.new("qdf_REC_TYPE[?]", #K)
  cD = ffi.cast("qdf_REC_TYPE *", cD)

  for i, k in pairs(K) do 
    local v = indf:get(k)
    assert(type(v) == "lqdf")
    assert(v:jtype() == "j_array")

    local d, m, c = v:compress()
    assert(type(d) == "lqdf")
    assert(type(m) == "lqdf")
    assert(type(c) == "number")
    D[#D+1] = d
    M[#M+1] = m
    C[#C+1] = c

    local xm = get_qdf_rec(m)
    ffi.copy(cM[i-1], xm, ffi.sizeof("qdf_REC_TYPE"))

    local xd = get_qdf_rec(d)
    ffi.copy(cD[i-1], xd, ffi.sizeof("qdf_REC_TYPE"))
  end
  -- make an qdf out of K 
  local df_K = lqdf(K)
  if ( G and G.debug ) then 
    assert(type(df_K) == "lqdf")
    assert(df_K:jtype() == "j_array")
    assert(df_K:qtype() == "SC")
    df_K:check()
  end
  -- make an qdf out of C 
  local df_C = lqdf(C)
  if ( G and G.debug ) then 
    assert(type(df_C) == "lqdf")
    assert(df_C:jtype() == "j_array")
    assert(df_C:qtype() == "F8")
    assert(df_C:check())
  end 
  -- make an qdf that is an array that contains each m in M 
  local cqdf = cmem.empty(false)
  local status = cqdf.make_mixed_array_or_object(cM, ffi.NULL, #K, cqdf)
  assert(status == 0)
  local df_M = setmetatable({}, lqdf)
  df_M._cmem = cqdf
  if ( G and G.debug ) then 
    assert(type(df_M) == "lqdf")
    assert(df_M:check())
  end
  --=====================================================
  -- make an qdf that is an array that contains each d in D 
  local cqdf = cmem.empty(false)
  local status = cqdf.make_mixed_array_or_object(cD, ffi.NULL, #K, cqdf)
  assert(status == 0)
  local df_D = setmetatable({}, lqdf)
  df_D._cmem = cqdf
  if ( G and G.debug ) then 
    assert(type(df_D) == "lqdf")
    assert(df_D:check())
  end
  --=====================================================
  -- make an qdf that is an object that contains D, M and C 
  local cqdf = cmem.empty(false)
  local OV = {df_K, df_C, df_D, df_M}
  local cOV = ffi.new("qdf_REC_TYPE[?]", #OV)
  cOV = ffi.cast("qdf_REC_TYPE *", cOV)
  for i, v in pairs(OV) do
    ffi.copy(cOV[i-1], get_qdf_rec(v), ffi.sizeof("qdf_REC_TYPE"))
  end

  local OK = {"keys", "compress_modes", "data", "metadata" }
  local cOK = tbl_of_str_to_C_array(OK)

  local status = cqdf.make_mixed_array_or_object(cOV, cOK, #OV, cqdf)
  assert(status == 0)
  local df_O = setmetatable({}, lqdf)
  df_O._cmem = cqdf
  if ( G and G.debug ) then 
    assert(type(df_O) == "lqdf")
    assert(df_O:check())
    for _, k in pairs(OK) do
      assert(df_O:is_key(k))
    end
  end
  --===============================================
  df_O:bindmp(outfile)
  return true
end
return df_compress
