local plstringx = require 'pl.stringx'
local split = plstringx.split
assert(type(split) == "function")
local function read_csv(infile, keys, optargs)
  assert(type(infile) == "string")
  assert(type(keys) == "table")
  assert(#keys >= 1)
  for k, v in ipairs(keys) do assert(type(v) == "string") end
  --====================================
  local is_hdr = false
  if ( optargs ) then 
    assert(type(optargs) == "table")
    if ( optargs.is_hdr ) then 
      assert(type(optargs.is_hdr) == "boolean")
      is_hdr = optargs.is_hdr
    end
  end
  --====================================
  io.input(infile)
  local nkeys = #keys
  local X = {}
  local lno = 1
  while true do 
    local line = io.read()
    if ( line == nil ) then break end
    local vals = split(line, ",")
    assert(#vals == nkeys)
    if ( ( is_hdr ) and ( lno == 1 ) ) then 
      for k, v in ipairs(vals) do
        assert(v == keys[k])
      end
    else
      local y = { }
      for k, v in pairs(keys) do 
        y[v] = assert(tonumber(vals[k]))
      end
      X[#X+1] = y
    end
    -- print("Read line ", lno)
    lno = lno + 1 
  end
  return X
end
return read_csv
-- for testing 
--[[
local X = read_csv("../DATA/10M_1_mdse_item_i_dist_loc_i.csv", 
  {"mdse_item_i", "dist_loc_i"}, { is_hdr = true })
for k, v in ipairs(X) do 
  print(k, v.mdse_item_i, v.dist_loc_i)
end
--]]
