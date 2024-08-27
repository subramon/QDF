-- Is this a table or an object (in JSON terminology)
local function is_jarray(T)
  assert(type(T) == "table")
  --== Check if table or object
  local is_jarray

  local nT = #T
  local idx = 1 
  local cnt = 0
  for k, v in pairs(T) do 
    cnt = cnt + 1 
  end
  -- TODO P0 assert(cnt > 0) -- TODO Need to undo this assumption
  if ( cnt ~= nT ) then return false end -- this is an object
  -- Now we *think* its a table but one more check needed
  -- All T[i] from i = 1 to i = n must be defined
  for i = 1, cnt do 
    if ( ( type(T[i]) ~= "boolean" ) and ( not T[i]) ) then
      return false
    end
  end
  return true
end
return is_jarray
