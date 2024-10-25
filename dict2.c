//ЗАГОЛОВОЧНЫЕ ФАЙЛЫ
/* определения макросов, константы и объявления функций и типов, используемых для различных операций стандартного ввода и вывода. */
#include <stdio.h>
/* функции, занимающиеся выделением памяти, контролем процесса выполнения программы, преобразованием типов и другие. */
#include <stdlib.h>
/* функции для работы со строками, оканчивающимися на 0, и различными функциями работы с памятью. */
#include <string.h>
/* объявления функций для классификации и преобразования отдельных символов.*/
#include <ctype.h>
/*размер буфера для хранения строки текста*/
#define MAXLINE 1023
/*предварительно выделенная область динамической памяти, 10 Мб*/
#define TENMB 10485760
// библиотека малок - выделение памяти

//ИНИЦИАЛИЗАЦИЯ ФУНКЦИЙ
char* load_dictionary(char *dictionary, FILE *stream); // Функция для загрузки данных
char* filter_dictionary(const char *pattern, const char *dictionary, char *entries); // Функция поиска шаблона
char entries[TENMB]; //Данные на вывод
char dictionary[TENMB]; // Загруженные данные со словаря


//ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ
int nac = 0; //nac-начало ^ 
int kon = 0; //kon-конец $
int fs = 0; // Флаг соответствия


//ГЛАВНАЯ ФУНКЦИЯ ПРОГРАММЫ 
/*argc - количество параметров, передаваемых в функцию main, >1;*/
/*argv[] - массив указателей на строки.*/
int main(int argc, char*argv[]) {  

    /* Проверка на наличие входного файла */
    if(argc < 2){
	printf("Введённый файл отсутствует\n");
	    return 0;
    }
    
    /* Файл словаря */
    FILE* dict = NULL;
    
    /* Название искомой статьи */
    char word[MAXLINE];
    
    /* Открываем файл словаря */
    dict = fopen(argv[1], "r");
    
    /* Проверка на открытие файла */
    if (dict == NULL){
        printf("Что-то введено не так\n");
        return 0;
    }

    /*Вызываем функцию, проверяем превышение размера файла */ 
    if (load_dictionary(dictionary, dict) == NULL) {
	printf("Превышение размера файла\n"); //ситуация, когда при чтении данных из файла обнаруживается, что выделенного объема недостаточно
	return 0;
    }

    fclose(dict);

    printf ("Введите слово для поиска:\n");

    /*Пока пользоватьель вводит новые слова */
    while (scanf("%s", word) != -1) {
    	nac = 0;
    	kon = 0;
    	
	/*Проверка наличия знака '^' в начале строки */
	if (word[0] == '^') {
	    nac = 1;
	    /*Убираем символ при наличии */
	    for (unsigned long i = 0; i < strlen(word); i++) {
		word[i] = word[i + 1];
	    }
	}

	/*Проверка наличия знака '$' в конце строки */
	if (word[strlen(word) - 1] == '$') {
	    kon = 1;
	    /*Убираем символ при наличии */
	    word[strlen(word) - 1] = '\0';
	}

	/* Вызываем функцию, проверяем возвращенное значение, если NULL, то заканчиваем итерацию */
	/* entries - данные на вывод, 
	   dictionary - загруженные данные со словаря */
	/* && - and, ==  - =*/
        if ((filter_dictionary(word, dictionary, entries) == NULL) && (fs != 1)) {
	    printf("Слово ещё не добавленно в наш словарь, но скоро мы это поправим.\n");
	}
	
	else if (fs == 1)
	{    printf("Превышение размера файла\n");
	     fs = 0;
	     return -1;
        }

	/*Выписываем найденные статьи */
	printf("%s", entries);
	
	printf("Введите новое слово:\n");
	/*Обнуляем массив entries */
	for (unsigned long i = 0; i < strlen(entries); i++) entries[i] = 0;
    }
    return 0;
}


