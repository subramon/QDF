-- Function to handle conditional specialization (when necessary)
local cutils   = require 'libcutils'
local specialize_tex_spec = require 'specialize_tex_spec'
local make_pdf_spec = require 'make_pdf_spec'
local call_llm = function() return true end -- TODO 
local lQDF     = require 'lQDF'

local function condl_specl(subs)
  assert(type(subs) == "table")
  local cfunc = assert(subs.__CFUNC__)
  if ( not lQDF.q_is_spec(cfunc) ) then 
    local specific_tex_spec_file = cfunc .. ".tex"
    local generic_tex_spec_file = lQDF.get_generic_tex_spec_file(subs.operator)
    specialize_tex_spec(generic_tex_spec_file, subs, specific_tex_spec_file, true)
    local specific_pdf_spec_file = cfunc .. ".pdf"
    assert(make_pdf_spec(specific_tex_spec_file, cfunc, specific_pdf_spec_file))
  
    -- doth, dotc, build_so = call_llm(pdf_spec) TODO FAKING
    local doth  = cfunc .. ".h" 
    local dotc  = cfunc .. ".c" 
    local dotso = "lib" .. cfunc .. ".so" 
    assert(cutils.isfile(doth), "File not found " .. doth)
    assert(cutils.isfile(dotc), "File not found " .. dotc)
    -- TODO build_so comes back from call_llm
    local build_so = "gcc -fPIC -shared  coalesce_F8_F8.c -Wno-implicit-function-declaration -Wno-int-conversion -o libcoalesce_F8_F8.so" 
    -- TODO Verify that build_so command links libqdf.so 
    cutils.delete(dotso)
    os.execute(build_so)
    assert(cutils.isfile(dotso), "File not found " .. dotso)
    -- START record specializations
    local specializations = {} 
    specializations.subs = subs
    specializations.tex_spec_file = specific_tex_spec_file
    specializations.pdf_spec = specific_pdf_spec_file
    specializations.doth     = doth
    specializations.dotc     = dotc
    specializations.build_so = build_so
    specializations.dotso    = dotso
    lQDF.q_add(cfunc, dotso, doth)
    lQDF.q_rec_spec(cfunc, specializations)
  else
    print("Specializations for " .. cfunc .. " exist.")
  end
  local exec = assert(lQDF.q_get(cfunc))
  return cfunc, exec
end
return condl_specl
