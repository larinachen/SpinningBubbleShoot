#include "view.hxx"

ge211::Rectangle_sprite shooter_bg =
        ge211::Rectangle_sprite({40,40},ge211::Color().black());

View::View(Model const& model, ge211::Mixer& mixer)
        : model_(model),
          bullet_sprite_(model_.get_bullet().get_radius(), model_
          .get_curr_color()),
          shooter_sprite_(model_.get_shooter_sprite()),
          brick_sprites_(model.brick_sprites_),
          forbidden_(model.forbidden_sprite_),
          mixer_(mixer)
{

    // music
    if (mixer_.is_enabled()) {
        load_audio();
    }

}

void
View::draw(ge211::Sprite_set& set)
{

    // drawing the bullet
    Ball bullet = model_.get_bullet();
    bullet_sprite_.recolor(model_.get_curr_color());
    set.add_sprite(bullet_sprite_,bullet.get_top_left().into<int>(),1);
    // drawing the shooter
    ge211::Transform smallerer = ge211::Transform().scale(0.075);
    ge211::Transform rotator = ge211::Transform().rotation(model_.get_bullet
                                ().get_direction());
    ge211::Transform transformer = smallerer * rotator;
    shooter_sprite_ = model_.get_shooter_sprite();
    set.add_sprite(shooter_sprite_,
                   {model_.config().scene_center().x - 20,
                    model_.config().scene_center().y - 20},
                    3,transformer);
    // drawing bubbles
    brick_sprites_ = model_.brick_sprites_;
    for(Bubble b : model_.get_bricks()){
        int i = b.index_;
        brick_sprites_.at(i).recolor(b.get_color());
        set.add_sprite(brick_sprites_.at(i),b.get_top_left().into<int>());
    }
    // draw the forbidden ground
    forbidden_ = model_.forbidden_sprite_;
    set.add_sprite(forbidden_, {220,220},0);
    set.add_sprite(shooter_bg,{280,280},2);
}

ge211::Dims<int>
View::initial_window_dimensions() const
{
    return model_.config().scene_dims();
}


// music -----------------------------


void
View::load_audio()
{
    losing_sound_ = ge211::Sound_effect("lose.mp3", mixer_);
    winning_sound_ = ge211::Sound_effect("win.mp3", mixer_);
}

