#include <cmath>
#include <vector>
#include <iostream>
#include <time.h>

using namespace std;


vector<double> search_coeffs() {

	srand(time(NULL));

	bool found = false;
	int search_its = 10000;
	int attempts = 0;

	vector<double> coeffs(12,0);

	while (!found) {

		attempts++;

		// generate random coefficients
		for (int i = 0; i < 12; i++) {
			coeffs[i] = (-10 + (rand() / (RAND_MAX/21))) / 10.;
		}

		// do a trial iteration run
		double x = 0, y = 0;
		double xp, yp;

		int i = 0;

		while (i < search_its) {

			xp = yp = 0;

			xp += coeffs[0];
			xp += coeffs[1] * x;
			xp += coeffs[2] * y;
			xp += coeffs[3] * (x * x);
			xp += coeffs[4] * (y * y);
			xp += coeffs[5] * (x * y);

			yp += coeffs[6];
			yp += coeffs[7] * x;
			yp += coeffs[8] * y;
			yp += coeffs[9] * (x * x);
			yp += coeffs[10] * (y * y);
			yp += coeffs[11] * (x * y);

			if (abs(xp) > 1000 || abs(yp) > 1000) {
				break;
			}

			x = xp;
			y = yp;

			i++;

		}

		// this is not a sufficient condition
		// we should also check the pixel density to avoid fixed points

		if (i < search_its) {
			cout << "out of bounds " << i << " " << xp << " " << yp << endl;
			found = false;
		} else {
			cout << "bounded " << i << " " << xp << " " << yp << endl;
			found = true;
		}

	}

	cout << "Found attractor after " << attempts << " attempts." << endl;

	return coeffs;

}


int main() {

	vector<double> coeffs = search_coeffs();



	return 0;
}