local ffi           = require 'ffi'
local register_type = require 'RSUTILS/lua/register_type'
local cutils        = require 'libcutils'
local rsutils       = ffi.load("librsutils.so") -- for read_csv()
local cQDF          = ffi.load("libqdf.so")
ffi.cdef([[
extern size_t strlen(const char *s);
extern void *malloc(size_t size);
extern void free(void *ptr);
typedef struct {
   char *fpos;
   void *base;
   unsigned short handle;
   short flags;
   short unget;
   unsigned long alloc;
   unsigned short buffincrement;
} FILE;
]])
local QDF_hdrs = require 'qdf_hdrs' -- created by ../src/Makefile 
ffi.cdef(QDF_hdrs)
local x            = require 'read_csv_hdr';      ffi.cdef(x)
--================================
local lqdfmem       = require 'lqdfmem'
--================== 
local qtypes         = require 'qtypes'
local compress_modes = require 'compress_modes'
local rev_qtypes     = require 'rev_qtypes'
local jtypes         = require 'jtypes'
local rev_jtypes     = require 'rev_jtypes'
local pr_jtype       = require 'pr_jtype'
local is_primitive   = require 'is_primitive'
local is_numeric     = require 'is_numeric'
local is_complex     = require 'is_complex'
local chk_tbl        = require 'chk_tbl'
local numeric_types  = require 'numeric_types'
local is_jarray      = require 'is_jarray'
local is_in_tbl      = require 'is_in_tbl'
local tbl_elem_type  = require 'tbl_elem_type'
local chk_expected_mode = require 'chk_expected_mode'
local chk_expected_elem_type = require 'chk_expected_elem_type'
local malloc_space_for_2d_array         = require 'malloc_space_for_2d_array'
local widths           = require 'widths'
local mk_c_qtypes      = require 'mk_c_qtypes'
local trim_qtype       = require 'trim_qtype'
local tbl_of_str_to_C_array = require 'RSUTILS/lua/tbl_of_str_to_C_array'
local assemble_keys    = require 'assemble_keys'
local sclr_as_lua_num  = require 'sclr_as_lua_num'
local lua_num_as_sclr  = require 'lua_num_as_sclr'
local binary_operators = require 'binary_operators'
local unary_operators  = require 'unary_operators'
--====================================================
local make_json -- NOTE: forward declaration

local is_debug = false
if ( G and G.debug ) then 
  print("Debugging turned on in lQDF"); is_debug = true 
end 


