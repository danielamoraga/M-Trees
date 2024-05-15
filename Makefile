APP_PATH = ./app

compile:
	g++ SS-algorithm.cpp -o $(APP_PATH)

runCP:
	g++ CP-algorithm.cpp -o $(APP_PATH)
	$(APP_PATH)

runSS:
	g++ SS-algorithm.cpp -o $(APP_PATH)
	$(APP_PATH)

run:
	g++ Main.cpp -o $(APP_PATH)
	$(APP_PATH)

clean:
	rm $(APP_PATH)