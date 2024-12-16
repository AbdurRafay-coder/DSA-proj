#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;
//ranking will done using BST
//AVL tree for balanced BST
//File handling for users data
//regx B1hterb0l!
class User
{
    string name;
    string password;
    static int ID;
    public:
    friend void add_user_file(User u,const string& filename);
    friend void add_user(vector<User>& user);
};

void set_name(User u,const string& filename)
{
    ofstream file(filename, ios::app);
    
}

void add_user(vector<User>& user)
{
    User new_user;
    cout<<"Please set your name: "<<endl;
    getline(cin,new_user.name);
    cout<<"Please set your password:"<<endl;
    getline(cin,new_user.password);
}

int main()
{
    return 0;
}