--=================================
local make_array_or_object = function(J, cqdf)
  local status = 0
  assert(type(J) == "table")
  local cqdf_ptr 
  if ( type(cqdf) == "lqdfmem") then 
    cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem))
  elseif ( type(cqdf) == "cdata" ) then 
    -- TODO P1 Figure out why control comes here
    cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", cqdf))
  else
    error("XXXX")
  end

  local b_is_jarray = is_jarray(J)
  --=============================
  if ( b_is_jarray ) then 
    local is_uniform, elem_type = tbl_elem_type(J)
    local nT = #J
    if ( is_uniform ) then 
      -- print("Start uniform array")
      if ( elem_type == "number" ) then
        -- convert J to double *
        local T = ffi.new("double[?]", nT)
        T = ffi.cast("double *", T)
        for i = 1, nT do T[i-1] = J[i] end
        --=====================
        status = cQDF.make_num_array(T, nT, 0, qtypes.F8, cqdf_ptr)
        assert(status == 0)
      elseif ( elem_type == "boolean" ) then
        -- convert J to double *
        local T = ffi.new("bool[?]", nT)
        T = ffi.cast("bool *", T)
        for i = 1, nT do T[i-1] = J[i] end
        --====================cmem
        status = cQDF.make_bool_array(T, nT, 0, cqdf_ptr)
        assert(status == 0)
      elseif ( elem_type == "string" ) then
        -- find max width of string
        local max_len = 0
        for _, j in ipairs(J) do
          assert(type(j) == "string")
          if ( #j > max_len ) then max_len = #j end
        end
        max_len = max_len + 1 -- space for nullc
        -- create a char ** vals array 
        local T, chk_nT = tbl_of_str_to_C_array(J)
        assert(chk_nT == nT)
        -- print("XX max_len = ", max_len)
        status = cQDF.make_SC_array(T, ffi.NULL, max_len, nT, 0, cqdf_ptr)
        assert(status == 0)
      else
        error("not valid type for uniform array")
      end
      if ( is_debug ) then status = cQDF.chk_qdf(cqdf_ptr) end 
      assert(status == 0 ) 
    else -- non uniform array
      -- print("Start non uniform array")
      status = cQDF.make_mixed_array_or_object(
        ffi.NULL, ffi.NULL, nT, cqdf_ptr)
      assert(status == 0)
      if ( is_debug ) then status = cQDF.chk_qdf(cqdf_ptr) end 
      assert(status == 0)
      for i = 1, nT do 
        -- print("START: adding element # " .. i .. " to mixed array")
        local elem_qdf = lqdfmem(0)
        local elem_qdf_ptr = 
          assert(ffi.cast("QDF_REC_TYPE *", elem_qdf._qdfmem))
        assert(make_json(J[i], elem_qdf))
        if ( is_debug ) then status = cQDF.chk_qdf(elem_qdf_ptr) end
        assert(status == 0)

        cQDF.append_mixed_array_or_object(cqdf_ptr, elem_qdf_ptr)
        if ( is_debug ) then status = cQDF.chk_qdf(cqdf_ptr)  end
        assert(status == 0)
        -- print("STOP : adding element number " .. i .. " to mixed array")
      end
      if ( is_debug ) then status = cQDF.chk_qdf(cqdf_ptr); end
      assert(status == 0)
      -- print("Stop  non uniform array")
    end
  else -- object
    -- print("Start object")
    local K = assemble_keys(J)
    -- create a char ** keys array 
    local Tk, nTk = tbl_of_str_to_C_array(K)
    status = cQDF.make_mixed_array_or_object(ffi.NULL, Tk, nTk, cqdf)
    assert(status == 0)
    if ( is_debug ) then status = cQDF.chk_qdf(cqdf_ptr); end
    assert(status == 0)
    local i = 1
    for k, v in pairs(J) do 
      -- print("START: adding element " .. k .. " to object")
      local elem_qdf = lqdfmem(0)
      local elem_qdf_ptr = ffi.cast("QDF_REC_TYPE *", elem_qdf._qdfmem)
      assert(make_json(v, elem_qdf))
      if ( is_debug ) then status = cQDF.chk_qdf(elem_qdf_ptr) end
        assert(status == 0)

      status = cQDF.append_mixed_array_or_object(cqdf, elem_qdf_ptr)
      assert(status == 0)
      if ( is_debug ) then status = cQDF.chk_qdf(cqdf_ptr) end
      assert(status == 0)
      -- print("STOP : adding element " .. k .. " to object")
      i = i + 1
    end
    if ( is_debug ) then status = cQDF.chk_qdf(cqdf); end
      assert(status == 0)
    -- print("Stop  object")
  end
  return true
end

local function get_mode(in_mode)
  local mode
  if ( in_mode ) then 
    assert(type(in_mode) == "string")
    assert( ( in_mode == "raw" ) or ( in_mode == "simplified" ) )
    mode = in_mode
  else
    mode = "simplified"
  end
  return mode
end
  --====================================================

local function trim(s)
  -- from PiL2 20.4
  return (s:gsub("^%s*(.-)%s*$", "%1"))
end
--====================================
local lQDF = {}
lQDF.__index = lQDF

setmetatable(lQDF, {
   __call = function (cls, ...)
      return cls.new(...)
   end,
})

register_type(lQDF, "lQDF")


local function get_data_ptr(x, cast_as)
  assert(x)
  assert(type(cast_as) == "string")
  local y = ffi.cast("QDF_REC_TYPE *", x)
  local z = ffi.cast(cast_as, y[0].data)
  assert(z ~= ffi.NULL)
  return z
end


function lQDF.pr(x)
  assert(x); 
  local jptr = assert(lQDF.cmem_ptr(x))
  --================================= 
  -- WORKS local outbuf = cmem.new(0, "pr outbuf")
  -- IMPORTANT: Use below instead of above for local stuff
  -- since ffi.new is faster than going through CMEM
  local outbuf = ffi.new("QDF_REC_TYPE[?]", 1)
  outbuf = ffi.cast("QDF_REC_TYPE *", outbuf)
  local n_used = ffi.new("int[?]", 1)
  -- NOTE: ffi.new() zero-fills the array by default
  local status = cQDF.pr_json(jptr, outbuf, n_used, ffi.NULL);
  assert(status == 0)
  local cptr = ffi.cast("char *", outbuf[0].data)
  local str_len = ffi.C.strlen(cptr)
  assert(str_len > 0)
  -- Note that we dont use below because cptr is null padded
  -- local str_len = tonumber(outbuf[0].size)
  local ret_str = ffi.string(ffi.cast("char *", outbuf[0].data), str_len)
  --========================
  return ret_str
end

lQDF.__index = lQDF
lQDF.__tostring = lQDF.pr

function lQDF:cmem_ptr()
  if ( not self._cmem ) then return ffi.NULL end 
  if ( not self._cmem._qdfmem ) then return ffi.NULL end 
  return ffi.cast("QDF_REC_TYPE *", self._cmem._qdfmem)
end

make_json = function (J, cqdf)
  assert(type(cqdf) == "lqdfmem")
  local cqdf_ptr = ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem)
  local status = 0
  if ( type(J) == "nil" ) then 
    -- print("making nil")
    status = cQDF.make_nil(ffi.NULL, cqdf_ptr); assert(status == 0)
  elseif ( type(J) == "string") then 
    -- print("making string: " .. J)
    status = cQDF.make_string(J, cqdf_ptr); assert(status == 0)
    assert(status == 0)
  elseif ( type(J) == "number") then 
    -- print("making number: " .. tostring(J))
    status = cQDF.make_number(J, cqdf_ptr); assert(status == 0)
  elseif ( type(J) == "boolean") then 
    -- print("making boolean: " .. tostring(J))
    status = cQDF.make_boolean(J, cqdf_ptr); assert(status == 0)
  elseif ( type(J) == "table") then 
    -- print("making array or object: " .. tostring(#J))
    assert(make_array_or_object(J, cqdf_ptr)); 
    -- print("DONE making array or object: " .. tostring(#J))
  else
    print("type(J) == ", type(J))
    error("bad JSON")
  end
  if ( is_debug ) then status = cQDF.chk_qdf(cqdf_ptr) end 
  assert(status == 0)
  return cqdf
end
function lQDF.new(J, optargs)
  -- J is table produced by parsing JSON elsewhere
  local M -- meta data 
  --===============================
  if ( optargs ) then 
    assert(type(optargs) == "table")
    if optargs.meta then
      assert(type(optargs.meta) == "table")
      M = optargs.meta
    end
  end
  --===============================
  -- TODO P1 do we ever send a CMEM in here?
  local ok = false
  if ( type(J) == "table") then ok = true end 
  if ( type(J) == "boolean") then ok = true end 
  if ( type(J) == "number") then ok = true end 
  if ( type(J) == "string") then ok = true end 
  local is_type_nil = false
  if ( type(J) == "userdata") then 
    local x = ffi.cast("void *", J)
    if ( x == ffi.NULL ) then ok = true end 
    J = nil
    is_type_nil = true 
  end
  --== START Special case: we send in the cqdf from outside
  if ( ( J == nil ) and ( is_type_nil == false ) ) then
    local X = assert(optargs.X)
    assert(type(X) == "userdata")
    local nX = assert(optargs.nX)
    assert(type(nX) == "number")
    assert(nX >= 0)
    local cqdf = lqdfmem(0)
    local cqdf_ptr = ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem)
    cqdf_ptr[0].data = X;
    cqdf_ptr[0].size = nX;
    cqdf_ptr[0].is_foreign = true -- IMPORTANT
    local qdf = setmetatable({}, lQDF)
    qdf._cmem = cqdf
    if ( is_debug ) then assert(qdf:check()) end
    return qdf
  end
  --== STOP Special case
  -- print("type(J) = ", type(J))
  assert(ok)
  --===============================
  local cqdf = lqdfmem(0)
  assert(make_json(J, cqdf))
  local qdf = setmetatable({}, lQDF)
  qdf._cmem = cqdf
  if ( is_debug ) then assert(qdf:check()) end
  return qdf
end
--======================================================

function lQDF:nop()
  local status = cQDF.nop(self:cmem_ptr())
  if ( status == 0 ) then return true else return false end
end

function lQDF:check()
  local status = cQDF.chk_qdf(self:cmem_ptr())
  if ( status == 0 ) then return true else return false end
end

function lQDF:clone()
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = {}
  local oldcmem = self._cmem
  assert(type(oldcmem) == "lqdfmem")
  local newcmem = oldcmem:clone()
  assert(type(newcmem) == "lqdfmem")
  newqdf._cmem = newcmem
  if ( is_debug ) then 
    assert(newqdf:check())
  end
  return newqdf
end


function lQDF:unset_nn()
  assert(self:jtype()  == "j_array")
  self._nn = nil 
end

function lQDF:set_nn(nn)
  assert(self:jtype()  == "j_array")

  assert(type(nn) == "lQDF")
  assert(nn:jtype()  == "j_array")
  assert(nn:qtype()  == "I1")

  local n1 = self:num_elements()
  local n2 = nn:num_elements()
  assert(n1 == n2)

  self._nn = nn
  return self
end

function lQDF:get_nn()
  return  self._nn
end

function lQDF:has_nn()
  local nn = self._nn
  if ( not nn ) then return false end 
  assert(type(nn) == "lQDF")
  assert(nn:jtype()  == "j_array")
  assert(nn:qtype()  == "I1")

  return true 
end

function lQDF.coalesce(x1, x2)
  assert(type(x1) == "lQDF")
  assert(x1:jtype()  == "j_array")
  assert(x1:qtype()  == "F4")
  local x1_ptr = x1:cmem_ptr()

  assert(type(x2) == "lQDF")
  assert(x2:jtype()  == "j_array")
  assert(x2:qtype()  == "F4")
  local x2_ptr = x2:cmem_ptr()

  local nn_x1_ptr = ffi.NULL
  if ( x1:has_nn() ) then nn_x1_ptr = x1:get_nn():cmem_ptr() end 

  local nn_x2_ptr = ffi.NULL
  if ( x2:has_nn() ) then nn_x2_ptr = x2:get_nn():cmem_ptr() end 

  local out    = lqdfmem(0)
  local out_ptr = ffi.cast("QDF_REC_TYPE *", out._qdfmem)
  local nn_out = lqdfmem(0)
  local nn_out_ptr = ffi.cast("QDF_REC_TYPE *", nn_out._qdfmem)

  local num_nulls = ffi.new("int[?]", 1)
  local status = cQDF.coalesce(x1_ptr, nn_x1_ptr, x2_ptr, nn_x2_ptr,
    out_ptr, nn_out_ptr, num_nulls) 
  num_nulls = tonumber(num_nulls[0]);
  assert(num_nulls >= 0)

  local outqdf = setmetatable({}, lQDF)
  outqdf._cmem = out
  local nn_outqdf = setmetatable({}, lQDF)
  nn_outqdf._cmem = nn_out

  if ( num_nulls > 0 ) then 
    outqdf._nn = nn_outqdf
  end
  return outqdf, num_nulls

end

function lQDF:set(inval)
  local dst = self:cmem_ptr()
  local dst_jtype = tonumber(cQDF.x_get_jtype(dst))
  local newqdf
  if ( dst_jtype == jtypes.j_number ) then 
    assert(type(inval) == "number")
    newqdf = self:clone()
    cQDF.x_set_num_val(newqdf:cmem_ptr(), inval)
  elseif ( dst_jtype == jtypes.j_array ) then 
    assert(type(inval) == "lQDF")
    assert(inval:jtype() == "j_array")
    local src = inval:cmem_ptr()
    local src_qtype = cQDF.x_get_qtype(src)
    local dst_qtype = cQDF.x_get_qtype(dst)
    assert(src_qtype == dst_qtype) -- TODO P3 Quite restrictive
    -- all elements of array must be of same type
    assert(rev_qtypes[src_qtype] ~= "Q0" )
    assert(rev_qtypes[dst_qtype] ~= "Q0" )

    local status = cQDF.qdf_copy_array(dst, src)
    assert(status == 0)
  else
    print(dst_jtype)
    error("Not supported as yet")
  end
  return newqdf
end

local function simplify_return_val(jtype, qtype, cqdf)
  assert(type(jtype)   == "number")
  assert(type(qtype) == "number")
  if ( jtype == jtypes.j_nil ) then 
    return nil
  elseif ( jtype == jtypes.j_number ) then
    assert(qtype == qtypes.F8 )
    local cqdf_ptr
    if ( (type(cqdf) == "lqdfmem") or (type(cqdf) == "table") ) then
      -- all is well but why do we have 2 options? TODO P1 
    else
      error("XXX")
    end
    cqdf_ptr = ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem)
    local x = cQDF.x_get_num_val(cqdf_ptr)
    return x
  elseif ( jtype == jtypes.j_string ) then
    --[[
    if ( type(cqdf) == "CMEM" ) then 
      return cmem.str_val(cqdf) -- TODO P1 
    else
      error("TODO")
    end
    --]]
    assert(qtype == qtypes.SC )
    local cqdf_ptr
    if ( (type(cqdf) == "lqdfmem") or (type(cqdf) == "table") ) then
      -- all is well but why do we have 2 options? TODO P1 
    else
      error("XXX")
    end
    cqdf_ptr = ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem)
    local x = cQDF.x_get_str_val(cqdf_ptr)
    return ffi.string(x)
  elseif ( jtype == jtypes.j_bool ) then
    local cqdf_ptr = ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem)
    local x =  cQDF.x_get_bool_val(cqdf_ptr)
    return x
  end
  error("control should not come here")
