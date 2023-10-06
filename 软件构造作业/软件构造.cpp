#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define EQUATION_NUMBER 12  // 这里可以改变生成的算式的个数，即题目中的任意一个 
#define COLUMN_NUMBER 6     // 规定每行的输出格式 

typedef struct Equation_Srtu 
{
	short int left_operand, right_operand;  // 这里因为不涉及正负，所以不用unsigned 
	char operators;
	short int value;
} Equation;

// 定义数据结构 其中包括两个左右值 + 符号 + 结果 

typedef Equation Exercise[EQUATION_NUMBER];

void ExerciseGeneratorOfAddition(int min, int max); 	//包装加法算式 ，加之输出等操作 

void ExerciseGeneratorOfSubtracts(int min, int max);	//包装减法算式 ，加之输出等操作 

short int calculatrAdditionEquation(Equation equation); //计算加法结果 

short int calculatrSubstactEquation(Equation equation); //计算减法结果 

void generateExerciseOfAdditionEquations(Exercise equationArray, int number, int min, int max); // 生成加法计算 + 包装是否已经存在雷同 

void generateExerciseOfSubtractEquations(Exercise equationArray, int number, int min, int max); // 生成减法计算 + 包装是否已经存在雷同 

Equation generateAdditionEquation(int min, int max);	//具体生成加法算式 

Equation generateSubtractEquation(int min, int max);	//具体生成减法算式 

// 使用min & max 实现题设中的不限数值范围 

Equation constructAdditionEquation(short int left, short int right, char op); // 另一种通过建造成的算式的方法 

Equation constructSubtractEquation(short int left, short int right, char op);

Equation generateEquation(int min, int max);   // 因为随机生成不同的加减计算，所以要有一个包罗加减随机生成加法或减法算式

Equation constructEquation(int min, int max);

int isEqual(Equation eq1, Equation eq2);

int occursIn(Equation anEquation, Exercise equationArray);

void formatAndDisplayExercise(Exercise anExercise, int columns);  //格式化打印：每行打印5道含结果的算式

void asString(Equation eq, char* str);

int main() {
	srand(time(NULL));  // 初始化随机数生成器
	ExerciseGeneratorOfAddition(0, 200);
	ExerciseGeneratorOfSubtracts(0, 200);
	ExerciseGeneratorOfAddition(-100, 100);
	ExerciseGeneratorOfSubtracts(-100, 100);
	return 0;
}



//包装加法算式 ，加之输出等操作 
void ExerciseGeneratorOfAddition(int min, int max) {
	Exercise anExercise;
	generateExerciseOfAdditionEquations(anExercise, EQUATION_NUMBER, min, max);
	//格式化打印：每行打印5道含结果的算式
	formatAndDisplayExercise(anExercise, COLUMN_NUMBER);
}
//包装减法算式 ，加之输出等操作
void ExerciseGeneratorOfSubtracts(int min, int max) {
	Exercise anExercise;
	generateExerciseOfSubtractEquations(anExercise, EQUATION_NUMBER, min, max);
	formatAndDisplayExercise(anExercise, COLUMN_NUMBER);
}




//计算加法结果 
short int calculatrAdditionEquation(Equation equation) {
	return equation.left_operand + equation.right_operand;
}
//计算减法结果 
short int calculatrSubstactEquation(Equation equation) {
	return equation.left_operand - equation.right_operand;
}


// 生成加法计算 + 包装是否已经存在雷同 
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

//具体生成加法算式 
Equation generateAdditionEquation(int min, int max) 
{
	Equation anEquation;
	// 随机生成左操作数和右操作数（在0到100之间）
	anEquation.left_operand = rand() % (max - min + 1) + min;
	anEquation.right_operand = rand() % (max - min + 1) + min;
	anEquation.operators = '+';
	anEquation.value = calculatrAdditionEquation(anEquation);  // 计算结果
	return anEquation;
}

// 构造一个加法或减法算式
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

// 比较两个算式是否相等
int isEqual(Equation eq1, Equation eq2) 
{
	return (eq1.left_operand == eq2.left_operand &&
	        eq1.right_operand == eq2.right_operand &&
	        eq1.operators == eq2.operators &&
	        eq1.value == eq2.value
		   );
}

// 检查一个算式是否已经存在于练习中
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

// 格式化打印练习
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
	printf("答案如下：\n");
	for(int i=0;i< EQUATION_NUMBER;i++){
		printf("%d\t",anExercise[i].value);
	}
	printf("\n\n");
}

// 将算式转换为字符串
void asString(Equation eq, char* str) {
	snprintf(str, 100, "%d %c %d = ", eq.left_operand, eq.operators, eq.right_operand);
}
// int snprintf(char* str, size_t size, const char* format, ...); 
//该函数的作用是将根据指定的 format 字符串格式化的数据写入到字符数组 str 中，
//最多写入 size-1 个字符，然后在末尾添加一个空字符 ‘\0’。
//如果格式化后的字符串长度超过了 size-1，则字符串将被截断，但仍然保证以空字符结尾。 


// 生成减法算式练习
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

// 生成减法算式
Equation generateSubtractEquation(int min, int max) {
	short int left = rand() % (max - min + 1) + min;
	short int right = rand() % (max - min + 1) + min;
	char op = '-';
	Equation anEquation = constructSubtractEquation(left, right, op);
	return anEquation;
}

// 构造一个减法算式
Equation constructSubtractEquation(short int left, short int right, char op) {
	Equation anEquation;
	anEquation.left_operand = left;
	anEquation.right_operand = right;
	anEquation.operators = op;
	anEquation.value = calculatrSubstactEquation(anEquation);
	return anEquation;
}

// 生成加法或减法算式
Equation generateEquation(int min, int max) 
{
	int choice = rand() % 2; // 随机选择是加法还是减法
	if (choice == 0) 
	{
		return generateSubtractEquation(min, max);
	} 
	else 
	{
		return generateAdditionEquation(min, max);
	}
}

// 构造一个加法或减法算式
Equation constructEquation(int min, int max) {
	Equation anEquation;
	int choice = rand() % 2; // 随机选择是加法还是减法
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
