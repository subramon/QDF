local lrbcmem = require 'lrbcmem'
local ffi = require 'ffi'

for i = 1, 100 do
  local size = 10
  local x = lrbcmem(size);
  assert(type(x) == "lrbcmem")
  assert(x:size() == size)
end
for i = 1, 100 do
  local size = 16
  local name = "name_" .. tostring(i)
  local x = lrbcmem({ size = size, name = name })
  assert(type(x) == "lrbcmem")
  assert(x:name() == name)
  assert(x:size() == size)
  x:nop()
end
for i = 1, 100 do
  local size = 16
  local name = "name_" .. tostring(i)
  local x = lrbcmem({ size = size, name = name })
  local d = assert(x:ptr())
  d = ffi.cast("RBC_REC_TYPE *", d)
  assert(d ~= ffi.NULL)
  assert(name == ffi.string(d[0].name))
end
-- okay to create an empty lrbcmem
for i = 1, 100 do
  local x = lrbcmem(0);
  assert(type(x) == "lrbcmem")
  local d = assert(x:ptr())
  assert(d ~= ffi.NULL)
  assert(x:name() == nil)
  assert(x:data() == nil)
end
-- testing clone()
for i = 1, 100 do
  local size = 16
  local name = "name_" .. tostring(i)
  local x = lrbcmem({ size = size, name = name })
  local y = x:clone()
  assert(type(y) == "lrbcmem")
  local d = assert(y:ptr())
  d = ffi.cast("RBC_REC_TYPE *", d)
  assert(d ~= ffi.NULL)
  assert(name == ffi.string(d[0].name))
  local p1 = x:ptr()
  local p2 = y:ptr()
  assert(p1 ~= p2)
  assert(x:data() ~= y:data())
end
-- testing dupe()
for i = 1, 100 do
  local size = 16
  local name = "name_" .. tostring(i)
  local x = lrbcmem({ size = size, name = name })
  local y = x:dupe()
  assert(type(y) == "lrbcmem")
  local d = assert(y:ptr())
  d = ffi.cast("RBC_REC_TYPE *", d)
  assert(d ~= ffi.NULL)
  assert(name == ffi.string(d[0].name))
  local p1 = x:ptr()
  local p2 = y:ptr()
  assert(x:data() == y:data())
  -- now delete new guy, old guy should exist
  y = nil
  collectgarbage()
  assert(x:name() == name)
  assert(x:size() == size)

end
collectgarbage()
print("All done")
