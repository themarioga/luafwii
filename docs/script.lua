negro = Screen.color(0,0,0,0)
fondo = Screen.imageLoad("fondo.png")
cd = Screen.imageLoad("cd.png")
musictype = 0
i=0
y=35
sel=0
reprod=""
while true do
	wpad1 = Controls.read(0)
	System.getDir()
	Screen.imageBlit(1, 1, fondo)
	maxd = System.getMaxDir() - 1
	while i <= maxd do
		if System.getDType(i) == 0 then
			if i == sel then
				Screen.print(5, y, "->"..System.getDName(i))
			else
				Screen.print(5, y, System.getDName(i))
			end
		else
			if i == sel then
				Screen.print(5, y, "-><D>"..System.getDName(i))
			else
				Screen.print(5, y, "<D>"..System.getDName(i))
			end
		end
		i=i+1
		y=y+15
		Screen.flip()
	end
	if Controls.pressUp(wpad1) == 1 and sel > 0 then
		sel=sel-1
		i=0
		y=35
	end
	if Controls.pressDown(wpad1) == 1 and sel < maxd then
		sel=sel+1
		i=0
		y=35
	end
	if Controls.pressA(wpad1) == 1 then
		if System.getDType(sel) == 1 then
			System.getDir(sel)
			sel = 0
		else
			curdir = System.getPathDir().."/"..System.getDName(sel)
			if System.isDExt(".PNG", sel) == 1 or System.isDExt(".png", sel) == 1 then
				Screen.imageLoadinOne(1, 1, curdir)
				Screen.flip()
				Screen.delay(5)
			elseif System.isDExt(".JPG", sel) == 1 or System.isDExt(".jpg", sel) == 1 then
				Screen.imageLoadinOne(1, 1, curdir)
				Screen.flip()
				Screen.delay(5)
			elseif System.isDExt(".JPEG", sel) == 1 or System.isDExt(".jpeg", sel) == 1 then
				Screen.imageLoadinOne(1, 1, curdir)
				Screen.flip()
				Screen.delay(5)
			elseif System.isDExt(".BMP", sel) == 1 or System.isDExt(".bmp", sel) == 1 then
				Screen.imageLoadinOne(1, 1, curdir)
				Screen.flip()
				Screen.delay(5)
			elseif System.isDExt(".TIF", sel) == 1 or System.isDExt(".tif", sel) == 1 then
				Screen.imageLoadinOne(1, 1, curdir)
				Screen.flip()
				Screen.delay(5)
			elseif System.isDExt(".TIFF", sel) == 1 or System.isDExt(".tiff", sel) == 1 then
				Screen.imageLoadinOne(1, 1, curdir)
				Screen.flip()
				Screen.delay(5)
			elseif System.isDExt(".GIF", sel) == 1 or System.isDExt(".gif", sel) == 1 then
				Screen.imageLoadinOne(1, 1, curdir)
				Screen.flip()
				Screen.delay(5)
			elseif System.isDExt(".MP3", sel) == 1 or System.isDExt(".mp3", sel) == 1 then
				Sound.mp3LoadandPlay(curdir,0)
				musictype=1
			elseif System.isDExt(".WAV", sel) == 1 or System.isDExt(".wav", sel) == 1 then
				Sound.wavLoadandPlay(curdir)
				musictype=2
			elseif System.isDExt(".OGG", sel) == 1 or System.isDExt(".ogg", sel) == 1 then
				Sound.oggLoadandPlay(curdir, 0, 0)
				musictype=3
			elseif System.isDExt(".LUA", sel) == 1 or System.isDExt(".lua", sel) == 1 then
				dofile(curdir)
			end
		end
		i=0
		y=35
	end
	while musictype ~= 0 do
		wpad1 = Controls.read(0)
		Screen.imageBlit(120, 50, cd)
		reprod = System.getDName(sel)
		Screen.print(5, 460, "Se esta reproduciendo: "..reprod.." Pulsa B para parar")
		Screen.flip()
		if Controls.pressB(wpad1) == 1 then
			if musictype == 1 then
				Sound.mp3Stop()
				musictype = 0
			elseif musictype == 3 then
				Sound.oggStop()
				musictype = 0
			end
			musictype = 0
			i=0
			y=35
		end
	end
	if Controls.pressHome(wpad1) == 1 then
		System.exitHBC()
	end
end
