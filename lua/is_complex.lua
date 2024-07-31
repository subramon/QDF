local jtypes = require 'jtypes'
local function is_complex(jtype) 
  if ( ( ( jtype == jtypes.j_array ) or
         ( jtype == jtypes.j_object ) ) ) then
    return true
  end
  -- print("jtype = ", jtype)
  return false
end
return is_complex
