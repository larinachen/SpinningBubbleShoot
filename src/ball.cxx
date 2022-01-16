
#include "ball.hxx"
#include "view.hxx"
#include <math.h>
#include <iostream>

static float PI = 3.1415926;
ge211::Color const black_color {0, 0, 0};
Bubble placeholder = Bubble(black_color);



Ball::Ball(Game_config const& config)
    : radius_(10),
      center_(config.scene_dims().width/2,config.scene_dims().height/2),
      direction_(0),
      velocity_({0,0}),
      live_(false)
{}


int Ball::get_radius() const{
    return radius_;
}

Position
Ball::get_center() const{
    return center_;
}

Position
Ball::get_top_left() const{
    return {center_.x-radius_, center_.y-radius_};
}

void Ball::set_top_left(float newx, float newy){
    center_.x = newx + radius_;
    center_.y = newy + radius_;
}

float Ball::get_direction() const{
    return direction_;
}

void Ball::set_direction(float spin){
    direction_+= spin;
    if(direction_==360.0 || direction_==-360.0) {
        direction_ = 0.0;
    }
}

bool Ball::get_live() const
{
    return live_;
}

void Ball::set_live(bool state){
    live_ = state;
}

Velocity Ball::get_velocity() const{
    return velocity_;
}

void Ball::set_velocity(){
    velocity_.width = 250 * cos(((90-direction_)*PI)/180);
    velocity_.height = -250 * sin(((90-direction_)*PI)/180);
}


bool
Ball::hits_bottom(Game_config const& config) const
{
    return ((center_.y+radius_) > config.scene_dims().height);
}

bool
Ball::hits_top(Game_config const&) const
{
    return ((center_.y-radius_)<0);
}

bool
Ball::hits_side(Game_config const& config) const
{
    bool lhs = (center_.x-radius_)<0;
    bool rhs = (center_.x+radius_)>config.scene_dims().width;
    return (lhs || rhs);
}

bool
Ball::hits_edge(Game_config const& config) const
{
    return (this->hits_top(config) || this->hits_bottom(config) ||
            this->hits_side(config));
}



Ball Ball::next(double dt){
    Ball result(*this);
    double x = velocity_.width*dt;
    double y = velocity_.height*dt;
    result.center_.x += x;
    result.center_.y += y;
    return result;
}

bool
Ball::hits_bubble_helper(Bubble const& bubble) const
{
    float ball_l = center_.x-radius_;
    float ball_r = center_.x+radius_;
    float ball_t = center_.y-radius_;
    float ball_b = center_.y+radius_;
    if((ball_r < bubble.get_top_left().x) ||
        ((bubble.get_top_left().x + 2*bubble.get_radius()) < ball_l)){
        return false;
    }else if((ball_b < bubble.get_top_left().y) ||
        ((bubble.get_top_left().y + 2*bubble.get_radius())<ball_t)){
        return false;
    }
    return true;
}

bool
Ball::hits_bubble(std::vector<Bubble>& bricks) const
{
    for(Bubble& bubble : bricks){
        if(!(bubble.get_color()==black_color) && hits_bubble_helper(bubble)){
            return true;
        }
    }
    return false;
}

// basically hits_bubble, but returns color instead
Bubble&
Ball::destroy_bubble(std::vector<Bubble>& bricks) const
{
    for(Bubble& bubble : bricks){
        if(hits_bubble_helper(bubble)){
            return bubble;
        }
    }
    //otherwise, return black color (purely by random choice)
    //ge211::Color otherwise = {0,0,0};
    return placeholder;
}

int Ball::hits_bubble_dir(Bubble const& bubble){
    float ball_l = center_.x-radius_;
    float ball_r = center_.x+radius_;
    float ball_t = center_.y-radius_;
    float ball_b = center_.y+radius_;
    if(ball_r < bubble.get_top_left().x){
        return 1; // ball -> bubble
    }else if((bubble.get_top_left().x+2*bubble.get_radius()) < ball_l){
        return 2; // bubble <- ball
    }else if(ball_b < bubble.get_top_left().y){
        return 3; // ball on top
    }else if((bubble.get_top_left().y + 2*bubble.get_radius()) < ball_t){
        return 4; // ball on bottom
    }
    return 0;
}




