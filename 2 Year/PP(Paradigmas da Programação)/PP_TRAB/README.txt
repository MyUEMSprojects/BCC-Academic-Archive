// Instação SFML LINUX

sudo apt-get remove libsfml-dev --> deletar arquivos sfml's anteriores 
sudo apt-get install libsfml-dev


g++ sfml.cpp -o prog -lsfml-graphics -lsfml-window -lsfml-system   // nao necessário no projeto atual, pois usa cmake

// Instalar CMAKE!!


// usando cmake para compilar o programa

cd build     // entra na pasta da build do programa
make clean  // remover arquivos temporarios
cmake ..   // constrói usando a config
make      // compila o projeto
./hipo   // roda o programa


OBS:

 1- para compilar um arquivo.hipo usar o CMakeLists.txt, e adicionar --> configure_file(${SOURCE_DIR}/files/seu_programa.hipo ${CMAKE_CURRENT_BINARY_DIR}/files/seu_programa.hipo COPYONLY)
 2 - basic_sum.hipo e seq_sum.hipo sao para teste