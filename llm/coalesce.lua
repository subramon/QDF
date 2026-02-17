-- Provide a PDF containing the specification of the operator
local cutils   = require 'libcutils'
local specialize_tex_spec = require 'specialize_tex_spec'
local make_pdf_spec = require 'make_pdf_spec'
local call_llm = function() return true end -- TODO 
local lQDF     = require 'lQDF'
local lqdfmem  = require 'lqdfmem'

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
C.generic_tex_spec_file = "coalesce.tex"
C.lua_calling_spec = "bogus spec for now"
-- STOP: Above created when operator is authored

-- C.run should be created programmatically 
C.run = function (x, y)
  -- check types
  assert(type(x) == "lQDF")
  assert(type(y) == "lQDF")
  local subs = get_subs(x, y)
  local cfunc = assert(subs.__CFUNC__)
  if ( rawget(C.specializations, cfunc) == nil ) then 
    C.specializations[cfunc] = {} 
    C.specializations[cfunc].subs = subs
  
    local specific_tex_spec_file = cfunc .. ".tex"
    specialize_tex_spec(C.generic_tex_spec_file, subs, specific_tex_spec_file, true)
    C.specializations[cfunc].tex_spec_file = specific_tex_spec_file
  
    local specific_pdf_spec_file = cfunc .. ".pdf"
    assert(make_pdf_spec(specific_tex_spec_file, cfunc, specific_pdf_spec_file))
    C.specializations[cfunc].pdf_spec = specific_pdf_spec
  
    -- local doth, dotc, build_so = call_llm(pdf_spec) TODO FAKING
    local doth  = cfunc .. ".h" 
    local dotc  = cfunc .. ".c" 
    local dotso = "lib" .. cfunc .. ".so" 
    assert(cutils.isfile(doth), "File not found " .. doth)
    assert(cutils.isfile(dotc), "File not found " .. dotc)
    -- TODO build_so comes back from call_llm
    local build_so = "gcc -fPIC -shared  coalesce_F8_F8.c -Wno-implicit-function-declaration -Wno-int-conversion -o libcoalesce_F8_F8.so" 
    -- TODO Verify that build_so command links libqdf.so 
    C.specializations[cfunc].doth     = doth
    C.specializations[cfunc].dotc     = dotc
    C.specializations[cfunc].build_so = build_so
    cutils.delete(dotso)
    print("Executing ", build_so)
    os.execute(build_so)
    assert(cutils.isfile(dotso), "File not found " .. dotso)
    C.specializations[cfunc].dotso    = dotso
    lQDF.q_add(cfunc, dotso, doth)
    print("XXXXXX")
    lQDF.q_rec_spec(cfunc, C.specializations[cfunc])
  else
    print("Specializations for " .. cfunc .. " exist.")
  end
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
