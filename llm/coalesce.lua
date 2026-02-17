-- Provide a PDF containing the specification of the operator
local cutils = require 'libcutils'
local specialize_tex_spec = require 'specialize_tex_spec'
local make_pdf_spec = require 'make_pdf_spec'

local C = {}

--== START: Following Created at run time by system
C.specializations  = {}
-- Each specialization contains
-- (1) substitutions made
-- (2) LaTeX file containing spec
-- (3) pdf file containing spec
-- (4) dotc file 
-- (5) doth file 
--== STOP : Above created at run time by system
--=======================================
-- Created at authoring time 
-- START: Following created when operator is authored
local function get_subs(x, y)
  local cfunc = { "coalesce_", x:qtype(), "_", y:qtype(), }
  cfunc = table.concat(cfunc, "")
  return { 
    __QTYPEX__ = x:qtype(), 
    __QTYPEY__ = y:qtype(), 
    __CFUNC__ = cfunc, 
  }
end
C.get_subs         = get_subs -- function

C.generic_tex_spec = "coalesce.tex"
C.lua_calling_spec = "bogus spec for now"
-- STOP: Above created when operator is authored


local call_llm = function() return true end -- TODO 

-- this should be created programmatically 
C.run = function (x, y)
  -- check types
  assert(type(x) == "lQDF")
  assert(type(y) == "lQDF")
  local subs = get_subs(x, y)
  local cfunc = assert(subs.__CFUNC__)
  if ( rawget(C.specializations, cfunc) == nil ) then 
    C.specializations[cfunc] = {} 
  end
  C.specializations[cfunc].subs = subs

  local specific_tex_spec = "_" .. cfunc .. ".tex"
  specialize_tex_spec(C.generic_tex_spec, subs, specific_tex_spec, true)
  C.specializations[cfunc].tex_spec = specific_tex_spec

  local specific_pdf_spec = "_" .. cfunc .. ".pdf"
  assert(make_pdf_spec(specific_tex_spec, cfunc, specific_pdf_spec))
  C.specializations[cfunc].pdf_spec = specific_pdf_spec

  -- local doth, dotc, build_so = call_llm(pdf_spec) TODO FAKING
  local doth = "coalesce_F8_F8.h" 
  local dotc = "coalesce_F8_F8.c" 
  local dotso = "libcoalesce_F8_F8.so" 
  local build_so = "gcc -fPIC -shared  coalesce_F8_F8.c -Wno-implicit-function-declaration -Wno-int-conversion -o libcoalesce_F8_F8.so" 
  C.specializations[cfunc].doth     = doth
  C.specializations[cfunc].dotc     = dotc
  C.specializations[cfunc].build_so = build_so
  cutils.delete(dotso)
  os.execute(build_so)
  assert(cutils.isfile(dotso))
  -- Now to call C from Lua 
  return true 

end
C.set_spec = function(spec)
  assert(type(spec) == "string")
  assert(#spec > 0)
  generic_tex_spec = spec
end
C.get_spec = function(cfunc)
  if ( cfunc ) then 
    -- return specialized specification
    assert(type(cfunc) == "string")
    return specializations[cfunc].tex_spec
  else
    return generic_tex_spec
  end
end
C.test = function()
  print("hello world")
end 
return C
