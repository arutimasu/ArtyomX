local notes = 0
print("Noteblocks Editor v. 0.1")
io.write("Enter the name of new file: ")
local filename = io.read()
-- Opens a file in read mode
file = io.open(filename, "a")
print("#\t _______________")
while true do
notes=notes+1
	io.write(notes.."\t|")
	local note = io.read()
	if note == "end" then
		-- closes the open file
		file:close()
		print()
		print("File writed.")
		break
	else
		file:write(note)
	end
	--print(note)

end
