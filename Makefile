clean:
	rm -rf bin

build:
	mkdir bin
	g++ *.cpp -o bin/gpp

run: clean build
	./bin/gpp
