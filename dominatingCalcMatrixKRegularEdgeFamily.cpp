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
map< vector<int>, int> memo;

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
        
        memo[state]=stateCtr++;
        matrixDim++;
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
    return memo[stateX];
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
    leaveCycleDegree=K-1;
    nbLeaves=2;
    nbNonLeaves=0;

    nbCellsFirstPart=leaveCycleDegree*nbLeaves;
    nbCellsSecondPart=nbLeaves;
    nbCellsThirdPart=nbNonLeaves;
    nbCells=nbCellsFirstPart+nbCellsSecondPart+nbCellsThirdPart;

    
    vector<int> emp;    
    // make adjacency list of whole graph
    // indices of cycle get indices 0...nbCellsFirstPart-1
    adjacencyListWholeGraph.assign(nbCells,emp);

    // connect vertices of the cycle to other vertices on the cycle from the other partite set (except neighbors)
    for(int start=0; start<2*leaveCycleDegree; start++)
    {
        for(int i=3; i<3+(K-3)*2; i+=2)
        {
            int finish=(start+i)%(2*leaveCycleDegree);
            adjacencyListWholeGraph[start].push_back(finish);
        }
    }
    // make inner edge
    adjacencyListWholeGraph[nbCells-2].push_back(nbCells-1);
    adjacencyListWholeGraph[nbCells-1].push_back(nbCells-2);

    // connect edge with cycle
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<leaveCycleDegree; j++)
        {
            adjacencyListWholeGraph[nbCells-2+i].push_back(i+2*j);
            adjacencyListWholeGraph[i+2*j].push_back(nbCells-2+i);    
        }
    }
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
