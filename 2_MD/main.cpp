#include <iostream>
#include <climits> 
#include <sstream>

using namespace std;

// RowElement klase, lai attēlotu punktu ar x un y koordinātām
class RowElement {
public:
    int x;
    int y;

    // Constructors
    RowElement() : x(0), y(0) {}
    RowElement(int x, int y) : x(x), y(y) {}
};

// Queue klase punktu līnijas pārvaldīšanai
class Queue {
private:
    RowElement* elements;  // Apļveida masīvs elementu glabāšanai
    int maxSize;          // Maksimālais garums
    int front;            // Rindas priekšpuses rādītājs(Index)
    int rear;             // Rindas aizmugures rādītājs(Index)
    int currentSize;      // Pašreizējais elementu skaits rindā

public:
    // Constructors lai iestatītu maksimālo līnijas garumu
    Queue(int size) : maxSize(size), front(-1), rear(-1), currentSize(0) {
        elements = new RowElement[maxSize];
    }

    // Destructors lai atbrīvotu piešķirto atmiņu
    ~Queue() {
        delete[] elements;
    }

    // Push metode lai rindā ievietotu elementu
    void push(int x, int y) {
        if (isFull()) {
            cout << "Overflow: Queue is full." << endl;
            return;
        }

        rear = (rear + 1) % maxSize;
        elements[rear] = RowElement(x, y);
        if (front == -1) {
            front = rear;
        }

        currentSize++;
    }

    // Pop metode lai noņemtu un atgrieztu elementu no rindas
    RowElement pop() {
        if (isEmpty()) {
            cout << "Underflow: Queue is empty." << endl;
            return RowElement();  // Atgriezt pēc noklusējuma izveidoto RowElement, ja rinda nav pilna
        }

        RowElement popped = elements[front];
        if (front == rear) {
            front = rear = -1;
        } else {
            front = (front + 1) % maxSize;
        }

        currentSize--;
        return popped;
    }

    // Show metode attēlo rindu kā punktu virkni 
    void show() {
        if (isEmpty()) {
            cout << " Queue is empty." << endl;
            return;
        }

        int index = front;
        do {
            cout << "{" << elements[index].x << ", " << elements[index].y << "} ";
            index = (index + 1) % maxSize;
        } while (index != (rear + 1) % maxSize);

        cout << endl;
    }

    // Pārbaude vai rinda ir pilna
    bool isFull() {
        return currentSize == maxSize;
    }

    // Pārbaude vai rinda ir tukša
    bool isEmpty() {
        return currentSize == 0;
    }
};

// Funkcija, lai parādītu komandrindas lietošanas rokasgrāmatu
void displayUsage() {
    cout << "Command line usage:" << endl;
    cout << "h: Display usage guide." << endl;
    cout << "+x,y: Insert point coordinates into the line. example +4,5" << endl;
    cout << "-: Remove an element from the queue and display it on the screen." << endl;
    cout << "*: Display the line on the screen." << endl;
    cout << "$: End the program." << endl;
}

int main() {
    int maxSize;
    cout << "Enter the maximum length of the line: ";
    cin >> maxSize;

displayUsage();

    Queue line(maxSize);

    char command;
    do {
        cout << "Enter command: ";
        
        cin >> command;

        switch (command) {
            case 'h':
                displayUsage();
                break;
            case '+': {
                int x, y;
                char comma;
                cin >> x >> comma >> y;

                if (cin.fail() || comma != ',') {
                    cin.clear();
                    cin.ignore(INT_MAX, '\n');
                    cout << "Error: Incorrect value. Enter valid coordinates." << endl;
                } else {
                    line.push(x, y);
                }
                break;
            }
            case '-': {
                RowElement popped = line.pop();
                if (popped.x != 0) {
                    cout << "Popped: {" << popped.x << ", " << popped.y << "}" << endl;
                }
                break;
            }
            case '*':
                line.show();
                break;
            case '$':
                cout << "Program ended." << endl;
                break;
             default:
                 cout << "Error: Unknown command. Enter a valid command." << endl;
        }

    } while (command != '$');

    return 0;
}