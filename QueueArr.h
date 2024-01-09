#ifndef QUEUEARR_H
#define QUEUEARR_H
template <typename T>

class Queue {
private:
    static const int MAX_SIZE = 100;
    int size = 0;
    T elements[MAX_SIZE];
    int front;
    int rear;

public:
    Queue() : front(0), rear(0) {}

    ~Queue() {}

    bool isEmpty() const {
        return front == rear;
    }

    bool isFull() const {
        return (rear + 1) % MAX_SIZE == front;
    }

    void enqueue(const T newElement) {
        if (isFull()) {
            dequeue();
            enqueue(newElement);
            return;
        }
        elements[rear] = newElement;
        rear = (rear + 1) % MAX_SIZE;
        size++;
    }

    void dequeue() {
        if (!isEmpty()) {
            front = (front + 1) % MAX_SIZE;
            size--;
        }
        else {
            cerr << "Queue is empty. Cannot dequeue.\n";
        }
    }

    friend ostream& operator << (ostream& os, const Queue& queue) {
        os << "\"activities\":";
        os << "[";
        if (!queue.isEmpty()) {
            int index = queue.front;

            os << queue.elements[index];
            index = (index + 1) % queue.MAX_SIZE;

            while (index != queue.rear) {
                os << ", " << queue.elements[index];
                index = (index + 1) % queue.MAX_SIZE;
            }
        }
        os << "]";
        return os;
    }

    string printString() {
        string result = "\"activities\":[";
        if (!isEmpty()) {
            int index = front;

            result += elements[index].printString();
            index = (index + 1) % MAX_SIZE;

            while (index != rear) {
                result += ", " + elements[index].printString();
                index = (index + 1) % MAX_SIZE;
            }
        }
        result += "]";
        return result;
    }
};

#endif 
