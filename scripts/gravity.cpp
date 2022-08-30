#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cmath>
#include <vector>
#include <iostream>

using namespace std;


const int n = 1000;

class Framework{
public:
    // Contructor which initialize the parameters.
    Framework(int width_, int height_): width(width_), height(height_){
        SDL_Init(SDL_INIT_VIDEO);       // Initializing SDL as Video
        SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);

        // does this do anything? - maybe replace CreateWindowAndRender by explicit calls
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);      // setting draw color 
        SDL_RenderClear(renderer);      // Clear the newly created window
        SDL_RenderPresent(renderer);    // Reflects the changes done in the
                                        //  window.
    }

    // Destructor
    ~Framework(){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void draw_pixel(double x, double y, int size){
        
        

        int k = size - 1;

        for (int i = x-k; i <= x+k; i++){
        	for (int j = y-k; j <= y+k; j++){

        		int dx = x-i;
        		int dy = y-j;

        		int dr = (dx * dx) + (dy * dy);

        		int alpha = 255 / (1 + dr);

        		SDL_SetRenderDrawColor(renderer, alpha, alpha, alpha, alpha); 
        		SDL_RenderDrawPoint(renderer, i, j);

        	}
        }

        

    }

    void sim_loop(vector<vector<double>> stars, int width, int height){
        // Setting the color to be RED with 100% opaque (0% trasparent).
        
        SDL_Event event;    // Event variable

        double x,y;
        int size;


        while(!(event.type == SDL_QUIT)){

        	SDL_Delay(0);
    		SDL_PollEvent(&event);
    		if(event.type == SDL_QUIT) return;
    		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    		SDL_RenderClear(renderer);

    		stars = gravity(stars);

    		for (int i = 0; i < n; i++){
    			x = stars[i][0];
    			y = stars[i][1];
    			size = stars[i][4];

    			if (x < 0) {
    				x = width + x;
    				stars[i][0] = x;
    			}

    			if (y < 0) {
    				y = height + y;
    				stars[i][1] = y;
    			}

    			if (x > width) {
    				x = (x - width);
    				stars[i][0] = x;
    			}

      			if (y > height) {
    				y = (y - height);
    				stars[i][1] = y;
    			}



    			draw_pixel(x,y,2);
    		}



    		
        	SDL_RenderPresent(renderer); 

        }
        
    }

vector<vector<double>> gravity(vector<vector<double>> stars){

	double dt = 0.1; 


	double x1, x2, y1, y2;
	double m1, m2;
	double ax, ay;

	int k;



	for (int i = 0; i < n; i++){

		for (int j = i + 1; j < n; j++){

			x1 = stars[i][0];
			x2 = stars[j][0];

			y1 = stars[i][1];
			y2 = stars[j][1];

			m1 = stars[i][4];
			m2 = stars[j][4];

			double dx = (x1 - x2);
			double dy = (y1 - y2);

			double a = atan2(dy, dx);

			double dr = (dx * dx) + (dy * dy);


			double f = 10 * m1 * m2 / (1 + dr);


			ax = f * cos(a);
			ay = f * sin(a);

			stars[i][2] += -ax * dt;
			stars[i][3] += -ay * dt;
			stars[j][2] += ax * dt;
			stars[j][3] += ay * dt;






		}
	}

	for (int i = 0; i < n; i++){

		stars[i][2] *= 0.75;
		stars[i][3] *= 0.75;
		stars[i][0] += stars[i][2] * dt;
		stars[i][1] += stars[i][3] * dt;
	}



	return stars;
}


private:
    int height;     // Height of the window
    int width;      // Width of the window
    SDL_Renderer *renderer = NULL;      // Pointer for the renderer
    SDL_Window *window = NULL;      // Pointer for the window
};



int main(int argc, char * argv[]){

	

	vector<vector<double>> stars(n,vector<double>(5));

	int width = 800;
	int height = 600;
	int max_size = 10;

	for (int i = 0; i < n; i++){
		stars[i][0] = static_cast <double> (rand()) / static_cast <double> (RAND_MAX/width); // x position
		stars[i][1] = static_cast <double> (rand()) / static_cast <double> (RAND_MAX/height); // y position 
		stars[i][2] = 0; // initial x velocity
		stars[i][3] = 0; // initial y velocity
		stars[i][4] = 2 + (rand() / (RAND_MAX/max_size)); // size
	}


    // Creating the object by passing Height and Width value.
    Framework fw(width, height);

    // Starting the animation
    fw.sim_loop(stars, width, height);
    return 1;
}
