local lRBC = require 'lRBC'
local cmem = require 'libcmem'
local ffi  = require 'ffi'
local calc_offset = require 'calc_offset' -- gamm/

local tests = {}

-- WARNING! Make sure that infile and outfile are sorted in same way
-- on week_start_date, co_loc_i
--
tests.t1 = function(infile, outfile)
  local status
  local in_col_names =  { 
    "mdse_item_i",
    "co_loc_ref_i",
    "dist_loc_i",
    "sls_unit_q",
    "week_start_date",

    "circular_flag",
    "tpc_flag",
    "baseprice",
    "offer_price",
    "beta",
    "alpha_tpc",
    "alpha_circ",
  }
  local in_qtypes = { "I4", "I2", "I2", "F4", "TM:%m/%d/%y", 
  "F4", "F4", "F4", "F4", "F4", "F4", "F4", }
  local optargs = { is_hdr = true }
  local indf = assert(lRBC.read_csv(in_col_names, in_qtypes, infile, optargs))
  assert(type(indf) == "lRBC")

  local out_col_names = {
    "week_start_date",
    "co_loc_ref_i",
    "baseprice_lift",
    "promo_lift",
    "promo_price_lift",
  }
  local out_qtypes = {
    "TM:%Y-%m-%d",
    "I2",
    "F4",
    "F4",
    "F4",
  }
  local outdf = assert(lRBC.read_csv(out_col_names, out_qtypes, outfile, optargs))
  assert(type(outdf) == "lRBC")

  local in_promo_price_lift = calc_offset(indf)
  local out_promo_price_lift = outdf:get("promo_price_lift")

  -- TODO Check that in and out are the same 

  print("Test t1 completed successfully")
end
local infile = "data_calc_offset_1_in.csv"
local outfile = "data_calc_offset_1_out.csv"
tests.t1(infile, outfile)
-- return tests
