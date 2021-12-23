CWARNING = -Wall -Wshadow --pedantic
CERROR = -Wvla -Werror
CFLAGS  = -g -std=c99 $(CWARNING) $(CERROR)
GCC = gcc $(CFLAGS)

build_test_gen:
	$(GCC) test_gen.c -o test_gen

build_brute_force: two_sum.h
	$(GCC) driver.c brute_force.c -o two_sum_c

build_two_pass_division_hash: two_sum.h
	$(GCC) driver.c two_pass_division_hash.c -o two_sum_c

build_one_pass_division_hash: two_sum.h
	$(GCC) driver.c one_pass_division_hash.c -o two_sum_c

test_memory: build_test_gen
	./test_gen -r test_cases/memory_test.b
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./two_sum_c -f test_cases/memory_test.b

clean:
	rm -f test_gen two_sum_c valgrind-out.txt