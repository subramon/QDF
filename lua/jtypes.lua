-- TODO P2 Figure out how to keep these in sync with enum in C
local J = {}
local idx = 0
J["j_undef"]   =  idx; idx = idx + 1 
J["j_nil"]     =  idx; idx = idx + 1 
J["j_string"]  =  idx; idx = idx + 1 
J["j_number"]  =  idx; idx = idx + 1 
J["j_date"]    =  idx; idx = idx + 1 
J["j_array"]   =  idx; idx = idx + 1 
J["j_object"]  =  idx; idx = idx + 1 
J["j_bool"]    =  idx; idx = idx + 1 

local rev_J = {}
for k, v in pairs(J) do
  rev_J[v] = k
end
assert(type(rev_J) == "table")
return J, rev_J
