Neural Net for fun
==================


Dumpfile format
---------------


| Offset |   Type   |       Description                 |
|:-------|:---------|:----------------------------------|
| 0000   | uint32   | Number of layers (N)              |
| 0004   | uint32   | Cost Function                     |
| 0008   | uint32   | Regularization Function           |
| 0012   | uint32   | Number of neurons in layer 0      |
| ....   | uint32   | ...                               |
| 00xx   | uint32   | Number of neurons in layer N-1    |
| 00xx   | uint32   | Number of inputs in neuron 0 (N0) |
| 00xx   | double   | Neuron 0 weight 0                 |
| ....   | double   | ...                               |
| 00xx   | double   | Neuron 0 weight N0-1              |
| 00xx   | double   | Neuron 0 weight N0 (bias)         |
| ....   | double   | ...                               |
| 00xx   | uint32   | Number of inputs in neuron n (Nn) |
| 00xx   | double   | Neuron n weight 0                 |
| ....   | double   | ...                               |
| 00xx   | double   | Neuron n weight Nn-1              |
| 00xx   | double   | Neuron n weight Nn (bias)         |


### Cost Fuctions

|    Function  | Value |
|:-------------|:------|
| Quadratic    | 0     |
| CrossEntropy | 1     |

### Regularization Functions

|    Function  | Value |
|:-------------|:------|
| None         | 0     |
| L1           | 1     |
| L2           | 2     |