# Connected dominating sets

This repository contains code and data related to the paper "The maximum number of connected sets in regular graphs". All code can be found in the directory "Code", whereas all data can be found in the directory "Data".

Below, we briefly describe the different programs and data. We will assume that Linux is being used and that GENREG is installed. The details on how to install GENREG can be found on [this link](https://www.mathe2.uni-bayreuth.de/markus/reggraphs.html). The graphs that we are dealing with will be in graph6 format.


## CODE
The different programs can be compiled by going to the directory "Code" and executing the makefile using
```bash
make
```

### calcMeanConnectedSetOrderSubsetIteration.cpp

This program expects as input a list of graph6 strings. It will calculate c(G) for each input graph and it will output the maximum value of c(G) over all these graphs together with an exhaustive list of maximizers.

For example, executing the following command:

```bash
./genreg/genreg-incl-bip-march-native-fastest/genreg-incl-bip-march-native-fastest/genreg 10 3 -g stdout | ./calcMeanConnectedSetOrderSubsetIterationExecutable
```
 will generate all connected 3-regular graphs on 10 vertices and calculate c(G) for each one of them. The expected output is as follows:

```bash
Best was: 569
The corresponding c(G): 0.942933859546
An exhaustive list of all best graphs in graph6 format:
IsP@PGXD_
```

This means that the graph "IsP@PGXD_" has 569 connected induced subgraphs and this is the maximum amongst all graphs from the input.

### calcMeanConnectedSetOrderTarjanRemove.cpp

This program has the same functionality as the previous program, but uses a different algorithm and can usually be expected to be slightly faster.

### calcMatrix3RegularCaseBinaryTreeInMiddle.cpp

This program expects as input an integer K specifying that there should be a binary tree in the middle consisting of 2^K-2 vertices, which is connected to vertices on the cycle (see Section 4 of the paper for details). This program will generate the coefficient matrix.

For example, the following command:
```bash
./calcMatrix3RegularCaseBinaryTreeInMiddleExecutable > testMatrix.txt
```

Will start the program and write its output to "testMatrix.txt". The program expects an integer as input. For example, if we follow the following input:

```bash
1
```
the program will compute the matrix related to the family described in Section 3.


If we now execute the command
```bash
python3 calcEigenvalues.py 
```

we get the following output:
```bash
[ 8.95241987+0.j          0.36741116+1.93741254j  0.36741116-1.93741254j
  1.53098484+0.j         -0.10911352+0.3717492j  -0.10911352-0.3717492j ] 
```

These values represent the 6 largest eigenvalues of the coefficient matrix. If we now take the fourth root of the largest eigenvalue, we get 1.729 (the constant from Section 3).


## DATA
### connectedSets
This folder contains two subfolders containing results about 3-regular and 4-regular graphs. We will discuss the content of the folder "3Regular" (the folder "4Regular" is structured in the same way). All files ending in ".txt" contain the value c_{3,g}(n) together with an exhaustive list of the graphs for which c(G)=c_{d,g}(n). The values of g and n can be derived from the file name. For example, the file "ResultOrd8Reg3NonDom" contains c_{3,8}(n). Additionally, this folder also contains a file ending in ".g6" which is a summary of all extremal graphs in graph6 format.

### connectedDominatingSets

This folder follows the same structure as the folder "connectedSets", but instead it contains the results concerning connected dominating sets.
