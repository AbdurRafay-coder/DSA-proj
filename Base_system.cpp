#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
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
    bool status = false;
    public:
    static int ID;
    friend void add_to_file(vector<User>& users,const string& filename);
    friend void add_user(vector<User>& users);
    friend void load_users(vector<User>& users,const string& filename);
    friend void login(vector<User>& users);
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

void login(vector<User>& users)
{
    User temp;
    cout<<"Please enter your username: ";
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
                    return;
                }
                else if(temp.password == "0")
                {
                    cout<<"exited the login system"<<endl;
                    return;
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
}



int main()
{
    vector<User> users;
    fstream MyFile("userdata.txt", ios::in | ios::out | ios::app);
    int choice = 0;
    cout<<"Please enter the number corresponding to the action you want to perform"<<endl;
    cout<<"Please enter 1 to create an account"<<endl;
    cout<<"Please enter 2 to login"<<endl;
    cout<<"Please enter 3 to logout"<<endl;
    cout<<"Please enter 4 to upload code"<<endl;
    cin>>choice;
    while(choice!=0)
    {
        if(choice == 1)
        {
            add_user(users);
        }
        if(choice == 2)
        {

        }

    }
    return 0;
}