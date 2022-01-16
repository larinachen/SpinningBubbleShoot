#include "model.hxx"
#include "ball.hxx"
#include <catch.hxx>
#include <iostream>

//
// ******************* IMPORTANT *******************
// **** PLEASE RUN EACH TEST_CASE INDIVIDUALLY! ****
// currently, running all at the same time does not work.
// I discussed this with the instructor in Campuswire (#1078)
// and just run each test individually was the solution we came up with
//              ***** thank you ! *****

using namespace ge211;
ge211::Color black = ge211::Color().black();



//Game_config config;
// TESTING MOVEMENTS OF THE BULLET
// bullet initialized at center, bullet free flying properly, bullet hits edge
// and becomes back at center (dead)
TEST_CASE("hits edge of window"){
    Game_config config;
    Model m(config);

    // check if bullet is initialized at the center of scene
    CHECK(m.get_bullet().get_center().x == config.scene_dims().width/2);
    CHECK(m.get_bullet().get_center().y == config.scene_dims().height/2);

    m.get_bricks().clear(); // no bubbles now!
    m.get_bricks().push_back(Bubble(ge211::Color().black())); // 1 bubble

    m.launch();
    // check that ball is now live
    CHECK(m.get_bullet().get_live());
    float expected_y = m.get_bullet().get_center().y;
    m.on_frame(0.5);
    // test bullet direction is up upon start up
    CHECK(m.get_bullet().get_direction() == 0);

    // test free flying ball in moving at correct velocity
    expected_y += 0.5 * m.get_bullet().get_velocity().height;
    CHECK(m.get_bullet().get_center().x == config.scene_dims().width/2);
    CHECK(m.get_bullet().get_center().y == expected_y);

    // test ball hits top and it's center position should return back to center
    m.on_frame(0.5); // enough time to hit top
    // check back to center position
    CHECK(m.get_bullet().get_center().x == config.scene_dims().width/2);
    CHECK(m.get_bullet().get_center().y == config.scene_dims().height/2);
    // check ball is not live anymore
    CHECK(!m.get_bullet().get_live());

    Ball bullet = Ball(config);
    bullet.set_direction(135); // points to bottom right corner
    expected_y = m.get_bullet().get_center().y;
    m.set_bullet(bullet);
    m.launch();
    // check free flying to bottom right corner
    m.on_frame(0.5);
    CHECK(m.get_bullet().get_direction() == 135);
    expected_y += 0.5 * m.get_bullet().get_velocity().height;
    CHECK(m.get_bullet().get_center().y == expected_y);
    m.on_frame(0.5);
    // check hits bottom right corner
    m.on_frame(0.5); // enough time to hit corner
    // check back to center position
    CHECK(m.get_bullet().get_center().x == config.scene_dims().width/2);
    CHECK(m.get_bullet().get_center().y == config.scene_dims().height/2);
    // check ball is not live anymore
    CHECK(!m.get_bullet().get_live());
}

// TEST: BULLET COLLIDE WITH SAME COLOR BUBBLE ("RIGHT" BUBBLES)
// bubble becomes black when hit with same color bullet, and bullet
// goes back to center (dead)
TEST_CASE("good shot"){
    Game_config config;
    Model m(config);

    m.bricks_.at(4).set_color(m.get_curr_color());
    CHECK((m.bricks_.at(4).get_color()==m.get_curr_color()));

    m.launch();
    // check that ball is now live
    CHECK(m.get_bullet().get_live());
    m.on_frame(0.5);
    // test bullet direction is up upon start up
    CHECK(m.get_bullet().get_direction() == 0);

    // test ball hits top row of bubbles, and it's center position should
    // return to center
    m.on_frame(0.5); // enough time to hit top
    // check back to center position
    CHECK(m.get_bullet().get_center().x == config.scene_dims().width/2);
    CHECK(m.get_bullet().get_center().y == config.scene_dims().height/2);
    // check ball is not live anymore
    CHECK(!m.get_bullet().get_live());

    CHECK((m.get_bricks().at(4).get_color()==black));
}


