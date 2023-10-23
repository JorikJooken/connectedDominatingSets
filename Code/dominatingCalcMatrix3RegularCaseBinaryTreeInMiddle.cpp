#include <bits/stdc++.h>

using namespace std;

vector< vector<int> > matrix;
int leaveCycleDegree; // to how many vertices of the cycle is each leaf of the "binary tree" connected?
int nbLeaves; // number of leaves of "binary tree"
int nbNonLeaves; // number of non-leaves of "binary tree"

int nbCellsFirstPart;
int nbCellsSecondPart;
int nbCellsThirdPart;
int nbCells;

int matrixDim=0;

vector< vector<int> > adjacencyListBinaryTree;
vector< vector<int> > adjacencyListWholeGraph;

// 0 means not selected and is dominated (either by previous column or current column)
// 1 means selected and connected to first column ("fixed")
// 2 means selected, but loose
// 3 means not selected and must be dominated in the future
vector<int> state;

vector< vector<int> > allStates;

int stateCtr=0;
map< vector<int>, map< vector< vector<int> >, int > > memo;

int K;

void generateStatesCycle(int currIdx)
{
    //cerr << "currIdx2: " << currIdx << endl;
    if(currIdx==nbCellsFirstPart)
    {
        // check if there is at least one fixed component
        bool someFixed=false;
        for(int i=0; i<nbCellsFirstPart; i++)
        {
            if(state[i]==1) someFixed=true;
        }
        if(!someFixed) return;

        // every state 0 in the tree should have a 1 or 2 (i.e. selected) neighbor
        for(int i=nbCellsFirstPart; i<nbCells; i++)
        {
            if(state[i]!=0) continue;
            bool someSelected=false;
            for(int neigh : adjacencyListWholeGraph[i])
            {
                if(state[neigh]==1 || state[neigh]==2)
                {
                    someSelected=true;
                    break;
                }
            }
            if(!someSelected) return;
        }

        // check that each selected connected component of the tree is attached to some vertex on the cycle
        queue<int> q;
        vector<bool> vis(nbCells,false);
        for(int i=nbCellsFirstPart; i<nbCells; i++)
        {
            if(vis[i]) continue;
            if(state[i]==0) continue;
            if(state[i]==3)
            {
                cout << "Impossible!" << endl;
                exit(0);
            }
            q.push(i);
            vis[i]=true;
            bool someCycleVertexAttached=false;
            while(!q.empty())
            {
                int now=q.front();
                q.pop();
                for(int neigh : adjacencyListWholeGraph[now])
                {
                    if(vis[neigh]) continue;
                    if(state[neigh]==0) continue;
                    if(state[neigh]==3)
                    {
                        cout << "Impossible!" << endl;
                        exit(0);
                    }
                    q.push(neigh);
                    vis[neigh]=true;
                    if(neigh<nbCellsFirstPart) someCycleVertexAttached=true;
                }
            }
            if(!someCycleVertexAttached) return;
        }
        
        // check for each coloring of binary tree
        // and for each *leaveCycleDegree* vertices on the cycle how many unselected/fixed/loose vertices there are
        // unimplemented optimisation: can probably be done per component instead of per leave

        vector<int> coloringOfBinaryTree;
        for(int i=nbCellsFirstPart; i<nbCells; i++)
        {
            coloringOfBinaryTree.push_back(state[i]);
        }
        vector< vector<int> > counts(nbLeaves, vector<int>(4,0));
        for(int i=0; i<nbLeaves; i++)
        {
            for(int j=0; j<leaveCycleDegree; j++)
            {
                int idx=i*leaveCycleDegree+j;
                counts[i][state[idx]]++;
            }
        }

        // check if some ordering property is satisfied
        // the leaves of the binary tree should "in some sense" be sorted (details in the code below)
        bool orderingPropertySatisfied=true;
        if(K>=2)
        {
            for(int length=nbLeaves/2; 2*length<=nbLeaves && orderingPropertySatisfied; length*=2)
            {
                for(int i=0; i<nbLeaves && orderingPropertySatisfied; i+=2*length)
                {
                    vector<int> vec1(length);
                    vector<int> vec2(length);
                    for(int j=0; j<length; j++)
                    {
                        vec1[j]=state[nbCellsFirstPart+i+j];
                        vec2[j]=state[nbCellsFirstPart+i+length+j];
                    }
                    if(vec1>vec2) orderingPropertySatisfied=false;
                }
            }
        }

        // check if it is the first time such a state is encountered
        // for the current binary tree coloring, did we already see these counts? If yes: isomorphism
        if(orderingPropertySatisfied && memo[coloringOfBinaryTree].count(counts)==0)
        {
            memo[coloringOfBinaryTree][counts]=stateCtr++;
            matrixDim++;
            cout << "New state:" << endl;
            cout << "Coloring of binary tree: ";
            for(int x : coloringOfBinaryTree)
            {
                cout << x << " ";
            }
            cout << endl;
            cout << "Counts:" << endl;
            for(int i=0; i<nbLeaves; i++)
            {
                for(int j=0; j<=3; j++)
                {
                    cout << counts[i][j] << " vertices of the cycle attached to vertex " << i << " of the binary tree have state " << j << endl;
                }
            }
        }
        allStates.push_back(state);
        return;
    }
    // not selected, fixed or loose
    // only assign a color if it is compatible with its neighbors
    for(int i=0; i<=3; i++)
    {
        bool ok=true;
        for(int neigh : adjacencyListWholeGraph[currIdx])
        {
            if(state[neigh]==-1) continue;
            if(i==1 && (state[neigh]==2 || state[neigh]==3)) ok=false; // one fixed and one loose/must be dominated in the future
            if(i==2 && (state[neigh]==1 || state[neigh]==3)) ok=false; // one loose and one fixed/must be dominated in the future
            if(i==3 && (state[neigh]==1 || state[neigh]==2)) ok=false; // one must be dominated in the future and one fixed/loose
        }
        if(ok)
        {
            state[currIdx]=i;
            generateStatesCycle(currIdx+1);
            state[currIdx]=-1;
        }
    }
}

