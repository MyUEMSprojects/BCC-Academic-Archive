#include "Interface.h"
#include <filesystem>

// Private Functions

// Inicializar variaveis
void Interface::initVariables()
{
	this->window = nullptr;
	this->backspaceKeyPressed = false;
	this->enterPressed = false;
	this->unlockInput = true;
	this->unlockNext = true;
	this->inputHipo = -1;
	this->cont = 0;
}

// inicializar Janela/Tela
void Interface::initWindow()
{
	this->videoMode.height = 866;
	this->videoMode.width = 1574;

	this->window = new sf::RenderWindow(this->videoMode, "HIPO-I", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(60);
	this->window->setVerticalSyncEnabled(false);
}

// inicializa os shapes
void Interface::initShapes()
{
	rectangle.setSize(sf::Vector2f(1004, 669));
	rectangle.setPosition(423, 90);
	rectangle.setFillColor(sf::Color::Transparent); // sf::Color::Transparent

	nextButton.setSize(sf::Vector2f(259, 64));
	nextButton.setPosition(127, 90);
	nextButton.setFillColor(sf::Color::Transparent);

	runButton.setSize(sf::Vector2f(259, 64));
	runButton.setPosition(127, 178);
	runButton.setFillColor(sf::Color::Transparent);

	input.setSize(sf::Vector2f(259, (64 * 4) + 5));
	input.setPosition(127, 500);
	input.setFillColor(sf::Color::White);
}

// Inicializar Fontes
void Interface::initFonts()
{
	if (!(this->font.loadFromFile("Fonts/Inter-Regular.ttf")))
	{
		std::cout << "ERROR::INTERFACE::INITFONTS::Failed to load fonts!"
				  << "\n";
	}
}

// Inicializar Texto
void Interface::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(12);
	this->uiText.setFillColor(sf::Color::White);

	this->InstructionText.setFont(this->font);
	this->InstructionText.setPosition(430, 100);
	this->InstructionText.setCharacterSize(24);
	this->InstructionText.setFillColor(sf::Color::Black);
	this->InstructionText.setString("InstructionText-default");

	this->inputTextSeta.setFont(this->font);
	this->inputTextSeta.setPosition(130, 500);
	this->inputTextSeta.setCharacterSize(20);
	this->inputTextSeta.setFillColor(sf::Color::Black);
	this->inputTextSeta.setString(">");

	this->inputText.setFont(this->font);
	this->inputText.setPosition(145, 500);
	this->inputText.setCharacterSize(20);
	this->inputText.setFillColor(sf::Color::Black);
	this->inputText.setString(""); // input-text-default

	this->exitText.setFont(this->font);
	this->exitText.setPosition(130, 525);
	this->exitText.setCharacterSize(18);
	this->exitText.setFillColor(sf::Color::Black);
	this->exitText.setString(""); // input-text-default
}

// Inicializar background
void Interface::initBackground()
{

	if (!this->BackgroundTex.loadFromFile("Textures/background-interface.png"))
	{
		std::cout << "ERROR::GAME::COULD NOT LOAD BACKGROUND TEXTURE"
				  << "\n";
	}

	this->Background.setTexture(this->BackgroundTex);
}

// Inicializar Arquivo para leitura/interpretação
void Interface::initFile()
{
	this->m__index = 0;
	this->line_index = 0;
	this->source.open(this->path);
}

// inicializar HIPO
void Interface::initHIPO(int addr)
{
	std::cout << "HIPO = ON" << std::endl;
	this->cpu.setIndex(addr);
	this->cpu.setInstructionPointer(&memory[this->cpu.getIndex()]);
	this->cpu.setRunning(true);
}

// Constructor / Destructor
Interface::Interface(std::string &file) : memory(100), cpu(memory), fileName("")
{
	this->fileName += "files/";
	this->fileName += file;
	this->path = this->fileName.c_str();
	//std::cout << "Constructor::path: " << this->path << std::endl;
	this->initVariables();
	this->initWindow();
	this->initShapes();
	this->initFonts();
	this->initText();
	this->initBackground();
	this->initFile();  // inicializar o arquivo do programa
	this->ReadFile();  // ler o arquivo do programa
	this->initHIPO(0); // inicializar a interpretação do arquivo do programa
}

Interface::~Interface()
{
	delete this->window;
}

// retorna se a tela esta aberta
const bool Interface::running() const
{
	return this->window->isOpen();
}

void Interface::setFileName(const char* file)
{
}

// Imprime no Terminal quando finalizar leitura do arquivo
void Interface::Quit()
{
	std::cout << "HIPO = OFF" << std::endl;
}

void Interface::resetHIPO()
{
	// this->memory.clean();
	// this->initFile();
	// this->ReadFile();
	this->initHIPO(0);
	this->cpu.set_ler(0);
	this->cpu.set_imp(0);
	this->InstructionText.setString("");
	this->exitText.setString("");
	this->exit = "";
	std::string resetString = "";
	this->cpu.setCommand(resetString);
	this->backspaceKeyPressed = false;
	this->enterPressed = false;
	this->unlockInput = false;
	this->unlockNext = true;
	this->inputHipo = -1;
	this->cont = 0;
}

