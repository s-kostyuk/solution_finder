#include <stdio.h>

void getReversedBinary(unsigned int &result, int size) {
	//Считывает двочиное число с консоли и записывает его в int. И все бы хорошо. Только число получается задом наперед.
	//Хорошо, что для данной задачи порядок ввода не критичен.

	//Обнуляем переменную, в которую записываем полученное число
	result = 0;
	//В переменной ниже будем хранить один введенный символ
	char temp;

	/*
	//Начнем с начала
	int position = 0;

	//Этот цикл работает до тех пор, пока не получим на входе символ перехода строки
	//Почему закомментировал? Пользователь в текущем состоянии кода может ввести больше символов,
	//чем сказал в начале работы программы (больше, чем записано в vectorSize).
	//И никто ему ничего не скажет. Более того, функции ниже обработают только vectorSize битов в
	//числе и к каким это результатам приведет - неизвестно.
	do {
		//Считали символ
		temp = getchar();
		//Предохраняемся от завершения цикла сразу после старта (если в буфере остался символ перевода строки)
		if ((temp == '\n') && (position == 0)) {
			temp = 'f';
			continue;
		}
		//Заполнение числа
		switch(temp) {
		case '0':
			//Это можно опустить, на самом деле. Эффекта на обнуленном result - ноль
			result = result | (0 << position);
			break;

		case '1':
			//Берем единицу, смещаем ее на position бит, "складываем" с result и записываем результат обратно в result
			result = result | (1 << position);
			break;

		default:
			//Некорректные символы просто игнорируем
			continue;
		}
		position++;
	}
	while (temp!='\n');
	*/

	//Этот цикл считает size символов и запишет size битов в наше число
	for (int i = 0; i < size; ) {
		//Считали символ
		temp = getchar();
		//Предохраняемся от завершения цикла сразу после старта (если в буфере остался символ перевода строки)
		if ((temp == '\n') && (i == 0)) {
			temp = 'f';
			continue;
		}
		//Заполнение числа
		switch(temp) {
		case '0':
			//Это можно опустить, на самом деле. Эффекта на обнуленном result - ноль
			result &= ~(1 << i);
			break;

		case '1':
			//Берем единицу, смещаем ее на position бит, "складываем" с result и записываем результат обратно в result
			result = result | (1 << i);
			break;

		default:
			//Некорректные символы просто игнорируем
			continue;
		}
		i++;
	}
}

void printBinaryReverse(unsigned int result, int size) {
	//Вывод двоичного числа на экран. Тоже в обратном порядке
	bool temp;
	printf("\n");
	for (int i = 0; i < size; i++) {
		//Ах да. Это - работа с i-м битом числа. Она встречается в коде часто.
		temp = result & (1 << i);
		printf("%d", temp);
	}
}

unsigned int vectorXor(unsigned int &vector1, unsigned int &vector2) {
	//Просто xor
	return (vector1 ^ vector2);
}

bool bitDisjunction (unsigned int vector, int vectorSize) {
	//"Логическое сложение" всех битов вектора
	bool temp = vector & 1;
	for (int i = 1; i < vectorSize; i++) {
		temp = (vector & (1 << i)) | temp;
	}
	return temp;
}

void slc(unsigned int &vector, int vectorSize) {
	//Уплотнение

	//Счетчик единиц
	int oneCounter = 0;
	//Временная переменная для записи "сдвинутого" числа
	unsigned int temp = 0;
	for (int i = 0; i < vectorSize; i++) {
		//Если встречаем единицу:
		if (vector & (1 << i)) {
			//Записываем единицу в temp сразу после предыдущей единицы
			temp = temp | (1 << oneCounter);
			oneCounter++;
		}
	}
	//Записываем "сдвинутое" число обратно в vector
	vector = temp;
}

void findDiffs(unsigned int *vectors, int nOfVectors, unsigned int *result) {
	//Находим дизъюнкцию всех векторов (в статье - a ∧ b)
	unsigned int disjunction;
	disjunction = vectors[0];
	for (int i = 1; i < nOfVectors; i++) {
		disjunction = disjunction & vectors[i];
	}
	//Находим q для всех векторов (в статье - ((a ∧ b) ⊕ b))
	for (int i = 0; i < nOfVectors; i++) {
		result[i] = disjunction ^ vectors[i];
	}
}

void findBestSolution(unsigned int *vectors, int nOfVectors, int vectorSize, unsigned int metric) {
	//Массив временных векторов, в которые мы будем записывать результаты вычислений
	unsigned int *tempVectors = new unsigned int[nOfVectors];
	//xor (нахождение μ) для каждого из векторов
	for (int i = 0; i < nOfVectors; i++) {
		tempVectors[i] = vectorXor(vectors[i], metric);
	}
	//Уплотнялка (slc)
	for (int i = 0; i < nOfVectors; i++) {
		slc(tempVectors[i], vectorSize);
	}
	//Нахождение q (или ((a ∧ b) ⊕ a))
	findDiffs(tempVectors, nOfVectors, tempVectors);
	//Арифметическое сложение битов числа (нахождение Q), если результат сложения равен нулю - мы нашли лучшее решение!
	for (int i = 0; i < nOfVectors; i++) {
		if (!(bitDisjunction(tempVectors[i], vectorSize))) {
			printf("Vector number %d is the best solution \n", i);
		}
	}
	//Убираем за собой
	delete[] tempVectors;
}

int main() {
	//Используем целые беззнаковые переменные для хранения наших "векторов" (далее - вектор, без кавычек)
	//Вектор метрики m
	unsigned int metric;
	//Количество входных векторов и размер одного вектора
	int nOfVectors, vectorSize;
	//Общаемся с пользователем
	printf("What is the max size of the vector? ");
	scanf("%d", &vectorSize);
	//Кстати, размер самого длинного вектора можно вычислять функцией (считать позицию последней единицы в векторе +1), но это уже реализую в новых версиях
	printf("How many source vectors do you want to input? ");
	scanf("%d", &nOfVectors);
	//Создаем массив, в котором будем хранить исходные вектора
	unsigned int *vectors = new unsigned int[nOfVectors];
	//Пользователь вводит все нужные ему вектора (A, B, C и т.д.)
	for(int i = 0; i < nOfVectors; i++) {
		printf("Your vector number %d is: ", i);
		getReversedBinary(vectors[i], vectorSize);
	}
	//Пользователь вводит вектор метрики (m)
	printf("Your metric is: ");
	getReversedBinary(metric, vectorSize);
	//В этой функции вся магия
	findBestSolution(vectors, nOfVectors, vectorSize, metric);
	//Убираем за собой (удаляем массив исходных векторов)
	delete[] vectors;
	return 0;
}
