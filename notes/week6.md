# Week 6 Notes

---

# Day 1 — Graph Representation and CAN Dispatcher

## Main goals

```text
Learn graph representation.
Understand nodes, edges, neighbors, and adjacency lists.
Solve Flood Fill.
Create CanDispatcher.
Move frame ID routing out of TelemetryDecoder.
Continue using simulated CAN logs for decoder testing.
```

---

## Notes questions

```text
1. What is an adjacency list?
2. What is a dispatcher?
3. Why should frame routing be separated from decoding?
4. How does CAN traffic resemble message dispatching?
```

---

## What is a graph?

A graph is a structure made of nodes and edges.

```text
node = an object or point
edge = a connection between nodes
neighbor = a node connected to another node
```

Example:

```text
1 -- 2
|    |
3 -- 4
```

Simple explanation:

```text
A graph represents relationships between things.
```

---

## What is an adjacency list?

An adjacency list stores each node with a list of its neighbors.

C++ example:

```cpp
std::unordered_map<int, std::vector<int>> graph;
```

Example data:

```cpp
graph[1] = {2, 3};
graph[2] = {1, 4};
graph[3] = {1, 4};
graph[4] = {2, 3};
```

Meaning:

```text
Node 1 connects to nodes 2 and 3.
Node 2 connects to nodes 1 and 4.
Node 3 connects to nodes 1 and 4.
Node 4 connects to nodes 2 and 3.
```

Simple explanation:

```text
An adjacency list maps each node to the nodes connected to it.
```

---

## Why use an adjacency list?

An adjacency list is useful because it stores only the connections that actually exist.

This is efficient for many real graphs where each node only connects to a few other nodes.

Simple explanation:

```text
An adjacency list is a compact way to store graph connections.
```

---

## Flood Fill as graph traversal

LeetCode 733, Flood Fill, can be viewed as graph traversal.

Each pixel is a node.

Each up, down, left, or right pixel is a possible neighbor.

The algorithm starts at one pixel and spreads to connected pixels with the same original color.

Simple explanation:

```text
Flood Fill is graph traversal on a 2D grid.
```

---

## Flood Fill code idea

```cpp
class Solution {
public:
    vector<vector<int>> floodFill(vector<vector<int>>& image,
                                  int sr,
                                  int sc,
                                  int color) {
        int original_color = image[sr][sc];

        if (original_color == color) {
            return image;
        }

        fill(image, sr, sc, original_color, color);

        return image;
    }

private:
    void fill(vector<vector<int>>& image,
              int row,
              int col,
              int original_color,
              int new_color) {
        if (row < 0 || row >= image.size()) {
            return;
        }

        if (col < 0 || col >= image[0].size()) {
            return;
        }

        if (image[row][col] != original_color) {
            return;
        }

        image[row][col] = new_color;

        fill(image, row + 1, col, original_color, new_color);
        fill(image, row - 1, col, original_color, new_color);
        fill(image, row, col + 1, original_color, new_color);
        fill(image, row, col - 1, original_color, new_color);
    }
};
```

---

## What is a dispatcher?

A dispatcher receives an input and routes it to the correct handler.

In this project:

```text
CanDispatcher receives a CAN frame.
It checks the CAN ID.
It calls the correct TelemetryDecoder function.
```

Example:

```text
0x100 -> decode_0x100
0x101 -> decode_0x101
0x102 -> decode_0x102
0x200 -> future vehicle telemetry decoder
```

Simple explanation:

```text
A dispatcher decides where a message should go.
```

---

## Why should frame routing be separated from decoding?

Frame routing and payload decoding are different jobs.

```text
CanDispatcher = chooses which handler should process the frame
TelemetryDecoder = converts payload bytes into meaningful values
FaultAnalyzer = checks decoded values for faults
```

This separation makes the project easier to understand, test, and expand.

Simple explanation:

```text
The dispatcher chooses the route, and the decoder interprets the data.
```

---

