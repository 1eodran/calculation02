#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void createStack(Stack* stack) {
    stack->top = NULL;
}

void pushStack(Stack* stack, char chData, double dbData) {
    Node* tmp = (Node*)malloc(sizeof(Node));
    tmp->chData = chData;
    tmp->dbData = dbData;
    tmp->next = stack->top;
    stack->top = tmp;
}

double dbpopStack(Stack* stack) {
    if (isemptyStack(stack)) {
        return 0;
    }
    Node* tmp = stack->top;
    double topData = tmp->dbData;
    stack->top = stack->top->next;
    free(tmp);
    return topData;
}

char chpopStack(Stack* stack) {
    if (isemptyStack(stack)) {
        return 0;
    }
    Node* tmp = stack->top;
    char topData = tmp->chData;
    stack->top = stack->top->next;
    free(tmp);
    return topData;
}

char topStack(Stack* stack) {
    if (isemptyStack(stack)) {
        return 0;
    }
    return stack->top->chData;
}

int isemptyStack(Stack* stack) {
    return (stack->top == NULL);
}

void destroyStack(Stack* stack) {
    Node* tmp = stack->top;
    while (tmp != NULL) {
        Node* next = tmp->next;
        free(tmp);
        tmp = next;
    }
    stack->top = NULL;
}
