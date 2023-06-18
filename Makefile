clean:
	rm -rf bin

build: clean
	mkdir bin
	g++ *.cpp -o bin/gpp

run: build
	./bin/gpp