## How does CAN traffic resemble message dispatching?

CAN traffic contains many frames with different IDs.

Each ID represents a different message type.

Examples:

```text
0x100 = Analog Inputs
0x101 = Battery and Temperature
0x102 = Status Flags
0x200 = Vehicle Telemetry
```

The decoder project uses the CAN ID to route the frame to the correct handler.

Simple explanation:

```text
CAN IDs act like message types, and the dispatcher routes each message to the right decoder.
```

---

## Simulated logs for now

For now, the project continues using:

```text
data/sample_can_log.csv
```

The hardware CAN path worked with Waveshare USB-CAN, but live reading will come later.

Current workflow:

```text
sample CAN log
        ↓
C++ decoder
        ↓
decoded values, faults, and stats
```

Future workflow:

```text
STM32 CAN frame
        ↓
Waveshare USB-CAN
        ↓
live C++ reader
        ↓
decoded values, faults, and stats
```

Simple explanation:

```text
Simulated logs keep the decoder development clean until live USB-CAN reading is added.
```

---

## Day 1 main interview idea

```text
A graph can be represented with an adjacency list, where each node maps to its neighbors. Flood Fill is a graph traversal problem on a 2D grid. In the CAN decoder project, CanDispatcher separates frame routing from payload decoding by using the CAN ID to route each frame to the correct TelemetryDecoder function.
```

---

# Week 6 Day 1 — Graph Representation and CAN Dispatcher

## Interview questions

```text
1. What is a graph?
2. What is a node?
3. What is an edge?
4. What is a neighbor?
5. What is an adjacency list?
6. How can a graph be represented in C++?
7. How is Flood Fill related to graph traversal?
8. What is a dispatcher?
9. Why separate frame routing from decoding?
10. How does CAN traffic resemble message dispatching?
```

---

## What is a graph?

My answer:









Reference answer:

A graph is a structure made of nodes and edges.

It represents relationships or connections between items.

Simple version:

```text
A graph shows how things are connected.
```

---

## What is a node?

My answer:









Reference answer:

A node is an individual item or point in a graph.

Examples:

```text
a city
a person
a pixel
a network device
a CAN message handler
```

Simple version:

```text
A node is one item in a graph.
```

---

## What is an edge?

My answer:









Reference answer:

An edge is a connection between two nodes.

If node `1` connects to node `2`, then there is an edge between them.

Simple version:

```text
An edge is a link between nodes.
```

---

## What is a neighbor?

My answer:









Reference answer:

A neighbor is a node directly connected to another node.

If:

```text
1 -- 2
```

then node `2` is a neighbor of node `1`.

Simple version:

```text
A neighbor is a directly connected node.
```

---

## What is an adjacency list?

My answer:









Reference answer:

An adjacency list maps each node to a list of its neighbors.

Example:

```cpp
std::unordered_map<int, std::vector<int>> graph;
```

Example data:

```cpp
graph[1] = {2, 3};
graph[2] = {1, 4};
```

Simple version:

```text
An adjacency list stores each node with the nodes connected to it.
```

---

## How can a graph be represented in C++?

My answer:









Reference answer:

One common C++ representation is:

```cpp
std::unordered_map<int, std::vector<int>> graph;
```

The `int` key is the node.

The `std::vector<int>` stores that node's neighbors.

Simple version:

```text
Use a map from each node to a list of neighboring nodes.
```

---

## How is Flood Fill related to graph traversal?

My answer:









Reference answer:

Flood Fill treats an image like a graph.

Each pixel is a node.

Neighboring pixels are connected up, down, left, and right.

The algorithm visits connected pixels that match the original color.

Simple version:

```text
Flood Fill is graph traversal on a 2D grid.
```

---

## What is a dispatcher?

My answer:









Reference answer:

A dispatcher receives a message or input and routes it to the correct handler.

In this project, `CanDispatcher` receives a CAN frame and routes it based on the frame ID.

Simple version:

```text
A dispatcher decides which function should handle a message.
```