// Functions
// Atualizar eventos
void Interface::pollEvents()
{
	// Event polling
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type) // mouse click, keyboard
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:

			if (this->ev.key.code == sf::Keyboard::Escape)
			{

				this->window->close();
			}

			if (this->cpu.get_ler())
			{
				if (this->ev.key.code == sf::Keyboard::Backspace && this->input.getGlobalBounds().contains(mousePosView))
				{
					this->backspaceKeyPressed = true;
				}
				else if (this->ev.key.code == sf::Keyboard::Enter && this->input.getGlobalBounds().contains(mousePosView))
				{
					this->enterPressed = true;
				}
			}

			break;
		case sf::Event::KeyReleased:
			if (this->cpu.get_ler())
			{
				if (this->ev.key.code == sf::Keyboard::Backspace)
				{
					this->backspaceKeyPressed = false;
				}
				else if (this->ev.key.code == sf::Keyboard::Enter)
				{
					if (this->enterPressed && this->cpu.isRunning())
					{
						try
						{

							if (this->cpu.get_ler())
							{
								this->inputHipo = std::stoi(this->inputString);
								this->cpu.getMemory()[this->cpu.getOperand()] = inputHipo;
								this->cpu.set_ler(false);
								this->inputString = "";
								this->inputText.setString(this->inputString);
							}
						}
						catch (const std::exception &e)
						{
							std::cerr << e.what() << '\n';
							//
						}
					}
					this->enterPressed = false;
				}
			}

			break;
		case sf::Event::MouseButtonReleased:

			if (this->cpu.isRunning() && !this->cpu.get_ler())
			{
				if (this->nextButton.getGlobalBounds().contains(mousePosView))
				{
					if (this->ev.mouseButton.button == sf::Mouse::Left)
					{
						this->cont++;
						this->unlockInput = true;
						this->cpu.ReadInstructionPointer();
					}
				}
			}

			if (this->runButton.getGlobalBounds().contains(mousePosView))
			{
				if (this->ev.mouseButton.button == sf::Mouse::Left)
				{
					this->resetHIPO();
				}
			}

			if (this->cpu.get_ler())
			{
				if (this->input.getGlobalBounds().contains(mousePosView))
				{
					if (this->ev.mouseButton.button == sf::Mouse::Left)
					{
						this->unlockInput = false;
						this->inputText.setString("");
					}
				}
			}

			break;
		case sf::Event::TextEntered:
			if (this->cpu.get_ler())
			{

				if (this->ev.text.unicode < 128 && this->input.getGlobalBounds().contains(mousePosView))
				{
					if (this->backspaceKeyPressed) // Backspace
					{
						if (!this->inputString.empty())
							this->inputString.pop_back();
						this->inputText.setString(this->inputString);
					}
					else if (this->enterPressed)
					{
						// this->InstructionText.setString(inputString); // só para teste
					}
					else
					{
						this->inputString += static_cast<char>(this->ev.text.unicode);
						this->inputText.setString(this->inputString);
					}
				}
			}
			break;
		default:
			break;
		}
	}
}

// Atualizar a posição do mouse na tela
void Interface::updateMousePositions()
{
	/*
	  @ return void

	   updates the mouse positions;
		 - Mouse position relative to window (Vector2i)

	*/

	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

// atualizar texto
void Interface::updateText()
{

	// Posição do mouse --> Para desenvolvimento
	std::string mousePositionText = "Mouse X: " + std::to_string(this->mousePosView.x) +
									"\nMouse Y: " + std::to_string(this->mousePosView.y);
	this->uiText.setString(mousePositionText);

	if (this->cpu.get_ler() && unlockInput)
	{

		// animação da setinha
		if (clock.getElapsedTime().asSeconds() > 0.5)
		{
			this->inputTextSeta.setFillColor((this->inputTextSeta.getFillColor() == sf::Color::Black) ? sf::Color::Transparent : sf::Color::Black);
			clock.restart();
		}
	}

	if (this->cpu.get_imp())
	{
		exit += std::to_string(this->cpu.getMemory().Read(this->cpu.getOperand()));
		this->exitText.setString(exit);
		this->cpu.set_imp(false);
	}

	if (this->cpu.isRunning())
	{
		this->InstructionText.setString(this->cpu.getCommand());

		if (this->cont == 22)
		{
			std::string newCommand = "";
			this->cpu.setCommand(newCommand);
			this->cont = 0;
		}
	}
}

// atualizar background
void Interface::updateBackground()
{
	//
}

void Interface::ReadFile()
{
	if (source.is_open())
	{
		// std::string line;
		while ((std::getline(source, line) && line != "PAR"))
		{
			std::istringstream iss(line);
			std::string cmd, op;

			/*(sf::Mouse::isButtonPressed(sf::Mouse::Left) && nextButton.getGlobalBounds().contains(mousePosView)) */

			iss >> cmd;
			if (cmd != "FIM")
			{

				iss >> op;
				this->memory[m__index] = this->cpu.Encode(cmd.c_str(), op.c_str());
				m__index++;
			}
		}
	}

	source.close();
}

// Funcao Update principal
void Interface::update()
{
	this->pollEvents();
	this->updateBackground();

	this->updateMousePositions();
	this->updateText();
}

// Renderizar shapes
void Interface::renderShapes()
{
	this->window->draw(this->rectangle);
	this->window->draw(this->nextButton);
	this->window->draw(this->runButton);
	this->window->draw(this->input);
}

// Renderizar texto
void Interface::renderText(sf::RenderTarget &target)
{
	target.draw(this->uiText);
	target.draw(this->InstructionText);
	target.draw(this->inputTextSeta);
	target.draw(this->inputText);
	target.draw(this->exitText);
}

// Renderizar background
void Interface::renderBackground()
{
	this->window->draw(this->Background);
}

// Funcao Render Principal
void Interface::render()
{
	/*
	   @return void

	   - clear old frame
	   - render objects
	   - display frame in window

	   Render the game objects
	*/

	this->window->clear();

	// Draw Backgrounde
	this->renderBackground();

	// Render Shapes
	this->renderShapes();

	// Draw game Objects
	this->renderText(*this->window);

	this->window->display();
}