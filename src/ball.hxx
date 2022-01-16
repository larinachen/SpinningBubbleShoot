#pragma once

#include <ge211.hxx>
#include "config.hxx"

using Position = ge211::Posn<float>;
using Velocity = ge211::Dims<float>;



class Bubble;

class Ball
{
public:
    // constructor
    Ball(Game_config const& config);

    // getters
    int get_radius() const;
    Position get_center() const;
    Position get_top_left() const;
    float get_direction() const;
    bool get_live() const;
    Velocity get_velocity() const;

    //setters
    void set_direction(float);
    void set_live(bool);
    void set_velocity();
    void set_top_left(float,float);

    // modify game state
    Ball next(double); // next frame of the ball

    bool hits_edge(Game_config const&) const;

    bool hits_bubble(std::vector<Bubble>&) const;
    Bubble& destroy_bubble(std::vector<Bubble>&) const;

    int hits_bubble_dir(Bubble const&);


private:
    // radius of the ball in pixels
    int radius_;
    // position of the ball's center
    Position center_;
    // ball's direction in degrees
    float direction_;
    // ball's velocity
    Velocity velocity_;
    // if the ball is alive: true -> move , false -> back to center
    bool live_;

    // helper functions
    bool hits_bubble_helper(Bubble const&) const;
    bool hits_top(Game_config const&) const;
    bool hits_bottom(Game_config const&) const;
    bool hits_side(Game_config const&) const;

};

// Bubble class ------------------------------------------------

class Bubble{
public:
    // constructors
    Bubble(Position, int, int, Game_config const&);
    Bubble(ge211::Color);

    // getters
    ge211::Color get_color() const;
    Position get_top_left() const;
    int get_radius() const;

    //setters
    void set_black();
    void set_color(ge211::Color);

    // change game state
    bool overlap_forbidden(Position);
    void shift();

private:
    Game_config config_;
    Position position_; //top left corner
    int radius_;
    ge211::Color color_;
public:
    int index_;
    int waittime_;
};