#include <bn_core.h>
#include <bn_backdrop.h>
#include <bn_keypad.h>
#include <bn_sprite_ptr.h>
#include <bn_display.h>
#include <bn_random.h>
#include <bn_vector.h>
#include <bn_log.h>

#include "bn_sprite_items_dot.h"

// Set max/min x position to be the edges of the display
static constexpr int HALF_SCREEN_WIDTH = bn::display::width() / 2;
static constexpr bn::fixed MIN_X = -HALF_SCREEN_WIDTH;
static constexpr bn::fixed MAX_X = HALF_SCREEN_WIDTH;

//same as above but for y position
static constexpr int HALF_SCREEN_HEIGHT = bn::display::height() / 2;
static constexpr bn::fixed MIN_Y = -HALF_SCREEN_HEIGHT;
static constexpr bn::fixed MAX_Y = HALF_SCREEN_HEIGHT;

// Starting speed of a bouncer
static constexpr bn::fixed BASE_SPEED = 2;

// Maximum number of bouncers on screen at once
static constexpr int MAX_BOUNCERS = 20;

//class needs a semicolon at the end of it in C++
//only have to say once public
class Bouncer{
public:
    bn::sprite_ptr sprite;
    bn::fixed x_speed;
    bn::fixed y_speed;

    Bouncer(bn::fixed xs, bn::fixed ys) :
        sprite(bn::sprite_items::dot.create_sprite())
    {
        x_speed = xs;
        y_speed = ys;
    }

    void update(){
        bn::fixed x = sprite.x();
        bn::fixed y = sprite.y();

        x += x_speed;
        y += y_speed;

        // bounce X
        if(x > MAX_X){
            x = MAX_X;
            x_speed *= -1;
        }
        if(x < MIN_X){
            x = MIN_X;
            x_speed *= -1;
        }

        // bounce Y
        if(y > MAX_Y){
            y = MAX_Y;
            y_speed *= -1;
        }
        if(y < MIN_Y){
            y = MIN_Y;
            y_speed *= -1;
        }

        sprite.set_x(x);
        sprite.set_y(y);
    }
};

// bn::fixed x_average( bn::vector<bn::sprite_ptr, MAX_BOUNCERS>& sprites){
// // Add all x positions together
// bn::fixed x_sum = 0;
// for(bn::sprite_ptr sprite : sprites) {
//     x_sum += sprite.x();
// }
// bn::fixed x_average= x_sum;
// // Only divide if we have 1 or more
// // Prevents division by 0
// if(sprites.size() > 0) {
//     x_average /= sprites.size();
// }
// return x_average;
// }

bn::fixed x_average(const bn::vector<Bouncer, MAX_BOUNCERS>& bouncers)
{
    bn::fixed x_sum = 0;

    for(const Bouncer& b : bouncers)
    {
        x_sum += b.sprite.x();
    }

    if(bouncers.size() > 0)
    {
        return x_sum / bouncers.size();
    }

    return 0;
}

//method bouncer
void add_bouncer(bn::vector<Bouncer, MAX_BOUNCERS>& bouncers, bn::random& rng){
    // Only add if we're below the maximum
    if(bouncers.size() < bouncers.max_size()) {

        int xs = rng.get_int(-2, 3);
        int ys = rng.get_int(-2, 3);

        if(xs == 0) xs = 1;
        if(ys == 0) ys = -1;

        bouncers.push_back(Bouncer(xs, ys));
    }
}
//dont need return statmenent for void method


int main() {
    bn::core::init();

    bn::random rng;
    bn::vector<Bouncer, MAX_BOUNCERS> bouncers ={};

    while(true) {
        // if A is pressed add a new bouncer
        if(bn::keypad::a_pressed()) {
            add_bouncer(bouncers, rng);
        }

        // // if B is pressed print the average to the console
        // if(bn::keypad::b_pressed()) {
        //     //BN_LOG("Average x: ", x_average(sprites));
        // }

        // for each bouncer
        for(Bouncer& bouncer : bouncers){
            bouncer.update();
        }
 
        bn::core::update();
    }
}