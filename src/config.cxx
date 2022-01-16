
#include "config.hxx"
#include <string>
#include <iostream>

// game colors
static ge211::Color const red_color {255, 22, 22};
static ge211::Color const yellow_color {255, 222, 89};
static ge211::Color const green_color {126, 217, 87};
static ge211::Color const purple_color {203, 108, 230};


// equal operators for colors
bool operator==(ge211::Color c1, ge211::Color c2){
    return (c1.red() == c2.red()) && (c1.green() == c2.green())
           && (c1.blue() == c2.blue());
}


Game_config::Game_config()
    :scene_dims_{600, 600},
    bubble_colors_(std::vector<ge211::Color> {red_color,yellow_color,
                                             green_color,purple_color})

{}

ge211::Color Game_config::red() const{
    return bubble_colors_.at(0);
}
ge211::Color Game_config::orange() const{
    return bubble_colors_.at(1);
}
ge211::Color Game_config::green() const{
    return bubble_colors_.at(2);
}
ge211::Color Game_config::purple() const{
    return bubble_colors_.at(3);
}

ge211::Dims<int> Game_config::scene_dims() const{
    return scene_dims_;
}

ge211::Posn<int> Game_config::scene_center() const{
    ge211::Posn<int> center = {scene_dims_.width/2,
                               scene_dims_.height/2};
    return center;
}

ge211::Color Game_config::get_random_color() const
{
    //srand(time(0));
    int i = rand() % 4;
    return bubble_colors_.at(i);
}

std::string const Game_config::get_shooter_image_str(ge211::Color color)
const{
    if(color == red_color){
        return "red_triangle.png";
    }else if(color == yellow_color){
        return "yellow_triangle.png";
    }else if(color == green_color){
        return "green_triangle.png";
    }else if(color == purple_color){
        return "purple_triangle.png";
    }else{
        return "Error";
    }
}

/*
 * Citations:
finding how rand() and srand() works and troubleshooting random integer generation:
https://stackoverflow.com/questions/9421463/rand-generating-same-number-upon-compilation
 */
