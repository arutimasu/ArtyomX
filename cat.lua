local file, err = io.open(current_dir..args[2], "r")
if not file then
    print("cat: " .. err)
else
 -- perform file operations
	local contents = file:read( "*all" )
	print(contents)
	file:close()
end
