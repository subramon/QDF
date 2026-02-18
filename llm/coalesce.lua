-- Provide a PDF containing the specification of the operator
local cutils   = require 'libcutils'
local specialize_tex_spec = require 'specialize_tex_spec'
local make_pdf_spec = require 'make_pdf_spec'
local call_llm = function() return true end -- TODO 
local lQDF     = require 'lQDF'

local C = {}

-- Created at authoring time 
-- START: Following created when operator is authored
local function get_subs(x, y)
  local operator = "coalesce"
  local cfunc = { operator , "_", x:qtype(), "_", y:qtype(), }
  cfunc = table.concat(cfunc, "")
  return { 
    operator = operator, -- NECESSARY
    __QTYPEX__ = x:qtype(), 
    __QTYPEY__ = y:qtype(), 
    __CFUNC__ = cfunc, 
  }
end
C.get_subs         = get_subs -- function
C.generic_tex_spec_file = "coalesce.tex"
C.lua_calling_spec = "bogus spec for now"
-- STOP: Above created when operator is authored

-- C.run should be created programmatically 
C.run = function (x, y)
  -- Needed here as well because a dump() of function does not
  -- capture upvalues 
  local lQDF     = require 'lQDF' 
  local lqdfmem  = require 'lqdfmem'
  local condl_specl = require 'condl_specl'
  -- check types
  assert(type(x) == "lQDF")
  assert(type(y) == "lQDF")
  local get_subs = lQDF.get_get_subs("coalesce")
  local subs = get_subs(x, y)
  local cfunc, exec = condl_specl(subs)
  -- Now to call C from Lua 
  local cx = x:cmem_ptr() 
  local cy = y:cmem_ptr() 
  local z = lqdfmem(0)
  local zqdf = setmetatable({}, lQDF)
  zqdf._cmem = z
  local cz = zqdf:cmem_ptr() 
  local exec = assert(lQDF.q_get(cfunc))
  local status = exec[cfunc](cx, cy, cz)
  -- TODO P0 assert(status == 0) 

  return zqdf

end
C.test = function()
  print("test function hello world")
end 
return C
