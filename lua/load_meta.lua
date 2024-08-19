local ffi       = require 'ffi'
local stringify = require 'RSUTILS/lua/stringify'
local function load_meta(M)
  if ( not M ) then return end
    
  local n_cols = #M

  local col_names =  ffi.C.malloc(n_cols *  ffi.sizeof("char *"))
  ffi.fill(col_names, (n_cols *  ffi.sizeof("char *")), 0)
  col_names = ffi.cast("char **", col_names)

  local qtypes    =  ffi.C.malloc(n_cols *  ffi.sizeof("char *"))
  ffi.fill(qtypes, 0,  (n_cols *  ffi.sizeof("char *")), 0)
  qtypes = ffi.cast("char * *", qtypes)

  local is_load   =  ffi.C.malloc(n_cols *  ffi.sizeof("bool"))
  ffi.fill(is_load, (n_cols *  ffi.sizeof("bool")), 0)
  is_load = ffi.cast("bool *", is_load)

  local has_nulls   =  ffi.C.malloc(n_cols *  ffi.sizeof("bool"))
  ffi.fill(has_nulls, (n_cols *  ffi.sizeof("bool")), 0)
  has_nulls = ffi.cast("bool *", has_nulls)

  for i =  1, n_cols do

    local x = string.format("M[%d].name", i)
    col_names[i-1] =  stringify(M[i].name, x)
    local x = string.format("M[%d].qtype", i)
    qtypes[i-1]    =  stringify(M[i].qtype, x)

    is_load[i-1]   = true -- default assumption 
    if ( M[i].is_load ~= nil ) then
      assert(type(M[i].is_load) == "boolean")
      is_load[i-1]   =            M[i].is_load
    end

    has_nulls[i-1]   = true -- default assumption
    if ( M[i].has_nulls ~= nil ) then
      assert(type(M[i].has_nulls) == "boolean")
      has_nulls[i-1]   = M[i].has_nulls
    end
  end
  return n_cols, col_names, qtypes, is_load, has_nulls   
end
return load_meta
