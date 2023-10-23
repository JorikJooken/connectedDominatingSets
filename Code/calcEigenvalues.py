import numpy as np
from scipy.linalg import eigh as largest_eigh
from scipy.sparse.linalg import eigs
from scipy.sparse.linalg.eigen.arpack import eigsh as largest_eigsh

f=open("testMatrix.txt","r")

#f=open("matrix2MoreEfficient.txt","r")
#f=open("matrix2.txt","r")
#f=open("family2MoreEfficient.txt","r")
#f=open("family3MoreEfficient.txt","r")
#f=open("familyStarOn4Vertices.txt","r")
#f=open("domMatrixFam1.txt","r")
#f=open("domMatrixFam2.txt","r")
#f=open("domMatrixFam3.txt","r")
#f=open("testMatrix.txt","r")
#f=open("anotherTestMatrix.txt","r")
#f=open("testingMatrix.txt","r")
#f=open("family5Reg10Vertices.txt","r")

lines=f.readlines()
f.close()

#f=open("matrix3MoreEfficient.txt","w")
start=0
for i in range(len(lines)):
    if lines[i].count("s")>0 or lines[i].count("Coloring")>0:
        start=i+1
    #else:
    #    f.write(lines[i])
lines=lines[start:]
n=int(lines[0])
#f.close()

#print(f"n:{n}")
arr=[[] for i in range(n)]
for i in range(1,n+1):
    chunks=lines[i].split(" ")
    #print(len(chunks))
    for j in range(n):
        arr[i-1].append(int(chunks[j].strip("\n")))

"""
for i in range(5):
    for j in range(5):
        print(arr[i][j])
"""

# Define your matrix (replace this with your actual matrix)
matrix = np.array(arr,dtype=np.double)
arr=[]
lines=[]

# Calculate eigenvalues

vals, vecs = eigs(matrix, k=6)
print(vals)

"""
eigenvalues = np.linalg.eigvals(matrix)
eigenvalues=sorted(eigenvalues)[::-1]

for e in eigenvalues:
    print(e)
"""
