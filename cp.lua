infile, err = io.open(current_dir..args[2]args[2],"r")
if not infile then
    print("Cannot open file: " .. err)
else
	instr = infile:read("*a")
	infile:close()
end

outfile, err = io.open(args[3],"w")
if not outfile then
    print("Cannot open file: " .. err)
else
	outfile:write(instr)
	outfile:close()
end
