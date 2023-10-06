#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define EQUATION_NUMBER 12  // ������Ըı����ɵ���ʽ�ĸ���������Ŀ�е�����һ�� 
#define COLUMN_NUMBER 6     // �涨ÿ�е������ʽ 

typedef struct Equation_Srtu 
{
	short int left_operand, right_operand;  // ������Ϊ���漰���������Բ���unsigned 
	char operators;
	short int value;
} Equation;

// �������ݽṹ ���а�����������ֵ + ���� + ��� 

typedef Equation Exercise[EQUATION_NUMBER];

void ExerciseGeneratorOfAddition(int min, int max); 	//��װ�ӷ���ʽ ����֮����Ȳ��� 

void ExerciseGeneratorOfSubtracts(int min, int max);	//��װ������ʽ ����֮����Ȳ��� 

short int calculatrAdditionEquation(Equation equation); //����ӷ���� 

short int calculatrSubstactEquation(Equation equation); //���������� 

void generateExerciseOfAdditionEquations(Exercise equationArray, int number, int min, int max); // ���ɼӷ����� + ��װ�Ƿ��Ѿ�������ͬ 

void generateExerciseOfSubtractEquations(Exercise equationArray, int number, int min, int max); // ���ɼ������� + ��װ�Ƿ��Ѿ�������ͬ 

Equation generateAdditionEquation(int min, int max);	//�������ɼӷ���ʽ 

Equation generateSubtractEquation(int min, int max);	//�������ɼ�����ʽ 

// ʹ��min & max ʵ�������еĲ�����ֵ��Χ 

Equation constructAdditionEquation(short int left, short int right, char op); // ��һ��ͨ������ɵ���ʽ�ķ��� 

Equation constructSubtractEquation(short int left, short int right, char op);

Equation generateEquation(int min, int max);   // ��Ϊ������ɲ�ͬ�ļӼ����㣬����Ҫ��һ�����޼Ӽ�������ɼӷ��������ʽ

Equation constructEquation(int min, int max);

int isEqual(Equation eq1, Equation eq2);

int occursIn(Equation anEquation, Exercise equationArray);

void formatAndDisplayExercise(Exercise anExercise, int columns);  //��ʽ����ӡ��ÿ�д�ӡ5�����������ʽ

void asString(Equation eq, char* str);

int main() {
	srand(time(NULL));  // ��ʼ�������������
	ExerciseGeneratorOfAddition(0, 200);
	ExerciseGeneratorOfSubtracts(0, 200);
	ExerciseGeneratorOfAddition(-100, 100);
	ExerciseGeneratorOfSubtracts(-100, 100);
	return 0;
}



//��װ�ӷ���ʽ ����֮����Ȳ��� 
void ExerciseGeneratorOfAddition(int min, int max) {
	Exercise anExercise;
	generateExerciseOfAdditionEquations(anExercise, EQUATION_NUMBER, min, max);
	//��ʽ����ӡ��ÿ�д�ӡ5�����������ʽ
	formatAndDisplayExercise(anExercise, COLUMN_NUMBER);
}
//��װ������ʽ ����֮����Ȳ���
void ExerciseGeneratorOfSubtracts(int min, int max) {
	Exercise anExercise;
	generateExerciseOfSubtractEquations(anExercise, EQUATION_NUMBER, min, max);
	formatAndDisplayExercise(anExercise, COLUMN_NUMBER);
}




//����ӷ���� 
short int calculatrAdditionEquation(Equation equation) {
	return equation.left_operand + equation.right_operand;
}
//���������� 
short int calculatrSubstactEquation(Equation equation) {
	return equation.left_operand - equation.right_operand;
}


// ���ɼӷ����� + ��װ�Ƿ��Ѿ�������ͬ 
void generateExerciseOfAdditionEquations(Exercise equationArray, int number, int min, int max) 
{
	Equation equation;
	for (int i = 0; i < number; i++) 
	{
		do 
		{
			equation = generateAdditionEquation(min, max);
		} 
		while (occursIn(equation, equationArray));
		equationArray[i] = equation;
	}
}

//�������ɼӷ���ʽ 
Equation generateAdditionEquation(int min, int max) 
{
	Equation anEquation;
	// �����������������Ҳ���������0��100֮�䣩
	anEquation.left_operand = rand() % (max - min + 1) + min;
	anEquation.right_operand = rand() % (max - min + 1) + min;
	anEquation.operators = '+';
	anEquation.value = calculatrAdditionEquation(anEquation);  // ������
	return anEquation;
}