end

function lQDF:get(idx, in_mode)
  -- idx is what we use to index the JSON to get to what we want
  -- it can be a number or a string or a table of number/string
  -- if it is null, thene we return self

  -- mode can be raw or simplified. Default is simplified

  local in_qdf_rec = assert(self:cmem_ptr())
  local dataptr = get_data_ptr(in_qdf_rec, "void *")

  local jtype = tonumber(cQDF.x_get_jtype(in_qdf_rec))
  local qtype = tonumber(cQDF.x_get_qtype(in_qdf_rec))
  local mode = get_mode(in_mode)
  -- handle the simple case first
  if ( not idx ) then 
    if ( ( is_complex(jtype) ) or ( mode == "raw" ) ) then 
      local newqdf = setmetatable({}, lQDF)
      newqdf._cmem        = self._cmem:dupe() -- TODO P1 
      newqdf._parent      = self
      if ( is_debug ) then assert(newqdf:check()) end 
      return newqdf
    else
      return simplify_return_val(jtype, qtype, self._cmem) -- TODO P1 
    end
  end
  --================================================
  -- Control here => Now we are indexing the object 
  assert(is_complex(jtype), "jtype = " .. tostring(jtype))

  -- NOTE that is cmem below has is_foreign == true?
  -- because when we return out_qdf, are just handing out a pointer
  -- from the incoming qdf
  local out_qdf   = lqdfmem(0)
  local bak_out_qdf   = out_qdf -- IMPORTANT. Search for bak_ below
  local out_qdf_ptr   = ffi.cast("QDF_REC_TYPE *", out_qdf._qdfmem)

  local out_sclr = ffi.new("SCLR_REC_TYPE[?]", 1)

  local status
  if ( type(idx) == "string") then 
    status = cQDF.get_key_val(in_qdf_rec, -1, idx, out_qdf_ptr, ffi.NULL)
  elseif ( type(idx) == "number") then 
    status = cQDF.get_arr_val(dataptr, idx, out_sclr, out_qdf_ptr)
  else
    error("key must be number or string")
  end
  if ( status ~= 0 ) then 
    print("get_key_val failed for ", idx); return nil 
  end 

  local out_qtype =  tonumber(out_sclr[0].qtype)
  -- print("out_qtype = ", rev_qtypes[out_qtype])
  if ( out_qtype > 0 ) then 
    -- we got back a scalar => we have a primitive type to return 
    if ( mode == "raw" ) then
      -- however if user wants it raw, we have to make an QDF out of it
      if ( out_qtype == qtypes.BL ) then
        status = cQDF.make_boolean(out_sclr[0].bval, out_qdf_ptr)
      elseif ( is_numeric(out_qtype) ) then
        local num_val = sclr_as_lua_num(out_sclr[0], out_qtype)
        status = cQDF.make_number(num_val, out_qdf_ptr)
      elseif ( out_qtype == qtypes.SC ) then 
        local str_val = tostring(out_sclr[0].val.str)
        status = cQDF.make_string(str_val, out_qdf_ptr)
      else
        error("TODO")
      end
      local qdf = setmetatable({}, lQDF)
      qdf._cmem = out_qdf
      if ( is_debug ) then assert(qdf:check()) end 
      return qdf
    else -- ( mode == "simplified" ) 
      if ( out_qtype == qtypes.BL ) then 
        if ( out_sclr[0].val.bl ) then 
          return true
        else
          return false
        end
        --============
      elseif ( out_qtype == qtypes.I1 ) then 
        return tonumber(out_sclr[0].val.i1)
      elseif ( out_qtype == qtypes.I2 ) then 
        return tonumber(out_sclr[0].val.i2)
      elseif ( out_qtype == qtypes.I4 ) then 
        return tonumber(out_sclr[0].val.i4)
      elseif ( out_qtype == qtypes.I8 ) then 
        return tonumber(out_sclr[0].val.i8)
        --==================================
      elseif ( out_qtype == qtypes.UI1 ) then 
        return tonumber(out_sclr[0].val.ui1)
      elseif ( out_qtype == qtypes.UI2 ) then 
        return tonumber(out_sclr[0].val.ui2)
      elseif ( out_qtype == qtypes.UI4 ) then 
        return tonumber(out_sclr[0].val.ui4)
      elseif ( out_qtype == qtypes.UI8 ) then 
        return tonumber(out_sclr[0].val.ui8)
        --==================================
      elseif ( out_qtype == qtypes.F4 ) then 
        return tonumber(out_sclr[0].val.f4)
      elseif ( out_qtype == qtypes.F8 ) then 
        return tonumber(out_sclr[0].val.f8)
        --==================================
      elseif ( out_qtype == qtypes.SC ) then 
        return  ffi.string(out_sclr[0].val.str)
      else
        error("")
      end
      error("control does not come here")
    end
  end
    
  jtype = tonumber(cQDF.x_get_jtype(out_qdf_ptr))
  qtype = tonumber(cQDF.x_get_qtype(out_qdf_ptr))
  if ( ( is_complex(jtype) ) or ( mode == "raw" ) ) then 
    local newqdf = setmetatable({}, lQDF)
    newqdf._cmem        = bak_out_qdf -- IMPORTANT
    if ( is_debug ) then assert(newqdf:check()) end
    return newqdf
  else
    if ( rev_jtypes[jtype] == "j_bool" ) then 
      local x = cQDF.x_get_bool_val(out_qdf_ptr)
      return x
    end 
    if ( qtype == qtypes.F8 ) then 
      local x =  cQDF.x_get_num_val(out_qdf_ptr)
      return tonumber(x)
    end
    if ( qtype == qtypes.SC ) then 
      local x = cQDF.x_get_str_val(out_qdf_ptr)
      return ffi.string(x)
    end
    if ( qtype == qtypes.TM1 ) then 
      error("TODO")
    end
    error("control should not come here")
  end
  error("control should not come here")
end

function lQDF:qtype()
  local qtype = tonumber(cQDF.x_get_qtype(self:cmem_ptr()))
  assert(qtype >= 0)
  return rev_qtypes[qtype]
end
function lQDF:jtype()
  
  local jtype = cQDF.x_get_jtype(self:cmem_ptr())
  local jtype = tonumber(jtype)
  return rev_jtypes[jtype]
end

