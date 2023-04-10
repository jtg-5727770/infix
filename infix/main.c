#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 100
typedef char element;

typedef struct {
	element data[MAX_STACK_SIZE];
	int top;
} StackType;
// ���� �ʱ�ȭ �Լ�
void init_stack(StackType* s)
{
	s->top = -1;
}
// ���� ���� ���� �Լ�
int is_empty(StackType* s)
{
	return (s->top == -1);
}
// ��ȭ ���� ���� �Լ�
int is_full(StackType* s)
{
	return (s->top == (MAX_STACK_SIZE - 1));
}
StackType* s;
// �����Լ�
void push(StackType* s, element item)
{
	if (is_full(s)) {
		fprintf(stderr, "���� ��ȭ ����\n");
		return;
	}
	else s->data[++(s->top)] = item;
}
// �����Լ�
element pop(StackType* s)
{
	if (is_empty(s)) {
		fprintf(stderr, "���� ���� ����\n");
		return 0;
	}
	else return s->data[(s->top)--];
}
//��ũ�Լ�
element peek(StackType* s) {
	if (is_empty(s)) {
		fprintf(stderr, "���� ���� ����\n");
		return 0;
	}
	else return s->data[(s->top)];
}
//������ ���
int eval(char *exp)
{
	int op1, op2, value, i = 0;
	int len = strlen(exp);
	char ch;
	StackType s;
	init_stack(&s);
	for (i = 0; i < len; i++) {

		ch = exp[i];    


		if (ch != '+' && ch != '-' && ch != '*' && ch != '/') { 
				value = ch - '0';
				push(&s, value);
		}

		else
		{
			op2 = pop(&s);
			op1 = pop(&s);
			switch (ch) {
			case '+': push(&s, op1 + op2); break;
			case '-': push(&s, op1 - op2); break;
			case '*': push(&s, op1 * op2); break;
			case '/': push(&s, op1 / op2); break;
			}
		}
	}
	return pop(&s);
}
// �������� �켱������ ��ȯ�Ѵ�.
int prec(char op)
{
	switch (op) {
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	}
	return -1;
}
// �Է¹��� ������ ���� �˻� 
int is_valid_infix(char exp[]) {
	StackType s;
	init_stack(&s); // ���� �ʱ�ȭ
	int len = strlen(exp);

	int count = 0;
	// error 1 : ��ȣ�� ¦�� ���� ������ ����ó��
	for (int i = 0; i < len; i++) {
		if (exp[i] == '(') {
			count++;
		}
		else if (exp[i] == ')') {
			count--;
		}
	}
	if (count > 0) {
		printf("�ݴ� ��ȣ�� �����մϴ�.\n");
		exit(1);
	}
	else if (count < 0) {
		printf("���� ��ȣ�� �����մϴ�.\n");
		exit(1);
	}
	// error 2 : ���� ���� ����
	for (int i = 0; i < len; i++) {
		if (exp[i] == '(' || exp[i] == ')') {
			continue;
		}
		else if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/') {
			continue;
		}
		else if ('0' <= exp[i] && exp[i] <= '9') {
			continue;
		}
		else {
			printf("�������� �ƴ� ���ڴ� ����Ҽ� �����ϴ�.\n");
			exit(1);
		}
	}
	// error 3 : ���ڸ� �̻� ���� ����Ұ��
	int count_len = 0;
	int max_len = 0;
	for (int i = 0; i < len; i++) {
		if ('0' <= exp[i] && exp[i] <= '9') {
			count_len++;
			if (count_len >= max_len) {
				max_len = count_len;
			}
		}
		else {
			count_len = 0;
		}
	}
	if (max_len >= 2) {
		printf("���ڸ����� �Է����ּ���\n");
		exit(1);
	}
	// error 4 : �����ڰ� �ǿ����ں��� ���ų� �������
	int operand_count = 0;
	int operator_count = 0;
	for (int i = 0; i < len; i++) {
		if ('0' <= exp[i] && exp[i] <= '9') {
			operand_count++;
		}
		else if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/') {
			operator_count++;
		}
	}
	if (operator_count >= operand_count) {
		printf("�����ڰ� �ǿ����ں��� ���ų� �����ϴ�.\n");
		exit(1);
	}

}
// ���� ǥ�� ���� -> ���� ǥ�� ����
void infix_to_postfix(char exp[], char exp2[])
{
	if (!is_valid_infix(exp)) {
		return;
	}
		int i, j = 0;
		char ch, ch2, top_op;
		int len = strlen(exp);
		StackType s;
		init_stack(&s); // ���� �ʱ�ȭ
		for (i = 0; i < len; i++) {
			ch = exp[i];
			ch2 = exp[i + 1]; //ch�� ���� ���� ����
			switch (ch) {
				// �������� ���
			case '+': case '-': case '*': case '/':
				//���ÿ� �ִ� �������� �켱������ �� ũ�ų� ������ ���
				while (!is_empty(&s) && (prec(ch) <= prec(peek(&s)))) {
					exp2[j++] = pop(&s);

				}
				push(&s, ch);
				break;

				// ��ȣ�� ���
			case '(':           //���� ��ȣ
				push(&s, ch);
				break;
			case ')':           //������ ��ȣ
				top_op = pop(&s);

				while (top_op != '(') {          //���� ��ȣ�� ���� �� ���� ���
					exp2[j++] = top_op;


					top_op = pop(&s);
				}
				break;

				// �ǿ������� ���
			default:
				exp2[j++] = ch;
				if (ch2 != '+' && ch2 != '-' && ch2 != '*' && ch2 != '/' && ch2 != ' ' && ch2 != '\0' &&
					ch2 != '(' && ch2 != ')')
				{
					exp2[j++] = ch2; //ch2�� �񿬻��� �� ch�� ���� �ڸ��� ���� ����
					i++; //��� �Լ������� ���������� �ߺ��� ���ϱ� ���� i���� ������Ų��.
				}
				break;
			}
		}

		while (!is_empty(&s)) {
			exp2[j++] = pop(&s);
			exp2[j] = NULL;
		}
}

int main(void)
{
	char s[MAX_STACK_SIZE];
	char exp[MAX_STACK_SIZE];
	int result;
	printf("����ǥ���: ");
	scanf_s("%s", s, MAX_STACK_SIZE);
	infix_to_postfix(s,exp);
	printf("����ǥ���: %s\n", exp);
	result = eval(exp);
	printf("�����: %d\n", result);
	system("pause");
	return 0;
}