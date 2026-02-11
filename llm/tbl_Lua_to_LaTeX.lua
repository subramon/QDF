local infile  = assert(arg[1])
local label   = arg[2] or "label_undefined"
local caption = arg[3]or "caption undefined"

local T = dofile(infile)
assert(type(T) == "table")

print("\\begin{table}")
print("\\centering")
print("\\begin{tabular}{|l|l|} \\hline \\hline ")
for k, v in pairs(T) do 
  local s = string.format(" %s & \\verb+ %s + \\\\ \\hline", k, v)
  print(s)
end
print("\\hline")
print("\\end{tabular}")
print(string.format("\\caption{%s}", caption))
print(string.format("\\label{%s}", label))
print("\\end{table}")
