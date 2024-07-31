local J = require 'jtypes'
local rev_J = {}
for k, v in pairs(J) do
  rev_J[v] = k
end
assert(type(rev_J) == "table")
return rev_J
