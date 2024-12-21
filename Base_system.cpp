#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
using namespace std;
//ranking will done using BST
//AVL tree for balanced BST
//File handling for users data
//regx B1hterb0l!

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
    }
    if(u.status == true)
    {
        cout<<"insider codeloop"<<endl;
        //user will add their code here for complexity analysis
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

    }
    return 0;
}