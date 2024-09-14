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
        while (1) //�Է¸�
        {
            infix = getchar();
            if (Operand(infix - 48)) //�ǿ����� �϶�
            {
                opd = 1;
                if (infixQ.front == NULL) //ó�� �ǿ����� �Է�
                {
                    if (inpoint == 0) //ù�Է��� �Ҽ����̰� ���ڿö�
                    {
                        firstpoint = 1;
                    }

                    tmp = infix - 48;
                    enQ(&infixQ, 0, tmp);
                }
                else if (infixQ.rear->chData == 0 ) //���ڸ� �̻�(���� �ǿ����� �Է�)
                {
                    if (space > 0) // ���� �����̽� ���� �϶�
                    {
                        enQ(&infixQ, 'e', 0);
                    }
                    else if (inpoint >=0) //�տ� �Ҽ��� ���� ��
                    {
                        inpoint += 1;
                        tmp = (infixQ.rear->dbData) + (infix - 48)/pow(10, inpoint);
                        infixQ.rear->dbData = tmp;
                    }
                    else // ���ڸ��� �̻�
                    {
                        tmp = (infixQ.rear->dbData) * 10 + (infix - 48);
                        infixQ.rear->dbData = tmp;
                    }
                }
                else if (infixQ.front != NULL)//�ǿ����� �Է�
                {
                    tmp = infix - 48;
                    enQ(&infixQ, 0, tmp);
                 
                }
            }
            else if (infix == '(') //���� ��ȣ��
            {
                opd = 0;
                space = 0;
                if ((l >= 1 && o >= 1 && g >= 1) && ((l == o) && (o == g)&&(l==g))) //log�� ��
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
            else if (infix == ')') //�ݴ� ��ȣ��
            {
                opd = 0;
                space = 0;
                if ((l >= 1 && o >= 1 && g >= 1) && ((l == o) && (o == g) && (l == g))) //�α׳�
                {
                    l = 0;
                    o = 0;
                    g = 0;
                }
                enQ(&infixQ, infix, 0);
            }
            else if (infix == '.') //�Ҽ���
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
            else //���� �Էµ� ���
            {
                if (opd == 1 && infix == ' ') //���� ���� �����̽��� ����
                {
                    space += 1;
                }
                else // �ƴϸ� �ʱ�ȭ
                {
                    opd = 0;
                    space = 0;
                }

                //���� ����ó��
                if (inpoint == 0 && infixQ.front == NULL && infix == '\n') //'.'�� ������
                {
                    enQ(&infixQ, infix, 0);
                }
                else if (infix == 'n' || infix == 't'||infix=='&' || infix == '~' || infix == '=')
                {
                    enQ(&infixQ, 'e', 0); //n,t log�����ڰ� �׳� ���� ����
                }
                else if ((l > 0 || o > 0 || g > 0)&& infix == '\n')
                {
                    enQ(&infixQ, infix, 0); //log ���ڸ� ������
                }
                

                if (infixQ.front == NULL && infix == '\n') //enter�϶�
                {
                    printf("[enter]\nClosed");
                    return 0;
                }

                //�����Է�
                if (infix == '\n') 
                {
                    break; //���� '\n' ������ �ȵ�
                }
                else if ((l >= 1 && o >= 1 && g >= 1) && ((l == o) && (o == g) && (l == g)))
                {
                    if (infix == '^') //log�� �����ڿ��� ^ �϶�
                    {
                        enQ(&infixQ, '&', 0);
                    }
                    else if (Operator2(infix))
                    {
                        enQ(&infixQ, infix, 0);
                    }
                    inpoint = -1;
                }
                else if (infix != ' ') //������, ������ ���� �ޱ�
                {
                    inpoint = -1;
                    enQ(&infixQ, infix, 0);
                }
               
            }
        }

        if (matchOper(&infixQ) > 0) //�߸��� �����϶�
        {
            printf("Error! : Invalid character input\n\n");
            destroyQ(&infixQ);
            continue;
        }

        if (!matchParen(&infixQ)) //��ȣ�� �ȸ����� 
        {
            printf("Error! : Mismatched parentheses input\n\n");
            destroyQ(&infixQ);
            continue;
        }

        if (emptyParen(&infixQ) == 0) //���ȣ�϶�
        {
            printf("Error! : IsEmpty parentheses\n\n");
            destroyQ(&infixQ);
            continue;
        }
        
        if (unaryopt(&infixQ) == 0) //���׿������϶�
        {
            printf("Error! : Is Unary Operator1)\n\n");
            destroyQ(&infixQ);
            continue;
        }

        if (unaryopt2(&infixQ) == 0) //���׿����� 1--1
        {
            printf("Error! : Is Unary Operator2)\n\n");
            destroyQ(&infixQ);
            continue;
        }

        if (firstpoint == 1) //ù�Է��� �Ҽ����̰� ���ڿö�
        {
            printf("Error! : Invalid input(decimal point)\n\n");
            destroyQ(&infixQ);
            continue;
        }

        //�Է�, �����˻� ��
        Queue postQ = InToPost(&infixQ);
        destroyQ(&infixQ);
        calcPostfix(&postQ);
        destroyQ(&postQ);

    }

    return 0;
}
