clean:
	rm -rf bin

build: clean
	mkdir bin
	g++ -std=c++20 *.cpp -o bin/gpp

run: build
	./bin/gpp
