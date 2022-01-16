#include "controller.hxx"

Controller::Controller(Model& model)
        : model_(model),
          view_(model_,mixer())
{ }

void
Controller::draw(ge211::Sprite_set& set)
{
    view_.draw(set);
}

ge211::Dims<int>
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

int play_count = 0;
void
Controller::on_frame(double dt)
{
    model_.on_frame(dt);
    if(!model_.live()){
        if(play_count==0){
            ge211::Sound_effect_handle handle;
            if(model_.count_ > 0){ // loss
                handle = view_.mixer_.try_play_effect(view_.losing_sound_);
            }else{ // win
                handle = view_.mixer_.try_play_effect(view_.winning_sound_);
            }
            play_count = 1;
        }
    }
}

void
Controller::on_key_up(ge211::Key key)
{
    if (key == ge211::Key::right()) {
        model_.turn_cw();
    }
    if (key == ge211::Key::left()) {
        model_.turn_ccw();
    }
    if (key == ge211::Key::code(' ')){
        model_.launch();
    }
    if (key.code() == 'q') {
        exit(0);
    }

}
