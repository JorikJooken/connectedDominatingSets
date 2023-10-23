#include <bits/stdc++.h>

// Unsafe because no defined behaviour if character = 0. ctz and clz work with 32 bit numbers.
#define unsafePrev(character, current) (__builtin_ctz(character) - current >= 0 ? -1 : current -__builtin_clz((character) << (32 - current)) - 1)

#define prev(character,current) (character ? unsafePrev(character,current) : -1)

using namespace std;

const int nb_bits=32;
#define MAXVERTICES 32

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

#define left_shifted(n,x) ((n)<<(x))
#define set_bit(n,i) ((n)|=left_shifted(1LL,i))
#define count_bits(n) (__builtin_popcountll((n)))
//#define index_rightmost_bit(n) (count_bits((n)^(n-1))-1)
#define index_rightmost_bit(n) (__builtin_ctzll(n))
#define rightmost_bit_removed(n) ((n)&((n)-1))
#define traverse_through_neighbors_nest1(x,neighbor) for(long long (neighbor)=index_rightmost_bit((x)),DESTROYED_MASK1=(x); (DESTROYED_MASK1)!=0; (DESTROYED_MASK1)=rightmost_bit_removed((DESTROYED_MASK1)),(neighbor)=index_rightmost_bit((DESTROYED_MASK1)))
#define traverse_through_neighbors_nest2(x,neighbor) for(long long (neighbor)=index_rightmost_bit((x)),DESTROYED_MASK2=(x); (DESTROYED_MASK2)!=0; (DESTROYED_MASK2)=rightmost_bit_removed((DESTROYED_MASK2)),(neighbor)=index_rightmost_bit((DESTROYED_MASK2)))

int n;
long long adjacencyList[MAXVERTICES];

long long totSumVertices;
long long totNumberConnectedGraphs;

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
        totNumberConnectedGraphs=0;
        for(long long mask=1;mask<=(1LL<<n);mask++)
        {
            long long visited=0;
            queue<int> q;
            long long cp_mask=mask;
            traverse_through_neighbors_nest1(cp_mask,neighbor)
            {
                set_bit(visited,neighbor);
                q.push(neighbor);
                break;
            }
            while(!q.empty())
            {
                int nd=q.front();
                q.pop();
                long long all_unvisited_neighbors=((adjacencyList[nd]&mask)&(~visited));
                traverse_through_neighbors_nest1(all_unvisited_neighbors,neighbor)
                {
                    set_bit(visited,neighbor);
                    q.push(neighbor);
                }
            }
            if(visited==mask)
            {
                totSumVertices+=count_bits(mask);
                totNumberConnectedGraphs++;
            }
        }
        //double mean=1.0*totSumVertices/totNumberConnectedGraphs;
        if(totNumberConnectedGraphs>best)
        {
            best=totNumberConnectedGraphs;
            all_best_graphs.clear();
            all_best_graphs.push_back(line);
        }
        else if(totNumberConnectedGraphs==best)
        {
            all_best_graphs.push_back(line);
        }
    }
    printf("Best was: %lld\n",best);
    printf("The corresponding c(G): %.12f\n", 0.5*pow(totNumberConnectedGraphs,1.0/n));
    printf("An exhaustive list of all best graphs in graph6 format:\n");
    for(string best_graph : all_best_graphs)
    {
        printf("%s\n",best_graph.c_str());
    }
    cerr << nb_graphs_read_from_input << " graphs were read from input" << endl;
    return 0;
}
