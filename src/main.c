//repo: https://github.com/9999years/i-c-the-light
//MIT/expat license
//rebecca turner
//consult ../readme.md

//render code
#include "main.h"

void saveframe(SDL_Surface *screen, byte opacity)
{
	char filename[256] = "UNINITIALIZED.ppm";
	unsigned long int timeint = time(NULL);
	sprintf(filename, "%s/image%lu_f%d.ppm", outfile_base, timeint, frame);
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

	FILE *file;
	if(!FLAG(NO_HASH)) {
		char shacmd[256];
		sprintf(shacmd, "sha256sum %s", filename);
		file = popen(shacmd, "r");
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
		sprintf(hashfilename, "%s/hash/%s", outfile_base, sha);

		if(_access(hashfilename, 0) != -1) {
			//file exists, delete img
			printf("image already rendered, deleting\n");
			if(unlink(filename) != 0) {
				printf("image delete error!\n");
			}
		} else {
			FILE *hashfile = fopen(hashfilename, "w");
			if(hashfile == NULL)
				printf(
					"hash file write error!\nfilename: %s\n",
					hashfilename
				);
			fclose(hashfile);
		}
	}

	if(FLAG(CONVERT_IMMEDIATELY)) {
		printf("converting ppm to png\n");
		char mogrifycmd[512];
		sprintf(mogrifycmd, "mogrify -format png %s", filename);
		file = popen(mogrifycmd, "r");
		if(file == NULL) {
			printf("failed to get image hash!\n");
			return;
		}
		pclose(file);
		if(unlink(filename) != 0) {
			printf("ppm delete error!\n");
			return;
		}
	}
	return;
}

void printhelp()
{
	printf(
"I C the Light help:\n\n"

"-a or --antialiasing: Use SSAA to render a smoother image. Will cause render"
"    times to increase linearly. Positive integer values only.\n\n"

"-c or --convert: Convert the .ppm output to .png after saving. Might take up\n"
"    a little bit of time, requires mogrify to be in your PATH\n\n"

"-e or --exit: Exit after rendering one frame.\n\n"

"--frame: Specify a starting frame. Can be useful for re-starting a render.\n\n"

"--help: Shows this text and then quits.\n\n"

"-i or --iterations: Specify an iteration count. Default 64\n\n"

"--nobanner: Don't show my beautiful, glorious banner that I made by hand,\n"
"    and love with all of my heart. Put all that glorious work to waste.\n\n"

"--nohash: Don't hash images to check for collisions.\n\n"

"-o or --output: Output directory (where images are written).\n"
"    ./output/ by default, must have a hash/ subdirectory.\n\n"

"-q or --quaternion: Specify a quaternion to render.\n"
"    Format: 'Float, Float, Float, Float', where a Float consists of numbers\n"
"    0-9, with an optional decimal (.) or leading negation sign (-).\n"
"    All other characters are ignored, so make sure you don't write in\n"
"    garbage for no reason.\n\n"

"-r or --resolution: Specifies resolution via next argument, in wxh format.\n"
"    e.g. ./icthelight -r 500x500\n\n"

"-s or --scale: Specifies resolution scaling. (e.g. -r 500x500 -s 2 will render\n"
"    a 1000x1000 image.) Will also accept fractional values, for draft renders.\n\n"

"-w or --window: Request a window. (Does nothing if I C the Light was compiled\n"
"    without SDL.)\n\n"

"../readme.md, becca.ooo/i-c-the-light, or github.com/9999years/i-c-the-light\n"
"might have more information.\n"
	);
	return;
}

