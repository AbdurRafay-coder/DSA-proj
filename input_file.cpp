#include <iostream>
using namespace std;

// Function 1: Simple function
void simpleFunction() {
    cout << "Hello, World!" << endl;
}

// Function 2: Function with if-else
void conditionalFunction(int x) {
    if (x > 0) {
        cout << "Positive" << endl;
    } else {
        cout << "Non-positive" << endl;
    }
}

int sum(int a, int b) {
    int sum = a + b;//comment
    return sum;
}

// Function 3: Function with a loop
void loopFunction(int n) {
    for (int i = 0; i < n; i++) {
        cout << "Count: " << i << endl;
    }
}

// Function 4: Complex function with multiple conditions
void complexFunction(int x, int y) {
    if (x > 0 && y > 0) {
        cout << "Both are positive" << endl;
    } else if (x > 0 || y > 0) {
        cout << "At least one is positive" << endl;
    } else {
        cout << "None are positive" << endl;
    }

    switch (x) {
        case 1:
            cout << "x is 1" << endl;
            break;
        case 2:
            cout << "x is 2" << endl;
            break;
        default:
            cout << "x is something else" << endl;
            break;
    }
}

int main() {
    simpleFunction();
    conditionalFunction(5);
    loopFunction(3);
    complexFunction(1, -1);
    return 0;
}
