#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <string>

using namespace std;

// Apzīmē mezglu kokā
class TreeNode {
public:
    int id;
    int parentId;
    string name;
    string type;
    vector<TreeNode*> children;

    // Konstruktors mezgla inicializācijai
    TreeNode(int id, int parentId, const string& name, const string& type)
        : id(id), parentId(parentId), name(name), type(type) {}

    // Destruktors, lai atbrīvotu piešķirto atmiņu
    ~TreeNode() {
        for (TreeNode* child : children) {
            delete child;
        }
    }
};

// Apzīmē koka struktūru
class Tree {
private:
    TreeNode* root;
    map<int, TreeNode*> nodeMap;

public:
    // Konstruktors koka inicializācijai
    Tree() : root(nullptr) {}

    // Destruktors, lai atbrīvotu piešķirto atmiņu
    ~Tree() {
        delete root;
    }

    // Pievienojiet kokam jaunu mezglu
    void addNode(int id, int parentId, const string& name, const string& type) {
        TreeNode* newNode = new TreeNode(id, parentId, name, type);
        nodeMap[id] = newNode;

        if (parentId == -1) {
            root = newNode;
        } else {
            TreeNode* parent = nodeMap[parentId];
            if (parent) {
                parent->children.push_back(newNode);
            } else {
                cerr << "Error: Parent node with ID " << parentId << " not found.\n";
                delete newNode;
            }
        }
    }

    // Izdzēsiet mezglu no koka
    void deleteNode(int id) {
        if (nodeMap.find(id) != nodeMap.end()) {
            TreeNode* node = nodeMap[id];
            if (node->parentId != -1) {
                TreeNode* parent = nodeMap[node->parentId];
                auto it = remove_if(parent->children.begin(), parent->children.end(),
                                    [id](TreeNode* child) { return child->id == id; });
                parent->children.erase(it, parent->children.end());
            } else {
                delete root;
                root = nullptr;
            }
            delete node;
            nodeMap.erase(id);
        } else {
            cerr << "Error: Node with ID " << id << " not found.\n";
        }
    }

    // DParādiet zarotni, sākot no noteikta mezgla
    void displayBranch(int id) {
        if (nodeMap.find(id) != nodeMap.end()) {
            displayBranchHelper(nodeMap[id], 0);
        } else {
            cerr << "Error: Node with ID " << id << " not found.\n";
        }
    }

    // Parādīt visu koku
    void displayTree() {
        if (root) {
            displayBranchHelper(root, 0);
        } else {
            cout << "The tree is empty.\n";
        }
    }

private:
    // Palīdzības funkcija, lai parādītu zarošanos 
    void displayBranchHelper(TreeNode* node, int depth) {
        for (int i = 0; i < depth; ++i) {
            cout << "  ";
        }

        // Rādīt mezgla veidu un informāciju
        if (node->type == "directory") {
            cout << "Folder";
        } else if (node->type == "file") {
            cout << "File";
        } else {
            cout << "Unknown";
        }

        cout << " (" << node->id << ") - " << node->name << "\n";

        // parādīt pakārtotos (bērnus)
        for (TreeNode* child : node->children) {
            displayBranchHelper(child, depth + 1);
        }
    }
};

// Galvenā funkcija programmas palaišanai
int main() {
    Tree fileSystem;

    //Komandas ievades pārbaude
    while (true) {
        cout << "Enter command (h for help): ";
        string command;
        getline(cin, command);

        if (command.empty()) {
            cerr << "Error: Empty command.\n";
            continue;
        }
        //Ja ievade ir komanda tad...
        istringstream iss(command);
        char action;
        iss >> action;

        switch (action) {
            case 'h':
                // Parādīt komandu lietošanas rokasgrāmatu
                cout << "Commands:\n";
                cout << "h[Enter] - Application command line usage guide.\n";
                cout << "+id,high_level_node_id,name,type[Enter] - Add a new node. - examples - +1,-1,Root,directory; +3,1,Images,directory \n";
                cout << "-id[Enter] - Delete the node with the given ID.\n";
                cout << "*id[Enter] - Display the tree branch starting from the node with ID.\n";
                cout << "*[Enter] - Display the entire tree.\n";
                cout << "$[Enter] - End the program.\n";
                break;

            case '+':
                // Pievienot jaunu mezglu
                {
                    int id, parentId;
                    string name, type;
                    char comma;

                    iss >> id >> comma >> parentId >> comma;
                    getline(iss, name, ',');
                    iss >> type;

                    if (name.empty()) {
                        cerr << "Error: Node name is empty.\n";
                        break;
                    }

                    if (type != "directory" && type != "file") {
                        cerr << "Error: Invalid node type. Use 'directory' or 'file'.\n";
                        break;
                    }

                    fileSystem.addNode(id, parentId, name, type);
                }
                break;

            case '-':
                // Izdzēst mezglu
                {
                    int id;
                    iss >> id;
                    fileSystem.deleteNode(id);
                }
                break;

            case '*':
                // Parādīt koku vai zarojumu
                if (iss.peek() == '\n' || iss.peek() == EOF) {
                    fileSystem.displayTree();
                } else {
                    int id;
                    iss >> id;
                    fileSystem.displayBranch(id);
                }
                break;

            case '$':
                // izbeigt programmu
                return 0;

            default:
                cerr << "Error: Unknown command.\n";
                break;
        }
    }

    return 0;
}

// testējamas ievades.
// +1,-1,Root,directory
// +2,1,Documents,directory
// +3,1,Images,directory
// +4,2,TextFiles,directory
// +5,2,SpreadsheetFiles,directory
// +6,3,SummerVacation.jpg,file
// +7,4,ImportantDoc.txt,file
// +8,5,Budget.xlsx,file