---

## Why separate frame routing from decoding?

My answer:









Reference answer:

Frame routing and payload decoding are different responsibilities.

```text
CanDispatcher = chooses the correct decoder function
TelemetryDecoder = interprets payload bytes
FaultAnalyzer = checks decoded values for faults
```

This makes the project cleaner and easier to extend.

Simple version:

```text
Routing decides where the frame goes; decoding explains what the bytes mean.
```

---

## How does CAN traffic resemble message dispatching?

My answer:









Reference answer:

CAN traffic contains frames with different IDs.

Each ID represents a different message type.

Examples:

```text
0x100 = Analog Inputs
0x101 = Battery and Temperature
0x102 = Status Flags
0x200 = Vehicle Telemetry
```

The dispatcher uses the CAN ID to route the frame to the correct decoder.

Simple version:

```text
CAN IDs act like message types, and the dispatcher routes each message to the correct handler.
```

---

## Week 6 Day 1 Core Interview Summary

My answer:









Reference answer:

A graph can be represented with an adjacency list, where each node maps to its neighbors. Flood Fill is a graph traversal problem on a 2D grid. In the CAN decoder project, `CanDispatcher` separates frame routing from payload decoding by using the CAN ID to route each frame to the correct `TelemetryDecoder` function.
---

# Day 2 — Grid DFS and CSV Parser

## Main goals

```text
Study grid DFS.
Solve Number of Islands.
Create a CSV/text parser for CAN logs.
Read ID, DLC, and eight data bytes from a text line.
Create CanFrame objects from log input.
Pass parsed frames through the dispatcher.
Continue using simulated logs until live USB-CAN input is added later.
```

---

## Notes questions

```text
1. How does DFS visit connected components?
2. How does CSV parsing work?
3. What errors can happen when reading logs?
4. Why is log input important before live input?
```

---

## How does DFS visit connected components?

DFS starts at one node or grid cell, marks it as visited, then recursively visits connected neighbors.

In a grid, neighbors usually mean:

```text
up
down
left
right
```

For Number of Islands, DFS starts on a land cell and visits all connected land cells.

Simple explanation:

```text
DFS follows one connected path as far as possible before returning and trying other paths.
```

---

## Number of Islands

LeetCode 200 counts connected groups of land.

```text
'1' = land
'0' = water
```

When the algorithm finds land, it counts one island and uses DFS to mark the entire connected island as visited.

Simple explanation:

```text
Each DFS call removes one full island from the grid.
```

---

## Number of Islands code idea

```cpp
class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        int islands = 0;

        for (int row = 0; row < grid.size(); row++) {
            for (int col = 0; col < grid[0].size(); col++) {
                if (grid[row][col] == '1') {
                    islands++;
                    dfs(grid, row, col);
                }
            }
        }

        return islands;
    }

private:
    void dfs(vector<vector<char>>& grid, int row, int col) {
        if (row < 0 || row >= grid.size()) {
            return;
        }

        if (col < 0 || col >= grid[0].size()) {
            return;
        }

        if (grid[row][col] != '1') {
            return;
        }

        grid[row][col] = '0';

        dfs(grid, row + 1, col);
        dfs(grid, row - 1, col);
        dfs(grid, row, col + 1);
        dfs(grid, row, col - 1);
    }
};
```

---

## How does CSV parsing work?

CSV parsing reads a line of text and splits it by commas.

Example input line:

```text
100,8,00,08,10,00,FF,0A,01,05
```

Fields:

```text
100 = CAN ID
8 = DLC
00 = byte 0
08 = byte 1
10 = byte 2
00 = byte 3
FF = byte 4
0A = byte 5
01 = byte 6
05 = byte 7
```

The parser converts those text tokens into numbers and stores them in a `CanFrame`.

Simple explanation:

```text
CSV parsing turns text log lines into structured CAN frame objects.
```

---

## CAN log input format

Current parser input format:

```text
id,dlc,b0,b1,b2,b3,b4,b5,b6,b7
```

