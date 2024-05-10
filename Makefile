APP_PATH = ./app

compile:
	g++ SS-algorithm.cpp -o $(APP_PATH)

runCP:
	g++ CP-algorithm.cpp -o $(APP_PATH)
	$(APP_PATH)

runSS:
	g++ SS-algorithm.cpp -o $(APP_PATH)
	$(APP_PATH)

runWF:
	g++ WriteFile.cpp -o $(APP_PATH)
	$(APP_PATH)