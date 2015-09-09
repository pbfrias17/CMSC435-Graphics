
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <typeinfo>
using namespace std;

int main() {
	FILE *fp;
	char line[256];
	vector<float> b;
	vector<float> v;
	vector<float> from;
	vector<float> at;
	vector<float> up;
	vector<float> f;
	vector<vector<float> > p;
	float angle;
	float hither;
	float res_x;
	float res_y;
	fp = fopen("tetra-3.nff" , "r");
	if(fp == NULL)
		cout << "File not found" << endl;
	else
		cout << "File opened for reading" << endl;
	cout << "Parsing .nff file..." << endl << endl;

	while(fgets(line, sizeof(line), fp)) {
		printf("%s", line);
		char* words = strtok(line, " ");
		while(words != NULL) {
			printf("%s\n", words);

			words = strtok(NULL, " ");
		}
		/*
		if(line[0] == 'b')
			printf("%s\n", "background");
		else if(line[0] == 'v')
			printf("%s\n", "viewpoint");
		else if(line[0] == 'f')
			printf("%s\n", "fill");
		else if(line[0] == 'p')
			printf("%s\n", "polygon");
		else
			printf("%s\n", "-");
		*/
	}
    fclose(fp);
	return 0;
}




