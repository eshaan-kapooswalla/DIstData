# DistData: Distributed Database System

## Overview
DistData is a distributed database system designed for scalability, fault tolerance, and high availability. It features a modular architecture with Coordinator Nodes, Data Nodes, and a robust communication layer.

## Architecture
- **Coordinator Node:** Manages metadata, query routing, and cluster coordination.
- **Data Node:** Stores data partitions and handles read/write requests.
- **Client:** Sends queries to the system.
- **Communication Layer:** TCP/IP-based messaging between nodes.

## Features
- **Hash-Based Data Partitioning:** Evenly distributes data across nodes using a hash function.
- **Peer-to-Peer Replication:** Ensures high availability and fault tolerance by replicating data across multiple nodes.
- **Replication Factor Management:** Dynamically maintains the desired number of replicas for each data item.
- **Node Management:** Supports adding and removing nodes, with automatic data migration.
- **Test Suite:** Includes tests for data distribution and replication correctness.

## Getting Started
- Build with CMake
- Run Coordinator, Data Nodes, and Clients as separate processes

## How to Run Tests
- The `CoordinatorNode.cpp` file contains a test suite for data distribution and replication.
- To run the tests, uncomment the example `main()` function at the end of the file and build/run the project:
  ```cpp
  // int main() {
  //     try {
  //         initializeSystem();
  //         testBasicDataDistribution();
  //         testDataReplication();
  //         cleanupSystem();
  //     } catch (const std::exception &e) {
  //         std::cerr << "Test failed: " << e.what() << std::endl;
  //         return 1;
  //     }
  //     std::cout << "All tests passed successfully!" << std::endl;
  //     return 0;
  // }
  ```

## Roadmap
- [x] Define architecture
- [x] Implement data partitioning
- [x] Implement replication and replication factor management
- [x] Add data distribution and replication tests
- [ ] Implement communication layer
- [ ] Implement Coordinator and Data Node logic
- [ ] Add distributed query processing

## Next Steps
- Integrate network communication for real peer-to-peer replication
- Add consensus and conflict resolution mechanisms
- Expand test coverage (node failures, recovery, performance) 