quaternion parsequaternion(char *str)
{
	enum quaternion_indexes {
		QUATERNION_R,
		QUATERNION_A,
		QUATERNION_B,
		QUATERNION_C
	};
	int i, j = 0, k;
	float tmp;
	k = QUATERNION_R;
	char number_str[64] = "\0";
	quaternion ret = constq(0.0F, 0.0F, 0.0F, 0.0F);
	for(i = 0; k <= QUATERNION_C; i++) {
		if(
			((str[i] >= 0x30) && //0
			(str[i] <= 0x39)) || //9
			(str[i] == 0x2e)  || //.
			(str[i] == 0x2d)     //-
		) {
			//build string out of numbers
			number_str[j] = str[i];
			j++;
		} else if(
			(str[i] == 0x2c) ||
			(str[i] == 0)
		) {
			//comma or end
			//"finalize" string, reset index
			number_str[j + 1] = '\0';
			j = 0;
			//parse number, set to next quat index
			tmp = strtof(number_str, NULL);
			switch(k) {
			case QUATERNION_R:
				ret.r = tmp;
				break;
			case QUATERNION_A:
				ret.a = tmp;
				break;
			case QUATERNION_B:
				ret.b = tmp;
				break;
			case QUATERNION_C:
				ret.c = tmp;
				break;
			}
			//increment quat index
			k++;
			if(str[i] == 0) {
				//dont fuck the memory
				break;
			}
		}
	}
	return ret;
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

	if(searchargs(argc, argv, "--nobanner") == -1) {
		printf( "I C the Light by Rebecca Turner (MIT/Expat)\n"
			"See --help\n"
		);

		printf(
			"                  .V.                  \n"
			"              \\.\\_____/./            \n"
			"            \\-.=^ -  '^=././          \n"
			"       ===\\-\\(/`  /#\\  *\\)/-/===   \n"
			" >=------  <{.~  (#O#)   .}>  ------=< \n"
			"       ===/-/(\\  .\\#/  ~/)\\-\\===   \n"
			"           /-/*=-_____-=*\\^\\         \n"
			"              -/ /*~*\\^ \\            \n"
			"                   ^                   \n"
			"                                       \n"
			"╭─────────────────────────────────────╮\n"
			"│ ·           ╷         ╷ ·     ╷     │\n"
			"│ ╷   ╭─╮   ┼ ├─╮ ╭─╮   │ ╷ ╭─╮ ├─╮ ┼ │\n"
			"│ │   │     │ │ │ ├─┘   │ │ ╰─╯ │ │ │ │\n"
			"│ ╵   ╰─╯   ╰ ╵ ╵ ╰─╯   ╵ ╵ ╰─╮ ╵ ╵ ╰ │\n"
			"╰───────────────────────────╰─╯───────╯\n"
			"                                       \n"
		);
	}

	if(searchargs(argc, argv, "--help") != -1) {
		printhelp();
		return 0;
	}

	//don't want stuff to be the same every time...
	srand(unixtime);

	flags = 0x0000;

	initializelogfile();

	//The window we'll be rendering to
	SDL_Window *window = NULL;

	//The surface contained by the window
	SDL_Surface *screen = NULL;

	//screen w/h
	int width, height;

	int inx = searchargspair(argc, argv, "-r", "--resolution");
	if(inx != -1) {
		//should allow up to 5-digits each side
		char resolution_input[16];
		//put str into memory
		strcpy(resolution_input, argv[inx + 1]);
		if(resolution_input == NULL) {
			printf("couldn't get a resolution! try again with a "
				"better argument.\n");
			return -1;
		}
		//get the string after the x
		//(so in 100x100, we get "x100")
		char *second = strchr(resolution_input, 'x');
		if(second == NULL) {
			//no x, assume square res
			height = width = strtod(resolution_input, NULL);
		} else {
			//add one byte to strip off the leading x, get a number
			height = strtod(second + sizeof(char), NULL);
			//set the 'x' to \0 to make c think that's the end of
			//the string
			second = '\0';
			//get the first number
			width = strtod(resolution_input, NULL);
		}
	} else {
		//compile-time defaults
		width = SCREEN_WIDTH;
		height = SCREEN_WIDTH;
	}

	inx = searchargspair(argc, argv, "-s", "--scale");
	if(inx != -1) {
		//god help you if 16 chars is too little????
		char scale_input[16];
		strcpy(scale_input, argv[inx + 1]);
		if(scale_input == NULL) {
			printf("couldn't get a scale! try again with a "
				"better argument.\n");
			return -1;
		}
		float scale = strtof(scale_input, NULL);
		width = (float)width * scale;
		height = (float)height * scale;
	}

	if(
		(width < 1) ||
		(height < 1)
	) {
		printf(
			"Bad resolution! You tried to render at %dx%d.\n"
			"Cya!\n",
			width, height
		);
		return -1;
	}

	inx = searchargspair(argc, argv, "-a", "--antialiasing");
	if(inx != -1) {
		//already a global, for better or worse
		antialias = strtod(argv[inx + 1], NULL);
		if(antialias < 1) {
			printf(
				"Bad antialiasing value!\n"
				"Cya!\n"
			);
			return -1;
		}
	} else {
		antialias = 1;
	}

	inx = searchargspair(argc, argv, "-q", "--quaternion");
	if(inx != -1) {
		juliaconstant = parsequaternion(argv[inx + 1]);
		FLAGSET(USER_QUATERNION);
	}

	inx = searchargspair(argc, argv, "-i", "--iterations");
	if(inx != -1) {
		//god help you if 16 chars is too little????
		char iterations_input[16];
		strcpy(iterations_input, argv[inx + 1]);
		if(iterations_input == NULL) {
			printf("couldn't get a scale! try again with a "
				"better argument.\n");
			return -1;
		}
		//already a global, for better or worse
		iterations = strtod(iterations_input, NULL);
	} else {
		iterations = 128;
	}

	inx = searchargs(argc, argv, "--frame");
	if(inx != -1) {
		//god help you if 16 chars is too little????
		char frame_input[16];
		strcpy(frame_input, argv[inx + 1]);
		if(frame_input == NULL) {
			printf("couldn't get a frame! try again with a "
				"better argument.\n");
			return -1;
		}
		//already a global, for better or worse
		frame = strtod(frame_input, NULL);
	} else {
		frame = 0;
	}

	inx = searchargs(argc, argv, "--nohash");
	if(inx != -1) {
		FLAGSET(NO_HASH);
	}

	inx = searchargspair(argc, argv, "-o", "--output");
	if(inx != -1) {
		strcpy(outfile_base, argv[inx + 1]);
	} else {
		strcpy(outfile_base, "./output/");
	}

	inx = searchargspair(argc, argv, "-c", "--convert");
	if(inx != -1) {
		FLAGSET(CONVERT_IMMEDIATELY);
	}

	inx = searchargspair(argc, argv, "-e", "--exit");
	if(inx != -1) {
		FLAGSET(QUIT_IMMEDIATELY);
	}

	//set up the window
	if(
		searchargspair(argc, argv, "-w", "--window") != -1
	) {
		//Initialize SDL
		if(SDL_Init(SDL_INIT_VIDEO) < 0) {
			printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		} else {
			//Create window
			window = SDL_CreateWindow(
				"I C the Light",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				width, height,
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
			width, height,
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

	int quit;
	clock_t start, end;
	double total;
	do {
		start = clock();

		//poll for events, and handle the ones we care about.
		//this returns 1 if we need to quit
		quit = handleevents(screen);

		// render
		render(screen, frame);
		saveframe(screen);

		//Update the surface
		if(window != NULL) {
			SDL_UpdateWindowSurface(window);
		}

		end = clock();
		total = (double)(end - start) / CLOCKS_PER_SEC;
		printf("%.4f FPS = %.4f SPF\n", 1 / total, total);
		frame++;
		if(frame > ANIMATION_LENGTH + 1) {
			break;
		}
	} while(!quit && !FLAG(QUIT_IMMEDIATELY));

	printf("I feel free!\n");

	//Destroy window
	if(window != NULL) {
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();

	if(screen != NULL) {
		free(screen);
	}

	printf("Goodbye!\n");

	return 0;
}
