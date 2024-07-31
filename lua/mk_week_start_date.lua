-- TODO P1 require 'strict'
local lQDF = require 'lQDF'
local ffi  = require 'ffi'
local function mk_week_start_date(
  week_file
  )
  -- START: Create H for decompress
  local format = "%Y-%m-%d"
  local H = ffi.new("date_to_hol_t[?]", 1)
  H = ffi.cast("date_to_hol_t *", H)
  local mtmgcv = ffi.load("mtmgcv.so")
  local status = mtmgcv.read_date_to_hol(week_file, format, H)
  assert(status == 0)
  local nH = H[0].n
  H = ffi.cast("date_hol_t *", H[0].date_hol)

  return H, nH
end
return mk_week_start_date
