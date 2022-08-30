#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cmath>
#include <vector>


const double PI = 3.1415927;

class Framework{
public:
    // Contructor which initialize the parameters.
    Framework(int height_, int width_): height(height_), width(width_){
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

    void draw_circle(int t, int x0, int y0, int radius_, int r, int period, int kx, int ky){
        
        SDL_SetRenderDrawColor(renderer, kx * 2.55, ky * 2.55, 255, 255); // circle color

        // set position for this circle
        double x_pos = x0 + r * cos(t * 2 * PI / period + 2 * PI * kx/100);
        double y_pos = y0 + r * sin(t * 2 * PI / period + PI * ky/100);

        // draw circle        
        for(int x=x_pos-radius_; x<=x_pos+radius_; x++){

            for(int y=y_pos-radius_; y<=y_pos+radius_; y++){

                if((std::pow(y_pos-y,2)+std::pow(x_pos-x,2)) <= std::pow(radius_,2)){

                	/* literally fills pixels associated to circle
                    we should speed this up by only calculating the value of each pixel once
                    currently this takes n_grid * n_grid * O(radius_ * radius_) calculations 

                    we could count this by keeping track of the number of calls
                    to SDL_RenderDrawPoint()
                    */

                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
        }

    }

    void move_circle(int width, int height){
        // Setting the color to be RED with 100% opaque (0% trasparent).
        
        SDL_Event event;    // Event variable
        while(!(event.type == SDL_QUIT)){

        	// Rotating circle

        	int n_grid = 256;
        	int period = 1024;

        	int rt;

        	for (int i = 0; i < period; i++) {

        		rt = 30 * (1 + abs(sin(2 * PI * i / period)));


        		SDL_Delay(0);
        		SDL_PollEvent(&event);
        		if(event.type == SDL_QUIT) return;
        		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        		SDL_RenderClear(renderer);

        		for (int kx = 0; kx < n_grid; kx++ ){
        			for (int ky = 0; ky < n_grid; ky++){

        				int px = kx * (width/n_grid);
        				int py = ky * (height/n_grid);

        				draw_circle(i + 10 * (kx + ky), px, py, 1, rt, period, kx, ky);
        			}
        		}

        		SDL_RenderPresent(renderer); // call this after all changes have been made 
        	}

        }
        
    }

private:
    int height;     // Height of the window
    int width;      // Width of the window
    SDL_Renderer *renderer = NULL;      // Pointer for the renderer
    SDL_Window *window = NULL;      // Pointer for the window
};



int main(int argc, char * argv[]){

	int height = 768;
	int width = 1024;

    // Creating the object by passing Height and Width value.
    Framework fw(height, width);

    // Starting the animation
    fw.move_circle(width, height);
    return 1;
}
