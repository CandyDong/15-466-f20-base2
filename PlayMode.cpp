#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/ext.hpp>

#include <random>

GLuint hexapod_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > hexapod_meshes(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("brunch.pnct"));
	hexapod_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > hexapod_scene(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("brunch.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = hexapod_meshes->lookup(mesh_name);

		scene.drawables.emplace_back(transform);
		Scene::Drawable &drawable = scene.drawables.back();

		drawable.pipeline = lit_color_texture_program_pipeline;

		drawable.pipeline.vao = hexapod_meshes_for_lit_color_texture_program;
		drawable.pipeline.type = mesh.type;
		drawable.pipeline.start = mesh.start;
		drawable.pipeline.count = mesh.count;

	});
});

PlayMode::PlayMode() : scene(*hexapod_scene) {
	//get pointers to leg for convenience:
	for (auto &transform : scene.transforms) {
		// std::cout << transform.name << std::endl;
		if (transform.name == "bacon") {
			bacon = &transform;
			objects.emplace_back("bacon");
			scene_transforms.emplace_back(bacon);
			scene_base_positions.emplace_back(bacon->position);
		}
		else if (transform.name == "bacon.001") {
			bacon1 = &transform;
			objects.emplace_back("bacon.001");
			scene_transforms.emplace_back(bacon1);
			scene_base_positions.emplace_back(bacon1->position);
		}
		else if (transform.name == "bacon.002") {
			bacon2 = &transform;
			objects.emplace_back("bacon.002");
			scene_transforms.emplace_back(bacon2);
			scene_base_positions.emplace_back(bacon2->position);
		}
		else if (transform.name == "cake") {
			cake = &transform;
			objects.emplace_back("cake");
			scene_transforms.emplace_back(cake);
			scene_base_positions.emplace_back(cake->position);
		}
		else if (transform.name == "sandwich") {
			sandwich = &transform;
			objects.emplace_back("sandwich");
			scene_transforms.emplace_back(sandwich);
			scene_base_positions.emplace_back(sandwich->position);
		}
		else if (transform.name == "pancakes") {
			pancakes = &transform;
			objects.emplace_back("pancakes");
			scene_transforms.emplace_back(pancakes);
			scene_base_positions.emplace_back(pancakes->position);
		}
		else if (transform.name == "egg") {
			egg = &transform;
			objects.emplace_back("egg");
			scene_transforms.emplace_back(egg);
			scene_base_positions.emplace_back(egg->position);
		}
		else if (transform.name == "egg.001") {
			egg1 = &transform;
			objects.emplace_back("egg.001");
			scene_transforms.emplace_back(egg1);
			scene_base_positions.emplace_back(egg1->position);
		}
		else if (transform.name == "egg.002") {
			egg2 = &transform;
			objects.emplace_back("egg.002");
			scene_transforms.emplace_back(egg2);
			scene_base_positions.emplace_back(egg2->position);
		} 
		else if (transform.name == "table") {
			table = &transform;
			table_base_position = (&transform)->position;
		}
	}
	if (bacon == nullptr) throw std::runtime_error("bacon not found.");
	if (bacon1 == nullptr) throw std::runtime_error("bacon1 not found.");
	if (bacon2 == nullptr) throw std::runtime_error("bacon2 not found.");
	if (cake == nullptr) throw std::runtime_error("cake not found.");
	if (sandwich == nullptr) throw std::runtime_error("sandwich not found.");
	if (pancakes == nullptr) throw std::runtime_error("pancakes not found.");
	if (egg == nullptr) throw std::runtime_error("egg not found.");
	if (egg1 == nullptr) throw std::runtime_error("egg1 not found.");
	if (egg2 == nullptr) throw std::runtime_error("egg2 not found.");
	if (table == nullptr) throw std::runtime_error("table not found.");

	// bacon_base_rotation = bacon->rotation;
	// cake_base_rotation = cake->rotation;
	// sandwich_base_rotation = sandwich->rotation;

	// bacon_base_position = bacon->position;
	// cake_base_position = cake->position;
	// sandwich_base_position = sandwich->position;
	// pancakes_base_position = pancakes->position;
	// cactus_base_position = cactus->position;
	// egg_base_position = egg->position;

	//get pointer to camera for convenience:
	if (scene.cameras.size() != 1) throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	camera = &scene.cameras.front();
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
	
	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_ESCAPE) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.downs += 1;
			down.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_LEFT) {
			if (selected > 0) {
				selected -= 1;
			} else {
				selected = scene_transforms.size()-1;
			}
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			if (selected < scene_transforms.size()) {
				selected += 1;
			} else {
				selected = 0;
			}
		} 
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_a) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.pressed = false;
			return true;
		} 
	} 

	return false;
}

