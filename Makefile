clean:
	rm -rf bin

build: clean
	mkdir bin
	clang++ -g -std=c++20 *.cpp vm/*.cpp value/*.cpp parser/*.cpp compiler/*.cpp -o bin/gpp

run: build
	./bin/gpp ./**/*.gpp
