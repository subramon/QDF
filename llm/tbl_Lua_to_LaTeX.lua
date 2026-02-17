local infile  = assert(arg[1])
local label   = arg[2] or "label_undefined"
local caption = arg[3]or "caption undefined"
local headers = arg[4] or ' return { "Q Type", "C Type", }'

assert(type(headers) == "string")
local x = assert(loadstring(headers))
headers = x()
assert(type(headers) == "table")
assert(#headers == 2)


local T = dofile(infile)
assert(type(T) == "table")

print("\\begin{table}")
print("\\centering")
print("\\begin{tabular}{|l|l|} \\hline \\hline ")
local hdr = string.format("{\\bf %s } & {\\bf %s } \\\\ \\hline \\hline",
  headers[1], headers[2])

print(hdr)
for k, v in pairs(T) do 
  local s = string.format(" %s & \\verb+ %s + \\\\ \\hline", k, v)
  print(s)
end
print("\\hline")
print("\\end{tabular}")
print(string.format("\\caption{%s}", caption))
print(string.format("\\label{%s}", label))
print("\\end{table}")
