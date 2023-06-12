CC ?= gcc
LS ?= ls

generate: 
	$(CC) -c -fPIC weight_balanced_tree.c

created:
	$(CC) -shared -o weight_balanced_tree.so weight_balanced_tree.o

verify:
	$(LS) -l weight_balanced_tree.so
