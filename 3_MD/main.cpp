#include <algorithm>  // Add this line for find_if
#include <iostream>
#include <vector>
#include <sstream>
#include <climits>

using namespace std;

// TreeNode class to represent each node in the tree
class TreeNode {
public:
    int id;
    int parentId;
    string name;
    string type; // "d" for directory, "f" for file

    // Constructor
    TreeNode(int id, int parentId, const string& name, const string& type)
        : id(id), parentId(parentId), name(name), type(type) {}
};

// Tree class to manage the tree structure
class Tree {
private:
    vector<TreeNode> nodes;

public:
    // Method to add a new node to the tree
    void addNode(int nodeId, int parentId, const string& nodeName, const string& nodeType) {
        // Check if the node with the given id already exists
        auto it = find_if(nodes.begin(), nodes.end(), [nodeId](const TreeNode& node) {
            return node.id == nodeId;
        });

        // If the node exists and its type matches, replace it; otherwise, report an error
        if (it != nodes.end()) {
            if (it->type == nodeType) {
                it->parentId = parentId;
                it->name = nodeName;
            } else {
                cout << "Error: Node with ID " << nodeId << " already exists, but with a different type." << endl;
            }
        } else {
            // If the node doesn't exist, add it to the tree
            nodes.emplace_back(nodeId, parentId, nodeName, nodeType);
        }
    }

    // Method to delete a node by its ID
    void deleteNode(int nodeId) {
        auto it = find_if(nodes.begin(), nodes.end(), [nodeId](const TreeNode& node) {
            return node.id == nodeId;
        });

        if (it != nodes.end()) {
            // If the node is not a leaf, delete the entire branch using a recursive algorithm
            if (it->type == "d") {
                deleteBranch(nodeId);
            }

            // Remove the node from the vector
            nodes.erase(it);
        } else {
            cout << "Error: Node with ID " << nodeId << " not found." << endl;
        }
    }

    // Method to display the tree branch starting from a given ID
    void displayBranch(int nodeId, int level = 0) {
        auto it = find_if(nodes.begin(), nodes.end(), [nodeId](const TreeNode& node) {
            return node.id == nodeId;
        });

        if (it != nodes.end()) {
            // Display the node information
            for (int i = 0; i < level; ++i) {
                cout << "  ";
            }
            cout << it->type << " (" << it->id << ") - " << it->name << endl;

            // Recursively display the sub-nodes
            for (const auto& node : nodes) {
                if (node.parentId == nodeId) {
                    displayBranch(node.id, level + 1);
                }
            }
        } else {
            cout << "Error: Node with ID " << nodeId << " not found." << endl;
        }
    }

    // Method to display the entire tree
    void displayTree() {
        for (const auto& node : nodes) {
            if (node.parentId == -1) {
                displayBranch(node.id);
            }
        }
    }

private:
    // Recursive method to delete the entire branch starting from a given ID
    void deleteBranch(int nodeId) {
        auto it = find_if(nodes.begin(), nodes.end(), [nodeId](const TreeNode& node) {
            return node.id == nodeId;
        });

        if (it != nodes.end()) {
            // Recursively delete sub-nodes
            for (auto iter = nodes.begin(); iter != nodes.end();) {
                if (iter->parentId == nodeId) {
                    deleteBranch(iter->id);
                    iter = nodes.erase(iter);
                } else {
                    ++iter;
                }
            }
        }
    }
};

// Function to display command line usage guide
void displayUsage() {
    cout << "Command line usage:" << endl;
    cout << "h: Display usage guide." << endl;
    cout << "+id,parent_id,name,type: Add a new node to the node with a certain identifier 'parent_id'." << endl;
    cout << "-id: Delete the node with the given 'id'." << endl;
    cout << "*id: Display the tree branch starting from the 'id' node." << endl;
    cout << "*: Display the entire tree." << endl;
    cout << "$: End the program." << endl;
}

int main() {
    Tree fileSystem;

    displayUsage();
    
    char command;
    do {
        cout << "Enter command: ";
        cin >> command;

        switch (command) {
            case 'h':
                displayUsage();
                break;
            case '+': {
                int id, parentId;
                string name, type;
                char comma;
                cin >> id >> comma >> parentId >> comma >> name >> comma >> type;

                // Error handling
                if (name.empty() || type.empty() || (type != "d" && type != "f")) {
                    cout << "Error: Invalid input for adding a node." << endl;
                } else {
                    fileSystem.addNode(id, parentId, name, type);
                }
                break;
            }
            case '-': {
                int id;
                cin >> id;

                // Error handling
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(INT_MAX, '\n');
                    cout << "Error: Invalid input for deleting a node." << endl;
                } else {
                    fileSystem.deleteNode(id);
                }
                break;
            }
            case '*': {
                char optionalId;
                int id;
                cin >> optionalId;

                if (optionalId == '\n') {
                    fileSystem.displayTree();
                } else {
                    cin.unget();  // Put back the character for further processing
                    cin >> id;

                    // Error handling
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(INT_MAX, '\n');
                        cout << "Error: Invalid input for displaying a branch." << endl;
                    } else {
                        fileSystem.displayBranch(id);
                    }
                }
                break;
            }
            case '$':
                cout << "Program ended." << endl;
                break;
             default:
                cout << "Error: Unknown command. Enter a valid command." << endl;
        }

        cin.ignore(INT_MAX, '\n');  // Clear the input buffer
    } while (command != '$');

    return 0;
}