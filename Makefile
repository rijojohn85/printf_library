ALL=printf

all: ${ALL}

clean:
	rm -f bin/${ALL} *.o

#printf
printf.o: printf.c
	gcc -c printf.c
printf: printf.o
	gcc -O0 printf.o -o bin/printf

#test
compile_test: ./munit/munit.h ./munit/munit.c ./printf.h printf.c
	gcc -o bin/printf_test ./munit/munit.c ./printf.c ./test.c

unit-test: compile_test
	bin/printf_test
