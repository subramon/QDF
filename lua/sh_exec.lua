local function sh_exec(cmd)
  local handle = io.popen(cmd)
  if ( handle == nil ) then return nil end
  local rslt = handle:read("*a")
  handle:close()
  return rslt
end
return sh_exec
