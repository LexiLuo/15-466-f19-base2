#include "StoryMode.hpp"

#include "Sprite.hpp"
#include "DrawSprites.hpp"
#include "Load.hpp"
#include "data_path.hpp"
#include "gl_errors.hpp"
#include "MenuMode.hpp"
#include "Sound.hpp"

Sprite const *kitchen_empty = nullptr;
Sprite const *kitchen_fire = nullptr;
Sprite const *kitchen_water = nullptr;
Sprite const *kitchen_tomato = nullptr;
Sprite const *kitchen_fire_water = nullptr;
Sprite const *kitchen_fire_tomato = nullptr;
Sprite const *kitchen_water_tomato = nullptr;
Sprite const *kitchen_fire_water_tomato = nullptr;
Sprite const *garden_empty = nullptr;
Sprite const *garden_tomato = nullptr;
Sprite const *canteen_empty = nullptr;
Sprite const *canteen_done = nullptr;

Load< SpriteAtlas > sprites(LoadTagDefault, []() -> SpriteAtlas const * {
	SpriteAtlas const *kret = new SpriteAtlas(data_path("kitchen"));
	kitchen_empty = &kret->lookup("kitchen-empty");
	kitchen_fire = &kret->lookup("kitchen-fire");
	kitchen_water = &kret->lookup("kitchen-water");
	kitchen_tomato = &kret->lookup("kitchen-tomato");
	kitchen_fire_water = &kret->lookup("kitchen-fire-water");
	kitchen_fire_tomato = &kret->lookup("kitchen-fire-tomato");
	kitchen_water_tomato = &kret->lookup("kitchen-water-tomato");
	kitchen_fire_water_tomato = &kret->lookup("kitchen-fire-water-tomato");
	garden_empty = &kret->lookup("garden-empty");
	garden_tomato = &kret->lookup("garden-tomato");
	canteen_empty = &kret->lookup("canteen-empty");
	canteen_done = &kret->lookup("canteen-done");
	return kret;
});

Load< Sound::Sample > music_relaxing(LoadTagDefault, []() -> Sound::Sample * {
	return new Sound::Sample(data_path("teafortwo-background.opus"));
});
Load< Sound::Sample > music_cook_done(LoadTagDefault, []() -> Sound::Sample * {
	return new Sound::Sample(data_path("cook-done.opus"));
});
Load< Sound::Sample > music_wow(LoadTagDefault, []() -> Sound::Sample * {
	return new Sound::Sample(data_path("wow.opus"));
});
Load< Sound::Sample > music_oh_no(LoadTagDefault, []() -> Sound::Sample * {
	return new Sound::Sample(data_path("oh-no.opus"));
});
Load< Sound::Sample > music_water_cook(LoadTagDefault, []() -> Sound::Sample * {
	return new Sound::Sample(data_path("water-cook.opus"));
});

StoryMode::StoryMode() {
}

StoryMode::~StoryMode() {
}

bool StoryMode::handle_event(SDL_Event const &, glm::uvec2 const &window_size) {
	if (Mode::current.get() != this) return false;

	return false;
}

void StoryMode::update(float elapsed) {
	if (Mode::current.get() == this) {
		//there is no menu displayed! Make one:
		enter_scene();
	}
	// if(location==CanteenDone){
	// 	// background_music->stop(1.0f/60.f);
	// 	wow_music = Sound::play(*music_wow, 1.0f);
	// }else 
	if (!background_music || background_music->stopped) {
		background_music = Sound::play(*music_relaxing, 1.0f);
	}
}

