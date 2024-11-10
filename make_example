compress: compress.c
	gcc compress.c -o bin/compress

# compile_test: test.o compress.o munit.o
# 	gcc -g test.o compress.o -o bin/compress_test

compile_test: ./munit/munit.h ./munit/munit.c ./hashmap.c/hashmap.h ./hashmap.c/hashmap.c compress.c compress.h test.c ./huffman/huffman.h ./huffman/huffman.c
	gcc -o bin/compress_test ./munit/munit.c ./hashmap.c/hashmap.c ./huffman/huffman.c compress.c test.c

# test.o: test.c compress.h ./munit/munit.h
# 	gcc -g -c test.c
#
# compress.o: compress.c compress.h
# 	gcc -g -c compress.c
#
# munit.o: ./munit/munit.c ./munit/munit.h
# 	gcc -g -c ./munit/munit.c

run: compress
	bin/compress

clean:
	rm -rf bin/compress

test:
	bundle exec rspec

unit-test: compile_test
	bin/compress_test

format: *.c
	clang-format -style=Google -i *.c
