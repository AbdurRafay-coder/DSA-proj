#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <regex>
#include <cctype>
#include <algorithm>
#include <cmath>
#include <set>
#include <unordered_map>
#include <stack>
#define limit 5000
using namespace std;
//ranking will done using BST
//AVL tree for balanced BST
//File handling for users data
//regx B1hterb0l!

char methodAreaFile[] = "methodArea.txt";   // Method area range file
string savedFile[limit];
int saveFileSize = 0;
class CYCLONE;
vector<CYCLONE> methodTracer;
float averageComplexity;

class CYCLONE 
{
    int startLine;
    int finishLine;
    int complexity;
    string name;
    bool nameFlag;  // True if the method name exists in the line containing the opening brace
    public:
    void saveMethodArea() 
    {
        remove(methodAreaFile);
        ofstream oFile(methodAreaFile);

        if (oFile.is_open()) 
        {
            for (const auto& method : methodTracer) {
                if (method.nameFlag) 
                    oFile << method.startLine;
                else 
                    oFile << (method.startLine - 1);
                oFile << " " << method.finishLine << endl;
            }
            oFile.close();
        } 
        else 
        {
            cout << "File writing error\n";
        }
    }
    void calculateAverageComplexity() 
    {
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
    int countPredicates(int firstLineNo, int lastLineNo) 
    {
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
        for (auto& method : methodTracer) 
        {
            method.complexity = countPredicates(method.startLine, method.finishLine);
        }
    }

    void saveInString(char* inputFile) 
    {
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

    string findStringBeforeFirstBrace(string line) 
    {
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

    void assignName() 
    {
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

    void methodDetector(char* inputFile) 
    {
        saveInString(inputFile);

        stack<char> doubleQuote;
        stack<char> firstBracket;
        methodTracer.clear();

        for (int i = 0; i < saveFileSize; i++) {
            int strLen = savedFile[i].size();

            for (int j = 0; j < strLen; j++) {
                if (doubleQuote.empty() && savedFile[i][j] == '{' && firstBracket.size() == 2) {
                    CYCLONE tempNode;
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
        for (const auto& method : methodTracer) 
        {
            cout << left << setw(10) << " " << setw(20) << method.name
                << setw(10) << method.startLine << method.finishLine << endl;
        }
    }
};




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

class Queue
{
    public:
    struct Node
    {
        int data;
        Node* next;
    };
    Node* listhead = NULL;
    void push(int val);
    void pop();
    void display();
    int front();
};

void Queue::push(int val)
{
    Node* newnode = new Node();
    newnode->data = val;
    if(listhead == NULL)
    {
        listhead = newnode;
        listhead->next = NULL;
    }
    else
    {
        Node* temp = listhead;
         while(temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newnode;
        newnode->next = NULL;
    }
}

void Queue::pop()
{
    Node* temp = listhead;
    listhead = listhead->next;
    delete temp;
}

void Queue::display()
{
    Node* temp = listhead;
    while(temp != NULL)
    {
        cout<<"UserID: "<<temp->data<<" has logged in"<<endl;
        temp = temp->next;
    }
}

int Queue::front()
{
    if(listhead != NULL)
    {return listhead->data;}
    else
    {
        cout<<"EMPTY"<<endl;
        return -1;
    }
}

class User
{
    string name;
    string password;
    bool status = false;
    public:
    static int ID;
    friend void add_to_file(vector<User>& users,const string& filename);
    friend void add_user(vector<User>& users);
    friend void load_users(vector<User>& users,const string& filename);
    friend User* login(vector<User>& users, Queue& q);
    friend void logout(User& users);
    friend void codeloop(User& u);
};
int User::ID = 0;

bool isFileEmpty(const string& filename) 
{

    ifstream file(filename, ios::ate); 
    if (!file.is_open()) 
    {
        cout << "Failed to open file!" << endl;
        return true; 
    }
    return file.tellg() == 0; 
}

void add_to_file(vector<User>& users,const string& filename)
{
    ofstream file(filename, ios::app);
    User& lastuser = users.back();
    file<<"ID:"<<lastuser.ID<<"|"<<"Username:"<<lastuser.name<<"|"<<"Password:"<<lastuser.password<<endl;
    cout<<"Successfully registered"<<endl;
}

void add_user(vector<User>& users)
{
    User new_user;
    if(!users.empty())
    {
        User temp_user = users.back();
        new_user.ID = temp_user.ID + 1 ; //assigning a unique id to each user
    }
    else
    {
        new_user.ID += 1;
    }
    cin.ignore();
    cout<<"Please set your name: "<<endl;
    getline(cin,new_user.name);
    for(const auto& u: users)
    {
        if(u.name == new_user.name)//checking to make sure each user has a unique name, to avoid confusion
        {
            cout<<"This name is already taken"<<endl;
            cout<<"Please enter a different name"<<endl;
            while(u.name == new_user.name)
            {
                cout<<"Please set your name: "<<endl;
                getline(cin,new_user.name);
            }
        }
    }
    cout<<"Please set your password:"<<endl;
    getline(cin,new_user.password);
    users.push_back(new_user);
    add_to_file(users,"userdata.txt");
}

void load_users(vector<User>& users,const string& filename)
{
    ifstream file(filename);
    if(!isFileEmpty(filename))
    {
        string line;
        while(getline(file,line))
        {
            if(line.find("ID:") != string::npos)
            {
                User new_user;
                istringstream ss(line);
                string temp;
                getline(ss,temp,':');
                ss>>new_user.ID;
                getline(ss,temp,':');
                getline(ss,new_user.name,'|');
                getline(ss,temp,':');
                getline(ss,new_user.password);
                users.push_back(new_user);
            }
        }
    }
}

User* login(vector<User>& users, Queue& q)
{
    User temp;
    cout<<"Please enter your username: ";
    cin.ignore();
    getline(cin,temp.name);
    for(auto& u: users)
    {
        if(temp.name == u.name)
        {
            cout<<"Please enter your password: ";
            while(temp.password != "0" || temp.password != u.password)
            {
                getline(cin,temp.password);
                if(temp.password == u.password)
                {
                    cout<<"You are now logged in"<<endl;
                    u.status = true;
                    q.push(u.ID);
                    return &u;
                }
                else if(temp.password == "0")
                {
                    cout<<"exited the login system"<<endl;
                    return nullptr;
                }
                else
                {
                    cout<<"Incorrect password"<<endl;
                    cout<<"Please enter 0 if you want to exit the login procedure"<<endl;
                    cout<<"Otherwise enter the correct password"<<endl;
                }
            }
        }
    }
    cout<<"User Not Found"<<endl;
    return nullptr;
}

void logout(User& user)
{
    int opt;
    cout<<"Are you sure you want to logout?"<<endl;
    cout<<"Press 1 if you want to continue with the logout procedure"<<endl;
    cout<<"Otherwise press 0 to leave the logout procedure"<<endl;
    cin>>opt;
    if(opt == 1)
    {
        cout<<"You are now logged out"<<endl;
        user.status = false;
        return;
    }
    else
    {
        cout<<"You have chosen not to logout"<<endl;
        return;
    }
}

void codeloop(User& u)
{
    if(u.status == false)
    {
        cout<<u.name<<" is not logged in"<<endl;
        return;
    }
    if(u.status == true)
    {
        string inputfile;
        int choice = -1;
        while(choice != 0)
        {   
            system("cls");
            cout<<"Please enter the number corresponding to the action you want to perform"<<endl;
            cout<<"Please enter 1 to have your code analysed using halstead algorithm"<<endl;
            cout<<"Please enter 2 to have your code analysed using cyclomatic algorithm"<<endl;
            cin>>choice;
            if(choice != 0 )
            {
                cout<<"you are now inside codeloop"<<endl;
                cout<<"please enter the name of the cpp file"<<endl;
                cin.ignore();
                getline(cin,inputfile);
                
                if(choice == 1)
                {
                    HalsteadMetrics metrics;
                    metrics.analyzeFile(inputfile);
                }
                if(choice == 2)
                {
                    CYCLONE c;
                    char inputFile[] = "file2.cpp";
                    c.methodDetector(inputFile);
                    c.calculation();
                    c.saveMethodArea();
                    c.calculateAverageComplexity();
                    cout << "Average Cyclomatic Complexity: " << averageComplexity << endl;
                }
            }
            
            system("Pause");
        }
        
    }
}

int main()
{
    vector<User> users;
    Queue logbook;
    fstream MyFile("userdata.txt", ios::in | ios::out | ios::app);
    load_users(users,"userdata.txt");
    int choice = -1;
    
    while(choice!=0)
    {
        system("cls");
        cout<<"Please enter the number corresponding to the action you want to perform"<<endl;
        cout<<"Please enter 1 to create an account"<<endl;
        cout<<"Please enter 2 to login and use the system"<<endl;
        cout<<"Please enter 3 to logout"<<endl;
        cout<<"Please enter 4 to upload code for complexity analysis"<<endl;
        cin>>choice;
        User* logged;
        if(choice == 1)
        {
            add_user(users);
        }
        if(choice == 2)
        {
            logged = login(users,logbook);
        }
        if(choice == 3)
        {
            logout(*logged);
        }
        if(choice == 4)
        {
            codeloop(*logged);
        }
        system("Pause");
    }
    return 0;
}