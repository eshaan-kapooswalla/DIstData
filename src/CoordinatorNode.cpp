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

int main() {
    std::cout << "CoordinatorNode started. Listening for client queries..." << std::endl;
    CoordinatorNode coordinator;
    std::string data = coordinator.getData("partition1");
    std::cout << "CoordinatorNode received data: " << data << std::endl;

    // Example usage in main (add this to your main function to see the demo)
    // shardingReplicationDemo();

    return 0;
} 