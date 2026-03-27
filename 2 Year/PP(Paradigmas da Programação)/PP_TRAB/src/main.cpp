#include <iostream>
#include "Interface.h"

int main(int argc, char *argv[])
{

   srand(static_cast<unsigned>(time(NULL)));

   if (argc != 2)
   {
      std::cout << "Digite o nome do arquivo .hipo assim:" <<  std::endl;
      std::cerr << "Uso: " << argv[0] << " <nome_do_arquivo>" << std::endl;
      return 1;
   }

   //const char* nomeArquivo = argv[1];
   std::string nomeArquivo = argv[1];

   Interface interface(nomeArquivo);

   while (interface.running())
   {

      // Update
      interface.update();

      // Render
      interface.render();
   }

   interface.Quit();

   return 0;
}