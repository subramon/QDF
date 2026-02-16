-- register is used *ONLY* to register stuff 
-- All operators register themselves with QDF using register

-- TODO Why do we need setDefault and a meta-table at all?
local function setDefault (t, d)
  local mt = {
    __index = function (t, k) 
        print("Not registered", k) 
        return d 
      end
  }
  setmetatable(t, mt)
end

local qfns = {} -- list of functions registered for QDF
qfns[0] = function () return nil end 
setDefault(qfns, qfns[0])

qfns.register = function(fname, tbl)
  assert(type(fname) == "string")
  assert(#fname > 0)
  assert(type(tbl) == "table")
  assert(type(tbl.run) == "function") -- must exist 
  assert(type(tbl.get_subs) == "function") -- must exist 

  assert(not rawget(qfns, fname))
  qfns[fname] = tbl
  return tbl
end
return qfns
