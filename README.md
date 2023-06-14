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

## Benchmarking

**APIs benchmarking**

You can do the benchmarking using `make benchmark` command to get the comparison between Binary Tree and Weighted Binary Tree (it's worth to noting that both of the algorithm is using similar numbers of dataset). At my machine, both of them resulting with different performances with the binary tree it's slightly faster than Weighted Binary Tree

| Algorithm | Method | Result Time (seconds) |
| --------- | ------ | --------------------- |
| Binary Tree | Searching | 0.000036 seconds |
|  | Insertion | 0.000437 seconds |
| Weight Binary Tree | Searching | 0.001664 seconds |
|  | Insertion | 0.003228 seconds |

**Detection Anomaly benchmarking**

as you can see, for use in anomaly detection using the same dataset, the Weighted binary tree is relatively faster than the binary tree due of in the weighted binary tree there's a rebalancing tree process which tries to balance the nodes in the two objects. Thus, from here it leads to the faster performance when searching find anomaly, while the binary tree doesn't use the rebalance method, when the tree node dataset increases, the search process will gradually be slower due to an unbalanced occurrence between the two objects in the tree node

| Algorithm | Result Time (seconds) |
| --------- | --------------------- |
| Weighted Binary Tree (non-HTTP request) | 0.000083 seconds |
| Binary Tree (non-HTTP request) | 0.000331 seconds |
