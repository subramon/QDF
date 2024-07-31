local lRBC = require 'lRBC'
local tests = {}

tests.read_csv_1 = function ()
  local col_names =  {
"sls_unit_q","co_loc_i","baseprice_lift","promo_lift","promo_price_lift","columbusday","goodfriday","easter","fathersday.minus","fathersday","laborday.minus","laborday","mardigras","memorialday.minus","memorialday","mlkday","mothersday.minus","mothersday","presidentsday","superbowl.minus","superbowl","thanksgiving.minus","thanksgiving","cyber.monday","valentines.minus","valentines","stpatricks","cincodemayo","julyfourth.minus","julyfourth","halloween.minus","halloween","veteransday","christmas.minus","christmas","newyearsday.minus","newyearsday","t_o_y","n_week","n_ind", }
local qtypes = { "I4", "I2", "F4", "F4", "F4", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I1", "I4", "I2", "I1", }
  local ncols  = #col_names
  assert(#qtypes == #col_names)
  local infile = "in1.csv"
  local optargs = { is_hdr = true }
  local x = assert(lRBC.read_csv(col_names, qtypes, infile, optargs))
  assert(type(x) == "lRBC")
  assert(x:jtype() == "j_object")
  local K = x:keys()
  -- for k, v in pairs(K) do print(k, v) end 
  for k1, v1 in pairs(K) do 
    local found = false
    for k2, v2 in pairs(col_names) do 
      if ( v2 == v1 ) then
        found = true; break
      end
    end
    assert(found)
  end
  for i, v in ipairs(col_names) do
    local y = x:get(v)
    assert(type(y) == "lRBC")
    assert(y:jtype() == "j_array")
    assert(y:qtype() == qtypes[i])
  end

  print("Test read_csv_1 completed successfully")
end
tests.read_csv_1()
-- return tests