void StoryMode::enter_scene() {
	//just entered this scene, adjust flags and build menu as appropriate:
	std::vector< MenuMode::Item > items;
	glm::vec2 at(3.0f, view_max.y - 3.0f - 11.0f);
	auto add_text = [&items,&at](std::string text) {
		// while (text.size()) {
		// 	auto end = text.find('\n');
		// 	items.emplace_back(text, nullptr, 1.0f, glm::u8vec4(0x00, 0x00, 0x00, 0xff), nullptr, at);
		// 	at.y -= 15.0f;
		// 	if (end == std::string::npos) break;
		// 	text = text.substr(end+1);
		// }
		items.emplace_back(text, nullptr, 1.0f, glm::u8vec4(0x00, 0x00, 0x00, 0xff), nullptr, at);
		at.y -= 10.0f;
	};
	// auto add_choice = [&items,&at](std::string const &text, std::function< void(MenuMode::Item const &) > const &fn) {
	// 	// items.emplace_back(text, nullptr, 1.0f, glm::u8vec4(0x00, 0x00, 0x00, 0x88), fn, at + glm::vec2(16.0f, 0.0f));
	// 	// items.back().selected_tint = glm::u8vec4(0x00, 0x00, 0x00, 0xff);
	// 	items.emplace_back(text, nullptr, 1.0f, fn, at + glm::vec2(8.0f, 0.0f));
	// 	at.y -= 13.0f;
	// 	at.y -= 4.0f;
	// };

	auto add_choice = [&items,&at](std::string text, std::function< void(MenuMode::Item const &) > const &fn) {
		//assert(text);
		items.emplace_back(text, nullptr, 1.0f, glm::u8vec4(0x00, 0x00, 0x00, 0x88), fn, at + glm::vec2(16.0f, 0.0f));
		items.back().selected_tint = glm::u8vec4(0x00, 0x00, 0x00, 0xff);
		// items.back().selected_tint = glm::u8vec4(0x00, 0x00, 0x00, 0xff);
		//at.y -= text->max_px.y - text->min_px.y;
		at.y -= 15.0f;
	};
	if (location == CanteenEmpty) {
		add_choice("Go to the kitchen", [this](MenuMode::Item const &){
			location = kitchen_state;
			Mode::current = shared_from_this();
		});
		add_choice("Go to the garden", [this](MenuMode::Item const &){
			location = garden_state;
			Mode::current = shared_from_this();
		});
		at.y = view_min.y + 40.0f; //gap before choices
		add_text("Your customer want something to eat...");
	}else if (location == KitchenEmpty) {
		if (garden_state == GardenTomato) {
			add_text("You don't have any tomatos.");
		} else {
			add_text("You have enough tomatos.");
		}
		at.y -= 8.0f; //gap before choices
		add_choice("Add some water", [this](MenuMode::Item const &){
			location = Water;
			kitchen_state = Water;
			Mode::current = shared_from_this();
		});
		add_choice("Light fire", [this](MenuMode::Item const &){
			location = Fire;
			kitchen_state = Fire;
			Mode::current = shared_from_this();
		});
		if (garden_state == GardenEmpty) {
			add_choice("Add some tomatos", [this](MenuMode::Item const &){
				kitchen_state = Tomato;
				location = Tomato;
				Mode::current = shared_from_this();
			});
		}
		add_choice("Go to the garden", [this](MenuMode::Item const &){
			location = garden_state;
			Mode::current = shared_from_this();
		});
		add_choice("Go to the canteen", [this](MenuMode::Item const &){
			location = canteen_state;
			Mode::current = shared_from_this();
		});
		add_choice("Serve the dish",[this](MenuMode::Item const&){
			location = CanteenBad;
			Mode::current = shared_from_this();
		});
	}else if(location == GardenTomato){
		add_text("Oh, there are some tomatos!");
		at.y -= 8.0f;
		add_choice("Pick up some tomatos", [this](MenuMode::Item const &){
			garden_state = GardenEmpty;
			location = GardenEmpty;
			Mode::current = shared_from_this();
		});
		add_choice("Go to the kitchen",[this](MenuMode::Item const&){
			location = kitchen_state;
			Mode::current = shared_from_this();
		});
		add_choice("Go to the canteen",[this](MenuMode::Item const&){
			location = canteen_state;
			Mode::current = shared_from_this();
		});
	} else if (location == Tomato) {
		if (garden_state == GardenTomato) {
			add_text("You don't have any tomatos.");
		} else {
			add_text("You have enough tomatos.");
		}
		at.y -= 8.0f; //gap before choices
		add_choice("Add some water", [this](MenuMode::Item const &){
			location = WaterTomato;
			kitchen_state = WaterTomato;
			Mode::current = shared_from_this();
		});
		add_choice("Light fire", [this](MenuMode::Item const &){
			location = FireTomato;
			kitchen_state = FireTomato;
			Mode::current = shared_from_this();
		});
		add_choice("Go to the garden", [this](MenuMode::Item const &){
			location = garden_state;
			Mode::current = shared_from_this();
		});
		add_choice("Go to the canteen", [this](MenuMode::Item const &){
			location = canteen_state;
			Mode::current = shared_from_this();
		});
		add_choice("Serve the dish",[this](MenuMode::Item const&){
			location = CanteenBad;
			Mode::current = shared_from_this();
		});
	}else if (location == Water) {
		if (garden_state == GardenTomato) {
			add_text("You don't have any tomatos.");
		} else {
			add_text("You have enough tomatos.");
		}
		at.y -= 8.0f; //gap before choices
		add_choice("Light fire", [this](MenuMode::Item const &){
			location = FireWater;
			kitchen_state = FireWater;
			Mode::current = shared_from_this();
		});
		if (garden_state == GardenEmpty) {
			add_choice("Add some tomatos", [this](MenuMode::Item const &){
				kitchen_state = WaterTomato;
				location = WaterTomato;
				Mode::current = shared_from_this();
			});
		}
		add_choice("Go to the garden",[this](MenuMode::Item const&){
			location = garden_state;
			Mode::current = shared_from_this();
		});
		add_choice("Go to the canteen",[this](MenuMode::Item const&){
			location = canteen_state;
			Mode::current = shared_from_this();
		});
		add_choice("Serve the dish",[this](MenuMode::Item const&){
			location = CanteenBad;
			Mode::current = shared_from_this();
		});
	} else if (location == Fire) {
		if (garden_state == GardenTomato) {
			add_text("You don't have any tomatos.");
		} else {
			add_text("You have enough tomatos.");
		}
		at.y -= 8.0f; //gap before choices
		add_choice("Add some water", [this](MenuMode::Item const &){
			location = FireWater;
			kitchen_state = FireWater;
			Mode::current = shared_from_this();
		});
		if (garden_state == GardenEmpty) {
			add_choice("Add some tomatos", [this](MenuMode::Item const &){
				kitchen_state = FireTomato;
				location = FireTomato;
				Mode::current = shared_from_this();
			});
		}
		add_choice("Go to the garden",[this](MenuMode::Item const&){
			location = garden_state;
			Mode::current = shared_from_this();
		});
		add_choice("Go to the canteen",[this](MenuMode::Item const&){
			location = canteen_state;
			Mode::current = shared_from_this();
		});
		add_choice("Serve the dish",[this](MenuMode::Item const&){
			location = CanteenBad;
			Mode::current = shared_from_this();
		});
	}else if(location == GardenEmpty){
		add_text("Oh, there's no tomato in the garden!");
		at.y -= 8.0f;
		add_choice("Go to the kitchen",[this](MenuMode::Item const&){
			location = kitchen_state;
			Mode::current = shared_from_this();
		});
		add_choice("Go to the canteen",[this](MenuMode::Item const&){
			location = canteen_state;
			Mode::current = shared_from_this();
		});
	}else if(location == FireWater){
		if (garden_state == GardenTomato) {
			add_text("You don't have any tomatos.");
		} else {
			add_text("You have enough tomatos.");
		}
		at.y -= 8.0f; //gap before choices
		if (garden_state == GardenEmpty) {
			add_choice("Add some tomatos", [this](MenuMode::Item const &){
				kitchen_state = FireWaterTomato;
				location = FireWaterTomato;
				Mode::current = shared_from_this();
			});
		}
		add_choice("Go to the garden",[this](MenuMode::Item const&){
			location = garden_state;
			Mode::current = shared_from_this();
		});
		add_choice("Go to the canteen",[this](MenuMode::Item const&){
			location = canteen_state;
			Mode::current = shared_from_this();
		});
		add_choice("Serve the dish",[this](MenuMode::Item const&){
			location = CanteenBad;
			Mode::current = shared_from_this();
		});
	}else if(location == FireTomato){
		add_text("Oh, My tomato burned!");
		oh_no_music = Sound::play(*music_oh_no, 1.0f);
		add_choice("Start again", [this](MenuMode::Item const &){
			location = KitchenEmpty;
			kitchen_state = KitchenEmpty;
			garden_state = GardenTomato;
			canteen_state = CanteenEmpty;
			Mode::current = shared_from_this();
		});
	}else if(location == WaterTomato){
		add_choice("Light fire", [this](MenuMode::Item const &){
			location = FireWaterTomato;
			kitchen_state = FireWaterTomato;
			Mode::current = shared_from_this();
		});
		add_choice("Go to the garden",[this](MenuMode::Item const&){
			location = garden_state;
			Mode::current = shared_from_this();
		});
		add_choice("Go to the canteen",[this](MenuMode::Item const&){
			location = canteen_state;
			Mode::current = shared_from_this();
		});
		add_choice("Serve the dish",[this](MenuMode::Item const&){
			location = CanteenBad;
			Mode::current = shared_from_this();
		});
	}else if (location == FireWaterTomato) {
		water_cook_music = Sound::play(*music_water_cook, 4.0f);
		// sleep(2000);
	    if(water_cook_music && water_cook_music->stopped){
			cook_done_music = Sound::play(*music_cook_done, 2.0f);
		}
        
		// if(playtime < 0){
		// 	cook_done_music = Sound::play(*music_cook_done, 2.0f);
		// 	next_state = DishDone;
		// }else{
		//    playtime -= 1;
		//    next_state = CanteenBad;
		// }
		// sleep(2000);
		// water_cook_music->stop(2.0f);
		// if(water_cook_music->stopped){
		// 	cook_done_music = Sound::play(*music_cook_done, 2.0f);
		// 	next_state = DishDone;
		// 	// std::cout << "stop";
		// }else{
		// 	next_state = CanteenBad;
		// }
		add_choice("Serve the dish",[this](MenuMode::Item const&){
			location = CanteenBad;
			Mode::current = shared_from_this();
		});	

		add_choice("Wait for water boiling",[this](MenuMode::Item const&){
			// Sound::play(*music_relaxing, 1.0f);
			location = DishDone;
			Mode::current = shared_from_this();
		});
		// add_choice("Serve the dish",[this](MenuMode::Item const&){
		// 	location = DishDone;
		// 	Mode::current = shared_from_this();
		// });
	}else if (location == DishDone) {
		add_text("Your tomato soup is ready to serve.");
		cook_done_music = Sound::play(*music_cook_done, 2.0f);
		at.y -= 8.0f;
		add_choice("Serve the dish",[this](MenuMode::Item const&){
			location = CanteenDone;
			Mode::current = shared_from_this();
		});
	}
	else if (location == CanteenDone) {
		add_text("Yeah, your customer is very happy!");
		// background_music->stop(1.0f/60.0f);
		wow_music = Sound::play(*music_wow, 2.0f);
		at.y -= 8.0f;
		add_choice("Quit",[](MenuMode::Item const&){
			Mode::current = nullptr;
		});
	} else if (location == CanteenBad) {
		at.y -= 8.0f;
		add_text("Ah Oh, your customer is angry!!!");
		at.y -= 8.0f;
		oh_no_music = Sound::play(*music_oh_no, 1.0f);
		add_choice("Start again", [this](MenuMode::Item const &){
			location = KitchenEmpty;
			kitchen_state = KitchenEmpty;
			garden_state = GardenTomato;
			canteen_state = CanteenEmpty;
			Mode::current = shared_from_this();
		});
		add_choice("Quit",[](MenuMode::Item const&){
			Mode::current = nullptr;
		});
		at.y = view_min.y + 40.0f;
		add_text("Customer: 'What the heck is this!!!''");
	}

	std::shared_ptr< MenuMode > menu = std::make_shared< MenuMode >(items);
	menu->atlas = sprites;
	// menu->left_select = sprite_left_select;
	// menu->right_select = sprite_right_select;
	menu->view_min = view_min;
	menu->view_max = view_max;
	menu->background = shared_from_this();
	Mode::current = menu;

}

