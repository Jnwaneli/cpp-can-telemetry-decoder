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