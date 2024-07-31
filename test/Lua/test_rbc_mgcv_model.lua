G = {} -- to suppress strict
G.debug= true
require 'strict'
local ffi = require 'ffi'
local lstringify = require 'lstringify'
local X = ffi.load('configs.so')
local cRBC = ffi.load('rbc.so')
local lrbcmem = require 'lrbcmem'
local lRBC = require 'lRBC'
-- ffi.cdef([[
-- extern size_t strlen(const char *s);
-- extern void *malloc(size_t size);
-- extern void free(void *ptr);
-- typedef struct {
--    char *fpos;
--    void *base;
--    unsigned short handle;
--    short flags;
--    short unget;
--    unsigned long alloc;
--    unsigned short buffincrement;
-- } FILE;
-- ]])
-- local RBC_hdrs = require 'RBC_hdrs'
-- ffi.cdef(RBC_hdrs)
local mgcv_hdrs = require 'mgcv_hdrs'
ffi.cdef(mgcv_hdrs)

local n_components = 3 -- Assume 3 components 
local Ccomponents = ffi.new("component_t[?]", n_components)
Ccomponents = ffi.cast("component_t *", Ccomponents)
Ccomponents[0].name = lstringify("columbus_day")
Ccomponents[0].Rstr = lstringify("columbus_day")
Ccomponents[0].model_type = 1;

Ccomponents[1].name = lstringify("co_loc_i")
Ccomponents[1].Rstr = lstringify("s(co_loc_i, bs= 're')")
Ccomponents[1].model_type = 2;

Ccomponents[2].name = lstringify("t_o_y")
Ccomponents[2].Rstr = lstringify("s(t_o_y, bs = 'cc, k = 25)")
Ccomponents[2].model_type = 3;

--================================
local nF = 1
local M = ffi.new("mgcv_model_struct_t[?]", nF)
M = ffi.cast("mgcv_model_struct_t *", M)
--================================
-- Create model for one formula. 
M[0].n_components = n_components 

local components = ffi.new("model_class_t[?]", n_components)
components = ffi.cast("model_class_t *", components)
-- Set component 1 
components[0].model_type = 1
components[0].m.m1.c  = 0.5; 
--============================================
-- Set component 2
local n = 4 
components[1].m.m2.n = n
local f = ffi.new("int[?]", n)
f = ffi.cast("int *", f)
for i = 1, n do f[i-1] = i end 
components[1].m.m2.f = f

local c = ffi.new("double[?]", n)
c = ffi.cast("double *", c)
for i = 1, n do c[i-1] = 1.0/(2*i) end 
components[1].m.m2.c = c
components[1].model_type = 2
--============================================
-- Set component 3 
local n = 5 
components[2].m.m3.y0 = 0.1
local k = ffi.new("double[?]", n)
k = ffi.cast("double *", k)
for i = 1, n do k[i-1] = 3*i end 
components[2].m.m3.k = k

local d = ffi.new("double[?]", n)
d = ffi.cast("double *", d)
for i = 1, n do d[i-1] = 4*i end 
components[2].m.m3.d = d

components[2].m.m3.n = n
components[2].m.m3.is_by = false;  
components[2].model_type = 3
--============================================
M[0].intercept = 1234.5678
M[0].mdl_for_cmp = components
--============================================

local wrk_out = ffi.new("mgcv_worker_output_t[?]", 1)
wrk_out = ffi.cast("mgcv_worker_output_t *", wrk_out)

wrk_out[0].mgcv_model = M[0]
wrk_out[0].key_frmla = lstringify("f1")
wrk_out[0].is_valid = true
wrk_out = ffi.cast("const mgcv_worker_output_t * const ", wrk_out)

-- load configs
local cfg_dir = "mgcv"
local dsid = ffi.new("dsid_t[?]", 1)
ffi.fill(dsid, ffi.sizeof("dsid_t"))
dsid = ffi.cast("dsid_t *", dsid)

local C = ffi.new("config_t[?]", 1)
ffi.fill(C, ffi.sizeof("config_t"))
C = ffi.cast("config_t *", C)
local status = X.bridge_read_configs(cfg_dir, C)
assert(status == 0)
-----
local crbc = lrbcmem(0)
local crbc_ptr = ffi.cast("RBC_REC_TYPE *", crbc._rbcmem)
local status = X.rbc_mgcv_model(C, dsid[0], M, crbc_ptr)
assert(status == 0)

status = X.chk_rbc(crbc_ptr)
assert(status == 0)
local newrbc = setmetatable({}, lRBC)
newrbc._cmem = crbc
print(newrbc)

print("Printing done")

print("Test rbc_mgcv_model executed successfully")
