#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

const int LARGE = 10000000;

vector<vector<double>> iteration(int n, double *xmin, double *xmax, double *ymin, double *ymax) {

	vector<vector<double>> data(n, vector<double>(2));

	double a0 = -0.2, a1 = 0.8, a2 = 0.7, a3 = 0.7, a4 = 0.7, a5 = -0.9;
	double x = 0, y = 0;

	(*xmin) = LARGE;
	(*xmax) = -LARGE;
	(*ymin) = LARGE;
	(*ymax) = -LARGE;
 
	double xp, yp;

	for (int i = 0; i < n; i++) {

		xp = (a0 * x + a1 * (y * y) + a2);
		yp = (a3 * (x * x) + a4 * y + a5);

		x = xp;
		y = yp;

		if (x > (*xmax)) {
			(*xmax) = x;
		}
		if (y > (*ymax)) {
			(*ymax) = y;
		}
		if (x < (*xmin)) {
			(*xmin) = x;
		}
		if (y < (*ymin)) {
			(*ymin) = y;
		}
		data[i][0] = x;
		data[i][1] = y;	
	}
	return data;
}


vector<vector<int>> pixel_data(vector<vector<double>> data, 
	int n, int width, int height, double xmin, double xmax, double ymin, double ymax) {

	vector<vector<int>> pix_data(width, vector<int>(height));

	double x, y;
	int px, py;
	double xs, ys;

	for (int i = 0; i < n; i++){

		x = data[i][0];
		y = data[i][1];

		xs = (x - xmin)/(xmax - xmin);
		ys = (y - ymin)/(ymax - ymin);

		px = (width - 1) * xs;
		py = (height - 1) * ys;

		pix_data[px][py] += 1;
	}

	return pix_data;

}

int main(int argc, char *argv[]){

	// Calculate iteration data
	cout << "Calculating iterations..." << endl;
	int n = 25000000;
	double xmin, xmax, ymin, ymax;
	vector<vector<double>> data = iteration(n, &xmin, &xmax, &ymin, &ymax);

	// Construct bounding box
	cout << "Rescaling image..." << endl;
	double dx = xmax - xmin;
	double dy = ymax - ymin;
	double aspect = dx/dy;

	int height = 800;
	int width = aspect * height;

	// Calculate pixel data
	cout << "Calculate pixel data..." << endl;
	vector<vector<int>> pix_data = pixel_data(data, n, width, height, xmin, xmax, ymin, ymax);

	// Initialize SDL
	cout << "Initialize SDL..." << endl;
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL; 
	SDL_Event event;
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	// Draw attractor
	cout << "Drawing attractor..." << endl;
	int alpha = 0;
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // enable transparency
	
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++) {
			if (pix_data[i][j] > 0) {

				alpha = pix_data[i][j];
				if (alpha > 255) {
					alpha = 255;
				}
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
				SDL_RenderDrawPoint(renderer, i, height-j-1);
			}
		}
	}
	SDL_RenderPresent(renderer);

	// Keep window open
	while(!(event.type == SDL_QUIT)){
		SDL_Delay(50);
		SDL_PollEvent(&event);
	}

	return 0;
}


