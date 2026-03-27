#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

 int freq=0,controle=0,i;
 int matricula=0;
 float maior=0,menor=0,media=0,nota[3];
 float soma=0,cont_reprov=0,cont_freq=0;

nota[0]=0;
nota[1]=0;
nota[2]=0;

for(i=1;i<=100;i++){

do{
printf("\nALUNO: %i\n",i);
printf("\nMatricula:\n");
 scanf("%d",&matricula);
}while(matricula<0);

system("cls");

do{
system("cls");
printf("\nALUNO: %i\n",i);
printf("\nNOTA 1:\n");
 scanf("%f",&nota[0]);

if(nota[0]>=maior){
  maior = nota[0];
}
if(nota[0]<=menor){
   menor = nota[0];
}

}while(nota[0]<0 || nota[0]>100);

system("cls");

do{
system("cls");
printf("\nALUNO: %i\n",i);
printf("\nNOTA 2:\n");
 scanf("%f",&nota[1]);

if(nota[1]>=maior){
  maior = nota[1];
}
if(nota[1]<=menor){
  menor = nota[1];
}

}while(nota[1]<0 || nota[1]>100);

system("cls");

do{
system("cls");
printf("\nALUNO: %i\n",i);
printf("\nNOTA 3:\n");
 scanf("%f",&nota[2]);

if(nota[2]>=maior){
  maior = nota[2];
}
if(nota[2]<=menor){
  menor = nota[2];
}

}while(nota[2]<0 || nota[2]>100);

system("cls");

do{
system("cls");
printf("\nALUNO: %i\n",i);
printf("\nFrequencia:\n");
 scanf("%d",&freq);
}while(freq<0);

media = (nota[0]+nota[1]+nota[2])/3;

soma+=media;

if(freq<40 && media<60){
printf("\nALUNO: %i\n",i);
printf("\nMatricula: %d\n",matricula);
printf("\nFrequencia abaixo de 40 aulas e media menor que 60!\n");
printf("\nMEDIA: %2.f\n",media);
printf("\nFrequencia: %d\n",freq);
printf("\nStatus:\n");
printf("\nReprovado!\n");
printf("\n-----|-----\n");
cont_freq++;
cont_reprov++;
}
else if(freq>=40 && media<60){
printf("\nALUNO: %i\n",i);
printf("\nMatricula: %d\n",matricula);
printf("\nMedia menor que 60!\n");
printf("\nMEDIA: %2.f\n",media);
printf("\nFrequencia: %d\n",freq);
printf("\nStatus:\n");
printf("\nReprovado!\n");
printf("\n-----|-----\n");
cont_reprov++;
}
else if(freq<40 && media>=60){
printf("\nALUNO: %i\n",i);
printf("\nMatricula: %d\n",matricula);
printf("\nFrequencia abaixo de 40 aulas:\n");
printf("\nMEDIA: %2.f\n",media);
printf("\nFrequencia: %d\n",freq);
printf("\nStatus:\n");
printf("\nReprovado!\n");
printf("\n-----|-----\n");
cont_freq++;
cont_reprov++;
}
else{
printf("\nALUNO: %i\n",i);
printf("\nMatricula: %d\n",matricula);
printf("\nMEDIA: %2.f\n",media);
printf("\nFrequencia: %d\n",freq);
printf("\nStatus:\n");
printf("\nAprovado!\n");
printf("\n-----|-----\n");
}

}
printf("\nMaior nota: %2.f\n",maior);
printf("\nMenor nota: %2.f\n",menor);
printf("\nNota media da Turma: %2.f\n",soma/100);
printf("\nAlunos Reprovados: %d\n",cont_reprov);
printf("\nPercentagem de Alunos Reprovados por infrequencia: %f\n",(cont_freq*100)/100);


	system("pause");
	return 0;
}