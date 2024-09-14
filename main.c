#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "calc.h"

int main() {
    char infix;
    Queue infixQ;
    createQ(&infixQ);
    double tmp;
    int inpoint;
    int firstpoint;
    int l, o, g;
    int opd;
    int space;
 
    while (1)
    {
        inpoint = -1;
        firstpoint = 0;
        l = 0;
        o = 0;
        g = 0;
        opd = 0;
        space = 0;
        printf(">>");
        while (1) //입력만
        {
            infix = getchar();
            if (Operand(infix - 48)) //피연산자 일때
            {
                opd = 1;
                if (infixQ.front == NULL) //처음 피연산자 입력
                {
                    if (inpoint == 0) //첫입력이 소숫점이고 숫자올때
                    {
                        firstpoint = 1;
                    }

                    tmp = infix - 48;
                    enQ(&infixQ, 0, tmp);
                }
                else if (infixQ.rear->chData == 0 ) //두자리 이상(연속 피연산자 입력)
                {
                    if (space > 0) // 숫자 스페이스 숫자 일때
                    {
                        enQ(&infixQ, 'e', 0);
                    }
                    else if (inpoint >=0) //앞에 소수점 있을 때
                    {
                        inpoint += 1;
                        tmp = (infixQ.rear->dbData) + (infix - 48)/pow(10, inpoint);
                        infixQ.rear->dbData = tmp;
                    }
                    else // 두자리수 이상
                    {
                        tmp = (infixQ.rear->dbData) * 10 + (infix - 48);
                        infixQ.rear->dbData = tmp;
                    }
                }
                else if (infixQ.front != NULL)//피연산자 입력
                {
                    tmp = infix - 48;
                    enQ(&infixQ, 0, tmp);
                 
                }
            }
            else if (infix == '(') //여는 괄호쌍
            {
                opd = 0;
                space = 0;
                if ((l >= 1 && o >= 1 && g >= 1) && ((l == o) && (o == g)&&(l==g))) //log일 때
                {
                    
                    if (infixQ.rear->dbData == 2)
                    {
                        infixQ.rear->chData = 't';
                        infixQ.rear->dbData = 0;
                    }
                    else if (infixQ.rear->dbData == 10)
                    {
                        infixQ.rear->chData = 'n';
                        infixQ.rear->dbData = 0;
                    }
                }
                
                enQ(&infixQ, infix, 0);
            
            }
            else if (infix == ')') //닫는 괄호쌍
            {
                opd = 0;
                space = 0;
                if ((l >= 1 && o >= 1 && g >= 1) && ((l == o) && (o == g) && (l == g))) //로그끝
                {
                    l = 0;
                    o = 0;
                    g = 0;
                }
                enQ(&infixQ, infix, 0);
            }
            else if (infix == '.') //소수점
            {
                opd = 0;
                space = 0;
                inpoint = 0;
            }
            else if (infix == 'l')
            {
                opd = 0;
                space = 0;
                l += 1;
            }
            else if (infix == 'o')
            {
                opd = 0;
                space = 0;
                o += 1;
            }
            else if (infix == 'g')
            {
                opd = 0;
                space = 0;
                g += 1;
            }
            else //문자 입력될 경우
            {
                if (opd == 1 && infix == ' ') //숫자 다음 스페이스면 증가
                {
                    space += 1;
                }
                else // 아니면 초기화
                {
                    opd = 0;
                    space = 0;
                }

                //문자 오류처리
                if (inpoint == 0 && infixQ.front == NULL && infix == '\n') //'.'만 받을때
                {
                    enQ(&infixQ, infix, 0);
                }
                else if (infix == 'n' || infix == 't'||infix=='&' || infix == '~' || infix == '=')
                {
                    enQ(&infixQ, 'e', 0); //n,t log연산자가 그냥 들어가면 오류
                }
                else if ((l > 0 || o > 0 || g > 0)&& infix == '\n')
                {
                    enQ(&infixQ, infix, 0); //log 문자만 받을때
                }
                

                if (infixQ.front == NULL && infix == '\n') //enter일때
                {
                    printf("[enter]\nClosed");
                    return 0;
                }

                //문자입력
                if (infix == '\n') 
                {
                    break; //끝에 '\n' 받으면 안됨
                }
                else if ((l >= 1 && o >= 1 && g >= 1) && ((l == o) && (o == g) && (l == g)))
                {
                    if (infix == '^') //log안 연산자에서 ^ 일때
                    {
                        enQ(&infixQ, '&', 0);
                    }
                    else if (Operator2(infix))
                    {
                        enQ(&infixQ, infix, 0);
                    }
                    inpoint = -1;
                }
                else if (infix != ' ') //연산자, 오류날 문자 받기
                {
                    inpoint = -1;
                    enQ(&infixQ, infix, 0);
                }
               
            }
        }

        if (matchOper(&infixQ) > 0) //잘못된 문자일때
        {
            printf("Error! : Invalid character input\n\n");
            destroyQ(&infixQ);
            continue;
        }

        if (!matchParen(&infixQ)) //괄호쌍 안맞을때 
        {
            printf("Error! : Mismatched parentheses input\n\n");
            destroyQ(&infixQ);
            continue;
        }

        if (emptyParen(&infixQ) == 0) //빈괄호일때
        {
            printf("Error! : IsEmpty parentheses\n\n");
            destroyQ(&infixQ);
            continue;
        }
        
        if (unaryopt(&infixQ) == 0) //단항연산자일때
        {
            printf("Error! : Is Unary Operator1)\n\n");
            destroyQ(&infixQ);
            continue;
        }

        if (unaryopt2(&infixQ) == 0) //단항연산자 1--1
        {
            printf("Error! : Is Unary Operator2)\n\n");
            destroyQ(&infixQ);
            continue;
        }

        if (firstpoint == 1) //첫입력이 소숫점이고 숫자올때
        {
            printf("Error! : Invalid input(decimal point)\n\n");
            destroyQ(&infixQ);
            continue;
        }

        //입력, 오류검사 끝
        Queue postQ = InToPost(&infixQ);
        destroyQ(&infixQ);
        calcPostfix(&postQ);
        destroyQ(&postQ);

    }

    return 0;
}
