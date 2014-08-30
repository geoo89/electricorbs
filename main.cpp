/*
   Copyright (C) 1997-2014 Sam Lantinga <slouken@libsdl.org>

   This software is provided 'as-is', without any express or implied
   warranty.  In no event will the authors be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely.

   This file is created by : Nitin Jain (nitin.j4@samsung.com)
*/

/* Sample program:  Draw a Chess Board  by using SDL_CreateSoftwareRenderer API */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <vector>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   480
#define NUM_SPRITES     10
#define MAX_SPEED       1

static SDL_Texture *spr_orb;
static SDL_Texture *spr_orb_red;
static SDL_Texture *spr_orb_blue;
static SDL_Rect positions[NUM_SPRITES];
static SDL_Rect velocities[NUM_SPRITES];
//TODO: figure how to get these dynamically
static int sprite_w = 32, sprite_h = 32;


struct point {
    double x;
    double y;

    void reset() {
        x = 0;
        y = 0;
    }

    double length() {
        return sqrt(x*x + y*y);
    }
};

point operator-(point a) {
    point c;
    c.x = -a.x;
    c.y = -a.y;
    return c;
}

point operator+(point a, point b) {
    point c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

point operator-(point a, point b) {
    /*point c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    return c;*/
    return a + (-b);
}

/*operator+=(point a, point b) {
    point c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}*/

point operator*(double scalar, point p) {
    point c;
    c.x = scalar * p.x;
    c.y = scalar * p.y;
    return c;
}

double dist(point a, point b) {
    return (b-a).length();
}



class orb {

private:
    point pos;
    point vel;
    point force; // = accel
    int charge;
    SDL_Rect position; // sprite position and width
public:
    orb(double x, double y, double vx, double vy, int charge) {
        pos.x = x;
        pos.y = y;
        vel.x = vx;
        vel.y = vy;
        this->charge = charge;
        position.w = sprite_w;
        position.h = sprite_h;
    }

    /* get force */
    int get_charge() {
        return charge;
    }


    /* add a force vector to the force */
    void add_force(point f) {
        force = force + f;
    }

    /* Add the force as acceleration to the velocity and
     * reset the force to 0. */
    void reset_force() {
        vel = vel + force;
        force.reset();
    }

    /* Add current velocity to position. */
    void move() {
        pos = pos + vel;

        if ((pos.x - sprite_w/2 < 0) || (pos.x + sprite_w/2 >= WINDOW_WIDTH)) {
            vel.x = -vel.x;
            pos.x += vel.x;
        }

        if ((pos.y - sprite_h/2 < 0) || (pos.y + sprite_h/2 >= WINDOW_HEIGHT)) {
            vel.y = -vel.y;
            pos.y += vel.y;
        }
    }

    void render(SDL_Renderer *renderer) {
        // compute position of the actual sprite
        position.x = (int) (pos.x - sprite_w/2 + 0.5);
        position.y = (int) (pos.y - sprite_h/2 + 0.5);

        // draw main orb
        SDL_RenderCopy(renderer, spr_orb, NULL, &position);

        // now draw the overlay to shade orb red/blue
        if (charge > 0) {
            SDL_SetTextureAlphaMod(spr_orb_red, 2*charge);
            SDL_RenderCopy(renderer, spr_orb_red, NULL, &position);
        } else {
            SDL_SetTextureAlphaMod(spr_orb_blue, -2*charge);
            SDL_RenderCopy(renderer, spr_orb_blue, NULL, &position);
        }
        //SDL_RenderCopy(renderer, spr_orb_red, NULL, &position);
    }

    point dist(orb orb2) {
        return orb2.pos - this->pos;
    }

};

/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
static void
quit(int rc)
{
    exit(rc);
}

void
DrawChessBoard(SDL_Renderer * renderer)
{
	int row = 0, col = 0,x = 0;
	SDL_Rect rect, darea;

	/* Get the Size of drawing surface */
	SDL_RenderGetViewport(renderer, &darea);

	/*for(row; row < 8; row++)
	{
		coloum = row%2;
		x = x + coloum;
		for(coloum; coloum < 4+(row%2); coloum++)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

			rect.w = darea.w/8;
			rect.h = darea.h/8;
			rect.x = x * rect.w;
			rect.y = row * rect.h;
			x = x + 2;
			SDL_RenderFillRect(renderer, &rect);
		}
		x=0;
	}*/

    /* Draw horizontal and vertical lines */
    SDL_SetRenderDrawColor(renderer, 0x00, 0x7F, 0x00, 0xFF);
    for(row = 1; row < 8; row++) {
        SDL_RenderDrawLine(renderer, row*darea.w/8, 0, row*darea.w/8, darea.h-1);
    }
    for(col = 1; col < 8; col++) {
        SDL_RenderDrawLine(renderer, 0, col*darea.h/8, darea.w-1, col*darea.h/8);
    }
}


int
main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    int done;
    SDL_Event event;

	/* Enable standard application logging */
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) < 0) {
        quit(2);
    }

    if (!(spr_orb = IMG_LoadTexture(renderer, "gfx/orb.png"))) {
        quit(2);
    }
    if (!(spr_orb_red = IMG_LoadTexture(renderer, "gfx/orb_red.png"))) {
        quit(2);
    }
    if (!(spr_orb_blue = IMG_LoadTexture(renderer, "gfx/orb_blue.png"))) {
        quit(2);
    }

    /* Initialize the sprite positions */
    std::vector<orb> orbs;
    srand(time(NULL));
    for (int i = 0; i < NUM_SPRITES; ++i) {
        int x = rand() % (WINDOW_WIDTH - sprite_w) + sprite_w/2;
        int y = rand() % (WINDOW_HEIGHT - sprite_h) + sprite_h/2;
        int vx = (rand() % (MAX_SPEED * 2 + 1)) - MAX_SPEED;
        int vy = (rand() % (MAX_SPEED * 2 + 1)) - MAX_SPEED;
        int charge = (rand() % 0x100) - 0x80;
        orb neworb = orb(x, y, vx, vy, charge);
        orbs.push_back(neworb);
    }

    /* Main render loop */
    done = 0;
    while (!done) {
        /* Check for events */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN) {
                done = 1;
            }
        }

        // this really should go into a dedicated function
        for (int i = 0; i < NUM_SPRITES; ++i) {
            for (int j = i+1; j < NUM_SPRITES; ++j) {
                point d = orbs[i].dist(orbs[j]);
                double distance = d.length();
                if (distance == 0)
                    continue;
                double magnitude = orbs[i].get_charge() * orbs[j].get_charge() / (distance * distance);
                point force = 0.001 * magnitude * d;
                orbs[i].add_force(-force);
                orbs[j].add_force(force);
            }
        }

        /* Clear the rendering surface with the specified color
         * and then draw the lines. */
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(renderer);
        DrawChessBoard(renderer);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        // have the orbs move and then render them
        for (int i = 0; i < NUM_SPRITES; ++i) {
            orbs[i].reset_force();
            orbs[i].move();
            orbs[i].render(renderer);
        }

        /* Update the screen! */
        SDL_RenderPresent(renderer);

        /* framerate 33 */
        SDL_Delay(30);
    }

    quit(0);

    return 0; /* to prevent compiler warning */
}

/* vi: set ts=4 sw=4 expandtab: */