// ****************************************************************************



Bubble::Bubble(Position pos, int radius, int index, Game_config const& config)
    :config_(config),
     position_(pos),
     radius_(radius),
     color_(config.get_random_color()),
     index_(index),
     waittime_(0)
{

}

Bubble::Bubble(ge211::Color c)
    :position_(Position(0,0)),
     radius_(0),
     color_(c),
     index_(0)
{}

ge211::Color Bubble::get_color() const{
    return color_;
}

Position Bubble::get_top_left() const{
    return position_;
}

int Bubble::get_radius() const{
    return radius_;
}

void Bubble::set_black(){
    color_ = ge211::Color::black();
}

void Bubble::set_color(ge211::Color c){
    color_ = c;
}

bool Bubble::overlap_forbidden(Position pos){
    float ball_l = pos.x;
    float ball_r = pos.x + 2*radius_;
    float ball_t = pos.y;
    float ball_b = pos.y + 2*radius_;
    if((ball_r < 230) || (370 < ball_l)){
        return false;
    }else if((ball_b < 230) || (370<ball_t)){
        return false;
    }
    return true;
}

void Bubble::shift(){

    float x = this->get_top_left().x;
    float y = this->get_top_left().y;

    // outermost layer
    if(y==0 && x<=480){ // top edge bubbles
        position_ = {x+60,y};
    }else if(x==540 && y<=480){ // right edge bubbles
        position_ = {x,y+60};
    }else if(y==540 && x>=60){ // bottom bubbles
        position_ = {x-60,y};
    }else if(x==0 && y>=60){ // left edge bubbles
        position_ = {x,y-60};
    }

    // middle layer

    else if((x==60 && y==60) || (x==480 && y==60) ||
             (x==60 && y==480) || (x==480 && y==480)){
        waittime_ += 1 ;
        if(waittime_>=3){
            if(y==60 && x<=420){ // top edge bubbles
                position_ = {x+60,y};
            }else if(x==480 && y<=420){ // right edge bubbles
                position_ = {x,y+60};
            }else if(y==480 && x>=120){ // bottom bubbles
                position_ = {x-60,y};
            }else if(x==60 && y>=120){ // left edge bubbles
                position_ = {x,y-60};
            }
            waittime_ = 0;
        }
    }
    else if(y==60 && x<=420){ // top edge bubbles
        position_ = {x+60,y};
    }else if(x==480 && y<=420){ // right edge bubbles
        position_ = {x,y+60};
    }else if(y==480 && x>=120){ // bottom bubbles
        position_ = {x-60,y};
    }else if(x==60 && y>=120){ // left edge bubbles
        position_ = {x,y-60};
    }

    // innermost layer
    else if((x==120 && y==120) || (x==420 && y==120) ||
            (x==120 && y==420) || (x==420 && y==420)){
        waittime_ += 1 ;
        if(waittime_>=5){
            if(y==120 && x<=360){ // top edge bubbles
                position_ = {x+60,y};
            }else if(x==420 && y<=360){ // right edge bubbles
                position_ = {x,y+60};
            }else if(y==420 && x>=180){ // bottom bubbles
                position_ = {x-60,y};
            }else if(x==120 && y>=180){ // left edge bubbles
                position_ = {x,y-60};
            }
            waittime_ = 0;
        }
        std::cout << waittime_;
    }
    else if(y==120 && x<=360){ // top edge bubbles
        position_ = {x+60,y};
    }else if(x==420 && y<=360){ // right edge bubbles
        position_ = {x,y+60};
    }else if(y==420 && x>=180){ // bottom bubbles
        position_ = {x-60,y};
    }else if(x==120 && y>=180){ // left edge bubbles
        position_ = {x,y-60};
    }
}



