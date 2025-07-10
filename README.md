# DistData: Distributed Database System

## Overview
DistData is a distributed database system designed for scalability, fault tolerance, and high availability. It features a modular architecture with Coordinator Nodes, Data Nodes, and a robust communication layer.

## Architecture
- **Coordinator Node:** Manages metadata, query routing, and cluster coordination.
- **Data Node:** Stores data partitions and handles read/write requests.
- **Client:** Sends queries to the system.
- **Communication Layer:** TCP/IP-based messaging between nodes.

## Getting Started
- Build with CMake
- Run Coordinator, Data Nodes, and Clients as separate processes

## Roadmap
- [x] Define architecture
- [ ] Implement communication layer
- [ ] Implement Coordinator and Data Node logic
- [ ] Add query processing and replication 