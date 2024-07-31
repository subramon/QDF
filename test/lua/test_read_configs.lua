local ffi = require 'ffi'
local utils         = ffi.load("utils.so")
local cRBC          = ffi.load("rbc.so")
local cmem          = require 'libcmem'
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
local RBC_hdrs = require 'RBC_hdrs' -- created by ../src/Makefile 
ffi.cdef(RBC_hdrs)
local utils_hdrs = require 'utils_hdrs' -- created by ../src/Makefile 
ffi.cdef(utils_hdrs)
local mgcv_hdrs = require 'mgcv_hdrs' -- created by ../src/Makefile 
ffi.cdef(mgcv_hdrs)
--================== 
print("All done")
