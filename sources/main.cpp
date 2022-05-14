#include "../includes/get_graph_edges.hpp"
#include "../includes/utils.hpp"
#include "../includes/get_adjacency_matrix.hpp"
#include "../includes/algorithm.hpp"

// Declaration of visited array
int a, b;


// Function to detect if the
// graph contains a cycle or not
bool detect_cycle(int node, int par, vector<bool>& vis)
{
	// Marking the current node visited
	vis[node] = 1;
	// Traversing to the childs
	// of the current node
	// Simple DFS approach
	for (auto child : adj[node]) {
		if (vis[child] == 0) {
			if (detect_cycle(child, node))
				return true;
		}

		// Checking for a back-edge
		else if (child != par) {
			// A cycle is detected
			// Marking the end-vertices
			// of the cycle
			a = child;
			b = node;
			return true;
		}
	}
	return false;
}

vector<int> simple_cycle;

// Function to get the simple cycle from the
// end-vertices of the cycle we found from DFS
void find_simple_cycle(int a, int b)
{
	// Parent array to get the path
	vector<int> par(MAXN, -1);

	// Queue for BFS
	queue<int> q;
	q.push(a);
	bool ok = true;
	while (!q.empty()) {
		int node = q.front();
		q.pop();
		vis[node] = 1;
		for (auto child : adj[node]) {
			if (node == a && child == b)
				// Ignoring the direct edge
				// between a and b
				continue;

			if (vis[child] == 0) {
				// Updating the parent array
				par[child] = node;

				if (child == b) {
					// If b is reached,
					// we've found the
					// shortest path from
					// a to b already
					ok = false;
					break;
				}
				q.push(child);
				vis[child] = 1;
			}
		}
		// If required task is done
		if (ok == false)
			break;
	}

	// Cycle starting from a
	simple_cycle.push_back(a);
	int x = b;

	// Until we reach a again
	while (x != a) {
		simple_cycle.push_back(x);
		x = par[x];
	}
}

int main(int argc, char *argv[])
{
    size_t  len;
    if (argc < 2) {
        cerr << "ERROR: No path file from which to read the Graph and no specified length of graph\n";
        exit(1);
    } else if (argc < 3) {
        cerr << "ERROR: please specify which length of simple cycles you want to find!\n"
             << "By default length is chosen as 3\n";
        len = 3;
    }

    string  file = argv[2];
    if (!isValidFilename(file))
    {
        cerr << "ERROR: Not Valid file extension. MUST BE .txt\n";
        exit(2);
    }
    vector<edge> edges;
    getUserInput(edges, file);

    size_t  size = adjacencyMatrixSize(edges);
    size_t**  adjacency_matrix = newDynamic(size);
    buildAndSimplifyAdjacencyMx(adjacency_matrix, edges, size);
    displayMatrix(adjacency_matrix, size);

    len = *argv[1] - '0';
    // findSimpleCycles(adjacency_matrix, len, size);


    vector<bool> visited(size);
    if (detect_cycle(1, -1, visited) == true) {
		// If cycle is present

		// Resetting the visited array
		// for simple cycle finding
		visited = vector<bool>(size, false);
		find_simple_cycle(a, b);

		// Printing the simple cycle
		cout << "A simple cycle: ";
		for (auto& node : simple_cycle) {
			cout << node << " => ";
		}
		cout << a;
		cout << "\n";
	}
	else {
		cout << "The Graph doesn't "
			<< "contain a cycle.\n";
	}
}
