compiler=g++
flags=-g -std=c++11 -O3

all: calcMeanConnectedSetOrderSubsetIterationExecutable calcMatrixKRegularEdgeFamilyExecutable calcMatrix3RegularCaseBinaryTreeInMiddleExecutable calcMeanConnectedSetOrderTarjanRemoveExecutable dominatingCalcMatrix3RegularCaseBinaryTreeInMiddleExecutable calcMeanDominatingConnectedSetOrderSubsetIterationExecutable dominatingCalcMatrixKRegularEdgeFamilyExecutable calcMeanDominatingConnectedSetOrderTarjanRemoveExecutable

calcMeanConnectedSetOrderSubsetIterationExecutable: calcMeanConnectedSetOrderSubsetIteration.cpp
	$(compiler) -o calcMeanConnectedSetOrderSubsetIterationExecutable calcMeanConnectedSetOrderSubsetIteration.cpp $(flags)

calcMeanDominatingConnectedSetOrderSubsetIterationExecutable: calcMeanDominatingConnectedSetOrderSubsetIteration.cpp
	$(compiler) -o calcMeanDominatingConnectedSetOrderSubsetIterationExecutable calcMeanDominatingConnectedSetOrderSubsetIteration.cpp $(flags)

calcMatrixKRegularEdgeFamilyExecutable: calcMatrixKRegularEdgeFamily.cpp
	$(compiler) -o calcMatrixKRegularEdgeFamilyExecutable calcMatrixKRegularEdgeFamily.cpp $(flags)

calcMatrix3RegularCaseBinaryTreeInMiddleExecutable: calcMatrix3RegularCaseBinaryTreeInMiddle.cpp
	$(compiler) -o calcMatrix3RegularCaseBinaryTreeInMiddleExecutable calcMatrix3RegularCaseBinaryTreeInMiddle.cpp $(flags)

calcMeanConnectedSetOrderTarjanRemoveExecutable: calcMeanConnectedSetOrderTarjanRemove.cpp
	$(compiler) -o calcMeanConnectedSetOrderTarjanRemoveExecutable calcMeanConnectedSetOrderTarjanRemove.cpp $(flags)

calcMeanDominatingConnectedSetOrderTarjanRemoveExecutable: calcMeanDominatingConnectedSetOrderTarjanRemove.cpp
	$(compiler) -o calcMeanDominatingConnectedSetOrderTarjanRemoveExecutable calcMeanDominatingConnectedSetOrderTarjanRemove.cpp $(flags)

dominatingCalcMatrix3RegularCaseBinaryTreeInMiddleExecutable: dominatingCalcMatrix3RegularCaseBinaryTreeInMiddle.cpp
	$(compiler) -o dominatingCalcMatrix3RegularCaseBinaryTreeInMiddleExecutable dominatingCalcMatrix3RegularCaseBinaryTreeInMiddle.cpp $(flags)

dominatingCalcMatrixKRegularEdgeFamilyExecutable: dominatingCalcMatrixKRegularEdgeFamily.cpp
	$(compiler) -o dominatingCalcMatrixKRegularEdgeFamilyExecutable dominatingCalcMatrixKRegularEdgeFamily.cpp $(flags)

clean:
	rm calcMeanConnectedSetOrderSubsetIterationExecutable calcMatrixKRegularEdgeFamilyExecutable calcMatrix3RegularCaseBinaryTreeInMiddleExecutable calcMeanConnectedSetOrderTarjanRemoveExecutable dominatingCalcMatrix3RegularCaseBinaryTreeInMiddleExecutable dominatingCalcMatrixKRegularEdgeFamilyExecutable calcMeanDominatingConnectedSetOrderSubsetIterationExecutable calcMeanDominatingConnectedSetOrderTarjanRemoveExecutable
