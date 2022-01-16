#pragma once

#include <ge211.hxx>
#include "ball.hxx"
#include "config.hxx"

class Model
{
public:
    // constructor
    Model(Game_config const& config = Game_config());
    // getters
    Game_config config() const;
    Ball const& get_bullet() const;
    ge211::Image_sprite get_shooter_sprite() const;
    ge211::Color get_curr_color() const;
    std::vector<Bubble> get_bricks() const;
    bool live() const;
    bool spin() const;

    // changing game states
    void turn_cw(); // turn shooter clockwise
    void turn_ccw(); // turn shooter counter-clockwise
    void launch(); // launch the bullet
    void on_frame(double dt); // advance game by 1 frame


    void connected_left(Bubble& b); // destroy the left connected bubbles
    void connected_right(Bubble& b); // destroy the right connected bubbles
    Position stuck_pos(Ball, Bubble&);

    // for testing
    void set_bullet(Ball);

private:
    Game_config const config_;
    Ball bullet_;
    ge211::Image_sprite shooter_sprite_;
    ge211::Color curr_color_;


public:
    std::vector<ge211::Circle_sprite> brick_sprites_;
    ge211::Rectangle_sprite forbidden_sprite_;
    int count_;
    std::vector<Bubble> bricks_;

private:
    bool spin_; // if the game can spin
    bool live_; // if the game is live

};
