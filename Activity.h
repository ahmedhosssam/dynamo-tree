#ifndef ACTIVITY_H
#define ACTIVITY_H
#include <iostream>
#include <string>
using namespace std;

class Activity {
private:
    string type = "";
    string content = "";
    int id = -1;
public:
    Activity(const string& Atype, const string& Acontent = "") : type(Atype), content(Acontent) {
        id = std::rand() % 999999;
    }

    Activity () {
    }

    friend ostream& operator << (ostream& os, const Activity& activity) {
        os << "{";
        os << "\"activityContent\":" << activity.content << ",";
        os << "\"activityType\":" << activity.type << ",";
        os << "\"id\":" << activity.id;
        os << "}";
        return os;
    }

    string printString() {
        string res = "{";
        res += "\"activityContent\":" + content + ",";
        res += "\"activityType\":" + type + ",";
        res += "\"id\":" + to_string(id);
        res += "}";
        return res;
    }
};

#endif // ACTIVITY_H
