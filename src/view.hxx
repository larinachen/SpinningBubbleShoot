#pragma once

#include "model.hxx"
#include "config.hxx"


class View
{
public:
    explicit View(Model const&,ge211::Mixer&);

    void draw(ge211::Sprite_set& set);

    ge211::Dims<int> initial_window_dimensions() const;



private:
    Model const& model_;

    ge211::Circle_sprite bullet_sprite_;

    ge211::Image_sprite shooter_sprite_;

    std::vector<ge211::Circle_sprite> brick_sprites_;

    ge211::Rectangle_sprite forbidden_;


public:
    // music
    ge211::Mixer& mixer_;
    ge211::Sound_effect losing_sound_;
    ge211::Sound_effect winning_sound_;

    // Loads the audio clips if possible.
    void load_audio();


};
