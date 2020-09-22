#include "Mode.hpp"
// #include "MousePicker.hpp"

#include "Scene.hpp"
// Include the string library
#include <string>

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	bool game_over = false;
	bool success = false;
	bool light_on = true;
	int8_t selected = 0;

	float light_toggle = 2.0f;

	//local copy of the game scene (so code can change it during gameplay):
	Scene scene;
	// MousePicker mousepicker;
	//hexapod leg to wobble:
	// Scene::Transform *hip = nullptr;
	// Scene::Transform *upper_leg = nullptr;
	// Scene::Transform *lower_leg = nullptr;
	// glm::quat hip_base_rotation;
	// glm::quat upper_leg_base_rotation;
	// glm::quat lower_leg_base_rotation;
	// float wobble = 0.0f;
	std::vector<std::string> objects = {};

	//brunch
	Scene::Transform *bacon = nullptr;
	Scene::Transform *bacon1 = nullptr;
	Scene::Transform *bacon2 = nullptr;
	Scene::Transform *cake = nullptr;
	Scene::Transform *sandwich = nullptr;
	Scene::Transform *egg = nullptr;
	Scene::Transform *egg1 = nullptr;
	Scene::Transform *egg2 = nullptr;
	Scene::Transform *pancakes = nullptr;
	Scene::Transform *table = nullptr;
	// glm::quat bacon_base_rotation;
	// glm::quat cake_base_rotation;
	// glm::quat sandwich_base_rotation;
	// glm::vec3 bacon_base_position;
	// glm::vec3 cake_base_position;
	// glm::vec3 sandwich_base_position;
	// glm::vec3 cactus_base_position;
	// glm::vec3 egg_base_position;
	// glm::vec3 pancakes_base_position;
	std::vector<Scene::Transform*> scene_transforms;
	std::vector<glm::vec3> scene_base_positions;
	glm::vec3 table_base_position;

	float wobble = 0.0f;
	
	//camera:
	Scene::Camera *camera = nullptr;

};
