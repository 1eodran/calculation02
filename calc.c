#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "calc.h"

// ����������
bool Operator(char inf)
{
    return (inf == '+' || inf == '-' || inf == '*' || inf == '/' || inf == '%' || inf == '^' || inf == 't' || inf == 'n' || inf == '~'|| inf == '='||inf =='&');
}

bool Operator2(char inf)
{
    return (inf == '+' || inf == '-' || inf == '*' || inf == '/' || inf == '%' || inf == '^');
}


// �ǿ�����
bool Operand(int inf)
{
    return(inf >= 0 && inf <= 9);
}

//������,�ǿ�����,��ȣ�� ������ ��������
int matchOper(Queue* infQ) {
    Node* tmp = infQ->front;
    char opt;
    double opd;
    int bo = 0;
    while (tmp != NULL)
    {
        opt = tmp->chData;
        opd = tmp->dbData;
        if (opt == 0 || (Operator(opt) && opd == 0) || opt == '(' || opt == ')') //�ǿ�����, ������, ��ȣ, ��ĭ�϶� true
        {
        }
        else
        {
            bo += 1;
        }
        tmp = tmp->next;
    }
    return bo;
}

//��ȣ�ְ˻�
bool matchParen(Queue* infQ) {
    Stack stack;
    createStack(&stack);
    Node* tmp = infQ->front;
    char chData;
    while (tmp != NULL) {
        chData = tmp->chData;
        if (chData == '(') {
            pushStack(&stack, chData, 0);
        }
        else if (chData == ')') {
            if (isemptyStack(&stack)) {
                destroyStack(&stack);
                return false;
            }
            chpopStack(&stack);
        }
        tmp = tmp->next;
    }

    bool result = isemptyStack(&stack); // �������� ������ ��������� true
    destroyStack(&stack);
    return result;
}

//���ȣ�϶�
int emptyParen(Queue* infQ)
{
    Node* tmp = infQ->front;
    Node* tmp2 = tmp->next;
    char chData, chData2;
    int bo = 1;
    while (tmp2 != NULL)
    {
        chData = tmp->chData;
        chData2 = tmp2->chData;
        if (chData == '(' && chData2 == ')')
        {
            bo = 0;
        }
        tmp = tmp->next;
        tmp2 = tmp2->next;
    }
    return bo;
}

