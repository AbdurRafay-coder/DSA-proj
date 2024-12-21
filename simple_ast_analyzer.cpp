#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include <map>
#include <regex>

// Abstract Syntax Tree Node
struct ASTNode {
    std::string type;
    std::vector<std::shared_ptr<ASTNode>> children;

    ASTNode(const std::string& type) : type(type) {}
};

// Simplified Tokenization
std::vector<std::string> tokenize(const std::string& code) {
    std::vector<std::string> tokens;
    std::string current_token;
    for (char c : code) {
        if (isspace(c) || c == ';' || c == ',' || c == '(' || c == ')' || c == '=' || c == '+' || c == '-') {
            if (!current_token.empty()) {
                tokens.push_back(current_token);
                current_token.clear();
            }
            if (!isspace(c)) {
                tokens.push_back(std::string(1, c)); // Add delimiters as tokens
            }
        } else {
            current_token += c;
        }
    }
    if (!current_token.empty()) {
        tokens.push_back(current_token);
    }
    return tokens;
}

// AST Creation (Extended for Expressions and Initialization)
std::shared_ptr<ASTNode> createAST(const std::vector<std::string>& tokens) {
    auto root = std::make_shared<ASTNode>("program");
    std::shared_ptr<ASTNode> current_declaration;

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "int" || tokens[i] == "float" || tokens[i] == "double" || tokens[i] == "char" || tokens[i] == "bool") {
            current_declaration = std::make_shared<ASTNode>("declaration");
            current_declaration->children.push_back(std::make_shared<ASTNode>(tokens[i])); // Type
        } else if (tokens[i] == ";" && current_declaration) {
            root->children.push_back(current_declaration);
            current_declaration = nullptr;
        } else if (tokens[i] == "=" && current_declaration) {
            // Initialization
            auto var_node = std::make_shared<ASTNode>(current_declaration->children.back()->type);
            current_declaration->children.pop_back(); // Remove variable name
            auto assign_node = std::make_shared<ASTNode>("assignment");
            assign_node->children.push_back(var_node);
            assign_node->children.push_back(std::make_shared<ASTNode>(tokens[++i])); // Assigned value
            current_declaration->children.push_back(assign_node);
        } else if (tokens[i] != "," && current_declaration) {
            current_declaration->children.push_back(std::make_shared<ASTNode>(tokens[i])); // Variable name
        }
    }
    return root;
}

// Symbol Structure
struct Symbol {
    std::string name;
    std::string type;
    std::string value;
    int line_number;
};

// Symbol Table
class SymbolTable {
public:
    std::map<std::string, Symbol> symbols;

    void insert(const Symbol& symbol) {
        if (symbols.find(symbol.name) != symbols.end()) {
            std::cerr << "Warning: Variable " << symbol.name << " redeclared." << std::endl;
        }
        symbols[symbol.name] = symbol;
    }

    void print() const {
        for (const auto& [name, symbol] : symbols) {
            std::cout << "Variable: " << symbol.name
                      << ", Type: " << symbol.type
                      << ", Value: " << symbol.value
                      << ", Line: " << symbol.line_number << std::endl;
        }
    }
};

// Traverse AST and Populate Symbol Table
void traverseAST(const std::shared_ptr<ASTNode>& node, SymbolTable& symbolTable, int line = 1) {
    if (!node) return;

    if (node->type == "declaration") {
        std::string type = node->children[0]->type; // First child is the type
        for (size_t i = 1; i < node->children.size(); ++i) {
            if (node->children[i]->type == "assignment") {
                std::string name = node->children[i]->children[0]->type;
                std::string value = node->children[i]->children[1]->type;
                symbolTable.insert({name, type, value, line});
            } else {
                std::string name = node->children[i]->type;
                symbolTable.insert({name, type, "", line});
            }
        }
    }

    for (const auto& child : node->children) {
        traverseAST(child, symbolTable, line);
    }
}

// AST Visualization
void printAST(const std::shared_ptr<ASTNode>& node, int depth = 0) {
    if (!node) return;

    std::cout << std::string(depth * 2, ' ') << node->type << std::endl;
    for (const auto& child : node->children) {
        printAST(child, depth + 1);
    }
}

// Analyze Code
void analyzeCode(const std::string& code) {
    // Tokenize
    std::vector<std::string> tokens = tokenize(code);

    // Debug: Print Tokens
    std::cout << "Tokens:" << std::endl;
    for (const auto& token : tokens) {
        std::cout << token << " ";
    }
    std::cout << std::endl;

    // Create AST
    std::shared_ptr<ASTNode> root = createAST(tokens);

    // Print AST
    std::cout << "Abstract Syntax Tree:" << std::endl;
    printAST(root);

    // Traverse AST
    SymbolTable symbolTable;
    traverseAST(root, symbolTable);

    // Print Symbol Table
    std::cout << "Symbol Table:" << std::endl;
    symbolTable.print();
}

// Main Function
int main() {
    // Open the file
    std::ifstream codeFile("test_code.txt");
    if (!codeFile.is_open()) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return 1;
    }

    // Read the file content into a string
    std::string line, code;
    while (std::getline(codeFile, line)) {
        code += line + '\n';
    }
    codeFile.close();

    // Analyze the code
    analyzeCode(code);

    return 0;
}
