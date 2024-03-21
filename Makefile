build:
	rm -rf build/program
	gcc -Wall -Isrc src/main.c -o build/program

clean:
	rm -rf build/

run:
	$(MAKE) build
	./build/program

