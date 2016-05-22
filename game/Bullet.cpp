//
// Created by jasf on 4/21/16.
//

#include "Bullet.h"
#include "Camera.h"

Bullet::Bullet(float x, float y, float angle, float speed, float maxDistance, string sprite, float frameTime,
               int frameCount, bool targetsPlayer) :
        sp(sprite, frameCount, frameTime), targetsPlayer(targetsPlayer) {


    rotation = angle;

    this->speed = Vec2::getVec2FromPolar(speed, angle);

    distanceLeft = maxDistance;

    pos = Vec2(x, y);

    int width = sp.getWidth()/frameCount;
    int height = sp.getHeight();


    center_LT_displacement = -1 * Vec2(width * 0.5, height * 0.5);

    box = Rect(pos + center_LT_displacement, height, width);


}

void Bullet::update(float dt) {

    sp.update(dt);

    pos += speed * dt;
    box.setLT(pos + center_LT_displacement);

    distanceLeft -= (speed * dt).magnitude();

}

void Bullet::render() {
    GameObject::render();
    Vec2 c = pos - Camera::getPos(Camera::PLAYER_GROUND_VIEW) + center_LT_displacement;
    sp.render(c.x, c.y, rotation);

}

bool Bullet::isDead() {
    return distanceLeft <= 0;
}

void Bullet::notifyCollision(GameObject &other) {
    if(other.is("Penguins") && targetsPlayer)
        this->distanceLeft = 0;

    if(other.is("Alien") && !targetsPlayer)
        distanceLeft = 0;


}

bool Bullet::is(std::string type) {
    return type == "Bullet";
}
