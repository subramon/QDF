local json = require("json")
-- given a JSON string of the form 
-- { "dim1" : 123, "dim2" : 456, "dim3" : 789 }
-- return a table { 123, 456, 789 }
-- we assume that the keys are dim1, dim2, .. where
-- dim is the embed_prefix
-- and the "largest" key is dim<max_n_embeddings>
local function convert_json_str_to_tbl(
  json_embed_str, 
  embed_prefix, 
  max_n_embeddings
  )
  assert(type(json_embed_str) == "string")
  assert(#json_embed_str > 0)
  assert(type(embed_prefix) == "string")
  assert(type(max_n_embeddings) == "number")
  -- TODO P1 THIS IS A HACK BECAUSE INCOMING DATA IS NOT CSV
  json_embed_str = string.gsub(json_embed_str, "'", '"')
  local T = json.decode(json_embed_str)
  if (type(T) ~= "table") then print(json_embed_str) end 
  assert(type(T) == "table")
  local nT = 0 
  for k, v in pairs(T) do 
    nT = nT+1
  end
  assert(nT > 0)
  assert(nT <= max_n_embeddings)
  local T_in_order = {}
  for i = 1, nT do 
    local key = embed_prefix .. tostring(i)
    local dim_i = T[key]
    assert(type(dim_i) == "number")
    T_in_order[i] = dim_i
  end
  return T_in_order
end
return  convert_json_str_to_tbl
-- quick and dirty test below
--[[
local json_str = '{ "dim1" : 123, "dim2" : 456, "dim3" : 789 }'
local chk_T = {  123, 456, 789 }
local out_T = convert_json_str_to_tbl(json_str, "dim", 10)
assert(type(out_T) == "table")
assert(#out_T == #chk_T)
for i, v  in ipairs(chk_T) do
  assert(v == out_T[i])
end
print("test passed")
--]]
