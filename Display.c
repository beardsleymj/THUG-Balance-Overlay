#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class Display
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* sans;
	SDL_Rect pixel;
	SDL_Event event;
	Mix_Music* beep;

public:
	Renderer();
	//void drawText(const char* text);
	void drawPixels(bool[][32]);
	std::string getROM();
	bool checkChangeRom(std::string& rom);
	void playSound();
	void stopSound();
	~Renderer();
};
