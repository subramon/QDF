local qtypes = require 'qtypes'
local function is_numeric(qtype)
  if ( qtype == qtypes.I1 ) then return true end 
  if ( qtype == qtypes.I2 ) then return true end 
  if ( qtype == qtypes.I4 ) then return true end 
  if ( qtype == qtypes.I8 ) then return true end 
  if ( qtype == qtypes.F2 ) then return true end 
  if ( qtype == qtypes.F4 ) then return true end 
  if ( qtype == qtypes.F8 ) then return true end 
  if ( qtype == qtypes.UI1 ) then return true end 
  if ( qtype == qtypes.UI2 ) then return true end 
  if ( qtype == qtypes.UI4 ) then return true end 
  if ( qtype == qtypes.UI8 ) then return true end 
  return false
end
return is_numeric
