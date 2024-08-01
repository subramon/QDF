local function unsigned_qtype( qtype)
  assert(type(qtype) == "string")
  if ( qtype == "I1" ) then return "UI1" end 
  if ( qtype == "I2" ) then return "UI2" end 
  if ( qtype == "I4" ) then return "UI4" end 
  if ( qtype == "I8" ) then return "UI8" end 
  error("bad value of qtype" ..  qtype)
end
return  unsigned_qtype
