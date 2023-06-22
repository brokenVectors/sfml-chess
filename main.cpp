#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <cmath>
#include "game.h"

constexpr int SQUARE_SIZE = 100;
constexpr int SPRITE_SIZE = 320;

sf::Vector2f snap_to_grid(sf::Vector2f input) {
    int x = (int) input.x;
    int y = (int) input.y;
    return sf::Vector2f(x - (x % SQUARE_SIZE), y - (y % SQUARE_SIZE));
}

Coord pos_to_coord(sf::Vector2f pos) {
    Coord coord;
    coord.x = pos.x / SQUARE_SIZE;
    coord.y = pos.y / SQUARE_SIZE; 
    return coord;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(SQUARE_SIZE * 8, SQUARE_SIZE * 8), "Hello World!");
    sf::Texture texture;  // The spritesheet for the chess pieces.
    sf::RectangleShape* rects[8][8]; // 2D array of the checkerboard squares.
    sf::Sprite* sprites[32]; // Array of chess piece sprites.
    sf::Sprite* spriteGrid[8][8]; // to access the sprites by coordinate. prefer sprites, because looping through is quicker
    sf::Sprite* selectedSprite = NULL; // Points to the sprite that the user has selected by dragging.
    sf::Sprite* whiteKing = NULL; // Points to the sprite representing the white king.
    sf::Sprite* blackKing = NULL; // Points to the sprite representing the black king.
    sf::Vector2f selectedSpriteOrigin; // Stores the initial position of the selected sprite.
    int sprite_count = 0;
    Game game; // The game instance, contains the board representation and handles legal move generation.
    if (!texture.loadFromFile("pieces.png")) {
        std::cout << "Could not load texture pieces.png." << std::endl;
    }
    texture.setSmooth(true);
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            sf::RectangleShape* rect = new sf::RectangleShape();
            rect->setSize(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            rect->setPosition(sf::Vector2f(i*SQUARE_SIZE, j*SQUARE_SIZE));
            sf::Color color;
            if((i + j) % 2 == 0) {
                color = sf::Color(238,238,210);
            }
            else {
                color = sf::Color(118,150,86);
            }
            rect->setFillColor(color);
            rects[j][i] = rect;
            if(game.board[j][i] != 0) {
                sf::Sprite* sprite = new sf::Sprite();
                sf::IntRect intRect;
                intRect = sf::IntRect(SPRITE_SIZE*((game.board[j][i]-1)%6), SPRITE_SIZE*floor(game.board[j][i]/7), SPRITE_SIZE, SPRITE_SIZE);
                sprite->setTexture(texture);
                sprite->setPosition(sf::Vector2f(i * SQUARE_SIZE, j * SQUARE_SIZE));
                sprite->setTextureRect(intRect);
                sprite->scale(sf::Vector2f((float)SQUARE_SIZE / SPRITE_SIZE, (float)SQUARE_SIZE / SPRITE_SIZE));
                sprites[sprite_count] = sprite;
                spriteGrid[j][i] = sprite;
                sprite_count++;
                if(game.board[j][i] == 1) {
                    whiteKing = sprite;
                }
                if(game.board[j][i] == 7) {
                    blackKing = sprite;
                }
            }
        }
    }
    window.requestFocus();
    while(window.isOpen()) {
        sf::Event event;
        if(window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                        if(event.mouseButton.button == sf::Mouse::Left) {
                            for(int i = 0; i < sprite_count; i++) {
                                sf::Sprite* sprite = sprites[i];
                                if(sprite) {
                                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                                    sf::Vector2f spritePos = sprite->getPosition();
                                    if(
                                        (mousePos.x > spritePos.x)
                                    && (mousePos.y > spritePos.y)
                                    && (mousePos.x < spritePos.x + SQUARE_SIZE)
                                    && (mousePos.y < spritePos.y + SQUARE_SIZE)
                                    && (sprite != selectedSprite)
                                    ) {
                                        selectedSprite = sprite;
                                        selectedSpriteOrigin = selectedSprite->getPosition();
                                    }
                                }
                            }
                        }
                    break;
                case sf::Event::MouseButtonReleased:
                    // let go of piece, make the move
                    if(event.mouseButton.button == sf::Mouse::Left) {
                        if(selectedSprite) {
                                sf::Vector2f spritePos = selectedSprite->getPosition();
                                sf::Vector2f snappedPos = snap_to_grid(sf::Vector2f(spritePos.x + SQUARE_SIZE / 2, spritePos.y + SQUARE_SIZE / 2));
                                Coord origin = pos_to_coord(selectedSpriteOrigin);
                                Coord target = pos_to_coord(snappedPos);
                                Move move {origin, target};

                                printMove(move);
                                MoveResult result = game.makeMove(move); // makeMove replaces the move with another instance, containing additional information like if it was a castling move, promotion, etc.
                                if(result.success) { 
                                    for(int i = 0; i < sprite_count; i++) {
                                        sf::Sprite* sprite = sprites[i];
                                        if(sprite && sprite->getPosition() == snappedPos && sprite != selectedSprite) {
                                            delete sprite;
                                            sprites[i] = NULL;
                                            
                                        }
                                        if(result.enPassantSquare.x != -1 && sprites[i] == spriteGrid[result.enPassantSquare.y][result.enPassantSquare.x]) {
                                            spriteGrid[result.enPassantSquare.y][result.enPassantSquare.x] = NULL;
                                            delete sprites[i];
                                            delete spriteGrid[result.enPassantSquare.y][result.enPassantSquare.x];
                                            sprites[i] = NULL;
                                            spriteGrid[result.enPassantSquare.y][result.enPassantSquare.x] = NULL;
                                        }
                                    }
                                    spriteGrid[move.target.y][move.target.x] = spriteGrid[move.origin.y][move.origin.x];
                                    spriteGrid[move.origin.y][move.origin.x] = NULL;
                                    selectedSprite->setPosition(snappedPos);

                                    // update pieces
                                    if(result.kingside) {
                                        spriteGrid[move.target.y][move.target.x - 1] = spriteGrid[move.target.y][move.target.x + 1];
                                        spriteGrid[move.target.y][move.target.x + 1] = NULL;
                                        spriteGrid[move.target.y][move.target.x - 1]->setPosition(sf::Vector2f(SQUARE_SIZE * (move.target.x - 1), SQUARE_SIZE * move.target.y));
                                    }
                                    else if(result.queenside) {
                                        spriteGrid[move.target.y][move.target.x + 1] = spriteGrid[move.target.y][move.target.x - 2];
                                        spriteGrid[move.target.y][move.target.x - 2] = NULL;
                                        spriteGrid[move.target.y][move.target.x + 1]->setPosition(sf::Vector2f(SQUARE_SIZE * (move.target.x + 1), SQUARE_SIZE * move.target.y));
                                    }
                                    else if(result.promotionPiece != -1) {
                                        sf::IntRect intRect;
                                        intRect = sf::IntRect(SPRITE_SIZE*((game.board[move.target.y][move.target.x]-1)%6), SPRITE_SIZE*floor(game.board[move.target.y][move.target.x]/7), SPRITE_SIZE, SPRITE_SIZE);
                                        spriteGrid[move.target.y][move.target.x]->setTextureRect(intRect);
                                    }
                                }
                                else {
                                    // snap back to initial square if move was illegal
                                    selectedSprite->setPosition(selectedSpriteOrigin);
                                }
                                selectedSprite = NULL;
                        }
                    }
                case sf::Event::MouseMoved:
                    // dragging, move piece w/ mouse
                    if(selectedSprite) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        selectedSprite->setPosition(sf::Vector2f(mousePos.x - SQUARE_SIZE / 2, mousePos.y - SQUARE_SIZE / 2));
                    }
                default:
                    break;
            }
        }
        window.clear(sf::Color::White);
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                window.draw(*rects[j][i]);
            }
        }
        for(int i = 0; i < 32; i++) {
            if(sprites[i] && sprites[i] != selectedSprite) {
                window.draw(*sprites[i]);
            }
        }
        // drawing the selected sprite on top of everything else
        if(selectedSprite) {
            window.draw(*selectedSprite); 
        }
        window.display();
    }
}