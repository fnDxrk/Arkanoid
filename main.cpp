#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

//Скорость доски
int boardSpeed = 15;

//Скорость шарика
int ballSpeedX = 5;
int ballSpeedY = 5;

//
int checkFlag = 0;

//Путь иконок
String logoutFile = "Pictures/logout.png";
String reloadFile = "Pictures/reload.png";

//Рекорд
int scoreN = 0;
std::string scoreS = std::to_string(scoreN);

//Проверка игры
int checkMode = 0;

//Проверка позиции доски
int checkBoard = 0;

void initblock(RenderWindow &window, RectangleShape (&block)[10][7]) {
    for (int i = 1; i < 10; i++) {
        for (int j = 1; j < 7; j++) {
            block[i][j].setSize(Vector2f(90, 40));
            block[i][j].setFillColor(Color::White);
            block[i][j].setPosition(i * 110 - 95, j * 55 + 50);
        }
    }
}

void blockDraw(RenderWindow &window, RectangleShape (&block)[10][7]) {
    for (int i = 1; i < 10; i++) {
        for (int j = 1; j < 7; j++) {
            window.draw(block[i][j]);
        }
    }
}

void ballMove(RenderWindow &window, CircleShape &ball, RectangleShape &board, RectangleShape (&block)[10][7], Text& scoreT) {

    ball.move(ballSpeedX, 0);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 7; j++) {
            if (ball.getGlobalBounds().intersects(block[i][j].getGlobalBounds())) {
                block[i][j].setPosition(-100, -100);
                ballSpeedX = -ballSpeedX;
                scoreN += 50;
                scoreS = std::to_string(scoreN);
                scoreT.setString(scoreS);
            }
        }
    }

    ball.move(0, ballSpeedY);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 7; j++) {
            if (ball.getGlobalBounds().intersects(block[i][j].getGlobalBounds())) {
                block[i][j].setPosition(-100, -100);
                ballSpeedY = -ballSpeedY;
                scoreN += 50;
                scoreS = std::to_string(scoreN);
                scoreT.setString(scoreS);
            }
        }
    }
    
    if (ball.getGlobalBounds().intersects(board.getGlobalBounds())) ballSpeedY = -(rand()%7 + 3);

    if (ball.getPosition().x < 0 || ball.getPosition().x > 970) ballSpeedX = -ballSpeedX;
    if (ball.getPosition().y < 100  || ball.getPosition().y > 690) ballSpeedY = -ballSpeedY;
    if (ball.getPosition().y > 690 || scoreN == 2700)  {
        window.clear(Color::Black); 
        checkMode = 1;
    }
}

void initButton(RectangleShape& button, float xPos, float yPos) {
    button.setSize(Vector2f(160, 160));
    xPos = xPos - button.getGlobalBounds().width / 2;
    yPos = yPos - button.getGlobalBounds().height / 2;
    button.setPosition(xPos, yPos);
}

void initText(Text& text, Font& font, int size, String str, float xPos, float yPos,  Color textColor) {
    text.setFont(font);             //Шрифт
    text.setStyle(Text::Bold);      //Толщина шрифта
    text.setCharacterSize(size);    //Размер шрифта
    text.setString(str);            //Текст

    xPos = xPos - 10 - text.getGlobalBounds().width / 2;    //Выравнивание по X
    yPos = yPos - 20 - text.getGlobalBounds().height / 2;   //Выравнивание по Y

    text.setPosition(xPos, yPos);   //Позиция текста
    text.setFillColor(textColor);   //Цвет текста
}

void buttonExitCondition(RenderWindow& window, RectangleShape button[]) {
    if (Mouse::getPosition(window).x >= 350 + 12 - button[1].getGlobalBounds().width / 2 &&
        Mouse::getPosition(window).y >= 450 + 12 - button[1].getGlobalBounds().height / 2 &&
        Mouse::getPosition(window).x <= 350 - 10 + button[1].getGlobalBounds().width / 2 &&
        Mouse::getPosition(window).y <= 450 -10 + button[1].getGlobalBounds().height / 2) {
            if (Mouse::isButtonPressed(Mouse::Left)) {
                window.close();
            }
        }
}

void buttonReloadCondition(RenderWindow& window, RectangleShape button[]) {
        if (Mouse::getPosition(window).x >= 670 + 12 - button[2].getGlobalBounds().width / 2 &&
        Mouse::getPosition(window).y >= 450 + 12 - button[2].getGlobalBounds().height / 2 &&
        Mouse::getPosition(window).x <= 670 - 10 + button[2].getGlobalBounds().width / 2 &&
        Mouse::getPosition(window).y <= 450 -10 + button[2].getGlobalBounds().height / 2) {
            if (Mouse::isButtonPressed(Mouse::Left)) {
                checkMode = 0;
                scoreN = 0;
                scoreS = "0";
                checkFlag = 1;
            }
        }
}

