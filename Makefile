CC ?= gcc
LS ?= ls
CFLAGS ?= -Wall -Wextra -g

generate: 
	$(CC) -c -fPIC src/weight_balanced_tree.c
	$(CC) -c -fPIC src/binary_tree.c

created:
	$(CC) -shared -o weight_balanced_tree.so weight_balanced_tree.o
	$(CC) -shared -o binary_tree.so binary_tree.o

verify:
	$(LS) -l binary_tree.so
	$(LS) -l weight_balanced_tree.so

benchmark:
	$(CC) -c src/utils.c
	$(CC) -o wbt src/weight_balanced_tree.c utils.o
	./wbt

	$(CC) -c src/utils.c
	$(CC) -o bt src/binary_tree.c -lcurl utils.o
	./bt

debugbt:
	$(CC) -c src/utils.c
	$(CC) $(CFLAGS) src/binary_tree.c utils.o -o bt
	gdb ./bt

debugwbt:
	$(CC) -c src/utils.c
	$(CC) $(CFLAGS) src/weight_balanced_tree.c utils.o -o wbt
	gdb ./wbt

.PHONY: clean
clean:
	rm -rf bt wbt

remove:
	rm *.so
	rm *.o
	rm wbt
	rm bt
