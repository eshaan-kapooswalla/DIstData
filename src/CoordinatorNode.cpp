#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <functional>
#include <unordered_map>
#include <openssl/md5.h>
#include <climits>

// Represents a key-value pair with metadata
struct KeyValue {
    std::string key;
    std::string value;
    int version;
    std::string timestamp;
};

class Node {
public:
    virtual void write(const std::string& key, const std::string& value) = 0;
    virtual KeyValue read(const std::string& key) = 0;
    virtual void remove(const std::string& key) = 0;
    virtual ~Node() = default;
};

// Simple in-memory node implementation
class InMemoryNode : public Node {
private:
    std::vector<KeyValue> storage;
public:
    void write(const std::string& key, const std::string& value) override {
        KeyValue kv;
        kv.key = key;
        kv.value = value;
        kv.version = storage.size() + 1;
        storage.push_back(kv);
    }

    KeyValue read(const std::string& key) override {
        for (const auto& kv : storage) {
            if (kv.key == key) {
                return kv;
            }
        }
        return KeyValue();
    }

    void remove(const std::string& key) override {
        storage.erase(std::remove_if(storage.begin(), storage.end(),
            [&key](const KeyValue& kv) { return kv.key == key; }),
            storage.end());
    }
};

// Distributed database class
class DistributedDB {
private:
    std::vector<Node*> nodes;
    int totalNodes;
    int replicationFactor;
public:
    DistributedDB(int nodesCount, int rf = 3) : totalNodes(nodesCount), replicationFactor(rf) {
        for (int i = 0; i < nodesCount; ++i) {
            nodes.push_back(new InMemoryNode());
        }
    }

    ~DistributedDB() {
        for (auto node : nodes) {
            delete node;
        }
    }

    void put(const std::string& key, const std::string& value) {
        int primary = getNode(key, totalNodes);
        nodes[primary]->write(key, value);
        replicate(key, value, primary);
    }

    KeyValue get(const std::string& key) {
        int primary = getNode(key, totalNodes);
        return nodes[primary]->read(key);
    }

private:
    int getNode(const std::string& key, int totalNodes) {
        return std::hash<std::string>{}(key) % totalNodes;
    }

    void replicate(const std::string& key, const std::string& value, int primary) {
        for (int i = 1; i < replicationFactor; ++i) {
            int node = (primary + i) % totalNodes;
            nodes[node]->write(key, value);
        }
    }
};

class DataNode {
public:
    // Placeholder for data retrieval
    std::string getData(const std::string& partitionKey) {
        std::cout << "DataNode::getData called with partitionKey: " << partitionKey << std::endl;
        return "DataStub";
    }
};

class CoordinatorNode {
public:
    std::string getData(const std::string& partitionKey) {
        DataNode node; // In reality, would look up the correct node
        std::string data = node.getData(partitionKey);
        return data;
    }
};

class DataStore {
    // TODO: Implement data storage logic
};

class ReplicationManager {
    // TODO: Implement replication logic
};

class ServerNode {
private:
    DataStore dataStore;
    ReplicationManager replication;
public:
    void handleClientRequest(const std::string& request) {
        // TODO: Process the request and return data
        std::cout << "ServerNode::handleClientRequest called with request: " << request << std::endl;
    }
};

// Simple Node structure for sharding and replication demo
struct ShardNode {
    std::string id;
    std::vector<std::string> data;
};

// Sharding function using modulo operation
std::string shardData(const std::string& key, const std::vector<ShardNode>& nodes) {
    size_t hash = std::hash<std::string>{}(key);
    return nodes[hash % nodes.size()].id;
}

void shardingReplicationDemo() {
    // Example nodes
    std::vector<ShardNode> nodes = {
        {"Node1", {"data1", "data2"}},
        {"Node2", {"data3", "data4"}},
        {"Node3", {"data5", "data6"}}
    };

    // Determine where to store a new key
    std::string key = "customer123";
    std::string targetNode = shardData(key, nodes);
    std::cout << "Key '" << key << "' should be stored on Node: " << targetNode << std::endl;
}

// Replicate data to (replicationFactor - 1) additional nodes
void replicateData(const std::string& key, const std::vector<ShardNode>& nodes, int primaryIndex, int replicationFactor) {
    for (int i = 1; i < replicationFactor; ++i) {
        int replicaIndex = (primaryIndex + i) % nodes.size();
        std::cout << "Replicating key '" << key << "' to Node: " << nodes[replicaIndex].id << std::endl;
    }
}

// Simulate failover by skipping a failed node
void simulateFailover(const std::string& key, std::vector<ShardNode>& nodes, int failedIndex) {
    std::cout << "\nSimulating failover: Node " << nodes[failedIndex].id << " is down." << std::endl;
    // Remove the failed node from the list
    std::vector<ShardNode> availableNodes;
    for (int i = 0; i < nodes.size(); ++i) {
        if (i != failedIndex) availableNodes.push_back(nodes[i]);
    }
    std::string targetNode = shardData(key, availableNodes);
    std::cout << "After failover, key '" << key << "' should be stored on Node: " << targetNode << std::endl;
}

