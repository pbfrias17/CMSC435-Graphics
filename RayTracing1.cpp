
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
		result.push_back(a[i] - b[i]);
	}
	return result;
}

vector<float> vector_addition(vector<float> a, vector<float> b) {
	vector<float> result;
	for(int i = 0; i < a.size(); i++) {
		result.push_back(a[i] + b[i]);
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


void normalize(vector<float> &v) {
	float mag = sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
	for(int i = 0; i < v.size(); i++)
		v[i] = v[i]/mag;
}


float dot_product(vector<float> a, vector<float> b) {
	float result = 0;
	for(int i = 0; i < a.size(); i++) {
		result += a[i] * b[i];
	}
	return result;
}


vector<float> cross_product(vector<float> a, vector<float> b) {
	vector<float> result;
	result.push_back(a[1]*b[2] - a[2] * b[1]);
	result.push_back(a[2]*b[0] - a[0] * b[2]);
	result.push_back(a[0]*b[1] - a[1] * b[0]);
	return result;
}


vector<float> pixel_points(int i, int j, float r, int res) {
	//r = b = -l = -t
	vector<float> world;
	world.push_back(-r + (2*r)*(i+.5)/res);
	world.push_back(-r + (2*r)*(j+.5)/res);
	return world;
}


bool ray_tri_intersect(vector<float> eye, vector<float> dir, vector<vector<float> > vertices, float interval[]) {
	vector<float> a = vertices[0];
	vector<float> b = vertices[1];
	vector<float> c = vertices[2];
	int x = 0;
	int y = 1;
	int z = 2;

	//solve for beta, gamma, t
	//using dummy variable names for readability with cramer's rule
	float ei_hf = ((a[y] - c[y]) * dir[z]) - (dir[y] * (a[z] - c[z]));
	float gf_di = (dir[x] * (a[z] - c[z])) - ((a[x] - c[x]) * dir[z]);
	float dh_eg = ((a[x] - c[x]) * dir[y]) - (dir[x] * (a[y] - c[y]));
	float ak_jb = ((a[x] - b[x]) * (a[y] - eye[y])) - ((a[x] - eye[x]) * (a[y] - b[y]));
	float jc_al = ((a[x] - eye[x]) * (a[z] - b[z])) - ((a[x] - b[x]) * (a[z] - eye[z]));
	float bl_kc = ((a[y] - b[y]) * (a[z] - eye[z])) - ((a[y] - eye[y]) * (a[z] - b[z]));
	float M = ((a[x] - b[x]) * ei_hf) + ((a[y] - b[y]) * gf_di) + ((a[z] - b[z]) * dh_eg);

	float t = (a[z] - c[z]) * ak_jb + (a[y] - c[y]) * jc_al + (a[x] - c[x]) * bl_kc;
	t = t/M;
	if(t < interval[0] || t > interval[1])
		return false;

	float gamma = dir[z] * ak_jb + dir[y] * jc_al + dir[x] * bl_kc;
	gamma = gamma/M;
	if(gamma < 0 || gamma > 1)
		return false;


	float beta = (a[x] - eye[x]) * ei_hf + (a[y] - eye[y]) * gf_di + (a[z] - eye[z]) * dh_eg;
	beta = beta/M;
	if(beta < 0 || beta > 1 - gamma)
		return false;

	return true;
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
	int p_num = 0;

	fp = fopen("tetra-3.nff" , "r");
	if(fp == NULL) {
		cout << "File not found" << endl;
		return 1;
	}

	char str1[20];
	char str2[20];
	char str3[20];

	//parse through file and update data structures
	while(fgets(line, sizeof(line), fp)) {
		char poly_vertices_str[8];
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

	//initializing camera view
	float dist = sqrt(pow(from[0]-at[0], 2) + pow(from[1]-at[1], 2) + pow(from[2]-at[2], 2));
	float r = dist * tan((angle/2) * (PI/180));
	//in this case, r = t = -l = -b

	vector<float> z_axis = vector_difference(from, at);
	normalize(z_axis);

	vector<float> y_axis = cross_product(up, z_axis);
	normalize(y_axis);

	vector<float> x_axis = cross_product(z_axis, y_axis);
	normalize(x_axis);

	cout << "from = ";
	printv(from);
	cout << "at = ";
	printv(at);
	cout << "up = ";
	printv(up);

	cout << "z_axis = ";
	printv(z_axis);
	cout << "y_axis = ";
	printv(y_axis);
	cout << "x_axis = ";
	printv(x_axis);

	cout << "z.y = " << dot_product(z_axis, y_axis) << endl;
	cout << "y.x = " << dot_product(y_axis, x_axis) << endl;
	cout << "x.z = " << dot_product(x_axis, z_axis) << endl;

	cout << "from - dw = at = ";
	printv(vector_difference(from, scalar_multiplication(z_axis, dist)));


	int true_c = 0;
	int false_c = 0;

    unsigned char pixels[res_x][res_y][3];
	float interval[2] = {0, 10};
	for(int y = 255; y < 256; y++) {
		for(int x = 255; x < 256; x++) {
			//since a pixel color is decided by a rayhit, once we hit
			//any object we can color the pixel and move on to the next pixel
			bool hit = false;
			vector<float> pp = pixel_points(x, y, r, res_x);
			//direction = u(x-axis) + v(y-axis) + distance(-z_axis)
			vector<float> dir = vector_addition(scalar_multiplication(x_axis, pp[0]), scalar_multiplication(y_axis, pp[1]));
			dir = vector_addition(dir, scalar_multiplication(z_axis, -dist));

			vector<float> target = vector_addition(from, scalar_multiplication(x_axis, pp[0]));
			target = vector_addition(target, scalar_multiplication(y_axis, pp[1]));
			target = vector_addition(target, scalar_multiplication(z_axis, -dist));
			printv(target);

			//printv(dir);
			if(!hit) {
				for(int i = 0; i < p.size(); i++) {
					//check ray intersections with current polygons
					if(ray_tri_intersect(from, dir, p[i], interval)) {
						pixels[y][x][0] = f[0];
						pixels[y][x][1] = f[1];
						pixels[y][x][2] = f[2];
						true_c++;
						hit = true;
					} else {
						false_c++;
						pixels[y][x][0] = b[0];
						pixels[y][x][1] = b[1];
						pixels[y][x][2] = b[2];
					}
				}
			}
		}
	}
	fclose(fp);
	cout << "true: " << true_c << endl;
	cout << "false: " << false_c << endl;

	//write output to file
	FILE *fo = fopen("trace.ppm","wb");
	if(fo == NULL)
		printf("DIDN't WORK!!!\n");
	fprintf(fo, "P6\n%d %d\n%d\n", res_x, res_y, 255);
	fwrite(pixels, 1, res_x*res_y*3, fo);
	fclose(fo);

	return 0;

}






