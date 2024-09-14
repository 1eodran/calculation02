#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "calc.h"

// 연산자인지
bool Operator(char inf)
{
    return (inf == '+' || inf == '-' || inf == '*' || inf == '/' || inf == '%' || inf == '^' || inf == 't' || inf == 'n' || inf == '~'|| inf == '='||inf =='&');
}

bool Operator2(char inf)
{
    return (inf == '+' || inf == '-' || inf == '*' || inf == '/' || inf == '%' || inf == '^');
}


// 피연산자
bool Operand(int inf)
{
    return(inf >= 0 && inf <= 9);
}

//연산자,피연산자,괄호를 제외한 문자인지
int matchOper(Queue* infQ) {
    Node* tmp = infQ->front;
    char opt;
    double opd;
    int bo = 0;
    while (tmp != NULL)
    {
        opt = tmp->chData;
        opd = tmp->dbData;
        if (opt == 0 || (Operator(opt) && opd == 0) || opt == '(' || opt == ')') //피연산자, 연산자, 괄호, 빈칸일때 true
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

//괄호쌍검사
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

    bool result = isemptyStack(&stack); // 최종으로 스택이 비어있으면 true
    destroyStack(&stack);
    return result;
}

//빈괄호일때
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

//단항연산자일때 오류 ++ --
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
        if (chData == 0) //피연산자일때
        {
            opd += 1;
        }
        else if (Operator2(chData) && dbData == 0) //연산자일때
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

int unaryopt2(Queue* infQ) //숫자 연산자 연산자 숫자 안되게
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

// 연산자의 우선순위
int priority(char opt)
{
    if (opt == '+' || opt == '-')
        return 1;
    else if (opt == '*' || opt == '/' || opt == '%')
        return 2;
    else if (opt == '^')
        return 3;
    else if (opt == '~' || opt == '=') // 단항 -, +
        return 2;
    else if (opt == 't' || opt == 'n')
        return 5;
    else if (opt == '&') // 로그안 ^ 우선연산자로
        return 6;
    else
        return 0;
}

// infix->postfix 함수
Queue InToPost(Queue* infixQ) {
    Stack charStack;
    createStack(&charStack);
    Queue postQ;
    createQ(&postQ);
    Node* tmp = infixQ->front;
    char opt;
    double opd;
    char popOpt;
    bool isNotUnary = true; //단항 아님
    while (tmp != NULL)
    {
        opt = tmp->chData;
        opd = tmp->dbData;
        if (opt == 0) //postq에 피연산자 넣기
        {
            enQ(&postQ, 0, opd);
            isNotUnary = false;
        }
        else if (opt == '(')  //stack에 괄호와 연산자 넣기, 우선연산자로 postq에 넣기
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
        else if (Operator(opt) && opd == 0) //연산자일 때
        {     
            if (isNotUnary && (opt == '+' || opt == '-' || opt == '~')) {
                // 단항 연산자인 경우 부호를 변경
                if (opt == '-' || opt == '~') {
                    opt = '~';  // 단항 - -> '~'
                }
                else if (opt == '+')
                {
                    opt = '='; // 단항 - -> '='
                }
            }
            else 
            {
                // 이전이 피연산자였으므로 연산자 만나면 단항 아님
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

    while (!isemptyStack(&charStack)) //남은 stack에 있는거 전부 postQ에 enQ
    {
        popOpt = chpopStack(&charStack);
        enQ(&postQ, popOpt, 0);
    }

    destroyStack(&charStack);
    return postQ;
}
		

// postfix로 계산기 작동
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

        if (opt == 0) //피연산자일때

        {
            pushStack(&opdStack, 0, opd);
        }
        else if (Operator(opt) && opd == 0) //연산자일때
        {
            opd2 = dbpopStack(&opdStack);
            opd1 = dbpopStack(&opdStack);
           
            if (opt == '~') {
                pushStack(&opdStack, 0, opd1);
                calc = -opd2;  // 단항 마이너스 처리
            }
            else if (opt == '=') {
                pushStack(&opdStack, 0, opd1);
                calc = opd2;  // 단항 플러스 처리
            }
            else if (opt == '&') {
                calc = pow(opd1, opd2); //로그 안 제곱
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

    if (zero == 't') //오류 구문일 때 result 안나오게
    {
        printf("result: %.2lf\n\n", result);
    }

    destroyStack(&opdStack);
}

