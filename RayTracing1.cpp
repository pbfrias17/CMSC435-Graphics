
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


vector<float> vector_difference(vector<float> a, vector<float> b) {
	vector<float> result;
	for(int i = 0; i < a.size(); i++) {
		//cout << a[i] << " - " << b[i] << " = " << a[i] - b[i] << endl;
		result.push_back(a[i] - b[i]);
	}
	return result;
}

vector<float> vector_addition(vector<float> a, vector<float> b) {
	vector<float> result;
	for(int i = 0; i < a.size(); i++) {
		result.push_back(a[1] + b[i]);
	}
	return result;

}

vector<float> scalar_multiplication(vector<float> a, float scalar) {
	vector<float> result;
	for(int i = 0; i < a.size(); i++) {
		result.push_back(scalar * a[i]);
	}
	return result;
}

vector<float> world_points(int i, int j, float r, int res) {
	//r = b = -l = -t
	vector<float> world;
	world.push_back((2*r)*(i+.5)/res);
	world.push_back((2*r)*(j+.5)/res);
	return world;
}


bool ray_tri_intersect(vector<float> eye, vector<float> dir, vector<vector<float> > vertices) {
	cout << "Checking intersection on polygon " << endl;
	for(int i = 0; i < vertices.size(); i++)
		printv(vertices[i]);

	//solve for beta, gamma, t





	return true;
}

void view_ray(int x, int y) {
	cout << "Raycasting to pixel " << x << " " << y << endl;
}


void normalize(vector<float> &v) {
	float mag = sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
	for(int i = 0; i < v.size(); i++)
		v[i] = v[i]/mag;
}


vector<float> cross_product(vector<float> a, vector<float> b) {
	vector<float> result;
	result.push_back(a[1]*b[2] - a[2] * b[1]);
	result.push_back(a[2]*b[0] - a[0] * b[2]);
	result.push_back(a[0]*b[1] - a[1] * b[0]);
	return result;
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
		//printf("%s", line);

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
				//printf("Start polygon tracing\n");
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

	printf("from = %f %f %f\n", from[0], from[1], from[2]);
	printf("at = %f %f %f\n", at[0], at[1], at[2]);
	printf("up = %f %f %f\n", up[0], up[1], up[2]);

	//initializing camera view
	float dist = sqrt(pow(from[0]-at[0], 2) + pow(from[1]-at[1], 2) + pow(from[2]-at[2], 2));

	vector<float> z_axis;
	for(int i = 0; i < from.size(); i++)
		z_axis.push_back(from[i] - at[i]);
	normalize(z_axis);
	cout << "normalized z-axis = ";
	printv(z_axis);

	vector<float> x_axis = cross_product(up, z_axis);
	normalize(x_axis);
	cout << "normalized x-axis = ";
	printv(x_axis);

	vector<float> y_axis = cross_product(z_axis, x_axis);
	normalize(y_axis);
	cout << "normalized y-axis = ";
	printv(y_axis);

	float r = dist * tan((angle/2) * (PI/180));
	float rminl = r * 2;
	float t = dist * tan((angle/2) * (PI/180));
	float tminb = t * 2;
	printf("r = %f\n", r);

	/*cout << "polygons = " << p_num << endl;
	cout << "p.size = " << p.size() << endl;
	cout << "p0.size = " << p[0].size() << endl;
	cout << "p00.size = " << p[0][0].size() << endl;

	cout << p[0][0][0] << endl;
	cout << p[0][0][1] << endl;
	cout << p[0][0][2] << endl;
	 */

	//start raytracing
	//sample run
	int x = 205;
	int y = 205;
	int p_sub = 0;
	//cout << endl << x << ", " << y << ", dist:" << endl;
	vector<float> poop = world_points(x, y, r, res_x);
	poop.push_back(at[2]);
	//printv(poop);
	//cout << "Origin = ";
	//printv(from);
	//cout << "Toward = ";
	//printv(poop);
	//ray_tri_intersect(from, poop, p[p_sub]);

	vector<float> test1;
	vector<float> test2;
	test1.push_back(1);
	test1.push_back(1);
	test2.push_back(2);
	test2.push_back(2);
	printv(vector_difference(test2, test1));



	vector<float> bottom_left = vector_difference(at, scalar_multiplication(x_axis, r));
	bottom_left = vector_difference(bottom_left, scalar_multiplication(y_axis, r));
	cout << "bottom left = ";
	printv(bottom_left);
	cout << "hx = " << endl;
	printv(scalar_multiplication(x_axis, r));
	cout << "hy = " << endl;
	printv(scalar_multiplication(y_axis, r));
	//sample pixel 0, 0
	int polygon = 0;
	vector<float> pixel_pos = world_points(x, y, r, res_x);
	printv(pixel_pos);
	//pixel_pos = vector_addition(bottom_left, scalar_multiplication(x_axis, pixel_pos[0]));
	cout << "ipos = ";
	printv(scalar_multiplication(x_axis, pixel_pos[0]));
	cout << "jpos = ";
	printv(scalar_multiplication(y_axis, pixel_pos[1]));
	pixel_pos = vector_addition(pixel_pos, scalar_multiplication(y_axis, pixel_pos[1]));
	cout << "pixel position = ";
	printv(pixel_pos);

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






