local status, err = os.rename(args[2], args[3])
if not status then
	print("mv: ".. err)
end
