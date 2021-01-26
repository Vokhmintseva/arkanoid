// void handlePrize(sf::FloatRect brickBounds, PrizeType prizeType)
// {
//     sf::Sprite *spritePtr = new sf::Sprite();
//     spritePtr->setTexture(getPrizeSpriteTexture(prizeType));
//     spritePtr->setPosition(brickBounds.left, brickBounds.top);
//     activePrizes.push_back(spritePtr);
// }

// void drawPrizes(sf::RenderWindow &window)
// {
//     for (sf::Sprite *prizeSprite : activePrizes)
//     {
//         window.draw(*prizeSprite);
//     }
// }

// void loadPrizesTextures()
// {
//     expandPlatformSprite.setTexture(getExpandPlatformTexture());
//     twoBallsSprite.setTexture(getTwoBallsTexture());
//     slowBallDownSprite.setTexture(getSlowBallDownTexture());
//     accelerateBallSprite.setTexture(getAccelerateBallTexture());
//     extraLifeSprite.setTexture(getExtraLifeTexture());
//     portalDoorSprite.setTexture(getPortalDoorTexture());
//     stickyBallSprite.setTexture(getStickyBallTexture());
// }

// void changePlatformSize(float koeff)
// {
//     sf::Vector2f platformSize(PLATFORM_WIDTH * koeff, PLATFORM_HEIGHT);
//     platform.setScale(
//         platformSize.x / platform.getLocalBounds().width,
//         platformSize.y / platform.getLocalBounds().height);
// }

// void changeBallSpeed(float koeff)
// {
//     ballSpeed = regularBallSpeed * koeff;
// }

// void twoBalls()
// {
// }

// void extraLife()
// {
// }

// void portalDoor()
// {
// }

// void stickyBall()
// {
// }

// void applyPrizeEffect(PrizeType prizeType)
// {
//     switch (prizeType)
//     {
//     case expand_platform:
//         koeffOfPlatformExpansion += 0.35;
//         if (koeffOfPlatformExpansion < 2)
//             changePlatformSize(koeffOfPlatformExpansion);
//         break;
//     case accelerate_ball:
//         koeffOfBallSpeed += 0.35;
//         if (koeffOfBallSpeed < 2)
//             changeBallSpeed(koeffOfBallSpeed);
//         break;
//     case slow_ball_down:
//         koeffOfBallSpeed -= 0.35;
//         if (koeffOfBallSpeed > 0)
//             changeBallSpeed(koeffOfBallSpeed);
//         break;
//     case two_balls:
//         twoBalls();
//         break;
//     case extra_life:
//         extraLife();
//         break;
//     case portal_door:
//         portalDoor();
//         break;
//     case sticky_ball:
//         stickyBall();
//         break;
//     }
// }

// PrizeType getPrizeType(sf::Sprite *prizeSprite)
// {
//     const sf::Texture *prizeTexture = prizeSprite->getTexture();
//     return getPrizeType(prizeTexture);
// }

// void deletePtr(sf::Sprite *prizeSprite)
// {
//     prizeSprite = NULL;
//     delete prizeSprite;
// }

// void deletePrizeFromActivePrizes(sf::Sprite *prizeSprite)
// {
//     activePrizes.erase(std::remove(activePrizes.begin(), activePrizes.end(), prizeSprite), activePrizes.end());
//     deletePtr(prizeSprite);
// }

// void handlePrizeCollisionWithPlatform(sf::Sprite *prizeSprite, float &dt)
// {
//     prizeStartTime = dt;
//     PrizeType prizeType = getPrizeType(prizeSprite);
//     applyPrizeEffect(prizeType);
//     deletePrizeFromActivePrizes(prizeSprite);
//     PrizeEffect effect;
//     effect.prizeEffectType = getPrizeType(prizeSprite);
//     effect.timeOfEffectApplying = 0;
//     prizeEffects.push_back(effect);
// }

// void checkPrizeCollisionWithPlatform(float &dt)
// {
//     const sf::FloatRect platformBounds = platform.getGlobalBounds();
//     for (sf::Sprite *prizeSprite : activePrizes)
//     {
//         const sf::FloatRect prizeBounds = prizeSprite->getGlobalBounds();
//         if (prizeBounds.intersects(platformBounds))
//         {
//             handlePrizeCollisionWithPlatform(prizeSprite, dt);
//         }
//     }
// }

// void checkPrizeMiss()
// {
//     for (sf::Sprite *prizeSprite : activePrizes)
//     {
//         const sf::FloatRect prizeBounds = prizeSprite->getGlobalBounds();
//         const sf::Vector2f currPrizeSpritePos = prizeSprite->getPosition();
//         if (currPrizeSpritePos.y + prizeBounds.height > BOTTOM)
//             deletePrizeFromActivePrizes(prizeSprite);
//     }
// }

// void updateFallingPrizes(float &dt)
// {
//     if (isPaused || gameState != playing)
//         return;
//     const float prizeSpeed = 10;
//     for (sf::Sprite *prizeSprite : activePrizes)
//     {
//         sf::Vector2f currPos = prizeSprite->getPosition();
//         const float newYpos = currPos.y + dt * prizeSpeed;
//         prizeSprite->setPosition(currPos.x, newYpos);
//     }
//     checkPrizeCollisionWithPlatform(dt);
//     checkPrizeMiss();
// }

// void closePortalDoor()
// {
// }

// void nonStickyBall()
// {
// }

// void undoEffect(PrizeType prizeType)
// {
//     switch (prizeType)
//     {
//     case expand_platform:
//         koeffOfPlatformExpansion -= 0.35;
//         if (koeffOfPlatformExpansion > 0)
//             changePlatformSize(koeffOfPlatformExpansion);
//         break;
//     case accelerate_ball:
//         koeffOfBallSpeed -= 0.35;
//         if (koeffOfBallSpeed > 0)
//             changeBallSpeed(koeffOfBallSpeed);
//         break;
//     case slow_ball_down:
//         koeffOfBallSpeed += 0.35;
//         if (koeffOfBallSpeed < 2)
//             changeBallSpeed(koeffOfBallSpeed);
//         break;
//     // case two_balls_prize_state:
//     //     twoBalls();
//     //     break;
//     // case extra_life_prize_state:
//     //     extraLife();
//     //     break;
//     case portal_door:
//         closePortalDoor();
//         break;
//     case sticky_ball:
//         nonStickyBall();
//         break;
//     }
// }

// void updatePrizeEffects(float &dt)
// {
//     if (isPaused || gameState != playing)
//         return;
//     for (int i = 0; i < prizeEffects.size(); i++)
//     {
//         const float effectTime = prizeEffects[i].timeOfEffectApplying;
//         const float newEffectTime = effectTime + dt;
//         prizeEffects[i].timeOfEffectApplying = newEffectTime;

//         if (prizeEffects[i].timeOfEffectApplying > regularTimeOfPrizeEffect)
//         {
//             undoEffect(prizeEffects[i].prizeEffectType);
//             prizeEffects.erase(prizeEffects.begin() + i);
//         }
//     }
// }