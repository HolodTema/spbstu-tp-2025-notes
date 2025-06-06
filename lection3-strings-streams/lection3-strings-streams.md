### Лекция 3. Строки и потоки ввода-вывода
#### строки в стиле С
в С++ строки не являются фундаментальным типом данных. Строки в стиле С - массивы на char*, которые кончаются нуль-терминатором \0
#### модули для строк в стиле С
- cctype - обработка символов
- cstring - обработка строк в стиле C
- cstdlib - стандартная либа С (в том числе для строк)

#### ввод-вывод на консоль в С
- printf(), fprintf(), sprintf()
- scanf(), fscanf(), sscanf()
```
double d = 0.0;

char c[10];

scanf("%1f %s", &d, c);
printf("double: %.2f, char: %s\n", d, c);
```
Здесь в scanf() передают по ссылке, ибо функция будет менять значения переменных.

#### безопасность строк в стиле С
В модуле cstring есть методы с небезопасным дизайном. 

Например VisualStudio запрещает пользоваться такими методами (хотя это можно обойти аналогом @SuppressLint)

#### std::string и его методы 
Строки в стиле с++ - это std::string из модуля string.
- size() - размер строки
- find() - поиск подстроки
- insert()
- erase() - удаление символа
- substr()
- доступ к символу по индексу
- c_str() - конвертирование в char*

#### память для std::string
std::string берет память с запасом, чтобы не выделять ее при возможном увеличении строки. 
Есть методы std::string для ее памяти:
- capacity() - емкость строки
- shrink_to_fit() - уменьшить емкость до размера строки

#### потоки ввода-вывода
istream - это реализация шаблона basic_istream<char>

ostream - это реализация шаблона basic_ostream<char>

std::fstream - поток для файлов

std::stringstream - поток строк

#### глобальные объекты потоков (для вывода в консоль)
- std::cin
- std::cout
- std::cerr
- std::clog

#### флаги потоков
- good() - поток в норме
- bad() - критическая ошибка, поток больше не работает. Завершаем программу
- fail() - ошибка с данными, которую можно сбросить методом clear()
- eof() - конец файла (ctrl+D для Linux)

setstate() - метод позволяет задать состояние потока.
```
std::cin.setstate(std::ios_base::failbit);
```
rdstate() - метод позволяет получить битовую маску из флагов потока.

### конвертация потока в bool
Поток равен true, если он good()

Иначе поток = false

#### чтение std::istream
По умолчанию оператор >> считывает до пробела

std::istream.getline() - считывает строку до ближайшего \n.
Но если до этого использовался >>, то в буфере потока остался символ \n - нужна будет очистка буфера:
```
std::cin.ignore(size, '\n');
//очистка size символов из потока вплоть до \n
```
#### std::getline()
считывает 1 строку до символа \n
```
std::string str;
std::getline(std::cin, str);
```
#### манипуляторы потока
Хранятся в модуле iomanip

- метод ostream.width(n) - следующий вывод минимальной длины n символов
- std::setw(n) - аналог width()
- std::endl - перевод курсора на новую строку и сброс буфера в поток
- std::setprecision - точность чисел
- std::fixed - фиксированный формал числа
- std::scientific - научный формат числа
- std::skipws - пропустить пробелы

#### StreamGuard
работает на RAII
Объект класса StreamGuard при своем создании забирает ссылку на поток и изначальные значения полей потока.

И в своем деструкторе объект StreamGuard возвращает потоку изначальные значения его полей

### sentry
- std::istream::sentry
- std::ostream::sentry
Проверяет состояние потока на ошибки
```
int main() {
    std::istream::sentry s(std::cin);
    if (!sentry) {
        return 1;
    }
    ...
}
```

