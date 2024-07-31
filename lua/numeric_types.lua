local N = { "I1", "I2", "I4", "I8", "UI1", "UI2", "UI4", "UI8", "F4", "F8" }

local M = {}
for k, v in ipairs(N) do 
  M[v] = true
end
return M
