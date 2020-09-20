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
		std::cout << transform.name << std::endl;
		if (transform.name == "bacon") {
			bacon = &transform;
			scene_transforms.emplace_back("bacon", bacon);
			scene_base_positions.emplace_back("bacon", (&transform)->position);
		}
		else if (transform.name == "bacon.001") {
			bacon1 = &transform;
			scene_transforms.emplace_back("bacon1", bacon1);
			scene_base_positions.emplace_back("bacon1", (&transform)->position);
		}
		else if (transform.name == "bacon.002") {
			bacon2 = &transform;
			scene_transforms.emplace_back("bacon2", bacon2);
			scene_base_positions.emplace_back("bacon2", (&transform)->position);
		}
		else if (transform.name == "cake") {
			cake = &transform;
			scene_transforms.emplace_back("cake", cake);
			scene_base_positions.emplace_back("cake", (&transform)->position);
		}
		else if (transform.name == "sandwich") {
			sandwich = &transform;
			scene_transforms.emplace_back("sandwich", sandwich);
			scene_base_positions.emplace_back("sandwich", (&transform)->position);
		}
		else if (transform.name == "pancakes") {
			pancakes = &transform;
			scene_transforms.emplace_back("pancakes", pancakes);
			scene_base_positions.emplace_back("pancakes", (&transform)->position);
		}
		else if (transform.name == "cactus") {
			cactus = &transform;
			scene_transforms.emplace_back("cactus", cactus);
			scene_base_positions.emplace_back("cactus", (&transform)->position);
		}
		else if (transform.name == "egg") {
			egg = &transform;
			scene_transforms.emplace_back("egg", egg);
			scene_base_positions.emplace_back("egg", (&transform)->position);
		}
		else if (transform.name == "egg.001") {
			egg1 = &transform;
			scene_transforms.emplace_back("egg1", egg1);
			scene_base_positions.emplace_back("egg1", (&transform)->position);
		}
		else if (transform.name == "egg.002") {
			egg2 = &transform;
			scene_transforms.emplace_back("egg2", egg2);
			scene_base_positions.emplace_back("egg2", (&transform)->position);
		}
	}
	if (bacon == nullptr) throw std::runtime_error("bacon not found.");
	if (bacon1 == nullptr) throw std::runtime_error("bacon1 not found.");
	if (bacon2 == nullptr) throw std::runtime_error("bacon2 not found.");
	if (cake == nullptr) throw std::runtime_error("cake not found.");
	if (sandwich == nullptr) throw std::runtime_error("sandwich not found.");
	if (pancakes == nullptr) throw std::runtime_error("pancakes not found.");
	if (cactus == nullptr) throw std::runtime_error("cactus not found.");
	if (egg == nullptr) throw std::runtime_error("egg not found.");
	if (egg1 == nullptr) throw std::runtime_error("egg1 not found.");
	if (egg2 == nullptr) throw std::runtime_error("egg2 not found.");

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
		} else if (evt.key.keysym.sym == SDLK_a) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.downs += 1;
			down.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_p) {
			pick = true;
			// std::cout << "x: " + std::to_string(mousepicker.screen_x) + "y: " + 
			// 			std::to_string(mousepicker.screen_y) << std::endl;
			return true;
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
		} else if (evt.key.keysym.sym == SDLK_p) {
			pick = false;
			return true;
		}
	} else if (evt.type == SDL_MOUSEBUTTONDOWN) {
		if (SDL_GetRelativeMouseMode() == SDL_FALSE) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
			return true;
		}
	} else if (evt.type == SDL_MOUSEMOTION) {
		if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
			glm::vec2 motion = glm::vec2(
				evt.motion.xrel / float(window_size.y),
				-evt.motion.yrel / float(window_size.y)
			);
			camera->transform->rotation = glm::normalize(
				camera->transform->rotation
				* glm::angleAxis(-motion.x * camera->fovy, glm::vec3(0.0f, 1.0f, 0.0f))
				* glm::angleAxis(motion.y * camera->fovy, glm::vec3(1.0f, 0.0f, 0.0f))
			);
			return true;
		} else if (pick) {
			std::cout << "mouse motion while the p key is pressed" << std::endl;
			return true;
		}
	}

	return false;
}

void PlayMode::update(float elapsed) {
	//slowly rotates through [0,1):
	wobble += elapsed / 10.0f;
	wobble -= std::floor(wobble);

	// bacon->rotation = bacon_base_rotation * glm::angleAxis(
	// 	glm::radians(5.0f * std::sin(wobble * 2.0f * float(M_PI))),
	// 	glm::vec3(0.0f, 1.0f, 0.0f)
	// );
	// cake->rotation = cake_base_rotation * glm::angleAxis(
	// 	glm::radians(7.0f * std::sin(wobble * 2.0f * 2.0f * float(M_PI))),
	// 	glm::vec3(0.0f, 0.0f, 1.0f)
	// );
	// sandwich->rotation = sandwich_base_rotation * glm::angleAxis(
	// 	glm::radians(10.0f * std::sin(wobble * 3.0f * 2.0f * float(M_PI))),
	// 	glm::vec3(0.0f, 0.0f, 1.0f)
	// );

	//move cake
	{
		std::cout << "current selected: " + std::get<0>(scene_base_positions[selected]) << std::endl;
		Scene::Transform* currentTransform = std::get<1>(scene_transforms[selected]);
		currentTransform->position = std::get<1>(scene_base_positions[selected]) + glm::vec3(0.0f, 10.f, 0.0f);
	}

	//move camera:
	{

		//combine inputs into a move:
		constexpr float PlayerSpeed = 30.0f;
		glm::vec2 move = glm::vec2(0.0f);
		if (left.pressed && !right.pressed) move.x =-1.0f;
		if (!left.pressed && right.pressed) move.x = 1.0f;
		if (down.pressed && !up.pressed) move.y =-1.0f;
		if (!down.pressed && up.pressed) move.y = 1.0f;

		//make it so that moving diagonally doesn't go faster:
		if (move != glm::vec2(0.0f)) move = glm::normalize(move) * PlayerSpeed * elapsed;

		glm::mat4x3 frame = camera->transform->make_local_to_parent();
		glm::vec3 right = frame[0];
		//glm::vec3 up = frame[1];
		glm::vec3 forward = -frame[2];

		camera->transform->position += move.x * right + move.y * forward;
	}

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
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
	glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
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
		lines.draw_text("Mouse motion rotates camera; WASD moves; escape ungrabs mouse",
			glm::vec3(-aspect + 0.1f * H, -1.0 + 0.1f * H, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00));
		float ofs = 2.0f / drawable_size.y;
		lines.draw_text("Mouse motion rotates camera; WASD moves; escape ungrabs mouse",
			glm::vec3(-aspect + 0.1f * H + ofs, -1.0 + + 0.1f * H + ofs, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));
	}
}
