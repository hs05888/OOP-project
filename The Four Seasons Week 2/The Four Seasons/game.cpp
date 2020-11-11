#include "game.hpp"
bool Game::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow("The Years @ HU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL)
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

SDL_Texture* Game::loadTexture(std::string path){
    //Final Texture
    SDL_Texture* newTexture = NULL;

    //Load image present at specific path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    
    if (loadedSurface == NULL)
	{
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());

    }
    else
	{
        //Craete a texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

        if (newTexture == NULL)
		{
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Getting rid of the old loaded surface
        SDL_FreeSurface(loadedSurface);

    }
    return newTexture;
}

bool Game::loadMedia()
{

    //Load the success flag
    bool success = true;

    assets = loadTexture("assets/mon2_sprite_base.png");
	ground = loadTexture("assets/ground.png");
    gTexture = loadTexture("assets/snow.png");
	player = loadTexture("assets/mon2_sprite_base.png");

    if(assets == NULL || gTexture == NULL || ground == NULL || player == NULL)
	{
        printf("Unable to run due to error: %s\n",SDL_GetError());
        success = false;
    }

    bgMusic = Mix_LoadMUS("snowflake-waltz.mp3");

    if(bgMusic == NULL)
	{
        printf("Unable to load music: %s \n", Mix_GetError());
        success = false;
    }
    
    return success;
}

void Game::run()
{

    bool quit;
    SDL_Event e;

    while (!quit)
	{

        //Handling the events
        while(SDL_PollEvent(&e) != 0)
		{

            //Check if user requests quit
            if(e.type == SDL_QUIT)
			{
                quit = true;
            }
        }

        if (Mix_PlayingMusic() == 0)
		{

            //Play the music
            Mix_PlayMusic(bgMusic, 2);
        }

        SDL_RenderClear(gRenderer); //This removes each and everything from the renderer
        SDL_RenderCopy(gRenderer, gTexture, NULL, NULL); //This draws the background to the renderer
		SDL_RenderCopy(gRenderer, ground, NULL, NULL);
		SDL_RenderCopy(gRenderer, player, &player_srcRect, &player_moverRect);
        //The objects shall be drawn here

        SDL_RenderPresent(gRenderer); //This displays the updated renderer

        SDL_Delay(200);
    }
}

void Game::close()
{
	//Free loaded images
	SDL_DestroyTexture(assets);
	assets=NULL;
	SDL_DestroyTexture(ground);
	ground = NULL;
	SDL_DestroyTexture(gTexture);
	player = NULL;
	SDL_DestroyTexture(player);
	
	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	Mix_FreeMusic(bgMusic);
	bgMusic = NULL;
	//Quit SDL subsystems
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}