Example:

```text
100,8,00,08,10,00,FF,0A,01,05
```

The ID and data bytes are read as hexadecimal values.

The DLC is read as a decimal value.

The parser also accepts `0x` prefixes.

Example:

```text
0x100,8,0x00,0x08,0x10,0x00,0xFF,0x0A,0x01,0x05
```

---

## Parser flow

The parser does this:

```text
open CSV file
read one line
skip empty lines
skip first line only if it starts with id,dlc
split line by commas
validate token count
parse ID
parse DLC
parse eight data bytes
create CanFrame
add CanFrame to vector
```

Then the project sends parsed frames through:

```text
CanFrame
        ↓
CircularBuffer
        ↓
CanDispatcher
        ↓
TelemetryDecoder
        ↓
FaultAnalyzer
        ↓
DecoderStats
```

---

## Safer header detection

A bad header check would be:

```text
If the first line contains any letter, treat it as a header.
```

That is unsafe because hexadecimal CAN data can contain letters:

```text
FF
0A
0x100
```

So this valid CAN line should not be skipped:

```text
100,8,00,08,10,00,FF,0A,07,01
```

The safer method checks for actual column names:

```text
first token == id
second token == dlc
```

Simple explanation:

```text
Header detection should check for real header names, not just letters.
```

---

## What errors can happen when reading logs?

Common log-reading errors include:

```text
missing file
empty line
header line
missing fields
extra fields
invalid hex value
byte larger than 0xFF
invalid DLC value
malformed row
wrong delimiter
incorrect header detection
```

The parser should skip malformed lines instead of crashing the whole program when possible.

Simple explanation:

```text
Real logs can be messy, so the parser needs basic error handling.
```

---

## Why is log input important before live input?

Log input is important because it is repeatable.

A saved file can be tested again and again with the same results.

This makes it easier to verify:

```text
parsing
ID validation
DLC validation
dispatching
decoding
fault detection
statistics
```

Simple explanation:

```text
Logs let me test the decoder reliably before adding live USB-CAN reading.
```

---

## Simulated logs for now

For now, the project continues using:

```text
data/sample_can_log.csv
```

The hardware CAN path worked, but live reading will come later.

Current workflow:

```text
simulated or captured-style CSV log
        ↓
C++ decoder
        ↓
decoded values, faults, and stats
```

Future workflow:

```text
Waveshare USB-CAN live frames
        ↓
C++ decoder
        ↓
decoded values, faults, and stats
```

---

## Day 2 main interview idea

```text
Grid DFS visits connected components by marking one cell and recursively visiting its neighbors. Number of Islands uses DFS to count connected land groups. In the CAN decoder project, CSV parsing converts text log lines into CanFrame objects, allowing the decoder to test parsing, dispatching, decoding, fault detection, and stats before adding live USB-CAN input. Header detection must be careful because hexadecimal log values can contain letters.
```
---

# Week 6 Day 3 — Iterative DFS and Decode 0x200

## Interview questions

```text
1. What is iterative DFS?
2. How is iterative DFS different from recursive DFS?
3. Why might iterative DFS be useful in embedded systems?
4. How does Max Area of Island use DFS?
5. What does CAN ID 0x200 represent?
6. What fields are inside the 0x200 payload?
7. How is speed_raw decoded?
8. How is rpm decoded?
9. Why does 0x200 include a counter?
10. How would speed_raw be scaled later?
```

---

## What is iterative DFS?

My answer:









Reference answer:

Iterative DFS is depth-first search using an explicit stack instead of recursive function calls.

The programmer manually pushes and pops nodes from a stack.

Simple version:

```text
Iterative DFS uses a stack to explore deeply without recursion.
```

---

## How is iterative DFS different from recursive DFS?

My answer:









Reference answer:

Recursive DFS uses the function call stack automatically.

Iterative DFS uses a stack container controlled by the programmer.

Example:

```cpp
std::stack<std::pair<int, int>> cells;
```

