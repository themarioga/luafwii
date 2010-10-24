count = 0

function test_main()
	count = count + 1
	print("Count "..count)
	if count > 100 then
		os.exit()
	end
end
