local plpath = require 'pl.path'
local function chk_data_files_exist(file_name)
  assert(type(file_name) == "string")
  assert(plpath.isfile(file_name), "File missing " .. file_name)
  -- print("file_name = ", file_name)
  local x = loadfile(file_name)
  assert(type(x) == "function")
  local D = x()
  assert(type(D) == "table")
  assert(#D > 0)
  for k, v in ipairs(D) do 
    local datafile = v[1]
    assert(plpath.isfile(datafile), "File missing " .. datafile)
    -- print(datafile, " exists")
  end
  return true
end
return  chk_data_files_exist
