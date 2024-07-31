G = {} -- to suppres strict
G.debug= true
require 'strict'
local plfile        = require 'pl.file'
local ffi           = require 'ffi'
local register_type = require 'register_type'
local utils         = ffi.load("utils.so")
local cRBC          = ffi.load("rbc.so")
local cmem          = require 'libcmem'
local stringify     = require 'stringify'
local configs       = ffi.load("configs.so")
local lRBC = require 'lRBC'
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
extern int
prnt_date_to_hol(
    const date_to_hol_t *const H,
    const char * const format,
    const char * const outfile 
    );
    ]])
-- local RBC_hdrs      = require 'RBC_hdrs'
-- ffi.cdef(RBC_hdrs)
local mgcv_hdrs     = require 'mgcv_hdrs'
ffi.cdef(mgcv_hdrs)
ffi.cdef([[
extern int
read_xconfigs(
    config_t *C,
    xconfig_t *XC
    );
extern int
free_configs(
    config_t *C
    );
extern int
free_xconfigs(
    xconfig_t *XC
    );
]])
local tests = {}

tests.test1 = function ()
  local C = ffi.new("config_t[?]", 1)
  C = ffi.cast("config_t *", C)

  C[0].week_hol_file = stringify("../../test/week_start_date_to_hols.txt")
  C[0].week_hol_time_format = stringify("%Y-%m-%d")
  local XC = ffi.new("xconfig_t[?]", 1)
  configs.read_xconfigs(C, XC)
  cRBC.prnt_date_to_hol(XC[0].H, "%Y-%m-%d", "_temp.csv")
  local col_names =  { "bmask", "t_epoch", "week_start_date", }
  local qtypes = { "HL", "I4", "TM:%Y-%m-%d", }
  local infile = "_temp.csv"
  local optargs = { is_hdr = false, }
  local x = assert(lRBC.read_csv(col_names, qtypes, infile, optargs))
  assert(x:is_df())
  local nx = x:obj_arr_len()

  local t = x:get("t_epoch")
  local y = x:get("t_epoch"):t_epoch_to_hol(XC[0].H)
  assert(type(y) == "lRBC")
  assert(y:jtype() == "j_array")
  assert(y:qtype() == "HL")
  local oldn, oldptr = x:get("bmask"):get_arr_ptr()
  local newn, newptr = y:get_arr_ptr()
  oldptr = ffi.cast("uint32_t *", oldptr)
  newptr = ffi.cast("uint32_t *", newptr)
  assert(oldn == nx)
  assert(oldn == newn)
  for i = 1, oldn do 
    assert(oldptr[i-1] == newptr[i-1] )
  end
  -- Now check the holiday components for each date
  local holidays =  require 'holidays' -- lua/
  local qtypes = {}
  for i = 1, #holidays do qtypes[i] = "I1" end 
  local df = lRBC.make_empty_data_frame(holidays, qtypes, nx);
  assert(type(df) == "lRBC"); assert(df:check()); assert(df:is_df())
  for i, h in ipairs(holidays) do 
    local y = x:get("bmask"):holiday_extract(h)
    assert(type(y) == "lRBC"); 
    assert(y:jtype() == "j_array"); 
    assert(y:qtype() == "I1"); 
    df:get(h):set(y)
  end
  -- START DANGEROUS
  for i, h in ipairs(holidays) do 
    df:get(h):set_arr_len(nx)
  end
  df:set_obj_arr_len(nx)
  -- STOP  DANGEROUS
  df:pr_df_as_csv(nil, "_holidays.csv")
  os.execute("paste -d ',' _temp.csv _holidays.csv > _week_hols.csv")

  -- configs.free_configs(C)
  -- configs.free_xconfigs(XC)
  print("Test test1 completed successfully")
end
tests.test1()
-- return tests
