#include <bits/stdc++.h>

// Unsafe because no defined behaviour if character = 0. ctz and clz work with 32 bit numbers.
#define unsafePrev(character, current) (__builtin_ctz(character) - current >= 0 ? -1 : current -__builtin_clz((character) << (32 - current)) - 1)

#define prev(character,current) (character ? unsafePrev(character,current) : -1)

using namespace std;

const int nb_bits=32;
#define MAXVERTICES 32

// graph 
int n;
long long adjacencyList[MAXVERTICES];

// connected induced set 
long long totSumVertices;
long long totNumberDominatingConnectedGraphs;
unordered_set<long long> seen;

// cutvertices
long long articulation_vertex_bitset;
int dfs_low[MAXVERTICES];
int dfs_num[MAXVERTICES];
int dfsNumberCounter;
#define DFS_WHITE -1
int dfs_parent[MAXVERTICES];
int dfsRoot;
int rootChildren;

#define left_shifted(n,x) ((n)<<(x))
#define set_bit(n,i) ((n)|=left_shifted(1LL,i))
#define count_bits(n) (__builtin_popcountll((n)))
//#define index_rightmost_bit(n) (count_bits((n)^(n-1))-1)
#define index_rightmost_bit(n) (__builtin_ctzll(n))
#define rightmost_bit_removed(n) ((n)&((n)-1))
#define traverse_through_neighbors_nest1(x,neighbor) for(long long (neighbor)=index_rightmost_bit((x)),DESTROYED_MASK1=(x); (DESTROYED_MASK1)!=0; (DESTROYED_MASK1)=rightmost_bit_removed((DESTROYED_MASK1)),(neighbor)=index_rightmost_bit((DESTROYED_MASK1)))
#define traverse_through_neighbors_nest2(x,neighbor) for(long long (neighbor)=index_rightmost_bit((x)),DESTROYED_MASK2=(x); (DESTROYED_MASK2)!=0; (DESTROYED_MASK2)=rightmost_bit_removed((DESTROYED_MASK2)),(neighbor)=index_rightmost_bit((DESTROYED_MASK2)))

// complexity O(|V|+|E|)
void articulationPointAndBridge(long long vertexSet, int u)
{
    //cerr << "vertexSet and u: "<< vertexSet << " " << u << endl;
    dfs_low[u] = dfs_num[u] = dfsNumberCounter++; //dfs_low[u] <= dfs_num[u]
    traverse_through_neighbors_nest1((adjacencyList[u]&vertexSet),v)
    {
       if(dfs_num[v] == DFS_WHITE) //tree edge
       {
            dfs_parent[v] = u;
            if(u==dfsRoot) rootChildren++;
            articulationPointAndBridge(vertexSet, v);
            if(dfs_low[v] >= dfs_num[u]) //articulation point
            {
                //cerr << "u and v: " << u << " " << v << endl;
                //cerr << "low of v and num of u: " << dfs_low[v] << " " << dfs_num[u] << endl;
                if(u!=dfsRoot) articulation_vertex_bitset=(articulation_vertex_bitset|(1LL<<u));
            }
            if(dfs_low[v]<dfs_low[u])
            {
                dfs_low[u] = dfs_low[v]; 
            }              
       }
       else if(v != dfs_parent[u])
       {
            if(dfs_num[v]<dfs_low[u])
            {
                dfs_low[u]=dfs_num[v];
            }
       }
    }    
}

int getNumberOfVertices(string graphString) 
{
	if(graphString.size() == 0){
        printf("Error: String is empty.\n");
        abort();
    }
    else if((graphString[0] < 63 || graphString[0] > 126) && graphString[0] != '>') {
    	printf("Error: Invalid start of graphstring.\n");
    	abort();
    }

	int index = 0;
	if (graphString[index] == '>') { // Skip >>graph6<< header.
		index += 10;
	}

	if(graphString[index] < 126) { // 0 <= n <= 62
		return (int) graphString[index] - 63;
	}

	else if(graphString[++index] < 126) { 
		int number = 0;
		for(int i = 2; i >= 0; i--) {
			number |= (graphString[index++] - 63) << i*6;
		}
		return number;
	}

	else if (graphString[++index] < 126) {
		int number = 0;
		for (int i = 5; i >= 0; i--) {
			number |= (graphString[index++] - 63) << i*6;
		}
		return number;
	}

	else {
		printf("Error: Format only works for graphs up to 68719476735 vertices.\n");
		abort();
	}
}

