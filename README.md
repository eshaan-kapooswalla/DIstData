# DistData: Distributed Database System

## Overview
DistData is a modular, fault-tolerant distributed database system designed for scalability, high availability, and strong consistency. It features a robust communication layer, distributed query engine, consensus algorithms, and comprehensive fault tolerance and recovery mechanisms.

## Architecture
- **Coordinator Node:** Manages metadata, query routing, leader election, and cluster coordination.
- **Data Node:** Stores data partitions, handles read/write requests, and participates in replication and consensus.
- **Client:** Sends SQL-like queries to the system and receives results.
- **Communication Layer:** TCP/IP-based messaging and custom serialization for efficient node-to-node communication.

## Features
- **Hash-Based Data Partitioning:** Evenly distributes data across nodes using a hash function.
- **Peer-to-Peer Replication:** Ensures high availability and fault tolerance by replicating data across multiple nodes.
- **Replication Factor Management:** Dynamically maintains the desired number of replicas for each data item.
- **Node Management:** Supports adding/removing nodes with automatic data migration and rebalancing.
- **Distributed Query Engine:** Parses SQL-like queries, distributes sub-queries, and aggregates results from multiple nodes.
- **Consensus Algorithms:** Implements Raft and Two-Phase Commit for leader election, log replication, and distributed transactions.
- **Fault Tolerance:** Heartbeat-based node failure detection, network partition handling, split-brain prevention, and automated recovery.
- **Test Suite:** Includes tests for data distribution, replication, query processing, consensus, and failure recovery.

## Getting Started
1. **Build the Project:**
   - Requires CMake and a C++17-compatible compiler.
   - From the project root:
     ```sh
     mkdir build && cd build
     cmake ..
     make
     ```
2. **Run Components:**
   - Start Coordinator, Data Nodes, and Clients as separate processes.
   - Example:
     ```sh
     ./CoordinatorNode
     ./DataNode
     ./Client
     ```

## How to Run Tests
- **Query Processing:**
  - Run `QueryProcessingTest.cpp` to test query parsing, distribution, and aggregation with sample data.
- **Communication Layer:**
  - Run `TestServer.cpp` and `TestClient.cpp` to verify TCP communication and message exchange.
- **Consensus & Transactions:**
  - Run `ConsensusTest.cpp` to see Raft leader election and Two-Phase Commit in action.
- **Partition & Recovery:**
  - Run `PartitionTest.cpp` for network partition and reconciliation logic.
  - Run `FailureRecoveryTest.cpp` to simulate node failures and verify recovery.
- **Heartbeat & Node Failure Detection:**
  - Run `HeartbeatTest.cpp` to observe heartbeat-based node liveness tracking.

## Sample Data
- Place `customers.txt` and `orders.txt` in the project root for query processing tests.

## Roadmap
- [x] Modular architecture and communication layer
- [x] Data partitioning and replication
- [x] Distributed query engine and result aggregation
- [x] Consensus algorithms (Raft, 2PC)
- [x] Fault tolerance: heartbeat, partition, recovery
- [x] Comprehensive test suite
- [ ] Advanced query optimization and distributed transactions
- [ ] Security: encryption and authentication
- [ ] Performance tuning and large-scale testing

## Contributing
Contributions are welcome! Please open issues or pull requests for bug fixes, features, or documentation improvements.

## License
Distributed under the MIT License. 