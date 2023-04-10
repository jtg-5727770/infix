#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 100
typedef char element;

typedef struct {
	element data[MAX_STACK_SIZE];
	int top;
} StackType;
// 스택 초기화 함수
void init_stack(StackType* s)
{
	s->top = -1;
}
// 공백 상태 검출 함수
int is_empty(StackType* s)
{
	return (s->top == -1);
}
// 포화 상태 검출 함수
int is_full(StackType* s)
{
	return (s->top == (MAX_STACK_SIZE - 1));
}
StackType* s;
// 삽입함수
void push(StackType* s, element item)
{
	if (is_full(s)) {
		fprintf(stderr, "스택 포화 에러\n");
		return;
	}
	else s->data[++(s->top)] = item;
}
// 삭제함수
element pop(StackType* s)
{
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		return 0;
	}
	else return s->data[(s->top)--];
}
//피크함수
element peek(StackType* s) {
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		return 0;
	}
	else return s->data[(s->top)];
}
//후위식 계산
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
// 연산자의 우선순위를 반환한다.
int prec(char op)
{
	switch (op) {
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	}
	return -1;
}
// 입력받은 중위식 오류 검사 
int is_valid_infix(char exp[]) {
	StackType s;
	init_stack(&s); // 스택 초기화
	int len = strlen(exp);

	int count = 0;
	// error 1 : 괄호의 짝이 맞지 않으면 예외처리
	for (int i = 0; i < len; i++) {
		if (exp[i] == '(') {
			count++;
		}
		else if (exp[i] == ')') {
			count--;
		}
	}
	if (count > 0) {
		printf("닫는 괄호가 부족합니다.\n");
		exit(1);
	}
	else if (count < 0) {
		printf("여는 괄호가 부족합니다.\n");
		exit(1);
	}
	// error 2 : 예외 문자 포함
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
			printf("중위식이 아닌 문자는 사용할수 없습니다.\n");
			exit(1);
		}
	}
	// error 3 : 두자리 이상 수를 사용할경우
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
		printf("한자리수만 입력해주세요\n");
		exit(1);
	}
	// error 4 : 연산자가 피연산자보다 많거나 같을경우
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
		printf("연산자가 피연산자보다 많거나 같습니다.\n");
		exit(1);
	}

}
// 중위 표기 수식 -> 후위 표기 수식
void infix_to_postfix(char exp[], char exp2[])
{
	if (!is_valid_infix(exp)) {
		return;
	}
		int i, j = 0;
		char ch, ch2, top_op;
		int len = strlen(exp);
		StackType s;
		init_stack(&s); // 스택 초기화
		for (i = 0; i < len; i++) {
			ch = exp[i];
			ch2 = exp[i + 1]; //ch의 다음 값을 저장
			switch (ch) {
				// 연산자일 경우
			case '+': case '-': case '*': case '/':
				//스택에 있는 연산자의 우선순위가 더 크거나 같으면 출력
				while (!is_empty(&s) && (prec(ch) <= prec(peek(&s)))) {
					exp2[j++] = pop(&s);

				}
				push(&s, ch);
				break;

				// 괄호의 경우
			case '(':           //왼쪽 괄호
				push(&s, ch);
				break;
			case ')':           //오른쪽 괄호
				top_op = pop(&s);

				while (top_op != '(') {          //왼쪽 괄호를 만날 때 까지 출력
					exp2[j++] = top_op;


					top_op = pop(&s);
				}
				break;

				// 피연산자일 경우
			default:
				exp2[j++] = ch;
				if (ch2 != '+' && ch2 != '-' && ch2 != '*' && ch2 != '/' && ch2 != ' ' && ch2 != '\0' &&
					ch2 != '(' && ch2 != ')')
				{
					exp2[j++] = ch2; //ch2가 비연산자 즉 ch와 같은 자리의 수면 대입
					i++; //계산 함수에서와 마찬가지로 중복을 피하기 위해 i값을 증가시킨다.
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
	printf("중위표기식: ");
	scanf_s("%s", s, MAX_STACK_SIZE);
	infix_to_postfix(s,exp);
	printf("후위표기식: %s\n", exp);
	result = eval(exp);
	printf("결과값: %d\n", result);
	system("pause");
	return 0;
}