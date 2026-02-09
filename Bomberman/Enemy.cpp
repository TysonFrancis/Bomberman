#include "Enemy.h"

Enemy::Enemy(sf::Texture& tex, int input) : Entity(tex), type(input), speed(4.f) {}

void Enemy::move()
{
	switch (type)
	{
	case 1: //ballom, random movement, speed 2
		break;

	case 2: //onil, chases player if close, speed 3
		break;

	case 3: //dahl, random movement,speed 3
		break;

	case 4: //minvo, chases player, speed 4
		break;

	case 5: //doria, chases, avoids bombs, speed 1, moves through soft blocks
		break;

	case 6: //ovape, random movement, speed 2, moves through soft blocks
		break;

	case 7: //pass, always chases if encountered, speed 5
		break;

	case 8: //pontan, alwyays chases, speed 6, moves through soft blocks
		break;
	}
}

void Enemy::update()
{
    // *** Animation timing ***
	myTick++;                                   // Increment tick counter every game frame

    if (alive)								    // If alive,
    {
        if (myTick % 5 == 0)				        // Update frame every 5 ticks, 60fps -> 12 frames per second
            myFrame = (myFrame + 1) % 3;	        // Loop through frames for waling animation, 3 frames total
    }
        
    else                                        // Else,
    {
        if (myTick % 5 == 0)                        // Update frame every 5 ticks, but don't loop
            myFrame++;
        if (myFrame >= 7)
            myFrame = 7;                            // Freeze on empty frame once finished
    }

    // *** Sprite movement and texture updates ***
	if (type == 1)                              // Balloon enemy, WSAD controls
    {
		if (alive)                                  // Living animations + movement
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
                sprite.move({ 0, -speed });

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S))
                sprite.move({ 0, speed });

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
            {
                sprite.setTextureRect(sf::IntRect({ myFrame * 16 + 48, 240 }, { 16, 16 }));
                sprite.move({ -speed, 0 });
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
            {
                sprite.setTextureRect(sf::IntRect({ myFrame * 16, 240 }, { 16, 16 }));
                sprite.move({ speed, 0 });
            }
        }

        else                                        // Death animation
            sprite.setTextureRect(sf::IntRect({ myFrame * 16 + 96, 240 }, { 16, 16 }));
    }

    if (type == 2)                              // Ice Cream enemy, TFGH controls
    {
        if (alive)                                  // Living animations + movement
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::T))
                sprite.move({ 0, -speed });

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::G))
                sprite.move({ 0, speed });

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::F))
            {
                sprite.setTextureRect(sf::IntRect({ myFrame * 16 + 48, 256 }, { 16, 16 }));
                sprite.move({ -speed, 0 });
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::H))
            {
                sprite.setTextureRect(sf::IntRect({ myFrame * 16, 256 }, { 16, 16 }));
                sprite.move({ speed, 0 });
            }
        }

        else 									    // Death animation
        {
            if (myFrame == 0)                            // If frame is 0, show first frame
                sprite.setTextureRect(sf::IntRect({ 96, 256 }, { 16, 16 }));
            else                                        // Else, continue with purple frames
                sprite.setTextureRect(sf::IntRect({ myFrame * 16 + 112, 288 }, { 16, 16 }));
        }
    }

    if (type == 6)                              // Ghost enemy, IJKL controls
    {
        if (alive)                                  // Living animations + movement
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::I))
                sprite.move({ 0, -speed });

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::K))
                sprite.move({ 0, speed });

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::J))
            {
                sprite.setTextureRect(sf::IntRect({ myFrame * 16 + 48, 320 }, { 16, 16 }));
                sprite.move({ -speed, 0 });
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::L))
            {
                sprite.setTextureRect(sf::IntRect({ myFrame * 16, 320 }, { 16, 16 }));
                sprite.move({ speed, 0 });
            }
        }

		else 									    // Death animation
        {
            if(myFrame == 0)                            // If frame is 0, show first frame
                sprite.setTextureRect(sf::IntRect({ 96, 320 }, { 16, 16 }));
			else                                        // Else, continue with purple frames
                sprite.setTextureRect(sf::IntRect({ myFrame * 16 + 112, 272 }, { 16, 16 }));
        }
    }
}

void Enemy::die()
{
    if (!alive)
        return;

    alive = false;
	myFrame = myTick = 0;
}

bool Enemy::isAlive() { return alive; }
sf::FloatRect Enemy::getBounds() const { return sprite.getGlobalBounds(); }