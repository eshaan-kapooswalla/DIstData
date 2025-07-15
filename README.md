# DistData: Distributed Database System

## Overview

DistData is a modular, fault-tolerant distributed database system designed for real-world scalability, high availability, and strong consistency. It features a robust communication layer, distributed query engine, cost-based query optimization, distributed transactions, consensus algorithms, security, and comprehensive fault tolerance and recovery mechanisms.

---

## Architecture

- **Coordinator Node:**  
  Manages metadata, query routing, leader election, distributed transactions, and cluster coordination.

- **Data Node:**  
  Stores data partitions, handles read/write requests, participates in replication, consensus, and distributed locking.

- **Client:**  
  Sends SQL-like queries to the system and receives results.

- **Communication Layer:**  
  TCP/IP-based messaging with custom binary serialization for efficient, secure node-to-node and client-to-node communication.

---

## Features

- **Hash-Based Data Partitioning:**  
  Evenly distributes data across nodes using a hash function.

- **Peer-to-Peer Replication:**  
  Ensures high availability and fault tolerance by replicating data across multiple nodes.

- **Replication Factor Management:**  
  Dynamically maintains the desired number of replicas for each data item.

- **Node Management:**  
  Supports adding/removing nodes with automatic data migration and rebalancing.

- **Distributed Query Engine:**  
  Parses SQL-like queries, distributes sub-queries, and aggregates results from multiple nodes.

- **Query Optimization:**  
  Cost-based `QueryPlanner` selects optimal query plans for distributed execution.

- **Distributed Transactions:**  
  Implements transaction IDs, begin/commit/rollback, and distributed locking using Two-Phase Commit.

- **Consensus Algorithms:**  
  Implements Raft (leader election, heartbeat, log replication) and Two-Phase Commit for distributed transactions.

- **Security:**  
  TLS-encrypted communication (OpenSSL-based) and token-based authentication with user/role management.

- **Fault Tolerance:**  
  Heartbeat-based node failure detection, network partition handling, split-brain prevention, and automated recovery.

- **Metrics Collection:**  
  Utility for tracking latency, throughput, and resource usage.

- **Large-Scale Testing:**  
  Python script to launch multiple nodes and clients, collect performance data, and simulate real-world workloads.

- **Comprehensive Test Suite:**  
  Includes tests for query processing, communication, consensus, partitioning, recovery, authentication, and performance.

---

## Getting Started

### 1. Build the Project

- Requires CMake and a C++17-compatible compiler.
- From the project root:
  ```sh
  mkdir build && cd build
  cmake ..
  make
  ```

### 2. Run Components

- Start Coordinator, Data Nodes, and Clients as separate processes:
  ```sh
  ./CoordinatorNode
  ./DataNode
  ./Client
  ```

### 3. Large-Scale Testing

- Use the provided Python script to launch multiple nodes and clients:
  ```sh
  python3 scripts/large_scale_test.py
  ```

---

## How to Run Tests

- **Query Processing:**  
  Run the test harness for query parsing, distribution, and aggregation with sample data.

- **Communication Layer:**  
  Use the test files to verify TCP communication and message exchange.

- **Consensus & Transactions:**  
  Run the consensus and transaction test harnesses to see Raft leader election and Two-Phase Commit in action.

- **Partition & Recovery:**  
  Use the partition and recovery test files to simulate network partitions and node failures.

- **Heartbeat & Node Failure Detection:**  
  Run the heartbeat test to observe node liveness tracking.

- **Authentication & Security:**  
  Use the authentication test harness to verify token-based authentication and role management.

---

## Sample Data

- Place `customers.txt` and `orders.txt` in the project root for query processing tests.

---

## Roadmap

- [x] Modular architecture and communication layer
- [x] Data partitioning and replication
- [x] Distributed query engine and result aggregation
- [x] Query optimization and distributed transactions
- [x] Consensus algorithms (Raft, 2PC)
- [x] Security: encryption and authentication
- [x] Fault tolerance: heartbeat, partition, recovery
- [x] Metrics collection and large-scale testing
- [x] Comprehensive test suite
- [ ] Performance tuning and advanced query optimization

---

## Contributing

Contributions are welcome! Please open issues or pull requests for bug fixes, features, or documentation improvements.

---

## License

Distributed under the MIT License. 