void generateStatesBinaryTree(int currIdx)
{
    //cerr << "currIdx:" << currIdx << endl;
    if(currIdx==nbCells)
    {
        // now choose for each vertex of the cycle what should happen
        generateStatesCycle(0);
        return;
    }
    // not selected, fixed or loose
    // only assign a color if it is compatible with its neighbors
    // note that the state cannot be 3 for the vertices of the binary tree
    for(int i=0; i<=2; i++)
    {
        bool ok=true;
        for(int neigh : adjacencyListWholeGraph[currIdx])
        {
            if(state[neigh]==-1) continue;
            if(i==1 && (state[neigh]==2)) ok=false; // one fixed and one loose
            if(i==2 && (state[neigh]==1)) ok=false; // one loose and one fixed
            
        }
        if(ok)
        {
            state[currIdx]=i;
            generateStatesBinaryTree(currIdx+1);
            state[currIdx]=-1;
        }
    }
}

int state_to_index(vector<int> &stateX)
{
    vector<int> coloringOfBinaryTree;
    for(int i=nbCellsFirstPart; i<nbCells; i++)
    {
        coloringOfBinaryTree.push_back(stateX[i]);
    }
    vector< vector<int> > counts(nbLeaves, vector<int>(4,0));
    for(int i=0; i<nbLeaves; i++)
    {
        for(int j=0; j<leaveCycleDegree; j++)
        {
            int idx=i*leaveCycleDegree+j;
            counts[i][stateX[idx]]++;
        }
    }
    // order the leaves of the binary tree "in some sense" (details in the code below)
    if(K>=2)
    {
        for(int length=nbLeaves/2; 2*length<=nbLeaves; length*=2)
        {
            for(int i=0; i<nbLeaves; i+=2*length)
            {
                vector<int> vec1(length);
                vector<int> vec2(length);
                for(int j=0; j<length; j++)
                {
                    vec1[j]=coloringOfBinaryTree[i+j];
                    vec2[j]=coloringOfBinaryTree[i+length+j];
                }
                // swap necessary
                if(vec1>vec2)
                {
                    for(int j=0; j<length; j++)
                    {
                        swap(coloringOfBinaryTree[i+j],coloringOfBinaryTree[i+length+j]);
                        swap(counts[i+j],counts[i+length+j]);
                    }
                    // also swap the internal nodes if necessary
                    // only implemented for K=3!!!
                    if(length==2 && i==0)
                    {
                        swap(coloringOfBinaryTree[4],coloringOfBinaryTree[5]);
                    }
                }
            }
        }
    }

    return memo[coloringOfBinaryTree][counts];
}