Simple version:

```text
Recursive DFS lets function calls manage the stack; iterative DFS manages the stack directly.
```

---

## Why might iterative DFS be useful in embedded systems?

My answer:









Reference answer:

Embedded systems often have limited stack memory.

Deep recursion can cause stack overflow.

Iterative DFS can be safer because the programmer controls the stack container and traversal behavior.

Simple version:

```text
Iterative DFS can reduce recursion risk on memory-limited embedded systems.
```

---

## How does Max Area of Island use DFS?

My answer:









Reference answer:

Max Area of Island scans the grid for land cells.

When it finds land, it uses DFS to visit all connected land cells and count the area.

It tracks the largest area found.

Simple version:

```text
DFS counts each island, and the algorithm keeps the biggest count.
```

---

## What does CAN ID 0x200 represent?

My answer:









Reference answer:

CAN ID `0x200` represents vehicle telemetry.

It carries basic vehicle motion and driver input information.

Simple version:

```text
0x200 is the vehicle telemetry frame.
```

---

## What fields are inside the 0x200 payload?

My answer:









Reference answer:

The `0x200` payload contains:

```text
Byte 0-1: speed_raw
Byte 2-3: rpm
Byte 4: gear
Byte 5: throttle_percent
Byte 6: brake_percent
Byte 7: counter
```

Simple version:

```text
0x200 contains speed, RPM, gear, throttle, brake, and counter data.
```

---

## How is speed_raw decoded?

My answer:









Reference answer:

`speed_raw` is decoded from bytes 0 and 1 using little-endian packing.

```cpp
std::uint16_t speed_raw = pack_u16(frame.data[0], frame.data[1]);
```

Simple version:

```text
speed_raw uses byte 0 as the low byte and byte 1 as the high byte.
```

---

## How is rpm decoded?

My answer:









Reference answer:

`rpm` is decoded from bytes 2 and 3 using little-endian packing.

```cpp
std::uint16_t rpm = pack_u16(frame.data[2], frame.data[3]);
```

Simple version:

```text
rpm uses byte 2 as the low byte and byte 3 as the high byte.
```

---

## Why does 0x200 include a counter?

My answer:









Reference answer:

The counter helps detect whether frames are updating correctly.

It can help reveal:

```text
stale frames
repeated frames
dropped frames
communication gaps
```

Simple version:

```text
The counter helps verify that the CAN message stream is alive and in order.
```

---

## How would speed_raw be scaled later?

My answer:









Reference answer:

`speed_raw` is currently a raw integer.

Later, it can be converted into a real speed value with a scale factor.

Example:

```text
speed_mph = speed_raw * scale_factor
```

The exact scale factor depends on how the sender defines the signal.

Simple version:

```text
speed_raw becomes real speed after applying a scale factor.
```

---

## Week 6 Day 3 Core Interview Summary

My answer:









Reference answer:

Iterative DFS performs depth-first traversal using an explicit stack instead of recursive calls. This is useful when recursion depth or stack memory is a concern. In the CAN decoder project, ID `0x200` adds vehicle telemetry data such as `speed_raw`, `rpm`, `gear`, `throttle_percent`, `brake_percent`, and a counter. The counter helps detect stale or missing frames, while `speed_raw` can later be scaled into a physical speed value.
---

# Day 4 — BFS Spread and Buffer to Dispatcher

## Main goals

```text
Study BFS spread pattern.
Solve Rotting Oranges.
Connect the circular buffer clearly to the dispatcher pipeline.
Read frames from log input.
Push frames into the buffer.
Pop frames from the buffer.
Dispatch, decode, analyze, and print results.
Continue using simulated logs until live USB-CAN reading is added later.
```

---

## Notes questions

```text
1. What kind of problems use BFS?
2. Why does Rotting Oranges use levels/time?
3. How does a CAN RX buffer relate to a queue?
```

---

## What kind of problems use BFS?

BFS is useful for problems that spread outward in layers.

