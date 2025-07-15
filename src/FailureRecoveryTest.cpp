#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <map>
#include <mutex>
#include <set> // Added missing include for std::set

// Stub Node class for demonstration
class Node {
public:
    Node(const std::string& id, int port) : id_(id), port_(port), active_(true) {}
    void start() { std::cout << "Node " << id_ << " started on port " << port_ << std::endl; }
    void crash() { active_ = false; std::cout << "Node " << id_ << " crashed!" << std::endl; }
    void put(const std::string& key, const std::string& value) {
        if (active_) {
            std::lock_guard<std::mutex> lock(mtx_);
            data_[key] = value;
        }
    }
    std::string get(const std::string& key) {
        std::lock_guard<std::mutex> lock(mtx_);
        return data_.count(key) ? data_[key] : "";
    }
    std::string getId() const { return id_; }
    void blockCommunication(const std::string& otherId) {
        blocked_.insert(otherId);
        std::cout << "Node " << id_ << " blocks communication with " << otherId << std::endl;
    }
    void unblockCommunication(const std::string& otherId) {
        blocked_.erase(otherId);
        std::cout << "Node " << id_ << " unblocks communication with " << otherId << std::endl;
    }
    bool isActive() const { return active_; }
private:
    std::string id_;
    int port_;
    bool active_;
    std::map<std::string, std::string> data_;
    std::mutex mtx_;
    std::set<std::string> blocked_;
};

void simulateNodeFailure(Node& node) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    node.crash();
}

void simulateNetworkPartition(Node& node1, Node& node2) {
    node1.blockCommunication(node2.getId());
    std::this_thread::sleep_for(std::chrono::seconds(2));
    node1.unblockCommunication(node2.getId());
}

int main() {
    // Node Failure Test
    Node node1("node1", 8080);
    Node node2("node2", 8081);
    Node node3("node3", 8082);
    node1.start(); node2.start(); node3.start();
    for (int i = 0; i < 10; ++i) {
        node1.put("key_" + std::to_string(i), "value_" + std::to_string(i));
    }
    std::thread failureThread(simulateNodeFailure, std::ref(node1));
    for (int i = 10; i < 20; ++i) {
        node2.put("key_" + std::to_string(i), "value_" + std::to_string(i));
    }
    failureThread.join();
    for (int i = 0; i < 20; ++i) {
        std::string value = node2.get("key_" + std::to_string(i));
        std::cout << "Node2 get key_" << i << ": " << value << std::endl;
    }
    // Network Partition Test
    Node nodeA("nodeA", 9000);
    Node nodeB("nodeB", 9001);
    nodeA.start(); nodeB.start();
    for (int i = 0; i < 5; ++i) {
        nodeA.put("key_" + std::to_string(i), "value_" + std::to_string(i));
    }
    std::thread partitionThread(simulateNetworkPartition, std::ref(nodeA), std::ref(nodeB));
    for (int i = 5; i < 10; ++i) {
        nodeB.put("key_" + std::to_string(i), "value_" + std::to_string(i));
    }
    partitionThread.join();
    for (int i = 0; i < 10; ++i) {
        std::string valueA = nodeA.get("key_" + std::to_string(i));
        std::string valueB = nodeB.get("key_" + std::to_string(i));
        std::cout << "key_" << i << " nodeA: " << valueA << ", nodeB: " << valueB << std::endl;
    }
    return 0;
} 