bool can_be_preceded(vector<int> &state1, vector<int> &state2)
{
    bool ok=true;

    // a loose cell must be followed by another loose cell or a fixed cell
    queue<int> q;
    vector<bool> vis(nbCells,false);
    for(int i=0; i<nbCells && ok; i++)
    {
        if(vis[i]) continue;
        if(state2[i]==0 || state2[i]==1 || state2[i]==3) continue;
        vector<int> component;
        q.push(i);
        component.push_back(i);
        vis[i]=true;
        while(!q.empty())
        {
            int now=q.front();
            q.pop();
            for(int neigh : adjacencyListWholeGraph[now])
            {
                if(vis[neigh]) continue;
                if(state2[neigh]==0) continue;
                if(state2[neigh]==1 || state2[neigh]==3)
                {
                    cout << "This should be impossible!" << endl;
                    exit(0);
                }
                q.push(neigh);
                component.push_back(neigh);
                vis[neigh]=true;
            }
        }
        bool someFollows=false;
        for(int x : component)
        {
            if(x>=nbCellsFirstPart) continue;
            if(state1[x]==1 || state1[x]==2) someFollows=true;
        }
        if(!someFollows) ok=false;
    }

    // a loose cell cannot be preceded by a fixed cell
    for(int i=0; i<nbCellsFirstPart && ok; i++)
    {
        if(state1[i]==2 && state2[i]==1) ok=false;
    }

    // a fixed cell must be preceded by a fixed cell
    vis.assign(nbCells,false);
    for(int i=0; i<nbCells && ok; i++)
    {
        if(vis[i]) continue;
        if(state1[i]==0 || state1[i]==2 || state1[i]==3) continue;
        vector<int> component;
        q.push(i);
        component.push_back(i);
        vis[i]=true;
        while(!q.empty())
        {
            int now=q.front();
            q.pop();
            for(int neigh : adjacencyListWholeGraph[now])
            {
                if(vis[neigh]) continue;
                if(state1[neigh]==0) continue;
                if(state1[neigh]==2 || state1[neigh]==3)
                {
                    cout << "This should be impossible!" << endl;
                    exit(0);
                }
                q.push(neigh);
                component.push_back(neigh);
                vis[neigh]=true;
            }
        }
        bool somePrecedes=false;
        for(int x : component)
        {
            if(x>=nbCellsFirstPart) continue;
            if(state2[x]==1) somePrecedes=true;
        }
        if(!somePrecedes)
        {
            ok=false;
        }
    }

    // an unselected cell must be dominated by some neighbor
    for(int i=0; i<nbCells && ok; i++)
    {
        if(state1[i]>=1) continue;
        bool someNeighborSelected=false;
        for(int neigh : adjacencyListWholeGraph[i])
        {
            if(state1[neigh]==1 || state1[neigh]==2)
            {
                someNeighborSelected=true;
            }
        }
        if(!someNeighborSelected && i<nbCellsFirstPart)
        {
            if(state2[i]==1 || state2[i]==2) someNeighborSelected=true;
        }
        if(!someNeighborSelected) ok=false;
    }


    // a must be dominated in the future must be succeeded by a selected cell
    for(int i=0; i<nbCells && ok; i++)
    {
        if(state2[i]<=2) continue;
        if(i>=nbCellsFirstPart)
        {
            cout << "Should be impossible!" << endl;
            exit(0);
        }   
        if(state1[i]!=1 && state1[i]!=2) ok=false;
    }
    
    // a must be dominated in the future cannot be preceded by a selected cell
    for(int i=0; i<nbCells && ok; i++)
    {
        if(state1[i]<=2) continue;
        if(i>=nbCellsFirstPart)
        {
            cout << "Should be impossible!" << endl;
            exit(0);
        }
        if(state2[i]==1 || state2[i]==2) ok=false;  
    }
    return ok;
}

