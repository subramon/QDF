local cmem          = require 'libcmem'
local function mk_concat_keys(keys)
  assert(type(keys) == "table")
  -- calculate space for keys
  local len_keys = 0
  local n_keys = 0
  for _, key in ipairs(keys) do 
    assert(type(key) == "string")
    len_keys = len_keys + #key
    n_keys = n_keys + 1 
  end
  assert(n_keys >= 1)
  len_keys = len_keys + n_keys -- +1 for nullc for each key
  -- allocate keys
  local concat_keys = cmem.new(len_keys)
  -- copy keys 
  local  offset = 0
  for i, key in ipairs(keys) do 
    assert(cmem.strcat(concat_keys, key, offset))
    offset = offset + (#key + 1)
  end
  return  concat_keys, n_keys 
end
return  mk_concat_keys
