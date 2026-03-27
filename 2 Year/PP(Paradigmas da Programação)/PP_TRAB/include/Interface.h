#pragma once
#include "sfml-libs.h"
#include "CPU.h"
#include "Memory.h"
#include <vector>
#include <ctime>

class Interface
{
private:

    //Window
    sf::RenderWindow *window;
    sf::VideoMode videoMode;
    sf::Event ev;

    // HIPO
    Memory memory;
    CPU cpu;

    //Mouse positions
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    //Resources;
    sf::Font font;

    // RectangleShape
    sf::RectangleShape rectangle;
    sf::RectangleShape nextButton;
    sf::RectangleShape runButton;
    sf::RectangleShape input;

    //Text
    sf::Text uiText;
    sf::Text InstructionText;
    sf::Text inputText;
    sf::Text inputTextSeta;
    sf::Text exitText;

    // Animação da setinha do input
    sf::Clock clock;

    // input
    std::string inputString;
    std::string exit;
    int inputHipo;
    bool backspaceKeyPressed;
    bool enterPressed;
    bool unlockInput;
    bool unlockNext;
    
    //teste
    int cont;
    std::string nova;

    // File
    std::string line;
    int line_index;
    int m__index;
    std::string fileName;
    const char *path;
    std::ifstream source;


    //Background
    sf::Texture BackgroundTex;
	sf::Sprite Background;

    //Private Functions
    void initVariables();
    void initWindow();
    void initShapes();
    void initFonts();
    void initText();
    void initBackground();
    void initFile();
    void initHIPO(int addr);
public:
    Interface(std::string &file);
    virtual ~Interface();

    //Accesors
    const bool running() const;
    void setFileName(const char* file);
    
    void Quit();
    void resetHIPO();

    // Functions
    void pollEvents();
    void updateMousePositions();
    void renderShapes();
    void updateText();
    void updateBackground();
    void ReadFile();
    void update();

    void renderText(sf::RenderTarget &target);
    void renderBackground();
    void render();
};