Common BFS problem types:

```text
level order traversal
shortest path in an unweighted graph
spread over time
nearest source problems
grid expansion
multi-source search
```

Examples:

```text
rotting oranges
fire spreading
virus spreading
distance from nearest gate
shortest path in a grid
```

Simple explanation:

```text
BFS is useful when the problem moves level by level or step by step.
```

---

## Why does Rotting Oranges use levels/time?

Rotting Oranges uses BFS because rotting spreads one step per minute.

Each BFS level represents one minute.

```text
minute 0 = starting rotten oranges
minute 1 = adjacent fresh oranges rot
minute 2 = next layer rots
minute 3 = next layer rots
```

Simple explanation:

```text
BFS levels naturally model time passing.
```

---

## Rotting Oranges code idea

```cpp
class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int rows = grid.size();
        int cols = grid[0].size();

        queue<pair<int, int>> q;
        int fresh = 0;

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                if (grid[row][col] == 2) {
                    q.push({row, col});
                } else if (grid[row][col] == 1) {
                    fresh++;
                }
            }
        }

        int minutes = 0;

        vector<pair<int, int>> directions = {
            {1, 0},
            {-1, 0},
            {0, 1},
            {0, -1}
        };

        while (!q.empty() && fresh > 0) {
            int level_size = q.size();

            for (int i = 0; i < level_size; i++) {
                pair<int, int> current = q.front();
                q.pop();

                int row = current.first;
                int col = current.second;

                for (const pair<int, int>& direction : directions) {
                    int next_row = row + direction.first;
                    int next_col = col + direction.second;

                    if (next_row < 0 || next_row >= rows) {
                        continue;
                    }

                    if (next_col < 0 || next_col >= cols) {
                        continue;
                    }

                    if (grid[next_row][next_col] != 1) {
                        continue;
                    }

                    grid[next_row][next_col] = 2;
                    fresh--;
                    q.push({next_row, next_col});
                }
            }

            minutes++;
        }

        if (fresh > 0) {
            return -1;
        }

        return minutes;
    }
};
```

---

## Why level_size matters

This line is important:

```cpp
int level_size = q.size();
```

It records how many rotten oranges are active for the current minute.

Any new oranges added during the loop belong to the next minute.

Simple explanation:

```text
level_size separates the current time step from the next time step.
```

---

## How does a CAN RX buffer relate to a queue?

A CAN RX buffer stores received CAN frames until the program can process them.

This is similar to a queue:

```text
new frame arrives
        ↓
push into RX buffer
        ↓
program later pops frame
        ↓
frame is decoded
```

Simple explanation:

```text
A CAN RX buffer lets receiving and processing happen at different times.
```

---

## Buffer to dispatcher flow

Current project flow:

```text
CSV parser reads frame
        ↓
CanFrame created
        ↓
frame pushed into CircularBuffer
        ↓
frame popped from CircularBuffer
        ↓
frame passed to CanDispatcher
        ↓
dispatcher calls the correct TelemetryDecoder function
        ↓
FaultAnalyzer checks decoded values
        ↓
DecoderStats records summary
```

Simple explanation:

```text
The buffer temporarily stores frames, and the dispatcher decides which decoder handles each frame.
```

---

## Why buffer before dispatching?

Using a buffer separates input from processing.

Input stage:

```text
read frames from CSV or future USB-CAN source
```

Processing stage:

```text
validate, dispatch, decode, analyze, print stats
```

This makes the code closer to a real embedded or telemetry pipeline.

Simple explanation:

```text
The buffer lets the program collect frames first and process them in order later.
```

---

## Simulated logs for now

For now, the project still uses:

```text
data/sample_can_log.csv
```

Later hardware path:

```text
USB-CAN output
        ↓
CSV file
        ↓
decoder
```

Future live path:

```text
USB-CAN live input
        ↓
decoder
```

Simple explanation:

```text
Simulated logs keep the software pipeline testable before live CAN reading is added.
```

