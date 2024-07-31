-- TODO P2 Need to improve how Q environment is set 
local exec   = require 'exec_and_capture_stdout'
local env_str 
local function qenvs()
  if ( not env_str ) then 
    local T = {}
    local home = os.getenv("HOME")
    local cmd = 'bash ' .. home .. '/Q/setup.sh'
    local rslt = exec(cmd)
    for w in rslt:gmatch("([^\n]+)") do 
      w = string.gsub(w, "^", "export ")
      w = string.gsub(w, "= ", "=\"")
      w = string.gsub(w, "$", "\"")
      T[#T+1] = w
    end
    local path = os.getenv("PATH")
    local home = os.getenv("HOME")
    T[#T+1] = 'export PATH=' .. path .. ':' .. home .. '/local/Q/bin'
    env_str =  table.concat(T, "; ")
  end
  return env_str
end
return qenvs
