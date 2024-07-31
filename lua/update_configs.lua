local plp = require 'pl.pretty'
local plfile = require 'pl.file'
local json = require 'json'
local function update_config(mode, name, value)
  assert(type(G) == "table") -- global must exist in Lua state 
  assert(type(mode) == "string")
  assert(type(name) == "string")
  assert(type(value) == "string")
  assert((mode == "read") or ( mode == "write") )

  if ( name == "Recency" ) then 
    local cfg_dir = assert(G.C.cfg_dir)
    local C = require(cfg_dir .. "/configs")
    assert(type(C) == "table")
    if ( mode == "read" ) then 
      local rval = {} -- return values
      local r = assert(C.recency_in_weeks)
      assert(type(r) == "number")
      rval.Recency = r
      return json.encode(rval)
    end
    --==============================================
    local recency_in_weeks = assert(tonumber(value))
    assert(recency_in_weeks >= 0)
    C.recency_in_weeks = recency_in_weeks
    local C_as_str = plp.write(C, ' ')
    local T = {}
    T[#T+1] = assert(os.getenv("LIGHTR_ROOT"))
    T[#T+1] = "/setup/"
    T[#T+1] = C.cfg_dir
    T[#T+1] = "/configs.lua"
    local outfile = table.concat(T, "")
    plfile.write(outfile, "return " ..  C_as_str)
    local rval = {}
    rval.UpdateRecency = "OK"
    return json.encode(rval)
  else
    error("Unknown parameter " .. name)
  end
  error("Control should not come here")
end
return update_config
--[[ local testing 
G = require 'mgcv'
update_config("read", "Recency", 10)
update_config("write", "Recency", 10)

--]]
