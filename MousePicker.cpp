#include "Mesh.hpp"
#include "read_write_chunk.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <string>

glm::vec3 MousePicker::getMouseRay() {
    glutPassiveMotionFunc(passive_mouse_motion);
}

void passive_mouse_motion(int x, int y) {
    screen_x = x;
    screen_y = y;
}


// MousePicker::MousePicker(Scene::Camera camera, glm::mat4x4 projection) {
//     Scene::Camera camera = cam;
//     projectionMatrix = projection;
//     viewMatrix = Maths.createViewMatrix(camera);
//     this.terrain = terrain;
// }

// glm::vec3 MousePicker::getCurrentRay() {
//     return currentRay;
// }

// public void update() {
//     viewMatrix = Maths.createViewMatrix(camera);
//     currentRay = calculateMouseRay();
//     if (intersectionInRange(0, RAY_RANGE, currentRay)) {
//         currentTerrainPoint = binarySearch(0, 0, RAY_RANGE, currentRay);
//     } else {
//         currentTerrainPoint = null;
//     }
// }

// private Vector3f calculateMouseRay() {
//     float mouseX = Mouse.getX();
//     float mouseY = Mouse.getY();
//     Vector2f normalizedCoords = getNormalisedDeviceCoordinates(mouseX, mouseY);
//     Vector4f clipCoords = new Vector4f(normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f);
//     Vector4f eyeCoords = toEyeCoords(clipCoords);
//     Vector3f worldRay = toWorldCoords(eyeCoords);
//     return worldRay;
// }

// private Vector3f toWorldCoords(Vector4f eyeCoords) {
//     Matrix4f invertedView = Matrix4f.invert(viewMatrix, null);
//     Vector4f rayWorld = Matrix4f.transform(invertedView, eyeCoords, null);
//     Vector3f mouseRay = new Vector3f(rayWorld.x, rayWorld.y, rayWorld.z);
//     mouseRay.normalise();
//     return mouseRay;
// }

// private Vector4f toEyeCoords(Vector4f clipCoords) {
//     Matrix4f invertedProjection = Matrix4f.invert(projectionMatrix, null);
//     Vector4f eyeCoords = Matrix4f.transform(invertedProjection, clipCoords, null);
//     return new Vector4f(eyeCoords.x, eyeCoords.y, -1f, 0f);
// }

// private Vector2f getNormalisedDeviceCoordinates(float mouseX, float mouseY) {
//     float x = (2.0f * mouseX) / Display.getWidth() - 1f;
//     float y = (2.0f * mouseY) / Display.getHeight() - 1f;
//     return new Vector2f(x, y);
// }