---

## Day 4 main interview idea

```text
BFS is useful for level-by-level problems such as spreading, shortest paths, and time-step simulations. Rotting Oranges uses BFS because each BFS level represents one minute of spread. In the CAN decoder, the circular buffer acts like a receive queue: frames are pushed in, popped out, dispatched by CAN ID, decoded, analyzed for faults, and counted in DecoderStats.
```
---

# Day 5 — Clone Graph and FaultReport

## Main goals

```text
Study graph cloning.
Solve Clone Graph.
Create FaultReport.
Collect faults instead of only printing immediately.
Separate fault detection from fault output.
```

---

## Notes questions

```text
1. Why does Clone Graph need a map?
2. Why should faults be stored as reports?
3. What is the advantage of collecting faults before printing?
```

---

## Why does Clone Graph need a map?

Clone Graph needs a map because graphs can contain cycles.

Example:

```text
1 -- 2
|    |
4 -- 3
```

If the algorithm clones node `1`, then visits node `2`, node `2` may point back to node `1`.

Without a map, the algorithm may clone node `1` again and continue forever.

Simple explanation:

```text
The map remembers which original nodes already have cloned copies.
```

---

## Clone Graph map idea

The map connects original nodes to cloned nodes.

```cpp
unordered_map<Node*, Node*> cloned;
```

Meaning:

```text
key = original node pointer
value = copied node pointer
```

Simple explanation:

```text
If a node was already copied, reuse the copy instead of cloning it again.
```

---

## Clone Graph code idea

```cpp
class Solution {
public:
    Node* cloneGraph(Node* node) {
        if (node == nullptr) {
            return nullptr;
        }

        unordered_map<Node*, Node*> cloned;
        return clone(node, cloned);
    }

private:
    Node* clone(Node* node, unordered_map<Node*, Node*>& cloned) {
        if (cloned.find(node) != cloned.end()) {
            return cloned[node];
        }

        Node* copy = new Node(node->val);
        cloned[node] = copy;

        for (Node* neighbor : node->neighbors) {
            copy->neighbors.push_back(clone(neighbor, cloned));
        }

        return copy;
    }
};
```

---

## Why should faults be stored as reports?

Faults should be stored as reports because fault information can be used in multiple ways.

A fault report can be:

```text
counted
printed
filtered
saved to a file
tested in unit tests
sent to another system
```

Simple explanation:

```text
A fault report turns a printed message into reusable data.
```

---

## FaultReport structure

The project now uses:

```cpp
struct FaultReport {
    bool has_fault;
    std::string message;
};
```

Example:

```cpp
FaultReport report{true, "Battery voltage too low"};
```

Simple explanation:

```text
FaultReport stores whether a fault exists and what the fault message is.
```

---

## What is the advantage of collecting faults before printing?

Collecting faults before printing separates two jobs:

```text
FaultAnalyzer = detects faults
main.cpp = decides how to print faults
```

This is cleaner than printing inside the analyzer immediately.

Simple explanation:

```text
Collecting faults separates detection from output.
```

---

## Old fault flow

Old flow:

```text
FaultAnalyzer checks data
        ↓
FaultAnalyzer prints immediately
```

Problem:

```text
The fault message is only output text.
It cannot easily be counted, stored, filtered, or tested.
```

---

## New fault flow

New flow:

```text
FaultAnalyzer checks data
        ↓
FaultAnalyzer returns FaultReport objects
        ↓
main.cpp collects reports
        ↓
main.cpp counts and prints reports
        ↓
DecoderStats updates fault count
```

Simple explanation:

```text
Faults become structured data before they become printed output.
```

---

## Day 5 main interview idea

```text
Clone Graph needs a map because graphs can contain cycles and shared neighbors. The map prevents duplicate clones and infinite recursion. In the CAN decoder project, FaultReport stores fault information as data, allowing the program to collect, count, print, save, or test faults instead of only printing immediately inside FaultAnalyzer.
```