// bullet and the bubble it hit are different colors:
// bubble is still live (not set to black) and a new bubble (b2)
// is created, sticking to the bubble.
TEST_CASE("bad shot"){
    Game_config config;
    Model m(config);

    ge211::Color c = ge211::Color().medium_magenta();
    m.bricks_.at(4).set_color(c);
    CHECK(!(m.bricks_.at(4).get_color()==m.get_curr_color()));
    CHECK(m.bricks_.size()==36);

    m.launch();
    // check that ball is now live
    CHECK(m.get_bullet().get_live());
    m.on_frame(0.5);
    // test bullet direction is up upon start up
    CHECK(m.get_bullet().get_direction() == 0);

    // test ball hits bubble of different color and it's center position should
    // return back to center
    m.on_frame(0.5); // enough time to hit top
    // check back to center position
    CHECK(m.get_bullet().get_center().x == config.scene_dims().width/2);
    CHECK(m.get_bullet().get_center().y == config.scene_dims().height/2);
    // check ball is not live anymore
    CHECK(!m.get_bullet().get_live());

    CHECK((m.get_bricks().at(4).get_color()==c));

    // check that a new bubble is created and sticking at the correct position
    // check a new bubble is made
    CHECK(m.bricks_.size()==37);
    float new_bubble_x = m.bricks_.at(36).get_top_left().x;
    float new_bubble_y = m.bricks_.at(36).get_top_left().y;
    float old_bubble_x = m.bricks_.at(4).get_top_left().x;
    float old_bubble_y = m.bricks_.at(4).get_top_left().y;
    // check that the new bubble is sticking at the right place
    CHECK(new_bubble_x == old_bubble_x);
    CHECK(new_bubble_y == old_bubble_y+60);

    // check that when a bubble moves, the bubble(s) sticking to it also moves
    // (to the correct spot in the scene)
    for(int i=0; i<120; i++){
        m.on_frame(0.01);
    }
    // along an edge
    CHECK(m.bricks_.at(4).get_top_left().x == old_bubble_x+60);
    CHECK(m.bricks_.at(36).get_top_left().x == old_bubble_x+60);
    // at a corner
    for(int i=0; i<3*120; i++){
        m.on_frame(0.01);
    }
    CHECK(m.bricks_.at(4).get_top_left().x == old_bubble_x+5*60);
    CHECK(m.bricks_.at(4).get_top_left().y == old_bubble_y);
    CHECK(m.bricks_.at(36).get_top_left().x == old_bubble_x+4*60);
    CHECK(m.bricks_.at(36).get_top_left().y == old_bubble_y+60);
    // after a corner, now on the left edge
    for(int i=0; i<3*120; i++){
        m.on_frame(0.01);
    }
    CHECK(m.bricks_.at(4).get_top_left().x == old_bubble_x+5*60);
    CHECK(m.bricks_.at(4).get_top_left().y == old_bubble_y+3*60);
    CHECK(m.bricks_.at(36).get_top_left().x == old_bubble_x+4*60);
    CHECK(m.bricks_.at(36).get_top_left().y == old_bubble_y+3*60);
}


// also, the speed of the spin (i.e. how quickly the bubbles shift) should
// increase


// when a bubble moves, the bubble(s) sticking to it also moves (to the
// correct spot in the scene)
// 4 edges, 4 corners



// TESTING LOSING GAME
// when a bubble enters the forbidden zone in the center, the game is lost
TEST_CASE("game lost"){
    Game_config config;
    Model m(config);

    // check game is not over
    CHECK(m.live());


    // make a bubble that overlaps the forbidden zone & would end the game
    Bubble b = Bubble({295.0,250.0}, 10, 1, config);
    b.set_color(ge211::Color().medium_red());
    // check that b indeed overlaps
    CHECK(b.overlap_forbidden(b.get_top_left()));
    m.bricks_.push_back(b);

    m.launch();
    m.on_frame(0.17);
    CHECK(m.get_bullet().hits_bubble(m.bricks_));

    // check that game is over (got here via losing)
    CHECK(!m.live());
    // check stop spinning
    CHECK(!m.spin());
}

// TEST WINNING GAME **AND** DESTROYING CONNECTED BUBBLES
// when there's no bubbles left in the chain (i.e. all the colored bubbles
// from the original chain has been destroyed, but it's okay if there's grey
// bubbles left), the game is won
TEST_CASE("game_won"){
    Game_config config;
    Model m(config);

    // check game is not over
    CHECK(m.live());

    // set all bubbles to the shooter's color
    for(Bubble& b : m.bricks_){
        b.set_color(m.get_curr_color());
    }

    m.launch();
    m.on_frame(1); // using dt of 1, we can hit a bubble on the top row

    // check that ball is not live
    CHECK(!m.get_bullet().get_live());
    // check that game is not live (got here via destroying all bubbles)
    CHECK(!m.live());

    // this also tells us that the connected bubbles are all destroyed too

}





