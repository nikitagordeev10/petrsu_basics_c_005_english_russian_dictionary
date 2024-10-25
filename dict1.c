#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 1023

/* цикл обработки строк словаря и вывода отобранных статей */
void show_entries(const char *pattern, FILE *stream);

/* nac-наличие ^                                                                                                  
   pak-наличие $ */
int nak = 0, pak = 0;

int main(int argc, char*argv[]) 
{
    /* Проверка на наличие входного файла */
    if(argc == 1){
	printf("Введённый файл отсутствует\n");
	    return 0;
    }
    
    /* Файл словаря */
    FILE* dict = NULL;
    
    /*  Название искомой статьи */
    char word[MAXLINE];
    
    /* Открываем файл словаря */
    dict = fopen(argv[1], "r");
    
	    /* Проверка на открытие файла */
	    if (dict == NULL){
		printf("Что-то введено не так\n");
		return 0;
	    }
    
    /* Получаем название словарной статьи для поиска */
    scanf("%s", word);
    
	    /*Проверка наличия знака '^' в начале строки */
	    if (word[0] == '^') {
		nak = 1;
		/*Убираем символ при наличии */
		for (unsigned long i = 0; i < strlen(word); i++) {
		    word[i] = word[i + 1];
		}
	    }
	    
	    /*Проверка наличия знака '$' в конце строки */
	    if (word[strlen(word) - 1] == '$') {
		pak = 1;
		/*Убираем символ при наличии */
		word[strlen(word) - 1] = '\0';
	    }

	    show_entries(word, dict);
	    return 0;
}

/* цикл обработки строк словаря и вывода отобранных статей */
void show_entries(const char *pattern, FILE *stream)
{
    /*Текущая строка */
    char current_line[MAXLINE + 1] = "";
    /*Счетчик номера найденной статьи */
    int find_entry_number = 0;
    /*Флаг соответствия с шаблоном */
    int flag  = 0;
    
    while (fgets(current_line, MAXLINE, stream) != NULL)
    {
        if (isspace(current_line[0]) == 0)
	{   /*Если в текущей строке найден паттерн */
            if (strstr(current_line, pattern))
	    {   /*Запомним индекс первого совпадения текущей строки с паттерном */
                unsigned long index = strstr(current_line, pattern) - current_line;

                /*Проверка на наличие одного из условий:
                В веденной строке есть ^ и $, и длина строки совпадает с длиной паттерна */
		if (pak == 1 && nak == 1 && strlen(current_line) - 1 == strlen(pattern))
		{   flag = 1;
                    find_entry_number++;
                    printf("%d)", find_entry_number);
                }
		/* Есть символ ^ и начало паттерна совпадает с началом текущей строки */
		else if (pak == 0 && nak == 1 && index == 0)
		{    flag = 1;
                     find_entry_number++;
                     printf("%d)", find_entry_number);
                }
		/* Есть символ $ и конец паттерна совпадает с началом текущей строки */
		else if (pak == 1 && nak == 0 && index == strlen(current_line) - strlen(pattern) - 1)
		{    flag = 1;
                     find_entry_number++;
                     printf("%d)", find_entry_number);
                }
		/*Если ключевых символов нет */
		else if (pak == 0 && nak == 0)
		{    flag = 1;
                     find_entry_number++;
                     printf("%d)", find_entry_number);
                }

		else flag = 0;
            }

	    else flag = 0;
      }
       
        if (flag) printf("%s", current_line);
      
    }
   
    if (find_entry_number == 0) {
        printf("Слово ещё не добавленно в наш словарь, но скоро мы это поправим.\n");
    }
    
    /*Закрываем файл */
    fclose(stream);

}
