import ctypes

convert_function = ctypes.CDLL("weight_balanced_tree.so")
convert_function.newWeightBalancedTree.restype = ctypes.POINTER(ctypes.c_void_p)
convert_function.newWeightBalancedTree.argtypes = []

