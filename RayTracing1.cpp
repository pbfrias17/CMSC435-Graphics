
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <typeinfo>
#include <math.h>
#define PI 3.14159265
using namespace std;

void printv(vector<float> vf) {
	for(vector<float>::const_iterator i = vf.begin(); i != vf.end(); ++i)
		cout << *i << ' ';
	cout << endl;
}


vector<float> wp(float i, float j, float r, float res) {
	//r = b = -l = -t
	vector<float> world;
	world.push_back(-r+(2*r)*(i+.5)/res);
	world.push_back(-r+(2*r)*(j+.5)/res);
	return world;
}


bool ray_tri_intersect(vector<float> eye, vector<float> dir, vector<vector<float> > vertices) {
	cout << "Checking intersection on polygon " << endl;
	for(int i = 0; i < vertices.size(); i++)
		printv(vertices[i]);

	//solve for beta, gamma, t
	float beta = (vertices[0][0] - eye[0]) * (vertices[])




	return true;
}

void view_ray(int x, int y) {
	cout << "Raycasting to pixel " << x << " " << y << endl;
}



int main(int argc, char** argv) {
	FILE *fp;
	char line[256];
	vector<float> b;
	vector<float> from;
	vector<float> at;
	vector<float> up;
	vector<float> f;
	vector<vector<vector<float> > > p;
	float angle;
	float hither;
	int res_x;
	int res_y;
	int poly_num = 0;
	int p_num = 0;

	fp = fopen("tetra-3.nff" , "r");
	if(fp == NULL)
		cout << "File not found" << endl;
	else
		cout << "File opened for reading" << endl;
	cout << "Parsing .nff file..." << endl << endl;


	char str1[20];
	char str2[20];
	char str3[20];

	//parse through file and update data structures
	while(fgets(line, sizeof(line), fp)) {
		char poly_vertices_str[8];
		printf("%s", line);

		if(line[0] == 'v') {
			//indicates viewpoint
		} else if((line[0] == 'a' && line[1] == 'n') || line[0] == 'h') {
			//angle or hither
			sscanf(line, "%*s %s", str1);
			if(line[0] == 'h')
				hither = atof(str1);
			else
				angle = atof(str1);
		} else if(line[0] == 'r') {
			//resolution
			sscanf(line, "%*s %s %s", str1, str2);
			res_x = atof(str1);
			res_y = atof(str2);
		} else {
			//b, v[], f
			float f1;
			float f2;
			float f3;
			sscanf(line, "%*s %s %s %s", str1, str2, str3);
			f1 = atof(str1);
			f2 = atof(str2);
			f3 = atof(str3);
			if(line[0] == 'b') {
				b.push_back(f1);
				b.push_back(f2);
				b.push_back(f3);
			} else if(line[0] == 'f' && line[1] == 'r') {
				from.push_back(f1);
				from.push_back(f2);
				from.push_back(f3);
			} else if(line[0] == 'a') {
				at.push_back(f1);
				at.push_back(f2);
				at.push_back(f3);
			} else if(line[0] == 'u') {
				up.push_back(f1);
				up.push_back(f2);
				up.push_back(f3);
			} else if(line[0] == 'f') {
				f.push_back(f1);
				f.push_back(f2);
				f.push_back(f3);
			} else if(line[0] == 'p') {
				p_num++;
				printf("Start polygon tracing\n");
				sscanf(line, "%*s %s", poly_vertices_str);
				vector<vector<float> > vertices;
				vector<float> point;
				int poly_vertices = atoi(poly_vertices_str);
				for(int i = 0; i < poly_vertices; i++) {
					point.clear();
					fgets(line, sizeof(line), fp);
					sscanf(line, "%s %s %s", str1, str2, str3);
					point.push_back(atof(str1));
					point.push_back(atof(str2));
					point.push_back(atof(str3));
					vertices.push_back(point);
				}
				p.push_back(vertices);
				vertices.clear();
			}
		}
	}

	printf("%f %f %f\n", b[0], b[1], b[2]);
	printf("%f %f %f\n", from[0], from[1], from[2]);
	printf("%f %f %f\n", at[0], at[1], at[2]);
	printf("%f %f %f\n", up[0], up[1], up[2]);
	printf("%f %f %f\n", f[0], f[1], f[2]);

	float dist = sqrt(pow(from[0]-at[0], 2) + pow(from[1]-at[1], 2) + pow(from[2]-at[2], 2));
	printf("distance = %f\n", dist);
	float r = dist * tan((angle/2) * (PI/180));
	float rminl = r * 2;
	float t = dist * tan((angle/2) * (PI/180));
	float tminb = t * 2;
	printf("r = %f\n", r);
	vector<float> world_points = wp(0, 0, r, res_x);
	printv(world_points);
	world_points = wp(206, 206, r, res_x);
	printv(world_points);
	world_points = wp(511, 511, r, res_x);
	printv(world_points);

	cout << "polygons = " << p_num << endl;
	cout << "p.size = " << p.size() << endl;
	cout << "p0.size = " << p[0].size() << endl;
	cout << "p00.size = " << p[0][0].size() << endl;

	cout << p[0][0][0] << endl;
	cout << p[0][0][1] << endl;
	cout << p[0][0][2] << endl;


	//start raytracing
	//sample run
	int x = 0;
	int y = 0;
	int p_sub = 0;
	cout << endl << x << ", " << y << ", dist:" << endl;
	vector<float> poop = wp(x, y, r, res_x);
	poop.push_back(at[2]);
	printv(poop);
	cout << "Origin = ";
	printv(from);
	cout << "Toward = ";
	printv(poop);
	ray_tri_intersect(from, poop, p[p_sub]);


	/*
	unsigned char pixels[res_x][res_y][3];
	for(int x = 0; x < res_x; x++) {
		for(int y = 0; y < res_x; y++) {
			for(int i = 0; i < p.size(); i++) {
				//view_ray(x, y);
				//ray_tri_intersect(p[i]);
			}
		}
	}*/

	fclose(fp);
	/*
	FILE *fo = fopen("trace.ppm","wb");
	if(fo == NULL)
		printf("DIDN't WORK!!!\n");
	fprintf(fo, "P6\n%d %d\n%d\n", res_x, res_y, 255);
	fwrite(pixels, 1, res_x*res_y*3, fo);
	fclose(fo);
	*/

	return 0;

}