void StoryMode::draw(glm::uvec2 const &drawable_size) {
	//clear the color buffer:
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//use alpha blending:
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//don't use the depth test:
	glDisable(GL_DEPTH_TEST);

	{ //use a DrawSprites to do the drawing:
		DrawSprites draw(*sprites, view_min, view_max, drawable_size, DrawSprites::AlignPixelPerfect);
		glm::vec2 ul = glm::vec2(view_min.x, view_max.y);
		if (location == KitchenEmpty) {
			draw.draw(*kitchen_empty, ul);
		} else if (location == Fire) {
			draw.draw(*kitchen_fire, ul);
		} else if (location == Water) {
			draw.draw(*kitchen_water, ul);
		} else if (location == Tomato) {
			draw.draw(*kitchen_tomato, ul);
		} else if (location == FireWater) {
			draw.draw(*kitchen_fire_water, ul);
		} else if (location == FireTomato) {
			draw.draw(*kitchen_fire_tomato, ul);
		} else if (location == WaterTomato) {
			draw.draw(*kitchen_water_tomato, ul);
		} else if (location == FireWaterTomato) {
			draw.draw(*kitchen_fire_water_tomato, ul);
		} else if (location == GardenEmpty) {
			draw.draw(*garden_empty, ul);
		} else if (location == GardenTomato) {
			draw.draw(*garden_tomato, ul);
		} else if (location == CanteenEmpty) {
			draw.draw(*canteen_empty, ul);
		} else if (location == CanteenDone) {
			draw.draw(*canteen_done, ul);
		} else if (location == CanteenBad) {
			draw.draw(*canteen_empty, ul);
		} else if (location == DishDone) {
			draw.draw(*kitchen_fire_water_tomato, ul);
		}
	}
	GL_ERRORS(); //did the DrawSprites do something wrong?
}
