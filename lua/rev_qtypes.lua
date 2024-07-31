local qtypes = require 'qtypes'
local rev_qtypes = {}
for k, v in pairs(qtypes) do
  rev_qtypes[v] = k
end
return rev_qtypes
