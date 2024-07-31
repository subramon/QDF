-- splits string "str" on new line
-- looks for a line which contains "test status"
-- if it does, then looks for string "true" or string "false"
-- based on which it returns status
-- assert if no line matches "test status"
local function qrslt(str)
  local found = false
  local status = false
  for w in str:gmatch("([^\n]+)") do 
    if ( not found ) then 
      if ( string.find(w, "test status") ) then 
        found = true
        if ( string.find(w, "true") ) then 
          status = true
        elseif ( string.find(w, "false") ) then 
          status = false
        else
          error("XXX")
        end
      end
    end
  end
  assert(found, str)
  return status
end
return qrslt
