//https://www.geeksforgeeks.org/introduction-to-circular-queue/

#include <iostream>

using namespace std;

class FIFO {
private:
    int** elements;
    int front;
    int rear;
    int length;

public:
    FIFO(int len) {
        length = len;
        elements = new int*[length];
        for (int i = 0; i < length; ++i)
            elements[i] = new int[2]; 

        front = -1;
        rear = -1;
    }

    void enqueue(int x, int y) {
        if ((rear + 1) % length == front) {
            cout << "Queue is full. Cannot enqueue element.\n";
            return;
        }

        if (front == -1)
            front = 0;

        rear = (rear + 1) % length;
        elements[rear][0] = x;
        elements[rear][1] = y;
    }

    void dequeue() {
        if (front == -1) {
            cout << "Queue is empty. Cannot dequeue element.\n";
            return;
        }

        cout << "Dequeued: (" << elements[front][0] << ", " << elements[front][1] << ")\n";

        if (front == rear) {
            front = -1;
            rear = -1;
        } else {
            front = (front + 1) % length;
        }
    }

    void display() {
        if (front == -1) {
            cout << "Queue is empty.\n";
            return;
        }

        cout << "Queue elements:\n";
        int i = front;
        while (i != rear) {
            cout << "(" << elements[i][0] << ", " << elements[i][1] << ")\n";
            i = (i + 1) % length;
        }
        cout << "(" << elements[i][0] << ", " << elements[i][1] << ")\n";
    }

    ~FIFO() {
        for (int i = 0; i < length; ++i)
            delete[] elements[i];
        delete[] elements;
    }
};

int main() {

   cout<<"teksts"<<endl;

      
    FIFO fifo(5);

    fifo.enqueue(1, 2);
    fifo.enqueue(3, 4);
    fifo.enqueue(5, 6);
    fifo.display();

    fifo.dequeue();
    fifo.display();

    fifo.enqueue(7, 8);
    fifo.enqueue(9, 10);
    fifo.display();

    fifo.dequeue();
    fifo.display();

    fifo.enqueue(11, 12);
    fifo.display();

    fifo.enqueue(13, 14);
    fifo.display();

    return 0;
}