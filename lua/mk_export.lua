local plfile = require 'pl.file'
local configs = {}
configs.sql = "_x.sql"
configs.db = "mgcv"
configs.tbl = "dataset_main"
configs.where = nil
configs.flds = "id, dsid1, dsid2, file_name"
configs.outfile = "/tmp/_x.csv"-- must be created in /tmp/
configs.where = "is_read = 0"


local tmpl = [[
USE __DB__; 
SELECT __FLDS__ \
INTO OUTFILE '__OUTFILE__'  \
FIELDS TERMINATED BY ',' \
OPTIONALLY ENCLOSED BY '"' \
LINES TERMINATED BY '\n' \
FROM __TBL__ \
WHERE __WHERE__ ;
]]

tmpl = string.gsub(tmpl, "__DB__", configs.db)
tmpl = string.gsub(tmpl, "__TBL__", configs.tbl)
tmpl = string.gsub(tmpl, "__FLDS__", configs.flds)
tmpl = string.gsub(tmpl, "__OUTFILE__", configs.outfile)

if ( configs.where ) then 
  tmpl = string.gsub(tmpl, "__WHERE__", configs.where)
else
  tmpl = string.gsub(tmpl, "__WHERE__", "true")
end
plfile.write(configs.sql, tmpl)
print("All done")
