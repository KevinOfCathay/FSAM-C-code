#pragma once
#ifndef RANDOMPOINT_H
#define RANDOMPOINT_H

#include <iostream>
#include <random>
#include <vector>
#include <ctime>

#define vpoint vector<pair<float, float>>
using namespace std;


class Random_Point {
public:
	Random_Point() {};
	~Random_Point() {};

	// A series of points with normal distribution noise
	static vpoint quadratic(float mean, float sigma, int lower, int upper) {
		vpoint temp = {};

		// Create the random generator
		default_random_engine dis;
		normal_distribution<float> noise(mean, sigma);

		for ( int a = lower; a <= upper; a += 1 ) {
			float x = a;
			float y = pow(a, 2) + noise(dis);
			temp.push_back(make_pair(x, y));
		}
		return temp;
	}

	static void print_point(vpoint p) {
		for ( vpoint::iterator it = p.begin(); it != p.end(); it++ ) {
			cout << it->first << " , " << it->second << endl;
		}
	}

private:

};



#endif // !RANDOMPOINT_H
