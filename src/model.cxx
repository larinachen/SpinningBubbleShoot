#include "model.hxx"
#include <iostream>
#include <cmath>

static ge211::Color const black_color {0, 0, 0};
static ge211::Color const stuck_color {128,128,128};
int j=0; // vector of bubbles counter
int radius = 30; // radius of the bubbles
int threshold = 120; // determines speed of shifting
int tick = threshold - 30; // time tracker

Model::Model(Game_config const& config)
    :config_(config),
     bullet_(config),
     shooter_sprite_("red_triangle.png"),
     curr_color_(config.get_random_color()),
     forbidden_sprite_({160,160},stuck_color),
     count_(36),
     spin_(false),
     live_(true)
{
     // shooter
     shooter_sprite_ = ge211::Image_sprite(config.get_shooter_image_str
                        (curr_color_));

     // bubbles (10 on each edge, 600 square window -> bubble radius = 30)
     // pos: top left corner of bounding box, dim:{radius, radius}
     for(int i=0; i<9;i++){
        // top edge of bubbles
        ge211::Posn<float> pos = {(float) i*60,0};
        Bubble b = Bubble(pos,radius,j,config_);
        bricks_.push_back(b);
        brick_sprites_.push_back(ge211::Circle_sprite(radius,b.get_color()));
        j += 1;
     }

     for(int i=0; i<9;i++) {
        // right edge of bubbles
        ge211::Posn<float> pos = {(float) config_.scene_dims().width - 2 *
                                  radius, (float) i * 60};
        Bubble b = Bubble(pos, radius, j, config_);
        bricks_.push_back(b);
        brick_sprites_.push_back(ge211::Circle_sprite(radius, b.get_color()));
        j += 1;
     }
     for(int i=8; i>=0;i--) {
        // bottom edge of bubbles
        ge211::Posn<float> pos =
                {(float) (i + 1) * 60,
                 (float) config_.scene_dims().height - 2 * radius};
        Bubble b = Bubble(pos, radius, j, config_);
        bricks_.push_back(b);
        brick_sprites_.push_back(ge211::Circle_sprite(radius, b.get_color()));
        j += 1;
     }
     for(int i=8; i>=0;i--){
        // left edge of bubbles
        ge211::Posn<float> pos = {0,(float) (i+1)*60};
        Bubble b = Bubble(pos,radius,j,config_);
        bricks_.push_back(b);
        brick_sprites_.push_back(ge211::Circle_sprite(radius,b.get_color()));
        j += 1;
     }

}

Game_config Model::config() const{
    return config_;
}

Ball const& Model::get_bullet() const{
    return bullet_;
}

ge211::Image_sprite Model::get_shooter_sprite() const
{
    return shooter_sprite_;
}

void Model::turn_cw(){
    bullet_.set_direction(10);
}

void Model::turn_ccw(){
    bullet_.set_direction(-10);
}


void Model::launch(){
    bullet_.set_live(true);
    bullet_.set_velocity();
}

ge211::Color Model::get_curr_color() const
{
    return curr_color_;
}

std::vector<Bubble> Model::get_bricks() const
{
    return bricks_;
}

bool Model::live() const{
    return live_;
}

bool Model::spin() const{
    return spin_;
}

void
Model::on_frame(double dt)
{
    // if game is over
    if(!live_){
        return;
    }

    // spin bubbles if the game is live
    tick += 1;
    if(spin_){
        if(tick >= threshold){
            for(Bubble& b : bricks_){
                b.shift();
            }
            tick = 0;
        }
    }


    if(!bullet_.get_live()){
        return;
    }

    Ball nxt = bullet_.next(dt);
    bool game_over = false;
    if((nxt.hits_edge(config_) || nxt.hits_bubble(bricks_))){
        spin_ = true;
        Bubble& b = nxt.destroy_bubble(bricks_);
        // hits an active bubble of the same color
        if(curr_color_ == b.get_color()){
            b.set_black();
            count_ -= 1;
            connected_left(b);
            connected_right(b);
            // hits an active bubble of the wrong color
        }else if(!(b.get_color()==black_color) &&
             !(curr_color_==b.get_color())){
            ge211::Posn<float> pos = stuck_pos(bullet_,b);
            if(threshold>20){
                threshold -= 10;
            }
            if(b.overlap_forbidden(pos)){
                game_over = true;
            }
            Bubble new_b = Bubble(pos,radius,j,config_);
            new_b.set_color(stuck_color);
            bricks_.push_back(new_b);
            brick_sprites_.push_back(ge211::Circle_sprite(radius,stuck_color));
            j += 1;
        }

        // update shooter and bullet
        bullet_.set_live(false);
        bullet_.set_top_left(config_.scene_center().x-bullet_.get_radius(),
                             config_.scene_center().y-bullet_.get_radius());
        curr_color_ = config_.get_random_color();
        shooter_sprite_ = ge211::Image_sprite(config_.get_shooter_image_str
                                                            (curr_color_));

        // check is game over
        if(game_over){
            std::cout << "game over";
            forbidden_sprite_.recolor(ge211::Color().medium_red());
            spin_ = false;
            live_ = false;
        }else if(count_==0){
            std::cout << "game won";
            forbidden_sprite_.recolor(ge211::Color().medium_green());
            spin_ = false;
            live_ = false;
        }

        return;
    }

    bullet_ = bullet_.next(dt);

}

void Model::connected_left(Bubble& b)
{
    int i = b.index_;
    if(i==0){
        i=36;
    }
    if(!(bricks_.at(i-1).get_color()==curr_color_)){ // not target color -> end
        return;
    }else{
        bricks_.at(i-1).set_black();
        brick_sprites_.at(i-1).recolor(black_color);
        count_ -= 1;
        return this->connected_left(bricks_.at(i-1));
    }
}

void Model::connected_right(Bubble& b)
{
    int i = b.index_;
    if(i==35){
        i=-1;
    }
    if(!(bricks_.at(i+1).get_color()==curr_color_)){ // not target color -> end
        return;
    }else{
        bricks_.at(i+1).set_black();
        brick_sprites_.at(i+1).recolor(black_color);
        count_ -= 1;
        return this->connected_right(bricks_.at(i+1));
    }
}

// give the top left corner of the stuck ball's position
Position Model::stuck_pos(Ball ball, Bubble& bubble)
{
    int dir = ball.hits_bubble_dir(bubble);
    if(dir==1){ // ball -> bubb
        return {bubble.get_top_left().x-2*bubble.get_radius(),
                bubble.get_top_left().y};
    }else if(dir==2){ // bubb <- ball
        return {bubble.get_top_left().x+2*bubble.get_radius(),
                bubble.get_top_left().y};
    }else if(dir==3){ // ball on top
        return {bubble.get_top_left().x,
                bubble.get_top_left().y - 2 * bubble.get_radius()};
    }else if(dir==4){ // ball on bottom
        return {bubble.get_top_left().x,
                bubble.get_top_left().y+2*bubble.get_radius()};
    }

    return {0,0};
}



// ******************************************************************

// for testing

void Model::set_bullet(Ball b)
{
    bullet_ = b;
}

