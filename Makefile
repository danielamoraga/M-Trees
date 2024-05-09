compile:
	g++ SS-algorithm.cpp -o app

runCP:
	g++ CP-algorithm.cpp -o app
	./app

runSS:
	g++ SS-algorithm.cpp -o app
	./app

runWF:
	g++ WriteFile.cpp -o app
	./app