function lQDF:is_key(key)
  local status = 0
  local in_qdf_ptr = self:cmem_ptr()
  assert(type(key) == "string")
  local key_qdf = ffi.new("QDF_REC_TYPE[?]", 1)
  key_qdf = ffi.cast("QDF_REC_TYPE *", key_qdf);
  status = cQDF.get_keys(in_qdf_ptr, key_qdf)
  if ( status ~= 0 ) then print("is_key failed for ", key) end 
  assert(status == 0)
  if ( is_debug ) then status = cQDF.chk_qdf(key_qdf) end 
  assert(status == 0)
  local outidx = ffi.new("int[?]", 1)
  local ckey = ffi.new("char[?]", #key+1)
  ffi.copy(ckey, key)
  status = cQDF.is_val_in_SC_array(key_qdf, ckey, outidx)
  assert(status == 0)

  if ( outidx[0] < 0 ) then return false else return true end 
end

function lQDF:num_keys()
  assert(self:jtype() == "j_object")
  return tonumber(cQDF.x_get_obj_len(self:cmem_ptr()))
end
--==============================================================
function lQDF:keys()
  local out_qdf = ffi.new("QDF_REC_TYPE[?]", 1)
  out_qdf = ffi.cast("QDF_REC_TYPE *", out_qdf);
  local key_qdf = ffi.new("QDF_REC_TYPE[?]", 1)
  key_qdf = ffi.cast("QDF_REC_TYPE *", key_qdf);
  local in_qdf_rec = self:cmem_ptr()
  local keys = {}
  local status = 0
  local status = cQDF.get_keys(in_qdf_rec, key_qdf)
  assert(status == 0)
  if ( is_debug ) then status = cQDF.chk_qdf(key_qdf) end 
  assert(status == 0)
  local n_keys = cQDF.x_get_arr_len(key_qdf)
  if ( n_keys <= 0 ) then return nil end  
  for kidx = 1, n_keys do 
    local sclr = ffi.new("SCLR_REC_TYPE[?]", 1)
    local status = cQDF.get_arr_val(key_qdf[0].data, kidx-1, sclr, out_qdf)
    assert(status == 0)
    keys[kidx] = assert(ffi.string(sclr[0].val.str))
  end
  return keys
end

function lQDF:time_band(n_recent)
  assert(type(n_recent) == "number")
  assert(n_recent >= 0)
  assert(self:jtype() == "j_array")
  assert(self:qtype() == "TM1")
  local num_elements = cQDF.x_get_arr_len(self:cmem_ptr())
  assert(num_elements >= 0)

  local out_qdf = lqdfmem(0)
  local out_qdf_ptr = ffi.cast("QDF_REC_TYPE *", out_qdf._qdfmem)
  local status = cQDF.x_time_band(self:cmem_ptr(), n_recent, out_qdf_ptr)
  if ( status ~= 0 ) then return nil end 
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem        = out_qdf
  return newqdf
end
--==============================
function lQDF:get_by_idx(idx)
  assert(self:jtype() == "j_array")
  assert(self:qtype() ~= "Q0")
  assert(type(idx) == "number")
  assert(idx >= 0)

  local rslt = ffi.new("SCLR_REC_TYPE[?]", 1)
  local status = cQDF.qdf_get_by_idx(self:cmem_ptr(), idx, rslt)
  if ( status ~= 0 ) then return nil end 
  local x = sclr_as_lua_num(rslt)
  if ( not x  ) then print("bad value in get_by_idx") return nil end 
  return x
end
--==============================
lQDF.f_to_s = function(x, op, optargs)
  assert(type(x) == "lQDF")
  assert(x:jtype() == "j_array")
  assert(type(op) == "string")
  local in_sclr  = ffi.new("SCLR_REC_TYPE[?]", 1)
  -- handle special case for standard deviation
  if ( op == "sd" ) then
    assert(type(optargs) == "table")
    assert(type(optargs.mu) == "number")
    in_sclr[0].val.f8 = optargs.mu
    in_sclr[0].qtype = qtypes["F8"]
  else
    assert(type(optargs) == "nil")
  end
  --=================================
  local out_sclr = ffi.new("SCLR_REC_TYPE[?]", 1)
  -- num_sclr returns number of values used to create out_sclr
  local num_sclr  = ffi.new("SCLR_REC_TYPE[?]", 1)
  if ( x:has_nn() ) then 
    local status = cQDF.f_to_s_nn(x:cmem_ptr(), x:get_nn():cmem_ptr(),
      op, in_sclr, out_sclr, num_sclr);
    assert(status == 0)
    return sclr_as_lua_num(out_sclr), sclr_as_lua_num(num_sclr)
  else
    local status = cQDF.f_to_s(x:cmem_ptr(), op, in_sclr, out_sclr);
    assert(status == 0)
    return sclr_as_lua_num(out_sclr)
  end
end
--==============================
function lQDF:max(optargs)
  return lQDF.f_to_s(self, "max", optargs)
end
--==============================
function lQDF:sum(optargs)
  return lQDF.f_to_s(self, "sum", optargs)
end
--==============================
function lQDF:min(optargs)
  return lQDF.f_to_s(self, "min", optargs)
end
--==============================
function lQDF:sd(mu, optargs)
  if ( not optargs ) then 
    optargs = {} 
  else 
    assert(type(optargs) == "table" ) 
  end 
  optargs.mu = mu
  return lQDF.f_to_s(self, "sd", optargs)
end
--==============================
function lQDF:compress()
  local compress_mode = ffi.new("int[?]", 1)
  compress_mode = ffi.cast("int *", compress_mode)
  local in_qdf_ptr = self:cmem_ptr()
  local out_qdf1 = lqdfmem(0)
  local out_qdf1_ptr = ffi.cast("QDF_REC_TYPE *", out_qdf1._qdfmem)
  local out_qdf2 = lqdfmem(0)
  local out_qdf2_ptr = ffi.cast("QDF_REC_TYPE *", out_qdf2._qdfmem)
  local status = cQDF.compress(in_qdf_ptr, out_qdf1_ptr, out_qdf2_ptr, 
    compress_mode)
  if ( status ~= 0 ) then return nil end 
  local newqdf1 = setmetatable({}, lQDF)
  newqdf1._cmem        = out_qdf1
  local newqdf2 = setmetatable({}, lQDF)
  newqdf2._cmem        = out_qdf2
  return newqdf1, newqdf2, tonumber(compress_mode[0])
end
--==============================
function lQDF.decompress(x, y, compress_mode)
  assert(type(x) == "lQDF")
  local xptr = x:cmem_ptr()

  assert(type(y) == "lQDF")
  local yptr = y:cmem_ptr()

  print(compress_mode) 
  assert(type(compress_mode) == "string")
  compress_mode = assert(compress_modes[compress_mode])
  local cqdf = lqdfmem(0)
  local cqdf_ptr = ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem)
  local status
  if ( compress_mode == "week_to_offset" ) then
    error("TO BE IMPLEMENTED")
  else 
    status = cQDF.decompress(xptr, yptr, compress_mode, cqdf_ptr)
  end
  if ( status ~= 0 ) then return nil end 
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem        = cqdf
  return newqdf 
end
--==============================
-- returns a pointer to the data 
function lQDF:get_arr_ptr()
  local rslt = ffi.new("char *[?]", 1) -- IMPORTANT: Note params ton new()
  rslt = ffi.cast("const char **", rslt)
  local inptr = self:cmem_ptr()
  local n_elem = cQDF.x_get_arr_len(inptr)
  if ( n_elem <= 0 ) then return nil end 
  rslt[0] = cQDF.get_arr_ptr(get_data_ptr(inptr, "char *"))
  return n_elem, rslt[0]
end

function lQDF:obj_arr_len() 
  return cQDF.x_get_obj_arr_len(self:cmem_ptr())
end

function lQDF:obj_arr_size() 
  return cQDF.x_get_obj_arr_size(self:cmem_ptr())
end

function lQDF:is_data_frame()
  return  cQDF.x_get_is_df(self:cmem_ptr())
end

function lQDF:is_df() 
  return cQDF.x_get_is_df(self:cmem_ptr())
end

function lQDF:set_arr_len(n) -- DANGEROUS! USE WITH CAUTION
  local status = cQDF.x_set_arr_len(self:cmem_ptr(), n)
  assert(status == 0)
  return self
end

function lQDF:set_obj_arr_len(n) -- DANGEROUS! USE WITH CAUTION
  local status = cQDF.x_set_obj_arr_len(self:cmem_ptr(), n)
  assert(status == 0)
  return self
end

function lQDF:set_obj_arr_size(n) -- DANGEROUS! USE WITH CAUTION
  local status = cQDF.x_set_obj_arr_size(self:cmem_ptr(), n)
  assert(status == 0)
  return self
end

function lQDF:set_is_df(b_is_df) -- DANGEROUS! USE WITH CAUTION
  local status = cQDF.x_set_is_df(self:cmem_ptr(), b_is_df)
  assert(status == 0)
  return self
