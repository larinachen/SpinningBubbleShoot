#include "ball.hxx"
#include <catch.hxx>

Block const The_Paddle {100, 400, 100, 20};

// TO DO:
// - next with + velocities, next with - velocities
// - destroy_brick: ??
// DONE:
// - test above_block (constructor needs this when the ball is dead)
// - test top_left
// - hits bottom, hits top, hits left, hits right, hits top-left corner,
//   hits bottom-right corner
// - hits_block (cover all 4 cases)


TEST_CASE("Ball::Ball")
{
    Game_config config;
    Ball ball(The_Paddle, config);

    CHECK(ball.center.x == 150);
    CHECK(ball.center.y == 394);
}

TEST_CASE("Ball::top_left"){
    Game_config config;
    Ball ball(The_Paddle, config);
    CHECK(ball.center.x == 150);
    CHECK(ball.center.y == 394);
    CHECK(ball.top_left().x == 145);
    CHECK(ball.top_left().y == 389);
}


TEST_CASE("Ball::hits_side")
{
    Game_config config;
    Ball ball(The_Paddle, config);

    CHECK_FALSE(ball.hits_side(config));
    ball.center.x = 1;
    CHECK(ball.hits_side(config));
    ball.center.x = 5;
    CHECK_FALSE(ball.hits_side(config));
    ball.center.x = config.scene_dims.width - 1;
    CHECK(ball.hits_side(config));
    ball.center.x = config.scene_dims.width - 5;
    CHECK_FALSE(ball.hits_side(config));
    ball.center.x = config.scene_dims.width / 2;
    CHECK_FALSE(ball.hits_side(config));
}

TEST_CASE("Ball::hits_top/bottom"){
    Game_config config;
    Ball ball(The_Paddle, config);

    CHECK_FALSE(ball.hits_top(config));
    CHECK_FALSE(ball.hits_bottom(config));
    ball.center.y = 5;
    CHECK_FALSE(ball.hits_top(config));
    CHECK_FALSE(ball.hits_bottom(config));
    ball.center.y = 4;
    CHECK(ball.hits_top(config));
    ball.center.y = config.scene_dims.height;
    CHECK_FALSE(ball.hits_top(config));
    CHECK(ball.hits_bottom(config));
    ball.center.y = config.scene_dims.height-5;
    CHECK_FALSE(ball.hits_bottom(config));
}

TEST_CASE("hits corners"){
    Game_config config;
    Ball ball(The_Paddle, config);
    //ball is at top left corner
    ball.center={5,5};
    CHECK_FALSE(ball.hits_top(config));
    CHECK_FALSE(ball.hits_side(config));
    ball.center={4,2};
    CHECK(ball.hits_top(config));
    CHECK(ball.hits_side(config));
    //ball is at bottom right corner
    ball.center.x =config.scene_dims.width-5;
    ball.center.y =config.scene_dims.height-5;
    CHECK_FALSE(ball.hits_top(config));
    CHECK_FALSE(ball.hits_side(config));
    ball.center.x =config.scene_dims.width-4;
    ball.center.y =config.scene_dims.height-4;
    CHECK(ball.hits_bottom(config));
    CHECK(ball.hits_side(config));
}

TEST_CASE("Ball::hits_block"){
    Game_config config;
    Ball ball(The_Paddle, config);
    Block block = {100, 400, 100, 20};

    ball.center.y = 400;
    ball.center.x = 10;
    CHECK_FALSE(ball.hits_block(block));
    ball.center.x = 102;
    CHECK(ball.hits_block(block));
    ball.center.x = 300;
    CHECK_FALSE(ball.hits_block(block));
    ball.center.x = 100;
    ball.center.y = 295;
    CHECK_FALSE(ball.hits_block(block));
    ball.center.y = 402;
    CHECK(ball.hits_block(block));
    CHECK(ball.hits_block(block));
    ball.center.y = 426;
    CHECK_FALSE(ball.hits_block(block));
}

