local lRBC = require 'lRBC'
require 'strict'
local infiles= {
  "./data/merge11.csv",
  "./data/merge12.csv",
  "./data/merge13.csv",
  "./data/merge14.csv",
  "./data/merge15.csv", 
}
local col_names = {
  "sls_unit_q", "co_loc_i", "baseprice_lift", "promo_lift", "promo_price_lift", "columbusday", "goodfriday", "easter", "fathersday.minus", "fathersday", "laborday.minus", "laborday", "mardigras", "memorialday.minus", "memorialday", "mlkday", "mothersday.minus", "mothersday", "presidentsday", "superbowl.minus", "superbowl", "thanksgiving.minus", "thanksgiving", "cyber.monday", "valentines.minus", "valentines", "stpatricks", "cincodemayo", "julyfourth.minus", "julyfourth", "halloween.minus", "halloween", "veteransday", "christmas.minus", "christmas", "newyearsday.minus", "newyearsday", "t_o_y", "n_week", "n_ind", "some_date", }
local qtypes = { "I1", "I2", "F8", "I1", "F8", "UI1", "UI2", "UI4", "UI8", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "I4", "TM1:%Y-%m-%d", }
local outfile="./data/_merged_df.csv"

local function mk_rbc_for_test_merge(infiles, col_names, qtypes)
  local ncols  = #col_names
  assert(#qtypes == #col_names)
  local rbcs = {}
  for k, infile in ipairs(infiles) do
    local optargs = { is_hdr = true }
    rbcs[k] = assert(lRBC.read_csv(col_names, qtypes, infile, optargs))
    assert(type(rbcs[k]) == "lRBC")
    rbcs[k]:check() -- checking integrity of RBC
    local binfile = string.gsub(infile, ".csv", ".bin")
    print("dumping to " .. binfile)
    rbcs[k]:bindmp(binfile)
  end
end
mk_rbc_for_test_merge(infiles, col_names, qtypes)
