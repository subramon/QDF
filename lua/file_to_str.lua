local function file_to_str(file)
    local f = assert(io.open(file, "rb"), "File not found " .. file)
    local content = f:read("*all")
    f:close()
    return content
end
return file_to_str
