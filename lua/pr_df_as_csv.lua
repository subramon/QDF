local cutils = require 'libcutils'
G = {} -- to suppress strict 
-- TODO P1 require 'strict'
local lQDF = require 'lQDF'
assert(arg)
local infile  = assert(arg[1])
local mode    = assert(arg[2])
local outfile 
if ( arg[3] ) then
  outfile = arg[3]
else
  outfile = cutils.mkstemp("/tmp/XXXXXXXX")
end
local x = lQDF.binld(infile)
assert(infile ~= outfile)
if ( mode == "json" ) then
  print(x)
elseif ( mode == "csv" ) then 
  x:pr_df_as_csv(nil, outfile)
else
  error("bad mode " .. mode)
end
if ( not arg[3] ) then
  -- write to stdout
  local x = cutils.file_as_str(outfile)
  print(x) -- TODO P3 Adds an extra newline 
end
