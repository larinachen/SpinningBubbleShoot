#pragma once
#include <ge211.hxx>

bool operator==(ge211::Color,ge211::Color);

class Game_config{
public:
    //constructor
    Game_config();

    //getters
    ge211::Color red() const;
    ge211::Color orange() const;
    ge211::Color green() const;
    ge211::Color purple() const;
    ge211::Dims<int> scene_dims() const;
    ge211::Posn<int> scene_center() const;

    // color functions
    ge211::Color get_random_color() const;
    std::string const get_shooter_image_str(ge211::Color) const;

private:
    // The dimensions of the whole window:
    ge211::Dims<int> scene_dims_;
    // all available bubble colors
    std::vector<ge211::Color> bubble_colors_;


};