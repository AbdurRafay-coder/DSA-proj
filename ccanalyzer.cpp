#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stack>
#include <sstream>
#include <algorithm>
#include <iomanip> // For formatting output

#define limit 5000  // Assume no single file will exceed 5000 lines
using namespace std;

char commentFreeFile[] = "input_file.cpp";  // Input source code file
char advancedResultFile[] = "result.txt";   // Output result file
char methodAreaFile[] = "methodArea.txt";   // Method area range file
string savedFile[limit];
int saveFileSize = 0;

struct node {
    int startLine;
    int finishLine;
    int complexity;
    string name;
    bool nameFlag;  // True if the method name exists in the line containing the opening brace
};

vector<node> methodTracer;
float averageComplexity;

void saveMethodArea() {
    remove(methodAreaFile);
    ofstream oFile(methodAreaFile);

    if (oFile.is_open()) {
        for (const auto& method : methodTracer) {
            if (method.nameFlag) 
                oFile << method.startLine;
            else 
                oFile << (method.startLine - 1);
            oFile << " " << method.finishLine << endl;
        }
        oFile.close();
    } else {
        cout << "File writing error\n";
    }
}

void calculateAverageComplexity() {
    if (methodTracer.empty()) {
        cout << "No methods detected. Average complexity is undefined.\n";
        return;
    }

    averageComplexity = 0.0;
    for (const auto& method : methodTracer) {
        averageComplexity += method.complexity;
    }
    averageComplexity /= methodTracer.size();
}

int countPredicates(int firstLineNo, int lastLineNo) {
    int tempComplexity = 1; // Initial Complexity of a method

    for (int i = firstLineNo; i <= lastLineNo; i++) {
        string line = savedFile[i];

        // Debug: Print the line being checked
        cout << "Checking line " << i << ": " << line << endl;

        // Count occurrences of control flow keywords
        if (line.find("if") != string::npos) tempComplexity++;
        if (line.find("else if") != string::npos) tempComplexity++;
        if (line.find("for") != string::npos) tempComplexity++;
        if (line.find("while") != string::npos) tempComplexity++;
        if (line.find("case") != string::npos) tempComplexity++;
        if (line.find("&&") != string::npos) tempComplexity++;
        if (line.find("||") != string::npos) tempComplexity++;
        if (line.find("?") != string::npos) tempComplexity++;  // Ternary operator
    }

    // Debug: Print the computed complexity for this range
    cout << "Complexity for lines " << firstLineNo << " to " << lastLineNo << ": " << tempComplexity << endl;

    return tempComplexity;
}

void calculation() {
    for (auto& method : methodTracer) {
        method.complexity = countPredicates(method.startLine, method.finishLine);
    }
}

void saveInString(char* inputFile) {
    saveFileSize = 0; // Reset line count
    ifstream iFile(inputFile);

    if (iFile.is_open()) {
        string line;
        while (getline(iFile, line)) {
            // Skip empty or preprocessor lines
            if (line.empty() || line[0] == '#') continue;
            savedFile[saveFileSize++] = line;
        }
        iFile.close();
    } else {
        cout << "File not found\n";
    }
}

string findStringBeforeFirstBrace(string line) {
    int firstBracePosition = -1;
    int len = line.size();

    for (int i = 0; i < len; i++) {
        if (line[i] == '(') {
            firstBracePosition = i;
        } else if (line[i] == '{') {
            line[i] = ' ';  // Replace the brace
            break;
        }
    }

    if (firstBracePosition != -1) 
        return line.substr(0, firstBracePosition);
    else 
        return "";
}

void assignName() {
    for (auto& method : methodTracer) {
        int firstLine = method.startLine;
        string tempWords[10];

        istringstream iss(findStringBeforeFirstBrace(savedFile[firstLine]));
        int counter = 0;
        string word;
        while (iss >> word) tempWords[counter++] = word;

        if (counter > 1) {
            method.name = tempWords[1];
            method.nameFlag = true;
            continue;
        }

        istringstream iss1(findStringBeforeFirstBrace(savedFile[firstLine - 1]));
        counter = 0;
        while (iss1 >> word) tempWords[counter++] = word;
        if (counter > 1) {
            method.name = tempWords[1];
            method.nameFlag = false;
        }
    }
}

void methodDetector(char* inputFile) {
    saveInString(inputFile);

    stack<char> doubleQuote;
    stack<char> firstBracket;
    methodTracer.clear();

    for (int i = 0; i < saveFileSize; i++) {
        int strLen = savedFile[i].size();

        for (int j = 0; j < strLen; j++) {
            if (doubleQuote.empty() && savedFile[i][j] == '{' && firstBracket.size() == 2) {
                node tempNode;
                tempNode.startLine = i;
                doubleQuote.push('{');
                methodTracer.push_back(tempNode);
            } else if (!doubleQuote.empty() && savedFile[i][j] == '}' && firstBracket.size() == 2) {
                methodTracer.back().finishLine = i;
                doubleQuote.pop();
                firstBracket.pop();
                firstBracket.pop();
            } else if (savedFile[i][j] == '(' && doubleQuote.empty() && firstBracket.empty()) {
                firstBracket.push('(');
            } else if (savedFile[i][j] == ')' && doubleQuote.empty() && firstBracket.size() == 1) {
                firstBracket.push(')');
            }
        }
    }

    assignName();

    // Debug: Print all detected methods
    cout << left << setw(10) << "Method:" << setw(20) << "Name" 
         << setw(10) << "Start" << "End" << endl;
    for (const auto& method : methodTracer) {
        cout << left << setw(10) << " " << setw(20) << method.name
             << setw(10) << method.startLine << method.finishLine << endl;
    }
}

int main() {
    char inputFile[] = "input_file.cpp";

    methodDetector(inputFile);
    calculation();
    saveMethodArea();
    calculateAverageComplexity();

    cout << "Average Cyclomatic Complexity: " << averageComplexity << endl;

    return 0;
}
