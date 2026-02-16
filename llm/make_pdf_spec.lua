local cutils = require 'libcutils'
local specialize_tex_spec = require 'specialize_tex_spec'

local make_pdf_spec = function(
  infile,
  specialized_operator,
  outfile
)
  assert(type(infile) == "string")
  assert(type(outfile) == "string")
  assert(type(specialized_operator) == "string")
  assert(cutils.isfile(infile), "File not found " .. infile)
  assert(infile ~= outfile)

  local tmpfile = assert(cutils.mk_temp_file("_XXXXXX", ".tex"))
  specialize_tex_spec("qdf_definition.tex", 
    -- we have a convention that all temporary files start with uscore
    { __OPERATOR__ = "_" .. specialized_operator, }, 
    tmpfile, false)
  -- could not get pdflatex to send pdf file to desired directory
  -- string.format("pdflatex %s -output-directory /tmp/ ", tmpfile)
  os.execute(string.format("pdflatex %s ", tmpfile))
  local pdffile = string.gsub(tmpfile, ".tex", ".pdf")
  assert(cutils.rename(pdffile, outfile))
  -- cleanup
  local logfile = string.gsub(tmpfile, ".tex", ".log")
  cutils.unlink(logfile)
  local outfile = string.gsub(tmpfile, ".tex", ".out")
  cutils.unlink(outfile)
  local auxfile = string.gsub(tmpfile, ".tex", ".aux")
  cutils.unlink(auxfile)
  cutils.unlink(tmpfile)

  return true
end

return make_pdf_spec
