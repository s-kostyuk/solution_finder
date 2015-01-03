#include <stdio.h>

void ignoreNewline() {
	while(getchar() != '\n');
}

void getReversedBinary(unsigned int &result) {
	result = 0;
	char temp;
	int position = 0;

	do {
		temp = getchar();
		if ((temp == '\n') && (position == 0)) {
			temp = 'f';
			continue;
		}

		switch(temp) {
		case '0':
			result = result | (0 << position);
			break;

		case '1':
			result = result | (1 << position);
			break;

		default:
			continue;
		}
		position++;
	}
	while (temp!='\n');
}

void printBinaryReverse(unsigned int result, int size) {
	bool temp;
	printf("\n");
	for (int i = 0; i < size; i++) {
		temp = result & (1 << i);
		printf("%d", temp);
	}
}

unsigned int vectorXor(unsigned int &vector1, unsigned int &vector2) {
	return (vector1 ^ vector2);
}

bool bitDisjunction (unsigned int vector, int vectorSize) {
	bool temp = vector & 1;
	for (int i = 1; i < vectorSize; i++) {
		temp = (vector & (1 << i)) | temp;
	}
	return temp;
}

void slc(unsigned int &vector, int vectorSize) {
	int oneCounter = 0;
	for (int i = 0; i < vectorSize; i++) {
		if (vector & (1 << i)) {
			vector = vector | (0 << i);
			vector = vector | (1 << oneCounter);
		}
	}
}

void findDiffs(unsigned int *vectors, int nOfVectors, unsigned int *result) {
	unsigned int disjunction;
	disjunction = vectors[0];
	for (int i = 1; i < nOfVectors; i++) {
		disjunction = disjunction & vectors[i];
	}
	for (int i = 0; i < nOfVectors; i++) {
		result[i] = disjunction ^ vectors[i];
	}
}

void findBestSolution(unsigned int *vectors, int nOfVectors, int vectorSize, unsigned int metric) {
	unsigned int *tempVectors = new unsigned int[nOfVectors];
	for (int i = 0; i < nOfVectors; i++) {
		tempVectors[i] = vectorXor(vectors[i], metric);
	}
	for (int i = 0; i < nOfVectors; i++) {
		slc(tempVectors[i], vectorSize);
	}
	findDiffs(tempVectors, nOfVectors, tempVectors);
	for (int i = 0; i < nOfVectors; i++) {
		if (!(bitDisjunction(tempVectors[i], vectorSize))) {
			printf("Vector number %d is the best solution", i);
		}
	}
	delete[] tempVectors;
}

int main() {
	unsigned int metric;
	int nOfVectors, vectorSize;
	printf("What is the max size of the vector? ");
	scanf("%d", &vectorSize);
	printf("How many source vectors do you want to input? ");
	scanf("%d", &nOfVectors);
	unsigned int *vectors = new unsigned int[nOfVectors];
	printf("Use [Ctrl+Z] (for Windows) or [Ctrl+D] (for UNIX-like OS's) to finish input\n");
	for(int i = 0; i < nOfVectors; i++) {
		printf("Your vector number %d is: ", i);
		getReversedBinary(vectors[i]);
	}
	printf("Your metric is: ");
	getReversedBinary(metric);
	findBestSolution(vectors, nOfVectors, vectorSize, metric);
	delete[] vectors;
	return 0;
}
