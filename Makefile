make:
	cmake --build build

build-json:
	cmake -S . -B build -G Ninja 
run: 
	./build/PAINTOMATA.exe