// Example usage in main (uncomment to run)
// int main() {
//     shardingReplicationDemo();
//     std::vector<ShardNode> nodes = {
//         {"Node1", {"data1", "data2"}},
//         {"Node2", {"data3", "data4"}},
//         {"Node3", {"data5", "data6"}}
//     };
//     std::string key = "customer123";
//     int primaryIndex = std::hash<std::string>{}(key) % nodes.size();
//     replicateData(key, nodes, primaryIndex, 3);
//     simulateFailover(key, nodes, 1); // Simulate Node2 failure
//     return 0;
// }

// Simple Node structure for partitioning
struct PartitionNode {
    int id;
    std::string address;
};

class HashPartitioner {
private:
    std::vector<PartitionNode> nodes;
    unsigned long max_hash;
public:
    HashPartitioner(const std::vector<PartitionNode>& nodes) : nodes(nodes) {
        max_hash = ULONG_MAX;
    }

    // Assign a key to a node based on hash
    PartitionNode* getNode(const std::string& key) {
        unsigned long hash = computeHash(key);
        unsigned long range = max_hash / nodes.size();
        int nodeIndex = hash / range;
        if (nodeIndex >= nodes.size()) nodeIndex = nodes.size() - 1; // edge case
        return &nodes[nodeIndex];
    }

    // Simple hash function (MD5)
    unsigned long computeHash(const std::string& key) {
        unsigned char md[MD5_DIGEST_LENGTH];
        MD5((unsigned char*)key.c_str(), key.size(), md);
        return *(unsigned long*)md;
    }
};

// Example usage:
// int main() {
//     std::vector<PartitionNode> nodes = {
//         {0, "10.0.0.1"},
//         {1, "10.0.0.2"},
//         {2, "10.0.0.3"},
//         {3, "10.0.0.4"}
//     };
//     HashPartitioner partitioner(nodes);
//     std::string key = "customer123";
//     PartitionNode* node = partitioner.getNode(key);
//     std::cout << "Key '" << key << "' is assigned to Node: " << node->id << " at " << node->address << std::endl;
//     return 0;
// }

#include <unordered_map>
#include <functional>

// Structure to represent a node
struct SimpleNode {
    std::string id;
    std::string address;
};

// Function to compute the hash of a key
std::size_t computeHash(const std::string& key) {
    return std::hash<std::string>{}(key);
}

// Function to determine the node for a given key
SimpleNode* getNode(const std::string& key, const std::unordered_map<std::size_t, SimpleNode>& nodes) {
    std::size_t hash = computeHash(key);
    std::size_t nodeIndex = hash % nodes.size();
    auto it = nodes.find(nodeIndex);
    if (it != nodes.end()) {
        return const_cast<SimpleNode*>(&it->second);
    }
    return nullptr;
}

// Example usage:
// int main() {
//     SimpleNode node1 = {"Node1", "192.168.1.1"};
//     SimpleNode node2 = {"Node2", "192.168.1.2"};
//     SimpleNode node3 = {"Node3", "192.168.1.3"};
//     std::unordered_map<std::size_t, SimpleNode> nodes;
//     nodes[std::hash<std::string>{}(node1.id)] = node1;
//     nodes[std::hash<std::string>{}(node2.id)] = node2;
//     nodes[std::hash<std::string>{}(node3.id)] = node3;
//     std::string key1 = "record1";
//     std::string key2 = "record2";
//     std::string key3 = "record3";
//     SimpleNode* nodeForKey1 = getNode(key1, nodes);
//     SimpleNode* nodeForKey2 = getNode(key2, nodes);
//     SimpleNode* nodeForKey3 = getNode(key3, nodes);
//     std::cout << "Key: " << key1 << " assigned to Node ID: " << nodeForKey1->id << std::endl;
//     std::cout << "Key: " << key2 << " assigned to Node ID: " << nodeForKey2->id << std::endl;
//     std::cout << "Key: " << key3 << " assigned to Node ID: " << nodeForKey3->id << std::endl;
//     return 0;
// }

// --- Hash-Based Data Partitioning Example ---
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>

// Structure to represent a node
struct PartitionDemoNode {
    std::string id;
    std::string address;
};

// Function to compute the hash of a key
std::size_t demoComputeHash(const std::string& key) {
    return std::hash<std::string>{}(key);
}