void loadGraph(string graphString, int numberOfVertices, long long adjacencyList[]) {
	int startIndex = 0;
	if (graphString[startIndex] == '>') { // Skip >>graph6<< header.
		startIndex += 10;
	}
	if (numberOfVertices <= 62) {
		startIndex += 1;
	}
	else if (numberOfVertices <= MAXVERTICES) {
		startIndex += 4;
	}
	else {
		printf("Error: Program can only handle graphs with %d vertices or fewer.\n",MAXVERTICES);
		abort();
	}

	for (int vertex = 0; vertex < numberOfVertices; vertex++) { //Initialize adjacencyList.
		adjacencyList[vertex] = 0;
	}

	int currentVertex = 1;
	int sum = 0; 
	for (int index = startIndex; index<graphString.size(); index++) {
		int i;
		for (i = prev(graphString[index] - 63, 6); i != -1; i = prev(graphString[index] - 63, i)) {
			while(5-i+(index-startIndex)*6 - sum >= 0) {
				sum += currentVertex;
				currentVertex++;
			}
			sum -= --currentVertex;
			int neighbour = 5-i+(index - startIndex)*6 - sum;
            adjacencyList[currentVertex]|=(1LL<<neighbour);
            adjacencyList[neighbour]|=(1LL<<currentVertex);
		}
	}
}

void go(long long vertex_set)
{
    if(seen.count(vertex_set)>0) return;
    seen.insert(vertex_set);
    totSumVertices+=count_bits(vertex_set);
    totNumberDominatingConnectedGraphs++;
    if(count_bits(vertex_set)==1) return;

    // calculate cut vertices
    articulation_vertex_bitset=0;
    dfsNumberCounter=0;
    traverse_through_neighbors_nest1(vertex_set,u)
    {
        dfs_num[u]=DFS_WHITE;
    }
    traverse_through_neighbors_nest1(vertex_set,u)
    {
        if(dfs_num[u]==DFS_WHITE)
        {
            dfsRoot = u; rootChildren=0;
            //cerr << vertex_set << " " << u << endl;
            articulationPointAndBridge(vertex_set,u);
            if(rootChildren>1)
            {
                articulation_vertex_bitset=(articulation_vertex_bitset|(1LL<<dfsRoot));
            }
        }
    }
    traverse_through_neighbors_nest1((vertex_set&(~articulation_vertex_bitset)),neighbor)
    {
        long long new_vertex_set=(vertex_set^(1LL<<neighbor));
        bool neighOfNeighAttached=true;
        traverse_through_neighbors_nest2((adjacencyList[neighbor]&(~new_vertex_set)),neighbor2)
        {
            if(adjacencyList[neighbor2]&new_vertex_set)
            {
            }
            else
            {
                neighOfNeighAttached=false;
                break;
            }
        }
        if(neighOfNeighAttached) go(new_vertex_set);
    }
    return;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    long long nb_graphs_read_from_input=0;
    string line;
    long long best=0;
    vector<string> all_best_graphs;
    while(getline(cin,line))
    {
        //line+="\n";
        nb_graphs_read_from_input++;
        n = getNumberOfVertices(line);
        loadGraph(line,n,adjacencyList);
        // continue here later
        totSumVertices=0;
        totNumberDominatingConnectedGraphs=0;
        seen.clear();
        go((1LL<<n)-1);
        //double mean=1.0*totSumVertices/totNumberDominatingConnectedGraphs;
        if(totNumberDominatingConnectedGraphs>best)
        {
            best=totNumberDominatingConnectedGraphs;
            all_best_graphs.clear();
            all_best_graphs.push_back(line);
        }
        else if(totNumberDominatingConnectedGraphs==best)
        {
            all_best_graphs.push_back(line);
        }
    }
    printf("Best was: %lld\n",best);
    printf("The corresponding c(G): %.12f\n", 0.5*pow(best,1.0/n));
    printf("An exhaustive list of all best graphs in graph6 format:\n");
    for(string best_graph : all_best_graphs)
    {
        printf("%s\n",best_graph.c_str());
    }
    cerr << nb_graphs_read_from_input << " graphs were read from input" << endl;
    return 0;
}