end
function lQDF.read_csv(col_names, in_qtypes, csv_file, optargs)
  assert(type(col_names) == "table")
  assert(type(in_qtypes) == "table")
  local ncols = #col_names
  assert(ncols > 0)
  assert(ncols == #in_qtypes)
  for i = 1, ncols do 
    assert(#col_names[i] > 0)
    assert(type(col_names[i]) == "string")
    assert(type(in_qtypes[i]) == "string")
  end
  -- check names are distinct
  for i = 1, ncols do 
    for j = i+11, ncols do 
      assert(col_names[i] ~= col_names[j])
    end
  end
  -- check types of columns and get their widths
  local l_widths = {}
  for i = 1, ncols do 
    local qtype = trim_qtype(in_qtypes[i])
    assert(qtypes[qtype], qtype)
    l_widths[i] = assert(widths[qtype])
  end
  -- process optional arguments, if any
  local is_hdr = true
  local fld_sep = ","    -- hard coded  TODO P4
  local fld_delim = '"'  -- hard coded  TODO P4
  local buf_spec
  if ( optargs ) then 
    assert(type(optargs) == "table")
    if ( optargs.is_hdr ~= nil ) then 
      assert(type(optargs.is_hdr) == "boolean")
      is_hdr = optargs.is_hdr
    end
    if ( optargs.fld_sep ~= nil ) then 
      assert(type(optargs.fld_sep) == "string")
      assert(#fld_sep == 1)
      fld_sep = optargs.fld_sep
    end
    if ( optargs.fld_delim ~= nil ) then 
      assert(type(optargs.fld_delim) == "string")
      assert(#fld_delim == 1)
      fld_delim = optargs.fld_delim
    end
    if ( optargs.buf_spec ~= nil ) then 
      assert(type(optargs.buf_spec) == "table")
      buf_spec = optargs.buf_spec
      -- TODO P4 can do more checks here
    end
  end
  assert(fld_sep ~= fld_delim)
  ---------------------------------
  local nrows = cutils.num_lines(csv_file)
  assert(nrows >= 1)
  if ( is_hdr ) then nrows = nrows - 1 end
  local chk_ncols = cutils.num_cols(csv_file)
  assert(ncols == chk_ncols)
  ---------------------------------
  local c_qtypes = mk_c_qtypes(in_qtypes)
  c_qtypes = ffi.cast("char ** const ", c_qtypes)
  local nrows_to_allocate = nrows
  if ( buf_spec ) then 
    local x = buf_spec.absolute
    local y = buf_spec.times
    local z = buf_spec.plus
    if ( x ) then 
      if ( x > nrows ) then nrows_to_allocate = x end 
    end
    if ( y ) then 
      if ( y > 1 ) then nrows_to_allocate = nrows * y end 
    end
    if ( z ) then 
      if ( z > 0 ) then nrows_to_allocate = nrows + z end 
    end
  end
  local m = nrows_to_allocate
  local out = assert(malloc_space_for_2d_array(m, ncols, l_widths))
  out = ffi.cast("char ** const", out)
  local widths = ffi.NULL -- will need ot set this properly to support SC
  local status = rsutils.read_csv(csv_file, ffi.NULL, 0, c_qtypes, out, 
    widths, nrows, ncols, ",", '"', "\n", is_hdr)
  assert(status == 0)
  ---------------------------------
  local df_qdf = lqdfmem(0)
  local df_qdf_mem = ffi.cast("QDF_REC_TYPE *", df_qdf._qdfmem)
  ------
  -- create a char ** keys array 
  local Tk, nTk = tbl_of_str_to_C_array(col_names)
  status = cQDF.make_mixed_array_or_object(
        ffi.NULL, Tk, ncols, df_qdf_mem)
  assert(status == 0)
  for i = 1, ncols do 
    local status = 0
    local tmpqdf = ffi.new("QDF_REC_TYPE[?]", 1)
    local qtype = trim_qtype(in_qtypes[i])
    local c_qtype = assert(qtypes[qtype]) -- convert str to enum for C 
    status = cQDF.make_num_array(out[i-1], nrows, m, c_qtype, tmpqdf)
    assert(status == 0)
    if ( is_debug ) then status = cQDF.chk_qdf(tmpqdf) end 
    assert(status == 0)
    status = cQDF.append_mixed_array_or_object(df_qdf_mem, tmpqdf)
    assert(status == 0)
    if ( is_debug ) then status = cQDF.chk_qdf(df_qdf_mem) end 
    assert(status == 0)
  end
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem        = df_qdf
  -- indicate that newqdf is a dataframe 
  newqdf:set_is_df(true)
  assert(nrows <= m)
  newqdf:set_obj_arr_len(nrows)
  newqdf:set_obj_arr_size(m)
  return newqdf
end


function lQDF:width()
  assert(self:jtype() == "j_array")
  local width = cQDF.x_get_arr_width(self:cmem_ptr())
  if ( width == 0xFFFFFFFF ) then return nil end 
  return width
end
function lQDF:arr_len()
  assert(self:jtype() == "j_array")
  local num_elements = cQDF.x_get_arr_len(self:cmem_ptr())
  if ( num_elements == 0xFFFFFFFF ) then return nil end 
  return num_elements
end
function lQDF:num_elements()
  local num_elements = cQDF.x_get_arr_len(self:cmem_ptr())
  if ( num_elements == 0xFFFFFFFF ) then return nil end 
  return num_elements
end
function lQDF:arr_size()
  assert(self:jtype() == "j_array")
  local size = cQDF.x_get_arr_size(self:cmem_ptr())
  if ( size == 0xFFFFFFFF ) then return nil end 
  return size
end

function lQDF:tm_extract(fld)
  assert(type(fld)    == "string")
  assert(self:jtype() == "j_array")
  assert(self:qtype()  == "TM1")
  local cqdf = lqdfmem(0)
  local cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem))
  local status = cQDF.tm_extract(self:cmem_ptr(), fld, cqdf_ptr)
  assert(status == 0 )
  --===================================
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem        = cqdf
  if ( is_debug ) then 
    assert(newqdf:jtype() == "j_array")
    assert((newqdf:qtype() == "I1") or (newqdf:qtype() == "I2"))
  end
  return newqdf
end

function lQDF:is_unique()
  assert(self:jtype() == "j_array")
  -- second argument used to identify null values
  -- currently, we do not support uniques if there are nulls
  local  is_uq = ffi.new("bool[?]", 1)
  is_uq = ffi.cast("bool *", is_uq)
  local status = cQDF.x_is_unique(self:cmem_ptr(), ffi.NULL, is_uq)
  assert(status == 0)
  return is_uq[0]
end

function lQDF:num_unique_saturated(max_n_uq)
  assert(self:jtype() == "j_array") 
  assert(type(max_n_uq) == "number") 
  assert(max_n_uq >= 1)
  -- currently, we do not support uniques if there are nulls
  local n_uq = ffi.new("uint32_t[?]", 1)
  n_uq = ffi.cast("uint32_t *", n_uq)
  local status = cQDF.x_num_unique_saturated(self:cmem_ptr(), 
    max_n_uq, n_uq)
  assert(status == 0)
  local n = tonumber(n_uq[0])
  assert(n > 0)
  return n
end

function lQDF:num_unique()
  assert(self:jtype() == "j_array")
  -- second argument used to identify null values
  -- currently, we do not support uniques if there are nulls
  local n_uq = ffi.new("uint32_t[?]", 1)
  n_uq = ffi.cast("uint32_t *", n_uq)
  local status = cQDF.x_num_unique(self:cmem_ptr(), ffi.NULL, n_uq)
  assert(status == 0)
  local n = tonumber(n_uq[0])
  assert(n > 0)
  return n
end
function lQDF:set_lags(lag_start, lag_stop, lag_prefix, grpby, val, tim)
  assert(type(lag_start) == "number")
  assert(type(lag_stop) == "number")
  assert(lag_start > 0)
  assert(lag_stop >= lag_start)
  assert(type(grpby) == "lQDF"); assert(grpby:jtype() == "j_array")
  assert(type(val) == "lQDF");   assert(val:jtype() == "j_array")
  assert(type(tim) == "lQDF");   assert(tim:jtype() == "j_array")
  --=================================
  local num_lags = lag_stop - lag_start + 1 
  assert(num_lags >= 1)
  --=================================
  local status = cQDF.set_lags(
    self:cmem_ptr(), lag_start, lag_stop, lag_prefix, 
    grpby:cmem_ptr(), val:cmem_ptr(), tim:cmem_ptr())
  if ( status ~= 0 ) then return nil end 
  return true 
end

--- REVEIWED BELOW BUT STILL TO BE TESTED 
function lQDF:squeeze_where(where)
  assert(type(where) == "lQDF")
  assert(where:jtype() == "j_array")
  assert(where:qtype() == "I1")
  local num_good = ffi.new("uint32_t[?]", 1)
  local status = cQDF.squeeze_where(self:cmem_ptr(), 
    where:cmem_ptr(), num_good)
  assert(status == 0)
  return tonumber(num_good[0])
end

function lQDF:pr_csv(file_name)
  assert(type(file_name) == "string")
  local status = cQDF.pr_csv(self:cmem_ptr(), file_name)
  assert(status == 0)
  return true
end

function lQDF:pr_df_as_csv(keys, file_name)
  assert(self:is_df())
  assert(type(file_name) == "string")
  -- create a char ** vals array 
  local K = ffi.NULL
  local nK = 0
  if ( keys ) then 
    K, nK = tbl_of_str_to_C_array(keys)
  end
  K = ffi.cast("char ** const", K)
  local status = cQDF.pr_df_as_csv(self:cmem_ptr(), K, nK, file_name)
  assert(status == 0)
  return true
end

function lQDF:bindmp(file_name)
  assert(type(file_name) == "string")
  local status = cQDF.bindmp(self:cmem_ptr(), file_name)
  assert(status == 0)
  return true
end

function lQDF:set_qtype(new_qtype) -- DANGEROUS USE WITH CAUTION
  assert(type(new_qtype) == "string")
  local c_qtype = assert(qtypes[new_qtype])
  local status = cQDF.x_set_qtype(self:cmem_ptr(), c_qtype)
  assert(status == 0)
  return self
end

function lQDF:resize_df(new_sz)
  assert(type(new_sz) == "number")
  assert(new_sz > 0)
  local dst = lqdfmem(0)
  --==================================
  local status = cQDF.qdf_resize_df(self:cmem_ptr(), 
    ffi.NULL, ffi.NULL, 0, -- we are not supporting these options just yet
    new_sz, dst._qdfmem)
  assert(status == 0)
  local dstqdf = setmetatable({}, lQDF)
  dstqdf._cmem = dst
  return dstqdf
end

function lQDF:append_df(src)
  assert(type(src) == "lQDF")
  local n = ffi.new("uint32_t[?]", 1)
  n = ffi.cast("int *", n)
  local status = cQDF.qdf_append_df(self:cmem_ptr(), src:cmem_ptr(), n)
  if ( status == -2  ) then 
    return tonumber(n[0])--  this is num to expand
  end 
  assert(status == 0)
  return self
end

function lQDF.make_empty_data_frame(cols, qtypes, sz_rows)
  assert(type(cols)   == "table")
  assert(type(qtypes) == "table")
  local nC = #cols
  assert(nC >= 1)
  assert(nC == #qtypes)
  for k1, v1 in ipairs(cols) do 
    for k2, v2 in ipairs(cols) do 
     if ( k1 ~= k2 ) then assert( v1 ~= v2) end
    end
  end
  assert(type(sz_rows) == "number")
  assert(sz_rows > 0)
  --===========================
  local cqdf = lqdfmem(0)
  local C, nC = tbl_of_str_to_C_array(cols)
  local Q, nQ = tbl_of_str_to_C_array(qtypes)
  assert(nC == nQ)
  local status = cQDF.make_empty_data_frame(C, nC, Q, sz_rows, 
    ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem))
  assert(status == 0)
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = cqdf
  return newqdf
end

function lQDF:set_foreign() -- USE WITH GREAT CARE
  local x = get_qdf_rec(self)
  x[0].is_foreign = true 
end

function lQDF:free() 
  error("XXXXX")
  local status = cQDF.free_qdf(self:cmem_ptr())
  assert(status == 0)
  return true
end

function lQDF.binld(filename, is_writable)
  assert(type(filename) == "string")
  if ( is_writable == nil ) then is_writable = false end
  assert(type(is_writable) == "boolean")
  local cqdf = lqdfmem(0)
  local cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem))
  local status = cQDF.binld(cqdf_ptr, filename, is_writable)
  assert(status == 0, "Unable to load file " .. filename)
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = cqdf
  return newqdf
