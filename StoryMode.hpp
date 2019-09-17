
/*
 * StoryMode implements a story about The Planet of Choices.
 *
 */

#include "Mode.hpp"
#include "Sound.hpp"

struct StoryMode : Mode {
	StoryMode();
	virtual ~StoryMode();

	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//called to create menu for current scene:
	void enter_scene();

	//------ story state -------
	enum {
		KitchenEmpty,
		Fire,
		Water,
		Tomato,
		FireTomato,
		FireWater,
		WaterTomato,
		FireWaterTomato,
		GardenEmpty,
		GardenTomato,
		CanteenEmpty,
		CanteenDone,
		CanteenBad,
		DishDone
	} location = CanteenEmpty, garden_state = GardenTomato, kitchen_state = KitchenEmpty, canteen_state = CanteenEmpty, next_state = CanteenBad;
	
	
	glm::vec2 view_min = glm::vec2(0,0);
	glm::vec2 view_max = glm::vec2(259, 225);

	//------ background music -------
	std::shared_ptr< Sound::PlayingSample > background_music;
	std::shared_ptr< Sound::PlayingSample > cook_done_music;
	std::shared_ptr< Sound::PlayingSample > wow_music;
	std::shared_ptr< Sound::PlayingSample > oh_no_music;
	std::shared_ptr< Sound::PlayingSample > water_cook_music;
	int playtime = 2;
};