//ВЫДЕЛЕННЫЕ ФУНКЦИИ
/* Функция для загрузки данных */
// Функция fread считывает массив размером — count элементов, каждый из которых имеет размер size байт, из потока, и сохраняет его в блоке памяти
// Функция fseek перемещает указатель позиции в потоке. Устанавливает внутренний указатель положения в файле, в новую позицию, которая определяются путем добавления смещения к исходному положению.
char *load_dictionary(char *dictionary, FILE * stream) {

    /*Ставим указатель на конец открытого файла */
    fseek(stream, 0L, SEEK_END);
    
    size_t size = ftell(stream);
    
    /*Ставим указатель на начало */
    fseek(stream, 0L, SEEK_SET);

    if (size > TENMB) return NULL;
    
    /*Записываем все символы файла в символьный массив dictionary */
    if (fread(dictionary, 1, size, stream) != size) return NULL;

    return dictionary;
}

/* Функция однократного поиска шаблона  */
char *filter_dictionary(const char *pattern, const char *dictionary, char *entries) {
    
    //ИНИЦИАЛИЗАЦИЯ ФУНКЦИЙ
    /*Текущая строка */
    char current_line[MAXLINE + 1] = "";
    /*Счетчик номера найденной статьи */
    int find_entry_number = 0;
    /*find_entry_number в символьном типе */
    char number[20] = "";

    //ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ
    int l = 0; // k - индекс рассматриваемого элемента
    int k = 0; // l - индекс элемента текущей строки 
    int flag = 0; // Флаг соответствия с шаблоном
    
    /*Пока не достигнут конец файла */
    while (dictionary[k] != '\0') {
	l = 0;
	
	/* Проходим по строке  */
	while (dictionary[k] != '\n' && dictionary[k] != '\0') {   
	/*Переписываем строку из dictionary в current_line */
	    if (l >= MAXLINE) {   
	    	fs = 1;
		return NULL;
	    }
	    current_line[l++] = dictionary[k++];
	}
	
	/*Добавляем абзац в конец строки, и конец файла */
	current_line[l++] = '\n';
	current_line[l] = '\0';
	
	/*Если первый символ не пробел - isspace проверяет параметр, является ли он символом пробела. */
	if (!isspace(current_line[0])) {   
	
	/*Если в текущей строке есть паттерн - Функция ищет первое вхождение подстроки pattern в строке current_line */
	    if (strstr(current_line, pattern)) {   
	    	
	    	/*Обнуляем number */
		for (unsigned long i = 0; i < strlen(number); i++) number[i] = 0;
		
		/*Переводим find_entry number в символьный массив number */
		sprintf(number, "%d", find_entry_number);
		number[strlen(number)] = ')';
		
		/* Запоминаем индекс первого совпадения текущей строки с паттерном */
		unsigned long index = strstr(current_line, pattern) - current_line;

		//ПРОВЕРКА НА НАЛИЧИЕ ОДНОГО ИЗ УСЛОВИЙ:                                                               
		/*В веденной строке есть ^ и $, и длина строки совпадает с длиной паттерна */
		if (nac == 1 && kon == 1 && (strlen(current_line) - 1) == strlen(pattern)) {   
			flag = 1;
			/*Вставляем в конец массива entries строку number */
		    	strcat(entries, number);
		    	find_entry_number++;
		}
		
		/* Есть символ ^ и начало паттерна совпадает с началом текущей строки */
		else if (nac == 1 && kon == 0 && index == 0) {    
			flag = 1;
		     	strcat(entries, number);
		     	find_entry_number++;
		}
		
		/* Есть символ $ и конец паттерна совпадает с началом текущей строки */
		else if (nac == 0 && kon == 1 && index == (strlen(current_line) - strlen(pattern) - 1)) {    
			flag = 1;
		     	strcat(entries, number);
		     	find_entry_number++;
		}
		
		/*Если ключевых символов нет */
		else if (nac == 0 && kon == 0) {    
			flag = 1;
		   	strcat(entries, number);
		     	find_entry_number++;
		}
		
		else flag = 0;
	    }
	    else flag = 0;
	}
	if (flag) {  
	
	/* Вставляем в конец массива entries текущую строку */
	/* Функция добавляет копию строки entries в конец строки current_line */
	   strcat(entries, current_line);
	}
	
	/*Обнуляем current_line */
	for (unsigned long i = 0; i < strlen(current_line); i++)
	    current_line[i] = 0;
	k++;
    }
    /*Если счетчик не изменился, возвращаем NULL */
    if (find_entry_number == 0) {
	return NULL;
    }
    
    /*Возвращаем массив entries */
    return entries;
}
