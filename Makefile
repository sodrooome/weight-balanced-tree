CC ?= gcc
LS ?= ls

generate: 
	$(CC) -c -fPIC weight_balanced_tree.c
	$(CC) -c -fPIC binary_tree.c

created:
	$(CC) -shared -o weight_balanced_tree.so weight_balanced_tree.o
	$(CC) -shared -o binary_tree.so binary_tree.o

verify:
	$(LS) -l binary_tree.so
	$(LS) -l weight_balanced_tree.so

benchmark:
	$(CC) -o wbt weight_balanced_tree.c
	./wbt

	$(CC) -o bt binary_tree.c -lcurl
	./bt

remove:
	rm *.so
	rm *.o
	rm wbt
	rm bt