end

function lQDF:mktime()
  assert(self:jtype() == "j_array")
  assert(self:qtype() == "TM1")
  local cqdf = lqdfmem(0)
  local cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem))
  local status = cQDF.qdf_mktime(self:cmem_ptr(), cqdf_ptr)
  assert(status == 0)
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = cqdf
  return newqdf
end

function lQDF.unary_op(x, operator, optargs)
  assert(type(x) == "lQDF")
  local in_situ = false
  if ( optargs ) then
    assert(type(optargs) == "table")
    if ( optargs.in_situ ) then
      assert(type(optargs.in_situ) == "boolean")
      in_situ = optargs.in_situ
    end
  end
  -- if in_situ,     then we perform x:= op(x) and return x 
  -- if not in_situ, then we perform z:= op(x) and return z
  -- check validity of operator
  assert(type(operator) == "string")
  -- START some ugly special casing around convert
  local bak_operator = operator -- backup original value 
  local m, n = string.find(operator, "convert:")
  if ( m and m == 1 ) then 
    operator = "convert"
  end
  -- SSTOP some ugly special casing around convert
  local uu = unary_operators[operator]
  assert(type(uu) == "table", operator .. " is not a unary operator ")
  operator = bak_operator -- restore value 
  -- check that operator is okay for this type
  assert(is_in_tbl(x:qtype(), uu), 
    x:qtype() .. " not valid type for operator " .. operator)
  assert(x:jtype() == "j_array")

  local xptr = x:cmem_ptr()
  local status, cqdf, zptr
  if ( in_situ ) then
    zptr = xptr
  else
    cqdf = lqdfmem(0)
    zptr = ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem)
  end
  --=================================================
  local status = cQDF.v_op(xptr, operator, zptr)
  assert(status == 0)
  if ( in_situ ) then 
    return x
  else
    local newqdf = setmetatable({}, lQDF)
    newqdf._cmem = cqdf
    return newqdf
  end
end
--======================================================
function lQDF.binary_op(x, y, operator, optargs)
  assert(type(x) == "lQDF")
  local in_situ = false
  if ( optargs ) then
    assert(type(optargs) == "table")
    if ( optargs.in_situ ) then
      assert(type(optargs.in_situ) == "boolean")
      in_situ = optargs.in_situ
    end
  end
  -- if in_situ,     then we perform x:= x op y and return x 
  -- if not in_situ, then we perform z:= x op y and return z
  -- check validity of operator
  assert(type(operator) == "string")
  local bb = binary_operators[operator]
  assert(type(bb) == "table")
  -- check that operator is okay for this type
  assert(is_in_tbl(x:qtype(), bb), 
    x:qtype() .. " not valid type for operator " .. operator)
  assert(x:jtype() == "j_array")

  local y_is_sclr 
  local sclr
  if ( type(y) == "lQDF" ) then 
    y_is_sclr = false
    assert(y:jtype() == "j_array")
  elseif ( type(y) == "number" ) then 
    y_is_sclr = true
    sclr = assert(lua_num_as_sclr(y, x:qtype()))
  else
    error("argument to " .. operator .. " should be QDF or scalar")
  end
  local xptr = x:cmem_ptr()
  local status, cqdf, zptr
  if ( in_situ ) then
    zptr = xptr
  else
    cqdf = lqdfmem(0)
    zptr = ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem)
  end
  --=================================================
  if ( y_is_sclr ) then 
    status = cQDF.vs_op(xptr, sclr, operator, zptr)
  else
    local yptr = y:cmem_ptr()
    status = cQDF.vv_op(xptr, yptr, operator, zptr)
  end
  assert(status == 0)
  if ( in_situ ) then 
    return x 
  else
    local newqdf = setmetatable({}, lQDF)
    newqdf._cmem = cqdf
    return newqdf
  end
end
--======================================================
-- START: binary operators 
function lQDF:add(y, optargs)
  return lQDF.binary_op(self, y, "add", optargs)
end
function lQDF:sub(y, optargs)
  return lQDF.binary_op(self, y, "sub", optargs)
end
function lQDF:mul(y, optargs)
  return lQDF.binary_op(self, y, "mul", optargs)
end
function lQDF:div(y, optargs)
  return lQDF.binary_op(self, y, "div", optargs)
end
function lQDF:rem(y, optargs)
  return lQDF.binary_op(self, y, "rem", optargs)
end
function lQDF:min(y, optargs)
  if ( y == nil ) then
    return lQDF.f_to_s(self, "min", optargs)
  else
    return lQDF.binary_op(self, y, "min", optargs)
  end
end
function lQDF:max(y, optargs)
  if ( y == nil ) then
    return lQDF.f_to_s(self, "max", optargs)
  else
    return lQDF.binary_op(self, y, "max", optargs)
  end
end
function lQDF:bitwise_and(y, optargs)
  return lQDF.binary_op(self, y, "bitwise_and", optargs)
end
function lQDF:bitwise_or(y, optargs)
  return lQDF.binary_op(self, y, "bitwise_or", optargs)
end
function lQDF:bitwise_xor(y, optargs)
  return lQDF.binary_op(self, y, "bitwise_xor", optargs)
end
function lQDF:eq(y, optargs)
  return lQDF.binary_op(self, y, "eq", optargs)
end
function lQDF:neq(y, optargs)
  return lQDF.binary_op(self, y, "neq", optargs)
end
function lQDF:lt(y, optargs)
  return lQDF.binary_op(self, y, "lt", optargs)
end
function lQDF:leq(y, optargs)
  return lQDF.binary_op(self, y, "leq", optargs)
end
function lQDF:gt(y, optargs)
  return lQDF.binary_op(self, y, "gt", optargs)
end
function lQDF:geq(y, optargs)
  return lQDF.binary_op(self, y, "geq", optargs)
end
function lQDF:logical_and(y, optargs)
  return lQDF.binary_op(self, y, "logical_and", optargs)
end
function lQDF:logical_or(y, optargs)
  return lQDF.binary_op(self, y, "logical_or", optargs)
end
function lQDF:shift_left(y, optargs)
  return lQDF.binary_op(self, y, "shift_left", optargs)
end
function lQDF:shift_right(y, optargs)
  return lQDF.binary_op(self, y, "shift_right", optargs)
end
function lQDF:raised_to(y, optargs)
  return lQDF.binary_op(self, y, "raised_to", optargs)
end
-- STOP: binary operators 
--======================================================
-- START: unary operators 
--======================================================
function lQDF:sawtooth(optargs)
  return lQDF.unary_op(self, "sawtooth", optargs)
end
--======================================================
function lQDF:flatten_sawtooth(optargs)
  return lQDF.unary_op(self, "flatten_sawtooth", optargs)
end
--======================================================
function lQDF:bitwise_not(optargs)
  return lQDF.unary_op(self, "bitwise_not", optargs)