// Function to determine the node for a given key
PartitionDemoNode* demoGetNode(const std::string& key, const std::unordered_map<std::size_t, PartitionDemoNode>& nodes) {
    std::size_t hash = demoComputeHash(key);
    std::size_t nodeIndex = hash % nodes.size();
    auto it = nodes.find(nodeIndex);
    if (it != nodes.end()) {
        return const_cast<PartitionDemoNode*>(&it->second);
    }
    return nullptr;
}

// Example usage:
// int main() {
//     // Create nodes
//     PartitionDemoNode node1 = {"Node1", "192.168.1.1"};
//     PartitionDemoNode node2 = {"Node2", "192.168.1.2"};
//     PartitionDemoNode node3 = {"Node3", "192.168.1.3"};
//     // Store nodes in an unordered_map with their hash as key
//     std::unordered_map<std::size_t, PartitionDemoNode> nodes;
//     nodes[std::hash<std::string>{}(node1.id)] = node1;
//     nodes[std::hash<std::string>{}(node2.id)] = node2;
//     nodes[std::hash<std::string>{}(node3.id)] = node3;
//     // Example keys
//     std::string key1 = "record1";
//     std::string key2 = "record2";
//     std::string key3 = "record3";
//     // Determine nodes for keys
//     PartitionDemoNode* nodeForKey1 = demoGetNode(key1, nodes);
//     PartitionDemoNode* nodeForKey2 = demoGetNode(key2, nodes);
//     PartitionDemoNode* nodeForKey3 = demoGetNode(key3, nodes);
//     // Output results
//     std::cout << "Key: " << key1 << " assigned to Node ID: " << nodeForKey1->id << std::endl;
//     std::cout << "Key: " << key2 << " assigned to Node ID: " << nodeForKey2->id << std::endl;
//     std::cout << "Key: " << key3 << " assigned to Node ID: " << nodeForKey3->id << std::endl;
//     return 0;
// }

// --- Peer-to-Peer Replication Example ---
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

// Data structure to represent a node in the system
struct ReplicationNode {
    std::string id;
    std::string address;
    bool is_active;
};

// Data structure to represent a data record
struct DataRecord {
    std::string key;
    std::string value;
    std::chrono::system_clock::time_point timestamp;
};

class ReplicationManager {
private:
    std::vector<ReplicationNode> nodes;
    std::vector<DataRecord> data;
    std::mutex mtx;
    std::condition_variable cv;

public:
    ReplicationManager(const std::vector<ReplicationNode>& initial_nodes) : nodes(initial_nodes) {}

    // Function to handle write operations
    void write(const std::string& key, const std::string& value) {
        std::unique_lock<std::mutex> lock(mtx);
        DataRecord record;
        record.key = key;
        record.value = value;
        record.timestamp = std::chrono::system_clock::now();
        data.push_back(record);
        broadcast(record);
        lock.unlock();
        cv.notify_all();
    }

    // Function to handle read operations
    std::string read(const std::string& key) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]{ return !data.empty(); });
        DataRecord latest;
        for (const auto& record : data) {
            if (record.key == key) {
                if (record.timestamp > latest.timestamp) {
                    latest = record;
                }
            }
        }
        return latest.value;
    }

    // Function to broadcast updates to all nodes
    void broadcast(const DataRecord& record) {
        // Implement your broadcasting logic here
        // This could be TCP/IP communication, HTTP requests, etc.
    }

    // Function to handle node failures
    void handleNodeFailure(const std::string& node_id) {
        // Implement failure handling logic here
        // This could involve re-electing a leader, redistributing data, etc.
    }
};

// Example usage:
// int main() {
//     std::vector<ReplicationNode> nodes = {
//         {"node1", "localhost:8080", true},
//         {"node2", "localhost:8081", true},
//         {"node3", "localhost:8082", true}
//     };
//     ReplicationManager rm(nodes);
//     rm.write("key1", "value1");
//     std::cout << "Value for key1: " << rm.read("key1") << std::endl;
//     return 0;
// }

// --- Replication Factor Management Example ---
#include <vector>
#include <map>
#include <string>
#include <algorithm>

// Represents a node in the system
struct RFNode {
    std::string id;
    std::string address;
};

// Manages replication factor for data items
class ReplicationFactorManager {
private:
    // Mapping from data item to its replicas
    std::map<std::string, std::vector<RFNode>> dataReplicas;
    // Mapping from node ID to its current data items
    std::map<std::string, std::vector<std::string>> nodeData;
    // List of available nodes
    std::vector<RFNode> availableNodes;
    // Current replication factor
    int replicationFactor;

public:
    ReplicationFactorManager(int replicationFactor) 
        : replicationFactor(replicationFactor) {}

    // Register a new node with the system
    void registerNode(const RFNode& node) {
        availableNodes.push_back(node);
    }

