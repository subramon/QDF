-- Creating a new operator involves writing a file like this 
local C = {} -- table of stuff to return 
local function get_subs(x, y)
  local operator = "coalesce"
  local cfunc = { operator , "_", x:qtype(), "_", y:qtype(), }
  cfunc = table.concat(cfunc, "")
  return { 
    operator = operator, -- NECESSARY
    -- following for substitutions needed by LaTeX spec file 
    __QTYPEX__ = x:qtype(),  
    __QTYPEY__ = y:qtype(), 
    __CFUNC__ = cfunc, 
  }
end
C.get_subs         = get_subs -- function
C.generic_tex_spec_file = "coalesce.tex"
C.lua_calling_spec = "bogus spec for now" 
-- TODO C.run should be created programmatically from C.lua_calling_spec
-- C.run() is invoked when lQDF.coalesce() is called 
C.run = function (x, y) 
  -- Needed here as well because a dump() of function does not
  -- capture upvalues 
  local lQDF        = require 'lQDF' 
  local lqdfmem     = require 'lqdfmem'
  local condl_specl = require 'condl_specl'
  -- check types
  assert(type(x) == "lQDF")
  assert(type(y) == "lQDF")
  local get_subs = lQDF.get_get_subs("coalesce")
  local subs = get_subs(x, y)
  local cfunc, exec = condl_specl(subs)
  -- Now to call C from Lua 
  local cx = x:cmem_ptr() -- provide C access to x
  local cy = y:cmem_ptr() -- provide C access to y
  local z = lqdfmem(0) -- create an empty QDF for z 
  local zqdf = setmetatable({}, lQDF)
  zqdf._cmem = z
  local cz = zqdf:cmem_ptr()  -- provide C access to z
  local exec = assert(lQDF.q_get(cfunc)) -- get C function 
  local status = exec[cfunc](cx, cy, cz) -- execute C function
  -- TODO P0 assert(status == 0) 
  return zqdf
end
C.test = function() print("test function for coalesce") end 
return C
