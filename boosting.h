#include "randompoint.h"

// This is an example which uses the FSAM technique,
// covered in FOUNDATIONS OF MACHINE LEARNING
// courses, to do the regression

#ifndef BOOSTING_H
#define BOOSTING_H

#include <map>

class Boosting {
public:
	struct split_point {
		split_point(float x, float yl, float yr) : x_split_point(x), y_left_area(yl), y_right_area(yr) {}
		float x_split_point;
		float y_left_area;
		float y_right_area;
	};

	Boosting(vpoint sample_points) : original_points(sample_points) {
		for ( vpoint::iterator p = original_points.begin(); p != original_points.end(); p++ ) {
			residuals[p->first] = 0;
		}
		residuals_zeros = residuals;
	};
	~Boosting() {};

	void add_Spilt_Point(float x, float yl, float yr) {
		regression_function.push_back(split_point(x, yl, yr));
	}

	// Get the residuals on every points
	// We use l2, but we only don't care about the loss function
	// since we have a closed-form solution for the yl, yr.
	void Get_Residual() {
		// reset the residual map to 0
		residuals = residuals_zeros;
		// For each s point in the vector, calculate the residuals
		for ( vpoint::iterator p = original_points.begin(); p != original_points.end(); p++ ) {
			// For each point, calculate the correspondng y
			float y = 0;
			for ( vector<split_point>::iterator it = regression_function.begin(); it != regression_function.end(); it++ ) {
				float x = it->x_split_point, yl = it->y_left_area, yr = it->y_right_area;
				// if the split point is on the right, then use the yl value
				// else, use the yr value
				y = (x > p->first) ? (y + yl) : (y + yr);
			}

			float r = (p->second) - y;
			residuals[p->first] += r;
		}
	}

	// Brute-force every possible place and then find the best split point
	void Split() {
		float res_x; float res_yl; float res_yr;
		float min_loss = -1;

		int sz = original_points.size();
		for ( int idx = 0; idx < sz - 1; idx += 1 ) {
			// Get the half-point
			float half_x_point = (original_points[idx].first + original_points[idx + 1].first) / 2;

			float yl = 0; float yr = 0; float loss = 0;
			// Calculte the yl, yr
			for ( int m = 0; m < sz; m += 1 ) {
				if ( m <= idx ) {
					yl = yl + residuals[original_points[m].first];
				}
				else {
					yr = yr + residuals[original_points[m].first];
				}
			}
			yl = yl / (idx + 1);
			yr = yr / (sz - idx - 1);

			// Calculate the loss using l2
			for ( int m = 0; m < sz; m += 1 ) {
				if ( m <= idx ) {
					loss = loss + pow((yl - residuals[original_points[m].first]), 2);
				}
				else {
					loss = loss + pow((yr - residuals[original_points[m].first]), 2);
				}
			}

			// Compare the loss
			if ( min_loss == -1 || loss < min_loss ) {
				res_x = half_x_point; res_yl = yl; res_yr = yr; min_loss = loss;
			}
		}

		add_Spilt_Point(res_x, res_yl, res_yr);
	}

	void Loop() {
		for ( int step = 0; step < 10; step += 1 ) {
			Split();
			Get_Residual();
			Output_Residuals();
			cout << endl;
		}
		Show_Split_Points();
		cout << endl;
	}

	void Output_Residuals() {
		for ( map<float, float>::iterator it = residuals.begin(); it != residuals.end(); it++ ) {
			cout << it->first << " : " << it->second << endl;
		}
	}

	void Show_Split_Points() {
		for ( vector<split_point>::iterator it = regression_function.begin(); it != regression_function.end(); it++ ) {
			cout << it->x_split_point << " || yl:  " << it->y_left_area << " yr: " << it->y_right_area << endl;
		}
	}
private:
	vpoint original_points;
	map<float, float> residuals;  // A map from x --> its corresponding residual
	map<float, float> residuals_zeros;
	vector<split_point> regression_function;
};



#endif // !BOOSTING_H
