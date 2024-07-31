-- TODO P1 require 'strict' Uncomment for testing 
local debug = true -- TODO P3 Set to false for production runs 
local lqdf = require 'lqdf'
local ffi  = require 'ffi'
local get_qdf_rec = require 'get_qdf_rec'
local cqdf = ffi.load("qdf.so")
local cmem = require 'libcmem'
local tbl_of_str_to_C_array = require 'tbl_of_str_to_C_array'
local rev_compress_modes = require 'rev_compress_modes'
local mtmgcv = ffi.load("mtmgcv.so")
--===================
local function df_decompress(infile, XC, H, nH, outfile)
  assert(type(infile) == "string")
  assert(type(outfile) == "string")
  assert(infile ~= outfile)
  --===============================
  if ( XC ) then 
    assert(type(XC) == "userdata")
    XC = ffi.cast("xconfig_t *", XC)
    H = ffi.cast("date_hol_t *", XC[0].H.date_hol)
    nH = XC[0].H.n
  end 
  --===============================
  local indf = lqdf.binld(infile)
  if ( debug ) then 
    assert(type(indf) == "lqdf")
    indf:check()
  end
  local K = indf:get("keys")
  local C = indf:get("compress_modes")
  local M = indf:get("metadata")
  local D = indf:get("data")
  local nK, nC, nM, nD
  --===================
  if ( debug ) then
    assert(type(K) == "lqdf")
    assert(K:jtype() == "j_array")
    assert(K:qtype() == "SC")
    nK = K:num_elements()
    assert(nK > 0)

    assert(type(C) == "lqdf")
    assert(C:jtype() == "j_array")
    assert(C:qtype() == "F8")
    nC = C:num_elements()
    assert(nC == nK)

    assert(type(M) == "lqdf")
    assert(M:jtype() == "j_array")
    nM = M:num_elements()
    assert(nM == nK)

    assert(type(D) == "lqdf")
    assert(D:jtype() == "j_array")
    nD = D:num_elements()
    assert(nD == nK)

  end

  local outK = {}
  local W = {}
  for i = 1, nC do 
    local k = K:get(i-1)
    local c = C:get(i-1)
    local d = D:get(i-1)
    local m = M:get(i-1, "raw")
    local w 
    local str_c = rev_compress_modes[c]
    outK[#outK+1] = k
    if ( str_c == "distinct_vals" ) then
      w = lqdf.decompress(d, m, str_c)
    elseif ( str_c == "week_to_offset" ) then
      assert(type(H) == "cdata")
      assert(type(nH) == "number")
      assert(nH > 0)
      --===============================
      local cqdf = cmem.empty(false,  "decompress")
      local status = mtmgcv.decompress_week_to_offset(
        get_qdf_rec(d), get_qdf_rec(m), H, nH, cqdf)
      assert(status == 0)
      w = setmetatable({}, lqdf)
      w._cmem = cqdf
    else
      error(str_c)
    end
    assert(type(w) == "lqdf")
    W[#W+1] = w 
  end
  assert(#W > 0)
  assert(#W == #outK)

  local cW = ffi.new("qdf_REC_TYPE[?]", #W)
  cW = ffi.cast("qdf_REC_TYPE *", cW)

  for i, w in pairs(W) do 
    local xw = get_qdf_rec(w)
    ffi.copy(cW[i-1], xw, ffi.sizeof("qdf_REC_TYPE"))
  end
  -- make an qdf that is an array that contains each w in W
  local coutK = tbl_of_str_to_C_array(outK);
  local cqdf = cmem.empty(false)
  local status = cqdf.make_mixed_array_or_object(cW, coutK, #outK, cqdf)
  assert(status == 0)
  local df_W = setmetatable({}, lqdf)
  df_W._cmem = cqdf
  if ( debug ) then 
    assert(type(df_W) == "lqdf")
    assert(df_W:check())
    for k, v in pairs(outK) do
      assert(df_W:is_key(v))
    end
  end
  --=====================================================
  df_W:bindmp(outfile)
  return true
end
return df_decompress
