#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cpprest/http_listener.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include <chrono>
#include <thread>

#include "dynamoTree.h"

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace std;

int getRandomID() {
    return std::rand() % 999999;
}

void addNewUser (DynamoTree *tree, json::value obj) {
	tree->createUser(
		obj[U("username")].serialize(),
		obj[U("id")].serialize()
	);

	ofstream ofile("db.json");
	if (ofile.is_open()) {
		ofile << tree->printdynamo();
		ofile.close();
	} else {
		cerr << "err.\n";
	}
}

bool deleteUser(string username, DynamoTree *tree) {
	tree->deleteUser(username);

	ofstream ofile("db.json");
	if (ofile.is_open()) {
		ofile << tree->printdynamo();
		ofile.close();
	} else {
		cerr << "Error.\n";
	}
	return true;
}

// add new activity to the user, the user is chosen by username
void addNewActivity (
	json::value activityContent,
	json::value activityType,
	json::value activityID,
	json::value username,
	DynamoTree *tree
) {
	tree->addActivity(
		username.serialize(),
		activityType.serialize(),
		activityContent.serialize()
	);

	ofstream ofile("db.json");
	if (ofile.is_open()) {
		ofile << conversions::to_utf8string(tree->printdynamo());
		ofile.close();
	} else {
		cerr << "Error.\n";
	}
}

void deleteActivity (json::value username, DynamoTree *tree) {
	tree->dequeue(username.serialize());

	ofstream ofile("db.json");
	if (ofile.is_open()) {
		ofile << tree->printdynamo();
		ofile.close();
	} else {
		cerr << "Error.\n";
	}
}

DynamoTree* buildTreeFromJson(string file) {
	DynamoTree* tree = new DynamoTree;
	ifstream ifile(file);
	json::value arr;

	if (ifile.is_open()) {
		string fileContents(
			(istreambuf_iterator<char>(ifile)),
			istreambuf_iterator<char>());
		arr = json::value::parse(conversions::to_string_t(fileContents));
		ifile.close();
	}

	for (size_t i = 0; i < arr.size(); i++) {
		tree->createUser(arr[i][U("username")].serialize(), arr[i][U("id")].serialize());
		for (size_t j = 0; j < arr[i][U("activities")].size(); j++) {
			tree->addActivity (
				arr[i][U("username")].serialize(),
				arr[i][U("activities")][j][U("activityType")].serialize(),
				arr[i][U("activities")][j][U("activityContent")].serialize()
			);
		}
	}

	return tree;
}

int main() {
	http_listener listener(U("http://localhost:8080"));
	DynamoTree* tree = buildTreeFromJson("db.json");

	listener.support(methods::GET, [tree](http_request req) {
		http_response res(status_codes::OK);
        res.headers().add(U("Access-Control-Allow-Origin"), U("*")); // I have to study CORS
		json::value jsonTree;
		jsonTree = json::value::parse(conversions::to_string_t(tree->printdynamo()));
        res.set_body(jsonTree);
        req.reply(res);
	});

	listener.support(methods::POST, [=](http_request req) { 
		req.headers().add(U("Access-Control-Allow-Origin"), U("*"));

		req.extract_json().then([=](json::value reqData) {
			if (!reqData.is_null()) {
				// i use this because methods::DEL doesn't work
				if (reqData[U("order")] == json::value::string(U("delete"))) {
					string data = reqData.serialize();
					deleteUser(reqData[U("target")].serialize(), tree);
				} else if (reqData[U("order")] == json::value::string(U("addActivity"))) {
					addNewActivity (
						reqData[U("activityContent")],
						reqData[U("activityType")],
						reqData[U("activityID")],
						reqData[U("username")],
						tree
						);
				// I use this because methods::DEL doesn't work
				} else if (reqData[U("order")] == json::value::string(U("dequeue"))) { // delete from the queue
					deleteActivity (
						reqData[U("username")],
						tree
					);
				} else {
					addNewUser(tree, reqData); // add new user to the tree
				}
				cout << "received JSON: " << reqData.serialize() << endl;
				req.reply(status_codes::OK, U("Done."));

			} else {
				req.reply(status_codes::BadRequest, U("Error: Bad request."));
			}
		});
	});

	listener.support(methods::OPTIONS, [](http_request req) {
		http_response res(status_codes::OK);
   		res.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    	res.headers().add(U("Access-Control-Allow-Methods"), U("GET,PUT,POST,DELETE"));
    	res.headers().add(U("Access-Control-Allow-Headers"), U("Content-Type"));
        req.reply(res);
	});

	// TODO: doesn't work
	listener.support(methods::DEL, [](http_request req) {
		http_response res(status_codes::OK);

        res.headers().add(U("Access-Control-Allow-Origin"), U("*")); 
    	res.headers().add(U("Access-Control-Allow-Methods"), U("GET,PUT,POST,DELETE"));
    	res.headers().add(U("Access-Control-Allow-Headers"), U("Content-Type"));


		req.extract_json().then([=](json::value reqData) {
			if (!reqData.is_null()) {
				cout << "Received JSON: " << reqData.serialize() << endl;
        		req.reply(res);
			} else {
				req.reply(status_codes::BadRequest, U("Ah hell naa"));
			}
		});
	});


	listener.open().then([]() {
		wcout <<  "\n\nListening on localhost:8080..." << endl;
	}).wait();

	while (true);

	return 0;
}
