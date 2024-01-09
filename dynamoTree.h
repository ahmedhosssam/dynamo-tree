#ifndef DYNAMOTREE_H
#define DYNAMOTREE_H

#include"AVLTree.h"
#include"QueueArr.h"
#include"Activity.h"
#include <string>

using namespace std;

struct UserNode
{
	string userName;
	string id;
	Queue<Activity> activities;
	UserNode(string name, string id) :userName(name), id(id) {}
	UserNode(string name) :userName(name), id("-1") {}
   ~UserNode() {}

    bool operator<(const UserNode& other) const {
        return userName < other.userName;
    }

    bool operator>(const UserNode& other) const {
        return userName > other.userName;
    }

    bool operator == (const UserNode& other) const {
        return userName == other.userName;
    }

    friend ostream& operator << (ostream& os, const UserNode& displayedNode) {
        os << "{";
        os << displayedNode.activities << ",";
        os << "\"id\":" << displayedNode.id << ",";
        os << "\"username\":" << displayedNode.userName;
        os << "}";
        return os;
    }

    string printString() {
        string result = "{";
        result += activities.printString() + ",";
        result += "\"id\":" + id + ",";
        result += "\"username\":" + userName;
        result += "}";
        return result;
    }
};

class DynamoTree : protected AVLTree<UserNode>
{
public:
    void createUser(string userName ,string userID) {
        UserNode newNode(userName, userID);
        this->insert(newNode);
    }

    void deleteUser(string userName) {
        UserNode node(userName, "0");
        this->deletee(node);
    }

    UserNode getUser(string username) {
        UserNode node(username, "0");
        return this->search(node)->data;
    }

    void addActivity (
        string username,
        const string& activityType = "",
        const string& Activity_content = ""
    )
    {
        Activity newActivity(activityType, Activity_content);
        UserNode node(username);
        this->search(node)->data.activities.enqueue(newActivity);
    }

    void dequeue (
        string username
    ) {
        UserNode node(username);
        this->search(node)->data.activities.dequeue();
    }

    string printdynamo() {
        return this->print();
    }
};

#endif
