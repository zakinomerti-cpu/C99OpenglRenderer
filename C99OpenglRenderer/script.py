PyRotateCube("13", -25,0,0)
PyMoveCube("13", 0, 0, -3)

a = 1
while True:
	PyRotateCube("13", a, a, 0)
	a -= 1
	PyPause(16)
	PyPause(8)