a.out: main.cpp 
	g++ -std=c++11 $^ -o $@ -lcpprest -lboost_system -lboost_chrono -lboost_thread -lboost_random -lssl -lcrypto -lpthread

.PHONY: clean

clean:
	rm -f my_program

