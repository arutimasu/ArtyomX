for i = 2, #args do
	io.write(args[i])
    --uart.write(1, args[i])
    --out:send(args[i].."\r\n")
end
print()
