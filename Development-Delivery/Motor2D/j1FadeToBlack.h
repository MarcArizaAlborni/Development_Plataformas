#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class j1FadeToBlack : public j1Module
{
public:
	j1FadeToBlack();

	// Destructor
	virtual~j1FadeToBlack();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Calling FadeToBlack funtion
	bool FadeToBlack(const char* mapname, bool restartlvl, float time = 2.0f);

private:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	uint			start_time = 0;
	uint			total_time = 0;
	SDL_Rect		screen;
	
	bool restartlvl = true;
	const char* mapname;
};