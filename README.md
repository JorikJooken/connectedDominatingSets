# Connected dominating sets

This repository contains code and data related to the paper "The maximum number of connected sets in regular graphs". All code can be found in the directory "Code", whereas all data can be found in the directory "Data".

Below, we briefly describe the different programs and data. We will assume that Linux is being used and that GENREG is installed. The details on how to install GENREG can be found on [this link](https://www.mathe2.uni-bayreuth.de/markus/reggraphs.html). The graphs that we are dealing with will be in graph6 format.


## CODE
The different programs can be compiled by going to the directory code and executing the makefile using
```bash
make
```

### Title here
Description here
```bash
bashcommand
```

will result in the following output:

```
SsP@PGWD_A?C?A?A??_?G??g?AO?BG?Ao
21
1 20 160 660 1510 1908 1232 332 25 0 0 0 0 0 0 0 0 0 0 0 0 
SsP@P?SCOO?`?_?O_C??`?A_?C_?A_??[
21
1 20 160 660 1510 1912 1240 320 5 0 0 0 0 0 0 0 0 0 0 0 0
```
 
## DATA
### connectedSets
This folder contains two subfolders containing results about 3-regular and 4-regular graphs. We will discuss the content of the folder "3Regular" (the folder "4Regular" is structured in the same way). All files ending in ".txt" contain the value c_{3,g}(n) together with an exhaustive list of the graphs for which c(G)=c_{d,g}(n). The values of g and n can be derived from the file name. For example, the file "ResultOrd8Reg3NonDom" contains c_{3,8}(n). Additionally, this folder also contains a file ending in ".g6" which is a summary of all extremal graphs in graph6 format.

### connectedDominatingSets

This folder follows the same structure as the folder "connectedSets", but instead it contains the results concerning connected dominating sets.