end
--======================================================
function lQDF:convert(newtype, optargs)
  assert(type(newtype) == "string")
  local operator = "convert:" .. newtype
  return lQDF.unary_op(self, operator, optargs)
end
--======================================================
function lQDF:abs(optargs)
  return lQDF.unary_op(self, "abs", optargs)
end
--======================================================
function lQDF:exp(optargs)
  return lQDF.unary_op(self, "exp", optargs)
end
--======================================================
function lQDF:log(optargs)
  return lQDF.unary_op(self, "log", optargs)
end
--======================================================
function lQDF:logical_not(optargs)
  return lQDF.unary_op(self, "logical_not", optargs)
end
--======================================================
function lQDF:reciprocal(optargs)
  return lQDF.unary_op(self, "reciprocal", optargs)
end
--======================================================
function lQDF:round(optargs)
  return lQDF.unary_op(self, "round", optargs)
end
--======================================================
function lQDF.ifxthenyelsez(x, y, z)
  assert(type(x) == "lQDF")
  assert(x:jtype() == "j_array")
  assert(x:qtype() == "I1")
  local xptr = x:cmem_ptr()

  assert(type(y) == "lQDF")
  assert(y:jtype() == "j_array")
  local yptr = y:cmem_ptr()

  local cqdf = lqdfmem(0)
  local cqdf_ptr = ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem)
  local status 

  if ( type(z) == "lQDF" ) then 
    assert(z:jtype() == "j_array")
    local zptr = z:cmem_ptr()
    status = cQDF.ifxthenyelsez( xptr, yptr, zptr, cqdf_ptr)
  elseif ( type(z) == "number" ) then 
    local sclr = lua_num_as_sclr(z, y:qtype())
    sclr = ffi.cast("SCLR_REC_TYPE *", sclr)
    status = cQDF.ifxthenyelsez_sclr( xptr, yptr, sclr, cqdf_ptr)
  else
    error("")
  end
  assert(status == 0)
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = cqdf
  return newqdf
end
--==========================================================
function lQDF.const(val, str_qtype, n, sz)
  assert(type(val) == "number")
  assert(type(str_qtype) == "string")
  assert(type(n) == "number")
  if ( sz == nil ) then sz = 0 end
  assert(type(sz) == "number") 
  --===============================
  local val_sclr = lua_num_as_sclr(val, str_qtype)
  local cqdf = lqdfmem(0)
  local status = cQDF.qdf_const(val_sclr, n, sz, cqdf._qdfmem)
  assert(status == 0)
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = cqdf
  return newqdf
end
--==========================================================
function lQDF:is_sawtooth()
  assert(self:jtype() == "j_array")
  local rslt = ffi.new("bool[?]", 1)
  rslt = ffi.cast("bool *", rslt)
  local status = cQDF.qdf_is_sawtooth(self:cmem_ptr(), rslt)
  assert(status == 0)
  return rslt[0]
end
--==========================================================
function lQDF:incr_on_by(on, incr, reset)
  assert(type(on) == "number")
  assert(type(incr) == "number")
  assert(type(reset) == "boolean")
  local qtype = self:qtype()
  assert(self:jtype() == "j_array")
  --===============================
  local on_sclr   = assert(lua_num_as_sclr(on, qtype))
  local incr_sclr = assert(lua_num_as_sclr(incr, qtype))
  local cqdf = lqdfmem(0)
  local cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem))
  local status = cQDF.qdf_incr_on_by(self:cmem_ptr(), on_sclr, incr_sclr,
    reset, cqdf_ptr)
  assert(status == 0)
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = cqdf
  return newqdf
end
--==========================================================
function lQDF.seq(start, incr, str_qtype, n, sz)
  assert(type(start) == "number")
  assert(type(incr) == "number")
  assert(type(str_qtype) == "string")
  assert(type(n) == "number")
  if ( sz == nil ) then sz = 0 end
  assert(type(sz) == "number") 
  local qtype = assert(qtypes[str_qtype])
  --===============================

  local start_sclr = lua_num_as_sclr(start, str_qtype)
  local incr_sclr = lua_num_as_sclr(incr, str_qtype)
  local cqdf = lqdfmem(0)
  local cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem))
  local status = cQDF.qdf_seq(start_sclr, incr_sclr, n, sz, cqdf_ptr)
  assert(status == 0)
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = cqdf
  return newqdf
end
--==========================================================
function lQDF:sort(mode)
  assert(type(mode) == "string")
  local status = cQDF.qdf_sort(self:cmem_ptr(), mode)
  assert(status == 0)
  return self
end
--==========================================================
function lQDF.left_join( from_key, from_idx, from_val, to_key, to_idx)
  assert(type(from_key) == "lQDF")
  assert(type(from_val) == "lQDF")
  assert(type(from_idx) == "lQDF")
  assert(type(to_key)   == "lQDF")
  assert(type(to_idx)   == "lQDF")

  local from_key_ptr = lQDF.cmem_ptr(from_key)
  local from_val_ptr = lQDF.cmem_ptr(from_val)
  local from_idx_ptr = lQDF.cmem_ptr(from_idx)
  local to_key_ptr = lQDF.cmem_ptr(to_key)
  local to_idx_ptr = lQDF.cmem_ptr(to_idx)

  local to_n  = cQDF.x_get_arr_len(to_key_ptr)
  local to_sz = cQDF.x_get_arr_size(to_key_ptr)

  local from_val_qtype = from_val:qtype()
  local to_val    = lQDF.const(0, from_val_qtype, to_n, to_sz)
  local nn_to_val = lQDF.const(0, "I1",      to_n, to_sz)

  local to_val_ptr = lQDF.cmem_ptr(to_val)
  local nn_to_val_ptr = lQDF.cmem_ptr(nn_to_val)

  local status = cQDF.qdf_join(
    from_key_ptr, from_idx_ptr, from_val_ptr, 
    to_key_ptr, to_idx_ptr, to_val_ptr, nn_to_val_ptr)
  assert(status == 0)
  if ( is_debug ) then 
    assert(nn_to_val:min() >= 0)
    assert(nn_to_val:max() <= 1)
  end
  local num_nn = nn_to_val:sum()
  if ( num_nn  == to_n ) then -- all values defined
    nn_to_val = nil
  else
    print(num_nn, to_n)
  end
  return to_val, nn_to_val 

end
--==========================================================
-- dst[i] := src[i+offset] for i = 0 to length(src)-1
-- if not enough space in dst, then error thrown if truncate==false
function lQDF:write(src, offset, truncate)
  if ( type(offset) == "nil" ) then offset = 0 end 
  assert(type(offset) == "number")
  assert(offset >= 0)

  assert(type(src) == "lQDF")
  assert(src:jtype() == "j_array")
  assert(src:qtype() == self:qtype())

  if ( type(truncate) == "nil" ) then truncate = false end 
  assert(type(truncate) == "boolean")
 --==========================================================
  local status = cQDF.qdf_write(src:cmem_ptr(), offset, truncate, 
    self:cmem_ptr())
  assert(status == 0)
  return self
end
--==========================================================
function lQDF:place_in_data_frame(key, col)
  assert(type(key) == "string"); assert(#key > 0)
  assert(type(col) == "lQDF")
  assert(col:jtype() == "j_array")
  assert(self:is_data_frame())
  assert(self:is_key(key))
  assert(col:arr_len() <= self:obj_arr_size())
  print("Calling C", key)
  --==========================================================
  local status = cQDF.place_in_data_frame(self:cmem_ptr(), key, 
    col:cmem_ptr())
  assert(status == 0)
  if ( is_debug ) then assert(self:check()) end 
  return self
end
--==========================================================
-- Given 2 columns, create an index field which indicates order
-- assuming we want column 1 to be sorted ascending (primary) and 
-- column 2 to be sorted ascending (secondary) 
-- So, if input is columns s1, s2 and 
-- we return column idx and idx[0] == j then
-- it means that s1[j] == min_{k=1}^k=n} s1[k]
-- Also, let X = all the values of s2 for which the corresponding value 
-- of s1 == s1[j]. Them, s2[j] = min(X)
function lQDF.mk_sort_idx_2(s1, s2)
  assert(type(s1) == "lQDF")
  assert(s1:jtype() == "j_array")
  local s1qtype = s1:qtype()

  assert(type(s2) == "lQDF")
  assert(s2:jtype() == "j_array")
  local s2qtype = s2:qtype()

  local cqdf = lqdfmem(0)
  local cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem))
  local fn_key = "mk_sort_idx_" .. s1qtype .. "_" .. s2qtype
  local fn = assert(cQDF[fn_key])
  local status = fn(s1:cmem_ptr(), s2:cmem_ptr(), cqdf_ptr)
  assert(status == 0)
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = cqdf
  if ( is_debug ) then
    assert(newqdf:qtype() == "I4" ) -- alwasy return I4 
    assert(newqdf:jtype() == "j_array" )
  end
  return newqdf