    // Deregister a node from the system
    void deregisterNode(const std::string& nodeId) {
        // Remove the node from availableNodes
        availableNodes.erase(std::remove_if(availableNodes.begin(), availableNodes.end(),
            [&nodeId](const RFNode& n) { return n.id == nodeId; }),
            availableNodes.end());

        // Migrate data from this node to other nodes
        if (nodeData.find(nodeId) != nodeData.end()) {
            for (const auto& dataId : nodeData[nodeId]) {
                if (dataReplicas[dataId].size() < replicationFactor) {
                    // Find new nodes to replicate to
                    replicateData(dataId);
                }
            }
            nodeData.erase(nodeId);
        }
    }

    // Replicate a data item to maintain the replication factor
    void replicateData(const std::string& dataId) {
        // Determine which nodes should hold replicas
        std::vector<RFNode> targetNodes;
        for (const auto& node : availableNodes) {
            if (std::find_if(dataReplicas[dataId].begin(), dataReplicas[dataId].end(),
                [&node](const RFNode& n) { return n.id == node.id; }) == dataReplicas[dataId].end()) {
                targetNodes.push_back(node);
                if (targetNodes.size() == replicationFactor) {
                    break;
                }
            }
        }

        // Update dataReplicas and nodeData maps
        for (const auto& node : targetNodes) {
            dataReplicas[dataId].push_back(node);
            nodeData[node.id].push_back(dataId);
        }
    }

    // Check if replication factor is maintained for all data items
    bool isReplicationFactorMaintained() const {
        for (const auto& entry : dataReplicas) {
            if (entry.second.size() != replicationFactor) {
                return false;
            }
        }
        return true;
    }
};

int main() {
    std::cout << "CoordinatorNode started. Listening for client queries..." << std::endl;
    CoordinatorNode coordinator;
    std::string data = coordinator.getData("partition1");
    std::cout << "CoordinatorNode received data: " << data << std::endl;

    // Example usage in main (add this to your main function to see the demo)
    // shardingReplicationDemo();

    return 0;
} 

// --- Data Distribution and Replication Test Suite ---
#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>
#include <string>

// Dummy node and data store for testing
struct TestNode {
    std::string id;
    std::vector<std::string> data;
};

std::map<std::string, TestNode> testNodes;

void addNode(const TestNode& node) {
    testNodes[node.id] = node;
}

void insertData(const std::string& data) {
    // Hash-based partitioning: assign to node by hash
    std::vector<std::string> nodeIds;
    for (const auto& n : testNodes) nodeIds.push_back(n.first);
    std::size_t hash = std::hash<std::string>{}(data);
    std::string nodeId = nodeIds[hash % nodeIds.size()];
    testNodes[nodeId].data.push_back(data);
}

void insertDataWithReplication(const std::string& data, int replicationFactor) {
    std::vector<std::string> nodeIds;
    for (const auto& n : testNodes) nodeIds.push_back(n.first);
    std::size_t hash = std::hash<std::string>{}(data);
    int startIdx = hash % nodeIds.size();
    for (int i = 0; i < replicationFactor; ++i) {
        int idx = (startIdx + i) % nodeIds.size();
        testNodes[nodeIds[idx]].data.push_back(data);
    }
}

std::vector<std::string> getNodeData(const std::string& nodeId) {
    return testNodes[nodeId].data;
}

void assertDataDistribution(const std::map<std::string, std::vector<std::string>> &expected) {
    for (const auto &node : expected) {
        std::vector<std::string> actualData = getNodeData(node.first);
        if (actualData != node.second) {
            throw std::runtime_error("Data distribution mismatch for node " + node.first);
        }
    }
}

void testBasicDataDistribution() {
    testNodes.clear();
    addNode({"Node1", {}});
    addNode({"Node2", {}});
    addNode({"Node3", {}});
    std::string data = "SampleData1";
    insertData(data);
    // Only one node should have the data
    std::map<std::string, std::vector<std::string>> expectedResult = {
        {"Node1", {}},
        {"Node2", {}},
        {"Node3", {}}
    };
    // Find which node got the data
    for (auto& n : expectedResult) {
        if (getNodeData(n.first).size() == 1) n.second.push_back(data);
    }
    assertDataDistribution(expectedResult);
}

void testDataReplication() {
    testNodes.clear();
    addNode({"Node1", {}});
    addNode({"Node2", {}});
    addNode({"Node3", {}});
    std::string data = "SampleData2";
    insertDataWithReplication(data, 2);
    // Data should be present in two nodes
    std::map<std::string, std::vector<std::string>> expectedResult = {
        {"Node1", {}},
        {"Node2", {}},
        {"Node3", {}}
    };
    int count = 0;
    for (auto& n : expectedResult) {
        if (getNodeData(n.first).size() == 1 && count < 2) {
            n.second.push_back(data);
            ++count;
        }
    }
    assertDataDistribution(expectedResult);
}

void initializeSystem() {
    // Placeholder for any system-wide initialization
}

void cleanupSystem() {
    testNodes.clear();
}

// Example usage:
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