//���׿������϶� ���� ++ --
int unaryopt(Queue* infQ)
{
    Node* tmp = infQ->front;
    int opt = 0;
    int opd = 0;
    double dbData;
    char chData;
    while (tmp != NULL)
    {
        dbData = tmp->dbData;
        chData = tmp->chData;
        if (chData == 0) //�ǿ������϶�
        {
            opd += 1;
        }
        else if (Operator2(chData) && dbData == 0) //�������϶�
        {
            opt += 1;
        }
        else if (chData == '(' || chData == ')')
        {
            opt = 0;
            opd = 0;
            break;
        }

        tmp = tmp->next;
    }

    if (opt > opd)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int unaryopt2(Queue* infQ) //���� ������ ������ ���� �ȵǰ�
{
    Node* tmp = infQ->front;
    Node* tmp2 = tmp->next;
    char chData, chData2;
    int bo = 1;
    while (tmp2 != NULL)
    {
        chData = tmp->chData;
        chData2 = tmp2->chData;
        if (Operator2(chData) && Operator2(chData2))
        {
            bo = 0;
        }

        if (bo == 0 && (chData == '-' && (chData2 == 't' || chData2 == 'n')))
        {
            bo = 1;
        }

        tmp = tmp->next;
        tmp2 = tmp2->next;
    }
    return bo;
}

// �������� �켱����
int priority(char opt)
{
    if (opt == '+' || opt == '-')
        return 1;
    else if (opt == '*' || opt == '/' || opt == '%')
        return 2;
    else if (opt == '^')
        return 3;
    else if (opt == '~' || opt == '=') // ���� -, +
        return 2;
    else if (opt == 't' || opt == 'n')
        return 5;
    else if (opt == '&') // �α׾� ^ �켱�����ڷ�
        return 6;
    else
        return 0;
}

// infix->postfix �Լ�
Queue InToPost(Queue* infixQ) {
    Stack charStack;
    createStack(&charStack);
    Queue postQ;
    createQ(&postQ);
    Node* tmp = infixQ->front;
    char opt;
    double opd;
    char popOpt;
    bool isNotUnary = true; //���� �ƴ�
    while (tmp != NULL)
    {
        opt = tmp->chData;
        opd = tmp->dbData;
        if (opt == 0) //postq�� �ǿ����� �ֱ�
        {
            enQ(&postQ, 0, opd);
            isNotUnary = false;
        }
        else if (opt == '(')  //stack�� ��ȣ�� ������ �ֱ�, �켱�����ڷ� postq�� �ֱ�
        {
            pushStack(&charStack, opt, 0);
            isNotUnary = true;
        }
        else if (opt == ')')
        {
            while (!isemptyStack(&charStack) && topStack(&charStack) != '(') {
                popOpt = chpopStack(&charStack);
                enQ(&postQ, popOpt, 0);
            }

            if (!isemptyStack(&charStack) && topStack(&charStack) == '(') {
                chpopStack(&charStack); // '(' pop
            }

            isNotUnary = false;
        }
        else if (Operator(opt) && opd == 0) //�������� ��
        {     
            if (isNotUnary && (opt == '+' || opt == '-' || opt == '~')) {
                // ���� �������� ��� ��ȣ�� ����
                if (opt == '-' || opt == '~') {
                    opt = '~';  // ���� - -> '~'
                }
                else if (opt == '+')
                {
                    opt = '='; // ���� - -> '='
                }
            }
            else 
            {
                // ������ �ǿ����ڿ����Ƿ� ������ ������ ���� �ƴ�
                isNotUnary = true;
            }

            while (!isemptyStack(&charStack) && priority(opt) <= priority(topStack(&charStack)))
            {
                popOpt = chpopStack(&charStack);
                enQ(&postQ, popOpt, 0);
            }
            pushStack(&charStack, opt, 0);
        }
        tmp = tmp->next;
    }

    while (!isemptyStack(&charStack)) //���� stack�� �ִ°� ���� postQ�� enQ
    {
        popOpt = chpopStack(&charStack);
        enQ(&postQ, popOpt, 0);
    }

    destroyStack(&charStack);
    return postQ;
}
		

// postfix�� ���� �۵�
void calcPostfix(Queue* postQ)
{
    Stack opdStack;
    createStack(&opdStack);
    Node* tmp = postQ->front;
    char opt;
    double opd;
    double opd2, opd1;
    char zero = 't';
    double calc;
    double result;
    int in;

    while (tmp != NULL)
    {
        opt = tmp->chData;
        opd = tmp->dbData;

        if (opt == 0) //�ǿ������϶�

        {
            pushStack(&opdStack, 0, opd);
        }
        else if (Operator(opt) && opd == 0) //�������϶�
        {
            opd2 = dbpopStack(&opdStack);
            opd1 = dbpopStack(&opdStack);
           
            if (opt == '~') {
                pushStack(&opdStack, 0, opd1);
                calc = -opd2;  // ���� ���̳ʽ� ó��
            }
            else if (opt == '=') {
                pushStack(&opdStack, 0, opd1);
                calc = opd2;  // ���� �÷��� ó��
            }
            else if (opt == '&') {
                calc = pow(opd1, opd2); //�α� �� ����
            }
            else{

                switch (opt)
                {
                case '+':
                    calc = opd1 + opd2;
                    break;
                case '-':
                    calc = opd1 - opd2;
                    break;
                case '*':
                    calc = opd1 * opd2;
                    break;
                case '/':
                    if (opd2 == 0) {
                        zero = 'f';
                        printf("Error! : Invalid expression (division by zero)\n\n");
                        destroyStack(&opdStack);
                        return;
                    }
                    calc = opd1 / opd2;
                    break;
                case '%':
                    if (opd2 <= 0) {
                        zero = 'f';
                        printf("Error! : Invalid expression (modulo by zero)\n\n");
                        destroyStack(&opdStack);
                        return;
                    }
                    calc = (int)opd1 % (int)opd2;
                    break;
                case '^':
                    calc = pow(opd1, opd2);
                    break;
                case 't':
                    pushStack(&opdStack, 0, opd1);
                    if (opd2 < 0)
                    {
                        zero = 'f';
                        printf("Error! : Invalid expression (negative log)\n\n");
                        destroyStack(&opdStack);
                        return;
                    }
                    calc = log10(opd2) / log10(2);
                    break;
                case 'n':
                    pushStack(&opdStack, 0, opd1);
                    if (opd2 < 0)
                    {
                        zero = 'f';
                        printf("Error! : Invalid expression (negative log)\n\n");
                        destroyStack(&opdStack);
                        return;
                    }
                    calc = log10(opd2);
                    break;
                }
            }
            pushStack(&opdStack, 0, calc);
            
        }
        tmp = tmp->next;
    }

    result = dbpopStack(&opdStack);

    if (zero == 't') //���� ������ �� result �ȳ�����
    {
        printf("result: %.2lf\n\n", result);
    }

    destroyStack(&opdStack);
}

