-- I should have documented why I created this capability
-- I believe it was because I was running into trouble
-- when using JNI
local ffi               = require 'ffi'
local register_type	= require 'register_type'
--[==[
---- enable commented section for stand alone testing 
ffi.cdef([[
       void *malloc(size_t size);
       void free(void *ptr);
typedef struct _qdf_rec_type {
  void *data;
  char *name;
  uint32_t size;
  bool is_mmap; // we have mmapped data not malloc'd it 
  bool is_foreign; // true => do not delete 
  bool is_read_only; // true => don't modify
} QDF_REC_TYPE;
extern QDF_REC_TYPE *
qdf_cmem_make(
    size_t sz,
    const char * const name
);
extern int 
qdf_cmem_free(
  QDF_REC_TYPE *x
);
extern void
qdf_cmem_nop(
  QDF_REC_TYPE *x
);
extern QDF_REC_TYPE *
qdf_cmem_clone(
    QDF_REC_TYPE *y
    );
extern QDF_REC_TYPE *
qdf_cmem_dupe(
    QDF_REC_TYPE *y
    );
]]
)
--]==]

local lib = ffi.load("libqdf.so")
--====================================
local lqdfmem = {}
lqdfmem.__index = lqdfmem

setmetatable(lqdfmem, {
   __call = function (cls, ...)
      return cls.new(...)
   end,
})

register_type(lqdfmem, "lqdfmem")
function lqdfmem.new(args)
  local qdfmem = setmetatable({}, lqdfmem)
  qdfmem._meta = {} -- for meta data stored in qdfmem
  local size
  local c_name = ffi.NULL
  if ( type(args) == "number" ) then
    size = args
  elseif ( type(args) == "table" ) then
    size = assert(args.size)
  end
  assert(type(size) == "number")
  assert(size <= 4294967295) -- we do support larger sizes
  --=======================================
  if ( type(args) == "table" ) then
    if ( args.name ) then 
      assert(type(args.name) == "string")
      c_name = args.name
    end
  end
  --=======================================
  qdfmem._qdfmem = 
    ffi.gc(lib.qdf_cmem_make(size, c_name), lib.qdf_cmem_free)
  --=======================================
  return qdfmem
end
function lqdfmem:ptr()
  local x = ffi.cast("QDF_REC_TYPE *", self._qdfmem)
  return x
end
function lqdfmem:size()
  local x = ffi.cast("QDF_REC_TYPE *", self._qdfmem)
  return tonumber(x[0].size)
end
function lqdfmem:name()
  local x = ffi.cast("QDF_REC_TYPE *", self._qdfmem)
  if ( x[0].name == ffi.NULL ) then return nil end 
  return ffi.string(x[0].name)
end
function lqdfmem:data()
  local x = ffi.cast("QDF_REC_TYPE *", self._qdfmem)
  if ( x[0].data == ffi.NULL ) then return nil end 
  return x[0].data
end
function lqdfmem:nop()
  local x = ffi.cast("QDF_REC_TYPE *", self._qdfmem)
  lib.qdf_cmem_nop(self._qdfmem)
end
function lqdfmem:clone()
  local x = ffi.cast("QDF_REC_TYPE *", self._qdfmem)

  local qdfmem = setmetatable({}, lqdfmem)
  qdfmem._meta = {} -- for meta data stored in qdfmem
  qdfmem._qdfmem = ffi.gc(lib.qdf_cmem_clone(x), lib.qdf_cmem_free) 
  return qdfmem
end
function lqdfmem:dupe()
  local x = ffi.cast("QDF_REC_TYPE *", self._qdfmem)
  local cmem = lib.qdf_cmem_dupe(self._qdfmem)
  local qdfmem = setmetatable({}, lqdfmem)
  qdfmem._qdfmem = cmem
  return qdfmem
end
return lqdfmem
