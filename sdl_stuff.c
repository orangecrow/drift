
/* kwadracik.c ... */

/*
 * This example creates an SDL window and renderer, and then draws a
 * texture to it every frame.
 * the texture should be controlled with keyboard keypresses.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "wektor.h"
#include "car_physics.h"


/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static SDL_Texture *texture2 = NULL;
static int texture_width = 0;
static int texture_height = 0;
static int texture2_width = 0;
static int texture2_height = 0;
unsigned int key;

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_Surface *surface = NULL;
    char *png_path = "resources/car.png";

    SDL_SetAppMetadata("Example Renderer Textures", "1.0", "com.example.renderer-textures");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Drifting", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    /* Textures are pixel data that we upload to the video hardware for fast drawing. Lots of 2D
       engines refer to these as "sprites." We'll do a static texture (upload once, draw many
       times) with data from a png file. */

    /* SDL_Surface is pixel data the CPU can access. SDL_Texture is pixel data the GPU can access.
       Load a .png into a surface, move it to a texture from there. */
	
    surface = SDL_LoadPNG(png_path);
    if (!surface) {
        SDL_Log("Couldn't load png: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    texture_width = surface->w;
    texture_height = surface->h;

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
	//load another texture
	png_path= "resources/tor.png";

    surface = SDL_LoadPNG(png_path);
    if (!surface) {
        SDL_Log("Couldn't load png: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    texture2_width = surface->w;
    texture2_height = surface->h;

    texture2 = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture2) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    } else if (event->type == SDL_EVENT_KEY_DOWN){
		key = event->key.key;
		if (key == SDLK_W){
			keys[0]=1;
		}
		if (key == SDLK_S){
			keys[1]=1;
		}
		if (key == SDLK_D){
			keys[2]=1;
		}
		if (key == SDLK_A){
			keys[3]=1;
		}
    } else if (event->type == SDL_EVENT_KEY_UP){
		key = event->key.key;
		if (key == SDLK_W){
			keys[0]=0;
		}
		if (key == SDLK_S){
			keys[1]=0;
		}
		if (key == SDLK_D){
			keys[2]=0;
		}
		if (key == SDLK_A){
			keys[3]=0;
		}
	}
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
	const Uint64 now = SDL_GetTicks();

	/* as you can see from this, rendering draws over whatever was drawn before it. */
	SDL_SetRenderDrawColor(renderer, 40, 80, 40, SDL_ALPHA_OPAQUE);  /* black, full alpha */
	SDL_RenderClear(renderer);  /* start with a blank canvas. */

	static SDL_FRect dst_rect;
	static SDL_FRect dst_rect2;
	static bool sw = 1;
	if ( sw ){
		dst_rect.x = WINDOW_WIDTH/2;
		dst_rect.y = WINDOW_HEIGHT/2;
		dst_rect.w = (float) texture_width;
		dst_rect.h = (float) texture_height;
		dst_rect2.x = 0;
		dst_rect2.y = 0;
		dst_rect2.w = (float) texture2_width * 3;
		dst_rect2.h = (float) texture2_height * 3;
		sw = 0;
	}
	apply_phiscs();
	dst_rect2.x = -car.pos.x/3;
	dst_rect2.y = -car.pos.y/3;
	float angle = - car.direction_angle / 2 / M_PI * 360;
	SDL_FPoint center;
	center.x = texture_width / 2.0f;
	center.y = texture_height / 2.0f;

	SDL_RenderTexture(renderer, texture2, NULL, &dst_rect2);
	SDL_RenderTextureRotated(renderer, texture, NULL, &dst_rect, angle , &center, SDL_FLIP_NONE);
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
	//SDL_RenderLine(renderer, car.pos.x,car.pos.y,car.pos.x+line.x*50,car.pos.y+line.y*50);
	//SDL_RenderLine(renderer, pointA.x+texture_width/2, pointA.y+texture_height/2 ,pointB.x+texture_width/2, pointB.y+texture_height/2 );
	//SDL_RenderLine(renderer, pointC.x+texture_width/2, pointC.y+texture_height/2 ,pointD.x+texture_width/2, pointD.y+texture_height/2 );

	SDL_RenderPresent(renderer);  /* put it all on the screen! */

	return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_DestroyTexture(texture);
    /* SDL will clean up the window/renderer for us. */
}
