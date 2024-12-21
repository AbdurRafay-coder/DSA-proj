#include <iostream>
#include <string>
#include <set>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <regex>
#include <cctype>
#include <algorithm>

class HalsteadMetrics {
private:
    std::set<std::string> uniqueOperators;
    std::set<std::string> uniqueOperands;
    std::unordered_map<std::string, int> operatorFreq;
    std::unordered_map<std::string, int> operandFreq;

    // C++ operators and some keywords
    
    const std::vector<std::string> operators = {
        "+", "-", "*", "/", "%", "++", "--", "=", "+=", "-=", "*=", "/=", "%=",
        "==", "!=", "<", ">", "<=", ">=", "&&", "||", "!", "&", "|", "^", "~", "<<", ">>", 
        "[]", "*", "&", ".", "->", "::", "new", "delete", "sizeof"
    };

    const std::set<std::string> typeKeywords = {
        "void", "int", "char", "float", "double", "long", "short", "unsigned", "signed", 
        "bool", "struct", "class", "enum", "const", "static", "extern", "register", "volatile"
    };

    bool isNumber(const std::string& str) const {
        std::regex numberPattern("^-?\\d+\\.\\d*$|^\\d+$");
        return std::regex_match(str, numberPattern);
    }

    std::string extractToken(std::string& code) {
        if (code.empty()) return "";

        // Skip whitespace
        code.erase(0, code.find_first_not_of(" \t\n\r"));
        if (code.empty()) return "";

        char firstChar = code[0];
        std::string token;

        // Handle string literals
        if (firstChar == '"' || firstChar == '\'') {
            size_t end = 1;
            while (end < code.length() && code[end] != firstChar) {
                if (code[end] == '\\') end += 2;
                else end++;
            }
            if (end < code.length()) end++;
            token = code.substr(0, end);
            code.erase(0, end);
            return token;
        }

        // Handle multi-character operators like "++", "--", "<<"
        std::string possibleOp = code.substr(0, 2);
        if (std::find(operators.begin(), operators.end(), possibleOp) != operators.end()) {
            code.erase(0, 2);
            return possibleOp;
        }

        // Handle single-character operators
        if (std::find(operators.begin(), operators.end(), std::string(1, firstChar)) != operators.end()) {
            code.erase(0, 1);
            return std::string(1, firstChar);
        }

        // Handle identifiers and numbers (operands)
        if (std::isalnum(firstChar) || firstChar == '_') {
            size_t end = 1;
            while (end < code.length() && (std::isalnum(code[end]) || code[end] == '_')) {
                end++;
            }
            token = code.substr(0, end);
            code.erase(0, end);
            return token;
        }

        // Handle other characters
        code.erase(0, 1);
        return std::string(1, firstChar);
    }

    void processToken(const std::string& token) {
        if (token.empty() || token == " " || token == "\n" || token == "\t") return;

        // Check if it's an operator
        if (std::find(operators.begin(), operators.end(), token) != operators.end()) {
            addOperator(token);
        }
        // Skip type keywords
        else if (typeKeywords.find(token) == typeKeywords.end()) {
            // If it's not a type keyword, it's an operand
            if (token != ";" && token != "{" && token != "}" && !isNumber(token)) {
                addOperand(token);
            }
        }
    }

    void addOperator(const std::string& op) {
        uniqueOperators.insert(op);
        operatorFreq[op]++;
    }

    void addOperand(const std::string& operand) {
        uniqueOperands.insert(operand);
        operandFreq[operand]++;
    }

public:
    void analyzeCode(const std::string& code) {
        std::string workingCode = code;
        std::string token;

        while (!workingCode.empty()) {
            // Handle comments
            if (workingCode.substr(0, 2) == "//") {
                size_t end = workingCode.find('\n');
                if (end == std::string::npos) break;
                workingCode.erase(0, end + 1);
                continue;
            }
            if (workingCode.substr(0, 2) == "/*") {
                size_t end = workingCode.find("*/");
                if (end == std::string::npos) break;
                workingCode.erase(0, end + 2);
                continue;
            }

            token = extractToken(workingCode);
            processToken(token);
        }

        printMetrics();
    }

    void analyzeFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file " << filename << std::endl;
            return;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        analyzeCode(buffer.str());
    }

    int getUniqueOperators() const { return uniqueOperators.size(); }
    int getUniqueOperands() const { return uniqueOperands.size(); }
    int getTotalOperators() const {
        int total = 0;
        for (const auto& pair : operatorFreq) {
            total += pair.second;
        }
        return total;
    }
    int getTotalOperands() const {
        int total = 0;
        for (const auto& pair : operandFreq) {
            total += pair.second;
        }
        return total;
    }

    double getProgramVocabulary() const {
        return getUniqueOperators() + getUniqueOperands();
    }

    double getProgramLength() const {
        return getTotalOperators() + getTotalOperands();
    }

    double getCalculatedProgramLength() const {
        double n1 = getUniqueOperators();
        double n2 = getUniqueOperands();
        return n1 * log2(n1) + n2 * log2(n2);
    }

    double getProgramVolume() const {
        return getProgramLength() * log2(getProgramVocabulary());
    }

    double getDifficulty() const {
        if (getUniqueOperands() == 0) return 0;
        return (getUniqueOperators() * getTotalOperands()) / (2.0 * getUniqueOperands());
    }

    double getEffort() const {
        return getDifficulty() * getProgramVolume();
    }

    double getTimeToProgram() const {
        return getEffort() / 18.0;
    }

    void printMetrics() const {
        std::cout << "\nHalstead Metrics:\n";
        std::cout << "Basic Metrics:\n";
        std::cout << "Unique Operators (n1): " << getUniqueOperators() << "\n";
        std::cout << "Unique Operands (n2): " << getUniqueOperands() << "\n";
        std::cout << "Total Operators (N1): " << getTotalOperators() << "\n";
        std::cout << "Total Operands (N2): " << getTotalOperands() << "\n\n";

        std::cout << "Derived Metrics:\n";
        std::cout << "Program Vocabulary (n): " << getProgramVocabulary() << "\n";
        std::cout << "Program Length (N): " << getProgramLength() << "\n";
        std::cout << "Calculated Program Length (N'): " << getCalculatedProgramLength() << "\n";
        std::cout << "Volume (V): " << getProgramVolume() << "\n";
        std::cout << "Difficulty (D): " << getDifficulty() << "\n";
        std::cout << "Effort (E): " << getEffort() << "\n";
        std::cout << "Time to Program (T): " << getTimeToProgram() << " seconds\n";
    }
};

int main() {
    std::string inputFile = "file1.cpp";  // Update with your input file
    HalsteadMetrics metrics;
    metrics.analyzeFile(inputFile);
    return 0;
}
