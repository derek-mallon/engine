tests:
	clang -o bin/mem_tests -g -DDEBUG mem_tests.c ../src/mem.c ../src/utils.c -I../include
	./bin/mem_tests
	clang  -o bin/file_handling_tests -g -DDEBUG file_handling_tests.c ../src/file_handling.c ../src/utils.c ../src/mem.c -I../include
	./bin/file_handling_tests
	clang -o bin/sdl_wrapper_tests  -g -DDEBUG sdl_wrapper_tests.c ../src/sdl_wrapper.c ../src/utils.c ../src/geometry.c ../src/mem.c -lSDL2 -I../include
	./bin/sdl_wrapper_tests
	
clean:
	rm -rf bin

