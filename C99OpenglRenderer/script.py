PyRotateCube("cube", -25,0,0)
PyMoveCube("cube", 0, 0, -3)

a = 1
while True:
	PyRotateCube("cube", a, a, 0)
	a = int(input())

	PyPause(16)