void modeGame(RenderWindow& window, Event& ev) {
    if (checkMode == 0) {
        switch (ev.type) {
            case Event::Closed :
                window.close();
            case Event::KeyPressed :
                switch (ev.key.code) {
                    case Keyboard::Escape :
                        window.close();
                    case Keyboard::Space :
                        checkBoard = 1;
                }
        }
    }
}

void modeRestart(RenderWindow& window, Event& ev, RectangleShape button[]) {
    if (checkMode == 1) {
        buttonExitCondition(window, button);
        buttonReloadCondition(window, button);
        switch (ev.type) {
            case Event::Closed :
                window.close();
            case Event::KeyPressed :
                switch (ev.key.code) {
                    case Keyboard::Escape :
                        window.close();
                }
        }
    }
}

void windowGame(RenderWindow &window, CircleShape &ball, RectangleShape &board, Text& textScore, Text& score, RectangleShape (&block)[10][7]) {  
    window.clear(Color::Black); 
    window.draw(board);         //Отрисовка доски
    blockDraw(window, block);   //Отрисовка блоков
    window.draw(ball);          //Отрисовка шарика
    window.draw(textScore);
    window.draw(score);
    
    if (checkBoard == 0) {
        if (Keyboard::isKeyPressed(Keyboard::Right) && board.getPosition().x < 860) { 
            board.move(boardSpeed, 0);
            ball.move(boardSpeed, 0);
        }

        if (Keyboard::isKeyPressed(Keyboard::Left) && board.getPosition().x > 0) {
            board.move(-boardSpeed, 0);
            ball.move(-boardSpeed, 0);
        }
    }

    if (checkBoard == 1) {
        if (Keyboard::isKeyPressed(Keyboard::Right) && board.getPosition().x < 860) board.move(boardSpeed, 0);
        if (Keyboard::isKeyPressed(Keyboard::Left) && board.getPosition().x > 0) board.move(-boardSpeed, 0);
        ballMove(window, ball, board, block, score);
    }
}

void windowRestart(RenderWindow &window, RectangleShape button[], Text& scoreFinal, Text& score) {
    window.clear(Color::Black); 

    score.setCharacterSize(80);
    score.setPosition(680, 200);
    initButton(button[1], 350, 450);
    initButton(button[2], 670, 450);

    window.draw(scoreFinal);
    window.draw(score);
    window.draw(button[1]);
    window.draw(button[2]);
}

int main() {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Окно
    
    RenderWindow window;
    window.create(VideoMode(1000, 720), "Arkanoid");
    window.setFramerateLimit(60);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Шрифт

    Font font;
    //if (!font.loadFromFile("Font/Raleway-Thin.ttf")) return 1;
    if (!font.loadFromFile("Font/mont_extralightdemo.ttf")) return 1;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Доска
    
    RectangleShape board;
    board.setSize(Vector2f(140, 16));
    board.setFillColor(Color::White);
    float xBoardPos = 500 - board.getSize().x / 2;
    float yBoardPos = 650 - board.getSize().y / 2;
    board.setPosition(xBoardPos, yBoardPos);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Шарик

    CircleShape ball;
    ball.setRadius(12);
    ball.setFillColor(Color::White);
    float xBallPos = 500 - ball.getRadius();
    float yBallPos = 625 - ball.getRadius();
    ball.setPosition(xBallPos, yBallPos);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Блоки

    RectangleShape block[10][7];
    initblock(window, block);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Кнопки

    RectangleShape button[2]; 
    Texture logout;
    Texture reload;
    logout.loadFromFile(logoutFile);
    logout.setSmooth(true);
    reload.loadFromFile(reloadFile);
    reload.setSmooth(true);
    button[1].setTexture(&logout);
    button[2].setTexture(&reload);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Текст

    Text score;
    Text textScore;
    Text textScoreFinal;

    initText(textScore, font, 40, "Score : ", 810, 60, Color::White);
    initText(score, font, 40, scoreS, 890, 60, Color::White);
    initText(textScoreFinal, font, 70, L"Ваш рекорд : ", 440 , 250, Color::White);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    while (window.isOpen()) {
        Event ev;
        while (window.pollEvent(ev)) {
            modeGame(window, ev);
            modeRestart(window, ev, button) ;
        }

        window.clear(Color::Black);

        switch (checkMode) {
            case 0 :
                if (checkFlag == 1) {
                    board.setPosition(xBoardPos, yBoardPos);
                    ball.setPosition(xBallPos, yBallPos);
                    initblock(window, block);
                    window.draw(textScore);
                    
                    score.setCharacterSize(40);
                    score.setPosition(870, 30);
                    score.setString(scoreS);
                    window.draw(score);
                    
                    initButton(button[1], -100, -100);
                    initButton(button[2], -100, -100);

                    checkBoard = 0;
                    checkFlag = 0;
                }
                if (checkFlag == 0) windowGame(window, ball, board, textScore, score, block);
                break;
            case 1 :
                windowRestart(window, button, textScoreFinal, score);
                break;
        }

        window.display();
    }

    return 0;
}