// ����һ���ӷ��������ʽ
Equation constructAdditionEquation(short int left, short int right, char op) 
{
	Equation anEquation;
	anEquation.left_operand = left;
	anEquation.right_operand = right;
	anEquation.operators = op;
	if (op == '+') 
	{
		anEquation.value = calculatrAdditionEquation(anEquation);
	} 
	else if (op == '-') 
	{
		anEquation.value = calculatrSubstactEquation(anEquation);
	}
	return anEquation;
}

// �Ƚ�������ʽ�Ƿ����
int isEqual(Equation eq1, Equation eq2) 
{
	return (eq1.left_operand == eq2.left_operand &&
	        eq1.right_operand == eq2.right_operand &&
	        eq1.operators == eq2.operators &&
	        eq1.value == eq2.value
		   );
}

// ���һ����ʽ�Ƿ��Ѿ���������ϰ��
int occursIn(Equation anEquation, Exercise equationArray) 
{
	for (int i = 0; i < EQUATION_NUMBER; i++) 
	{
		if (isEqual(anEquation, equationArray[i])) 
		{
			return 1;
		}
	}
	return 0;
}

// ��ʽ����ӡ��ϰ
void formatAndDisplayExercise(Exercise anExercise, int columns) {
	for (int i = 0; i < EQUATION_NUMBER; i++) 
	{
		char str[100];
		asString(anExercise[i], str);
		printf("%-20s", str);
		if ((i + 1) % columns == 0) 
		{
			printf("\n");
		}
	}
	printf("\n");
	printf("�����£�\n");
	for(int i=0;i< EQUATION_NUMBER;i++){
		printf("%d\t",anExercise[i].value);
	}
	printf("\n\n");
}

// ����ʽת��Ϊ�ַ���
void asString(Equation eq, char* str) {
	snprintf(str, 100, "%d %c %d = ", eq.left_operand, eq.operators, eq.right_operand);
}
// int snprintf(char* str, size_t size, const char* format, ...); 
//�ú����������ǽ�����ָ���� format �ַ�����ʽ��������д�뵽�ַ����� str �У�
//���д�� size-1 ���ַ���Ȼ����ĩβ���һ�����ַ� ��\0����
//�����ʽ������ַ������ȳ����� size-1�����ַ��������ضϣ�����Ȼ��֤�Կ��ַ���β�� 


// ���ɼ�����ʽ��ϰ
void generateExerciseOfSubtractEquations(Exercise equationArray, int number, int min, int max) 
{
	Equation equation;
	for (int i = 0; i < number; i++) 
	{
		do 
		{
			equation = generateSubtractEquation(min, max);
		} 
		while (occursIn(equation, equationArray));
		equationArray[i] = equation;
	}
}

// ���ɼ�����ʽ
Equation generateSubtractEquation(int min, int max) {
	short int left = rand() % (max - min + 1) + min;
	short int right = rand() % (max - min + 1) + min;
	char op = '-';
	Equation anEquation = constructSubtractEquation(left, right, op);
	return anEquation;
}

// ����һ��������ʽ
Equation constructSubtractEquation(short int left, short int right, char op) {
	Equation anEquation;
	anEquation.left_operand = left;
	anEquation.right_operand = right;
	anEquation.operators = op;
	anEquation.value = calculatrSubstactEquation(anEquation);
	return anEquation;
}

// ���ɼӷ��������ʽ
Equation generateEquation(int min, int max) 
{
	int choice = rand() % 2; // ���ѡ���Ǽӷ����Ǽ���
	if (choice == 0) 
	{
		return generateSubtractEquation(min, max);
	} 
	else 
	{
		return generateAdditionEquation(min, max);
	}
}

// ����һ���ӷ��������ʽ
Equation constructEquation(int min, int max) {
	Equation anEquation;
	int choice = rand() % 2; // ���ѡ���Ǽӷ����Ǽ���
	if (choice == 0) 
	{
		short int left = rand() % 201;  // [0, 200]
		short int right = rand() % 201; // [0, 200]
		char op = '-';
		anEquation = constructSubtractEquation(left, right, op);
	} 
	else 
	{
		anEquation = generateAdditionEquation(min, max);
	}
	return anEquation;
}                                                                                                                                                                                                                                                                                                                                                                                                                                               
