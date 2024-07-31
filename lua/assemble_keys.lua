local function assemble_keys(X)
  assert(type(X) == "table")
  local K = {}
  for k, v in pairs(X) do
    assert(type(k) == "string")
    K[#K+1] = k
  end
  return K
end
return  assemble_keys
--[[ quick test
local T = {}
T.abc = 123
T.def = true
T.ghi = "some string"
local K = assemble_keys(T)
for k, v in pairs(K) do print(k, v) end 
--]]