// warning: only implemented correctly for K=1, K=2 and K=3
// for K>=4, I did not implement the "binary tree" logic that is required for sorting 
// (I only looked at the special cases of K=1, K=2 and K=3), because the matrix is too big for K>=4 anyways.
int main()
{
    cerr << "Program started" << endl;
    cin >> K;
    if(K==1)
    {
        leaveCycleDegree=3;
        nbLeaves=1;
        nbNonLeaves=0;
    }
    else
    {
        leaveCycleDegree=2;
        nbLeaves=(1LL<<(K-1));
        nbNonLeaves=nbLeaves-2;
    }
    nbCellsFirstPart=leaveCycleDegree*nbLeaves;
    nbCellsSecondPart=nbLeaves;
    nbCellsThirdPart=nbNonLeaves;
    nbCells=nbCellsFirstPart+nbCellsSecondPart+nbCellsThirdPart;

    // make adjacency list of binary tree
    // indices start from "left bottom leave" and go from left to right, then down to up
    // e.g.
    //  4 ---- 5
    // /  \  /   \
    // 0   1 2    3
    int nbVerticesBinaryTree=nbLeaves+nbNonLeaves;
    //cerr << nbLeaves << " " << nbNonLeaves << " " << nbVerticesBinaryTree << endl;
    vector<int> emp;
    adjacencyListBinaryTree.assign(nbVerticesBinaryTree,emp);

    int powerOfTwo=nbLeaves;
    int decrementingCounter=powerOfTwo;
    int binaryTreeIDCounter=0;
    while(true)
    {
        if(decrementingCounter==0)
        {
            powerOfTwo/=2;
            decrementingCounter=powerOfTwo;
        }
        if(powerOfTwo<=2) break;
        int u=binaryTreeIDCounter;
        int x=decrementingCounter+(decrementingCounter%2);
        int v=u-(u%2)+(powerOfTwo-((powerOfTwo-x)/2));
        adjacencyListBinaryTree[u].push_back(v);
        adjacencyListBinaryTree[v].push_back(u);
        binaryTreeIDCounter++;
        decrementingCounter--;
    }
    // add last edge of "binary tree"
    if(K>=2)
    {
        int u=nbVerticesBinaryTree-1;
        int v=nbVerticesBinaryTree-2;
        adjacencyListBinaryTree[u].push_back(v);
        adjacencyListBinaryTree[v].push_back(u);
    }
    
    // make adjacency list of whole graph
    // indices of cycle get indices 0...nbCellsFirstPart-1
    adjacencyListWholeGraph.assign(nbCells,emp);
    for(int i=0; i<nbLeaves; i++)
    {
        for(int j=0; j<leaveCycleDegree; j++)
        {
            int u=nbCellsFirstPart+i;
            int v=i*leaveCycleDegree+j;
            adjacencyListWholeGraph[u].push_back(v);
            adjacencyListWholeGraph[v].push_back(u);
        }
    }
    for(int i=0; i<nbVerticesBinaryTree; i++)
    {
        for(int neigh : adjacencyListBinaryTree[i])
        {
            int u=nbCellsFirstPart+i;
            int v=nbCellsFirstPart+neigh;
            adjacencyListWholeGraph[u].push_back(v);
        }
    }


    // print binary tree
    /*for(int i=0; i<nbVerticesBinaryTree; i++)
    {
        cout << "Neighbors of " << i << ":";
        for(int neighbor : adjacencyListBinaryTree[i])
        {
            cout << " " << neighbor;
        }
        cout << endl;
    }*/

    state.assign(nbCells,-1);
    // generate all states and calculate the matrix dimension
    // first choose for each vertex of the binary tree whether it should be not selected/fixed/loose
    // then choose for each vertex of the cycle whether it should be not selected/fixed/loose
    cerr << "Starting to generate states" << endl;
    generateStatesBinaryTree(nbCellsFirstPart);
    cerr << "Matrix dim: " << matrixDim << endl;
    cout << matrixDim << endl;
    cerr << "total number of states: " << allStates.size() << endl;

    matrix.assign(matrixDim,vector<int>(matrixDim,0));
    vector<bool> indexSeen(matrixDim,false);
    int progressCtr=0;
    for(vector<int> state1 : allStates)
    {
        if(progressCtr%1000==0) cerr << "progressCtr: " << progressCtr << endl;
        progressCtr++;
        int idx1=state_to_index(state1);
        if(indexSeen[idx1]) continue;
        indexSeen[idx1]=true;
        for(vector<int> state2 : allStates)
        {
            if(can_be_preceded(state1,state2))
            {
                int idx2=state_to_index(state2);
                /*if(idx1==3 && idx2==1)
                {
                    cout << "state1:" << endl;
                    for(int x : state1)
                    {
                        cout << x << " ";
                    }
                    cout << endl;
                    cout << "state2:" << endl;
                    for(int x : state2)
                    {
                        cout << x << " ";
                    }
                    cout << endl;
                }*/
                matrix[idx1][idx2]++;
            }
        }
    }
    for(int i=0; i<matrixDim; i++)
    {
        for(int j=0; j<matrixDim; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}
