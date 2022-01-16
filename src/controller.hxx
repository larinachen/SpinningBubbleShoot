#pragma once

#include "model.hxx"
#include "view.hxx"

#include <ge211.hxx>

class Controller : public ge211::Abstract_game
{
public:
    // constructor
    Controller(Model&);
    // ge211 commands
    ge211::Dims<int> initial_window_dimensions() const override;
    void on_frame(double dt) override;
    // key commands
    void on_key_up(ge211::Key) override;

protected:
    void draw(ge211::Sprite_set& set) override;

private:
    Model model_;
    View view_;
};