void PlayMode::update(float elapsed) {
	if (game_over) {
		light_on = true;
		return;
	}
	//slowly rotates through [0,1):
	wobble += elapsed / 10.0f;
	wobble -= std::floor(wobble);

	if (light_toggle > 0.0f && light_toggle - elapsed < 0.0f) {
		light_on = !light_on;
		light_toggle = 2.0f;
		// scene.drawables.erase(scene.drawables.begin());
	} else {
		light_toggle -= elapsed;
	}

	if (light_on) {
		for (int i = 0; i < scene_transforms.size(); i++) {
			if (scene_transforms[i]->position != scene_base_positions[i]) {
				game_over = true;
				light_on = true;
				return;
			}
		}
		return;
	}
	//move selected object
	if (up.pressed) {
		(scene_transforms[selected])->position += glm::vec3(0.0f, 1.0f, 0.0f);
		
		if ((scene_transforms[selected])->position.y >= 50.0f) {
			for (auto itr = scene.drawables.begin(); itr != scene.drawables.end(); itr++) {
				std::string name = itr->transform->name;
				if (objects[selected] == name) {
					scene.drawables.erase(itr);
				}
			}

			scene_transforms.erase(scene_transforms.begin()+selected);
			objects.erase(objects.begin()+selected);
			scene_base_positions.erase(scene_base_positions.begin()+selected);
			selected = 0;

			if (objects.size() == 0) {
				success = true;
			}
			return;
		}
	} else if (down.pressed) {
		(scene_transforms[selected])->position -= glm::vec3(0.0f, 1.0f, 0.0f);
		if (((scene_transforms[selected])->position).y < scene_base_positions[selected].y) {
			(scene_transforms[selected])->position = scene_base_positions[selected];
		} 
	}
	

	//move camera:
	{

		//combine inputs into a move:
		// constexpr float PlayerSpeed = 30.0f;
		// glm::vec2 move = glm::vec2(0.0f);
		// if (left.pressed && !right.pressed) move.x =-1.0f;
		// if (!left.pressed && right.pressed) move.x = 1.0f;
		// if (down.pressed && !up.pressed) move.y =-1.0f;
		// if (!down.pressed && up.pressed) move.y = 1.0f;

		//make it so that moving diagonally doesn't go faster:
		// if (move != glm::vec2(0.0f)) move = glm::normalize(move) * PlayerSpeed * elapsed;

		// glm::mat4x3 frame = camera->transform->make_local_to_parent();
		// glm::vec3 right = frame[0];
		// //glm::vec3 up = frame[1];
		// glm::vec3 forward = -frame[2];

		// camera->transform->position += move.x * right + move.y * forward;
	}

	//reset button press counters:
	up.downs = 0;
	down.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//update camera aspect ratio for drawable:
	camera->aspect = float(drawable_size.x) / float(drawable_size.y);

	//set up light type and position for lit_color_texture_program:
	// TODO: consider using the Light(s) in the scene to do this
	glUseProgram(lit_color_texture_program->program);
	glUniform1i(lit_color_texture_program->LIGHT_TYPE_int, 1);
	GL_ERRORS();
	glUniform3fv(lit_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f,-1.0f)));
	GL_ERRORS();
	if (!light_on) {
		glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
	} else {
		glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	}
	
	GL_ERRORS();
	glUseProgram(0);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	scene.draw(*camera);

	{ //use DrawLines to overlay some text:
		glDisable(GL_DEPTH_TEST);
		float aspect = float(drawable_size.x) / float(drawable_size.y);
		DrawLines lines(glm::mat4(
			1.0f / aspect, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		));
		constexpr float H = 0.09f;
		float ofs = 2.0f / drawable_size.y;

		if (game_over) {
			//print game over text
			lines.draw_text("You are caught! Game over!",
			glm::vec3(-0.5f + 0.1f * H,0.0f, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0x00, 0x00, 0x00));
			lines.draw_text("You are caught! Game over!",
			glm::vec3(-0.5f + 0.1f * H + ofs, ofs, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));
		}
		if (success) {
			// print success text
			lines.draw_text("Success!!",
			glm::vec3(-0.5f + 0.1f * H,0.0f, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0x00, 0x00, 0x00));
			lines.draw_text("Success!!",
			glm::vec3(-0.5f + 0.1f * H + ofs, ofs, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));
		}

		
		lines.draw_text("Left/Right arrow to select food; WASD moves selected food.",
			glm::vec3(-aspect + 0.1f * H, -1.0 + 0.1f * H, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00));
		lines.draw_text("Left/Right arrow to select food; WASD moves selected food.",
			glm::vec3(-aspect + 0.1f * H + ofs, -1.0 + 0.1f * H + ofs, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));
		//print selected object
		lines.draw_text("Current Selection: " + objects[selected],
			glm::vec3(-aspect + 0.1f*H, 1.0 - H, 0.0f),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00));
		lines.draw_text("Current Selection: " + objects[selected],
			glm::vec3(-aspect + 0.1f*H + ofs, 1.0 - H + ofs, 0.0f),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));
	}
}
