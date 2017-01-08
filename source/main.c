//repo: https://github.com/9999years/i-c-the-light
//MIT/expat license
//rebecca turner
//consult ../readme.md

//render code
#define USE_REAL_SDL
#include "main.h"

void saveframe(SDL_Surface *screen)
{
	char filename[256] = "output/UNINITIALIZED.ppm";
	unsigned long int timeint = time(NULL);
	sprintf(filename, "../output/image%lu_f%d.ppm", timeint, frame);
	printf("printing %s\n", filename);
	int writestatus;
	if(
		(writestatus = writeppm(
			filename,
			screen->w,
			screen->h,
			screen->pixels
		)) != 0
	) {
		switch(writestatus) {
		case PPM_FILE_OPEN_FAILURE:
			fprintf(logfile, "image write error!\n");
			break;
		case PPM_WRITE_ALL_BLACK:
			fprintf(logfile, "image is all black!\n");
			break;
		default:
			fprintf(logfile, "unknown write error!\n");
			break;
		}
		return;
	}
	char shacmd[256];
	sprintf(shacmd, "sha256sum %s", filename);
	FILE *file = popen(shacmd, "r");
	if(file == NULL) {
		fprintf(logfile, "failed to get image hash!\n");
		return;
	}
	//the hash is 64 characters but we need a 0 at the end too
	char sha[68];
	int i;
	char c;
	for(i = 0; (i < 64) && (c != EOF) && (c != 0x20); i++) {
		sha[i] = c = fgetc(file);
	}
	sha[i++] = 0;
	printf("SHA: %s\n", sha);
	fprintf(logfile, "SHA: %s\n", sha);
	pclose(file);

	char hashfilename[256];
	sprintf(hashfilename, "../output/hash/%s", sha);

	if(_access(hashfilename, 0) != -1) {
		//file exists, delete img
		printf("image already rendered, deleting\n");
		if(unlink(filename) != 0) {
			fprintf(logfile, "image delete error!\n");
		}
	} else {
		FILE *hashfile = fopen(hashfilename, "w");
		if(hashfile == NULL)
			fprintf(logfile,
				"hash file write error!\nfilename: %s\n",
				hashfilename
			);
		fclose(hashfile);
	}
	return;
}

int handleevents(SDL_Surface *screen)
{
	SDL_Event event;
	while((SDL_PollEvent(&event))) {
		switch(event.type) {
		case SDL_KEYDOWN:
			// if escape is pressed, quit
			if((event.key.keysym.sym == SDLK_ESCAPE) || (event.key.keysym.sym == SDLK_q)) {
				return 1;
			} else if(event.key.keysym.sym == SDLK_s) {
				saveframe(screen);
			}
			break;

		case SDL_QUIT:
			return 1;
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	printf("Hello!\n");
	const time_t unixtime = time(NULL);
	printf( "I C the Light by Rebecca Turner (MIT/Expat)\n"
		"%s\n", ctime(&unixtime)
	);

	printf(
		"               , .\n"
		"           \\.\\_____/./\n"
		"         \\-.=^ -  '^=././\n"
		"    ===\\-\\(/`  /#\\  *\\)/-/===\n"
		"        <{.~  (#O#)   .}>\n"
		"    ===/-/(\\  .\\#/  ~/)\\-\\===\n"
		"        /-/*=-_____-=*\\^\\\n"
		"           -/ /*~*\\^ \\\n"
		"                  \\n"
		"                                       \n"
		"╭─────────────────────────────────────╮\n"
		"│ ·           ╷         ╷ ·     ╷     │\n"
		"│ ╷   ╭─╮   ┼ ├─╮ ╭─╮   │ ╷ ╭─╮ ├─╮ ┼ │\n"
		"│ │   │     │ │ │ ├─┘   │ │ ╰─╯ │ │ │ │\n"
		"│ ╵   ╰─╯   ╰ ╵ ╵ ╰─╯   ╵ ╵ ╰─╮ ╵ ╵ ╰ │\n"
		"╰───────────────────────────╰─╯───────╯\n"
		"                                       \n"
	);

	frame = 0;

	initializelogfile();

	//The window we'll be rendering to
	SDL_Window *window = NULL;

	//The surface contained by the window
	SDL_Surface *screen = NULL;

	char *window_arg = "window";

	if(searchargs(argc, argv, window_arg) == 1) {
		//Initialize SDL
		if(SDL_Init(SDL_INIT_VIDEO) < 0) {
			printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		} else {
			//Create window
			window = SDL_CreateWindow(
				"I C the Light",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				SDL_WINDOW_SHOWN
			);
			if(window == NULL) {
				printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			} else {
				//Get window surface
				screen = SDL_GetWindowSurface(window);
			}
		}
	} else {
		unsigned int rmask, gmask, bmask, amask;
#define SCREEN_BIT_DEPTH 32
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
#endif
		screen = SDL_CreateRGBSurface(
			0,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SCREEN_BIT_DEPTH, rmask, gmask, bmask, amask
		);
	}

	if(screen == NULL) {
		printf("Render surface undefined, quitting!\n");
		return -1;
	}

	if(window != NULL) {
		SDL_FillRect(screen, NULL, 0x000000);
		SDL_UpdateWindowSurface(window);
	}

	int quit = 0;
	clock_t start, end;
	double total;
	do {
		start = clock();
		//printf("frame: %d\n", frame);

		//poll for events, and handle the ones we care about.
		//this returns 1 if we need to quit
		quit = handleevents(screen);
		quit = 1;

		// render
		//if(frame == 0) {
			render(screen, frame);
			saveframe(screen);
		//}

		//Update the surface
		if(window != NULL) {
			SDL_UpdateWindowSurface(window);
		}

		end = clock();
		//if(frame%30 == 0) {
			////if(frame == 0)
				////saveframe(screen);
		//}
		total = (double)(end - start) / CLOCKS_PER_SEC;
		printf("%.4f FPS = %.4f SPF\n", 1 / total, total);
		frame++;
		//if(frame > FRAMES_IN_ROTATION + 1) {
			//quit = 1;
		//}
	} while(!quit);

	//Destroy window
	if(window != NULL) {
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();

	if(screen != NULL) {
		free(screen);
	}

	return 0;
}