end
--==========================================================
function lQDF:permute(idx, direction)
  if ( type(direction) == "nil") then
    direction = "forward"
  end
  assert(type(direction) == "string")
  assert(type(idx) == "lQDF")
  assert(idx:jtype() == "j_array")
  -- idx:qtype() must be I1/I2/I4/I8

  local cqdf = lqdfmem(0)
  local cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem))
  local status = cQDF.permute(self:cmem_ptr(), idx:cmem_ptr(), 
    direction, cqdf_ptr)
  assert(status == 0)
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = cqdf
  return newqdf
end
--==========================================================
function lQDF:srt_grp_cnt()
  local cqdf = lqdfmem(0)
  local cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem))
  local status = cQDF.srt_grp_cnt(self:cmem_ptr(), cqdf_ptr)
  assert(status == 0)
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = cqdf
  return newqdf
end
function lQDF:fill(n_in_grp, tim)
  local nn = self:get_nn()
  assert(type(nn) == "lQDF", "No null values: do not call fill")
  assert(type(n_in_grp) == "lQDF")
  assert(type(tim) == "lQDF")
  local cqdf = lqdfmem(0)
  local cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem))
  local status = cQDF.fill(self:cmem_ptr(), nn:cmem_ptr(), 
    n_in_grp:cmem_ptr(), tim:cmem_ptr(), cqdf_ptr)
  assert(status == 0)
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = cqdf
  return newqdf
end

function lQDF:t_epoch_to_tm(XC)
  assert( ( type(XC) == "userdata") or ( type(XC) == "cdata") )
  local cqdf = lqdfmem(0)
  local cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem))
  local status = cQDF.t_epoch_to_tm(self:cmem_ptr(), XC, cqdf_ptr)
  assert(status == 0)
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = cqdf
  return newqdf
end
function lQDF:prefix_sums(direction)
  if ( direction ) then
    assert(type(direction) == "string")
    assert( ( direction == "forward" ) or ( direction == "backward" ) )
  else
    direction = "forward"
  end
  local cqdf = lqdfmem(0)
  local cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem))
  local status = cQDF.prefix_sums(self:cmem_ptr(), direction, cqdf_ptr)
  assert(status == 0)
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = cqdf
  return newqdf
end
function lQDF:smear(offset, src_dirn)
  -- src_dirn == source direction. In other words, do you use the offset
  -- to get something ahead of you (forward) or something before you
  -- (backward)
  assert(type(offset) == "lQDF")
  if ( src_dirn ) then
    assert(type(src_dirn) == "string")
    assert( ( src_dirn == "forward" ) or ( src_dirn == "backward" ) )
  else
    src_dirn = "forward"
  end
  local cqdf = lqdfmem(0)
  local cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem))
  local status = cQDF.smear(self:cmem_ptr(), offset:cmem_ptr(), 
    src_dirn, cqdf_ptr)
  assert(status == 0)
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = cqdf
  return newqdf
end
--==========================================================
function lQDF:bool_val()
  assert(self:jtype() == "j_bool")
  local x = cQDF.x_get_bool_val(self:cmem_ptr())
  return x
end
--==========================================================
function lQDF:num_val()
  assert(self:jtype() == "j_number")
  local x = cQDF.x_get_num_val(self:cmem_ptr())
  return x
end
--==========================================================
function lQDF:name()
  local x = cQDF.qdf_cmem_get_name(self:cmem_ptr())
  if ( x == ffi.NULL ) then 
    return nil 
  else
    return ffi.string(x)
  end
end
--==========================================================
function lQDF:set_name(name)
  local status = cQDF.qdf_cmem_set_name(self:cmem_ptr(), name)
  return self
end
--==========================================================
function lQDF:vals_counts()
  local vals = lqdfmem(0)
  local vals_ptr = assert(ffi.cast("QDF_REC_TYPE *", vals._qdfmem))
  local cnts = lqdfmem(0)
  local cnts_ptr = assert(ffi.cast("QDF_REC_TYPE *", cnts._qdfmem))

  local status = cQDF.vals_counts(self:cmem_ptr(), vals_ptr, cnts_ptr)
  assert(status == 0)

  local vals_qdf = setmetatable({}, lQDF)
  vals_qdf._cmem = vals

  local cnts_qdf = setmetatable({}, lQDF)
  cnts_qdf._cmem = cnts
  return vals_qdf, cnts_qdf
end

function lQDF.vals_sums(key, num)
  assert(type(key) == "lQDF"); assert(key:jtype() == "j_array")
  assert(type(num) == "lQDF"); assert(num:jtype() == "j_array")

  local vals = lqdfmem(0); 
  local vals_ptr = assert(ffi.cast("QDF_REC_TYPE *", vals._qdfmem))
  local cnts = lqdfmem(0); 
  local cnts_ptr = assert(ffi.cast("QDF_REC_TYPE *", cnts._qdfmem))

  local key_ptr = key:cmem_ptr()
  local num_ptr = num:cmem_ptr()
  local status = cQDF.vals_sums(key_ptr, num_ptr, vals_ptr, cnts_ptr)
  assert(status == 0)

  local vals_qdf = setmetatable({}, lQDF)
  vals_qdf._cmem = vals

  local cnts_qdf = setmetatable({}, lQDF)
  cnts_qdf._cmem = cnts
  return vals_qdf, cnts_qdf
end

function lQDF:junk()
  --[[ Used to test that the following are equivalent
  x:junk()
  lQDF.junk()
  --]]
  print("hello world")
end
--======================================================
-- Let m = by
-- dst[m-1..n-1] == undefined
-- dst[i] = src[i+m] 
function lQDF:shift_up(by)
  assert(type(by) == "number"); assert(by > 0)
  return self:shift_up_down(by, "up")
end
-- Let m = by
-- dst[0..m-1] == undefined
-- dst[i] = src[i-m] 
function lQDF:shift_down(by)
  assert(type(by) == "number"); assert(by > 0)
  return self:shift_up_down(by, "down")
end
--==================================================================
function lQDF:shift_up_down(by, direction)
  assert(type(by) == "number")
  assert(type(direction) == "string")
  assert(by ~= 0)
  local cqdf    = lqdfmem(0)
  local cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem))
  local nn_cqdf = lqdfmem(0)
  local nn_cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", nn_cqdf._qdfmem))
  local status = cQDF.shift_up_down(self:cmem_ptr(), by, 
    direction, cqdf_ptr, nn_cqdf_ptr)
  assert(status == 0)
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = cqdf
  local nn_newqdf = setmetatable({}, lQDF)
  nn_newqdf._cmem = nn_cqdf
  return newqdf, nn_newqdf
end
--==================================================================
function lQDF:grp_sawtooth(n_grp, dst_qtype)
  assert(type(n_grp) == "lQDF")
  assert(n_grp:jtype() == "j_array")
  if ( type(dst_qtype) ~= nil ) then 
    dst_qtype = ffi.NULL
  else
    assert(type(dst_qtype) == "string")
  end
  --==========================================
  local cqdf    = lqdfmem(0)
  local cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem))
  local status = cQDF.qdf_grp_sawtooth(self:cmem_ptr(), n_grp:cmem_ptr(),
    dst_qtype, cqdf_ptr)
  assert(status == 0)
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = cqdf
  return newqdf
end
--==================================================================
function lQDF:grp_flatten_sawtooth(n_grp)
  assert(type(n_grp) == "lQDF")
  assert(n_grp:jtype() == "j_array")
  --==========================================
  local cqdf    = lqdfmem(0)
  local cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem))
  local status = cQDF.qdf_grp_flatten_sawtooth(self:cmem_ptr(), 
    n_grp:cmem_ptr(), cqdf_ptr)
  assert(status == 0)
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = cqdf
  return newqdf
end
--==================================================================
function lQDF.next_prev(x, op, cmp)
  local rslt = ffi.new("bool[?]", 1)
  rslt = ffi.cast("bool *", rslt)
  local status = cQDF.qdf_next_prev(x:cmem_ptr(), op, cmp, rslt)
  assert(status == 0)
  return rslt[0]

end
function lQDF:is_prev(cmp)
  assert(type(cmp) == "string")
  return lQDF.next_prev(self, "is_prev", cmp)
end
function lQDF:is_next(cmp)
  assert(type(cmp) == "string")
  return lQDF.next_prev(self, "is_next", cmp)
end
function lQDF:clone()
  local cqdf    = lqdfmem(0)
  local cqdf_ptr = assert(ffi.cast("QDF_REC_TYPE *", cqdf._qdfmem))
  local status = cQDF.qdf_clone(self:cmem_ptr(), cqdf_ptr)
  assert(status == 0)
  local newqdf = setmetatable({}, lQDF)
  newqdf._cmem = cqdf
  return newqdf
end
return lQDF
