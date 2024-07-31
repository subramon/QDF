local compress_modes = require 'compress_modes'
local rev_compress_modes = {}
for k, v in pairs(compress_modes) do
  rev_compress_modes[v] = k
end
return rev_compress_modes
