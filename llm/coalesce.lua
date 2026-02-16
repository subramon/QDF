-- Provide a PDF containing the specification of the operator
local cutils = require 'libcutils'
local specialize_tex_spec = require 'specialize_tex_spec'

local C = {}

--== START: Following Created at run time by system
local specializations = {}  -- specializations
specializations.build_so = "command to build .so file "
-- Each specialization contains
-- (1) substitutions made
-- (2) LaTeX file containing spec
-- (3) pdf file containing spec
-- (4) dotc file 
-- (5) doth file 
--== STOP : Above created at run time by system
--=======================================
C.specializations  = specializations 
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

local make_pdf_spec = function(
  infile,
  outfile
)
  assert(type(infile) == "string")
  assert(type(outfile) == "string")
  assert(cutils.isfile(infile), "File not found " .. infile)
  assert(infile ~= outfile)

  local tmpfile = "/tmp/_xx.tex"  -- TODO IMPROVE 
  specialize_tex_spec(infile, { __OPERATOR__ = "XXX", }, tmpfile)
  local cmd = string.format("pdflatex %s -o %s ", tmpfile, outfile)
  print("cmd = ", cmd)
  error("XXXX")
  os.execute(cmd)
  cutils.unlink(tmpfile)
end

local call_llm = function() return nil end -- TODO 

-- this should be created programmatically 
C.run = function (x, y)
  -- check types
  assert(type(x) == "lQDF")
  assert(type(y) == "lQDF")
  local subs = get_subs(x, y)
  local cfunc = assert(subs.__CFUNC__)
  if ( rawget(specializations, cfunc) == nil ) then 
    specializations[cfunc] = {} 
  end
  specializations[cfunc].subs = subs

  local specific_tex_spec = "_" .. cfunc .. ".tex"
  specialize_tex_spec(C.generic_tex_spec, subs, specific_tex_spec)
  specializations[cfunc].tex_spec = specific_tex_spec

  local specific_pdf_spec = "_" .. cfunc .. ".pdf"
  make_pdf_spec(specific_tex_spec, specific_pdf_spec)
  error("XXXX")
  specializations[cfunc].pdf_spec = specific_pdf_spec

  local doth, dotc, build_so = call_llm(pdf_spec)

 --[[
  local xqtype = x:qtype()
  local yqtype = y:qtype()
  assert(xqtype == yqtype)
  local func_name = coalesce .. "_" .. xqtype 
  local subs = { 
    xqtype = xqtype, 
    yqtype = yqtype, 
    func_name = func_name
  }
  local custom_spec = substitute(spec, subs)
  local dotc, doth = call_llm(custom_spec)
  --]]
  print(spec)
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
