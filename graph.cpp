#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <stack>
#include <algorithm>

// Trim function to remove leading/trailing spaces
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return ""; // no content
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

class ControlFlowGraph {
private:
    std::map<std::string, std::vector<std::string>> graph;
    int blockCount = 0;

public:
    void addBlock(const std::string& blockName) {
        graph[blockName] = std::vector<std::string>();
        blockCount++;
    }

    void addEdge(const std::string& from, const std::string& to) {
        graph[from].push_back(to);
    }

    int getBlockCount() const {
        return blockCount;
    }

    void printGraph() const {
        for (const auto& pair : graph) {
            std::cout << pair.first << " -> ";
            for (const auto& neighbor : pair.second) {
                std::cout << neighbor << " ";
            }
            std::cout << std::endl;
        }
    }

    void exportToDot(const std::string& filename) const {
        std::ofstream outFile(filename);
        outFile << "digraph CFG {" << std::endl;
        for (const auto& pair : graph) {
            for (const auto& neighbor : pair.second) {
                outFile << "  \"" << pair.first << "\" -> \"" << neighbor << "\";" << std::endl;
            }
        }
        outFile << "}" << std::endl;
    }
};

void buildCFG(const std::vector<std::string>& code, ControlFlowGraph& cfg) {
    cfg.addBlock("start");
    std::stack<std::string> controlStack;
    std::string prevBlock = "start";

    for (const auto& line : code) {
        std::string trimmedLine = trim(line);

        if (trimmedLine.empty()) continue; // Skip empty lines

        if (trimmedLine.find("if") == 0) {
            std::string conditionBlock = "if_" + std::to_string(cfg.getBlockCount());
            cfg.addBlock(conditionBlock);
            cfg.addEdge(prevBlock, conditionBlock);
            controlStack.push(conditionBlock);
            prevBlock = conditionBlock;
        } 
        else if (trimmedLine.find("else") == 0) {
            // Handle the else condition (ensure previous block connects properly)
            if (!controlStack.empty()) {
                std::string ifBlock = controlStack.top();
                std::string elseBlock = "else_" + std::to_string(cfg.getBlockCount());
                cfg.addBlock(elseBlock);
                cfg.addEdge(ifBlock, elseBlock); // Connect 'if' block to 'else'
                controlStack.push(elseBlock);
                prevBlock = elseBlock;
            }
        }
        else if (trimmedLine.find("while") == 0 || trimmedLine.find("for") == 0) {
            std::string loopBlock = "loop_" + std::to_string(cfg.getBlockCount());
            cfg.addBlock(loopBlock);
            cfg.addEdge(prevBlock, loopBlock);
            controlStack.push(loopBlock);
            prevBlock = loopBlock;
        }
        else if (trimmedLine == "}") {
            // Handle the closing brace of control structures (pop stack)
            if (!controlStack.empty()) {
                std::string endBlock = "end_" + std::to_string(cfg.getBlockCount());
                cfg.addBlock(endBlock);
                cfg.addEdge(prevBlock, endBlock);
                prevBlock = endBlock;
                controlStack.pop();
            }
        }
        else {
            // For regular statements
            std::string statementBlock = "stmt_" + std::to_string(cfg.getBlockCount());
            cfg.addBlock(statementBlock);
            cfg.addEdge(prevBlock, statementBlock);
            prevBlock = statementBlock;
        }
    }

    // End block to mark the end of the CFG
    cfg.addBlock("end");
    cfg.addEdge(prevBlock, "end");
}

int main() {
    // Read code from a file
    std::ifstream codeFile("code_sample.txt");
    if (!codeFile.is_open()) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return 1;
    }

    // Read the code lines
    std::vector<std::string> codeLines;
    std::string line;
    while (std::getline(codeFile, line)) {
        codeLines.push_back(line);
    }
    codeFile.close();

    // Create and build the CFG
    ControlFlowGraph cfg;
    buildCFG(codeLines, cfg);

    // Print CFG for debugging
    cfg.printGraph();

    // Export CFG to DOT file
    cfg.exportToDot("cfg.dot");

    // Visualization Instructions
    std::cout << "Use the following command to visualize the graph:" << std::endl;
    std::cout << "dot -Tpng cfg.dot -o cfg.png" << std::endl;

    return 0;
}
