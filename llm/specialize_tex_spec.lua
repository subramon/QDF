local cutils = require 'libcutils'
local function specialize_tex_spec(infile, subs, outfile)
  assert(type(infile) == "string")
  assert(type(outfile) == "string")
  assert(cutils.isfile(infile), "File not found " .. infile)
  assert(infile ~= outfile)
  assert(type(subs) == "table")

  local ofp = assert(io.open(outfile, "w"))

  for line in io.lines(infile) do 
    for k, v in pairs(subs) do 
      -- handle underscore 
      local vv = string.gsub(v, "_", "\\_")
      line = string.gsub(line, k, vv)
    end
    ofp:write(line)
    ofp:write("\n")
  end
  ofp:close()

  
end
return specialize_tex_spec
