typedef struct Node_ {
    char chData;
    double dbData;
    struct Node_* next;
}Node;

typedef struct Stack {
    Node* top;
} Stack;

void createStack(Stack* stack);
void pushStack(Stack* stack, char chData, double dbData);
double dbpopStack(Stack* stack);
char chpopStack(Stack* stack);
char topStack(Stack* stack);
int isemptyStack(Stack* stack);
void destroyStack(Stack* stack);
