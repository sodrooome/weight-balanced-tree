## Weighted Balanced Tree for Anomaly Detection

Exploring weighted-balanced tree as the alternative solution for detecting anomaly. It's worth to notice, the anomaly detection will employ the isolation forest algorithm as its core, with a binary search tree utilized for the anomaly classification process. Currently, im suspecting and have an assumption that the binary search tree may encounter unsuccessful operations when the tree nodes become unbalanced / skeewed tree operation (let's say we assume there are 5 tree nodes which of them weren't sorted by default), making it challenging to maintain balance between the two nodes.

This is just proof-of-concept for my research for a master's degree, it may have biased and unxepected results since it wasn't properly tested and evaluated

## Usage

Eventually, you need to generate the `.so` files first if you want to run and ported the `WeightBalancedTree` function in Python. You need to compile the main code first by execute this command

```bash
gcc -c -fPIC weight_balanced_tree.c
```

And afterwards, create the shared library so that can be execute 

```bash
gcc -shared -o weight_balanced_tree.so weight_balanced_tree.o
```

Ensures that the shared library was generated and converted successfully

```bash
ls -l weight_balanced_tree.so
```
