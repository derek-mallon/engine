tests:
	clang -o heap_tests -DDEBUG heap_tests.c ../src/heap.c ../src/utils.c -I../include
	./heap_tests
	clang -o file_handling_tests -DDEBUG -g file_handling_tests.c ../src/file_handling.c ../src/utils.c -I../include
	./file_handling_tests
	

