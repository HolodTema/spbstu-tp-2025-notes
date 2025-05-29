### Лекция 6. Функторы и алгоритмы
Напишем пузярьковую сортировку:
```c++
template<class T>
void bubbleSort(T* array, int len) {
    for (int i = 0; i < len-1; ++i) {
        for (int j = 0; j < len-1-i; ++j) {
            if (array[j] > array[j+1]) {
                std::swap(array[j], array[j+1]);
            }
        }
    }
}

int a[] {5, 2, 3, 1, 4};
bubbleSort(a, std::size(a));
```
P.S. здесь мы применяем std::size для нахождения длины массива - удобно.

Возможные проблемы:
- что, если у T нету оператора >
- что, если нужно поменять порядок сортировки (например, с возрастания на убывание)
- что если нужно сортировать объекты по их разным полям (людей можно сортировать по ФИО и по дате рождения)

### Функция высшего порядка
Функция высшего порядка - принимает как параметр или возвращает другую функцию.

В этом примере функция передается в функцию как параметр через шаблоны.
```c++
template <class T, class Comp>
void bubbleSort(T* array, int len, const Comp& isLess) {
    for (int i = 0; i < len-1; ++i) {
        for (int j = 0; j < len-1-i; ++j) {
            if (isLess(array[j+1], array[j])) {
                std::swap(array[j], array[j+1]);
            }
        }
    }
}
```
### Функция обратного вызова (callback)
callback - это функция, передаваемая в другую функцию как параметр (странное определение).

Напишем функцию-компаратор для строк и передадим в функцию из предыдущего примера:
```c++
bool compareStringLen(const std::string& s1, const std::string& s2) {
    return s1.length() < s2.length();
}

std::string array[] {"qwe", "rt", "y"};
bubbleSort(array, std::size(array), compareStringLen);
```

### Функтор 
Функтор - функциональный объект. Это любой объект, который реализует операцию вызова с круглыми скобками.

Есть 2 типа функторов:
- указатель на функцию
- объект класса с перегруженным оператором ()

```c++
class StringComparator {
public:
    bool operator()(const std::string& s1, const std::string& s2) const {
        return s1.length() < s2.length();
    }
};

std::string array[] {"qwe", "rt", "y"};
StringComparator comparator;
bubbleSort(array, std::size(array), comparator);
```

### Замыкание (closure)
Замыкание - функция, которая содержит ссылки на контекст, в котором она создана.

Пример из js:
```js
function makeCounter() {
    var count = 0;
    return function() {
        return count++;
    }
}

var counter1 = makeCounter();
alert(counter1()); // 0
alert(counter1()); // 1

var counter2 = makeCounter();
alert(counter2()); // 0
alert(counter1()); // 2
```
В с++ можно сделать замыкание через поля объекта, который используется как функтор.

Превратим функтор StringComparator из предыдущего примера в замыкание:
```C++
class StringComparator {
public:

    StringComparator(bool desc = false): desc_(desc)
        { }

    bool operator()(const std::string& s1, const std::string& s2) {
        if (desc_) {
            return s1.length() > s2.length();
        }
        else {
            return s1.length() < s2.length();
        }
    }
private:
    bool desc_;

};

std::string array[] {"qwe", "rt", "y"};
StringComparator comparator(true);

bubbleSort(array, std::size(array), comparator);
// qwe rt y
```

### Лямбда-выражения 
Анонимные функции, которые компилятор переделывает в локальные функторы.

Синтаксис:
- [] - список захвата. Туда указываются переменные из внешней области видимости, чтобы превратить лямбду в замыкание.
- () - список параметров
- {} - тело функции
```c++
std::string e[]{ "qwe", "y", "rt" };

bubbleSort2(e, std::size(e), [](const std::string &s1, const std::string &s2) {
    return s1.length() < s2.length();
});

// "y", "rt", "qwe"
```

### Подробнее о списке захвата
- [=] - все переменные захвачены по значению
- [&] - все переменные захвачены по ссылке
- [&name] - переменная name по ссылке
- [name] - переменная name по значению
- [=, &name] - переменная name по ссылке, а все остальные по значению.

```c++
int swapCount = 0;

std::string array[]{ "qwe", "rt", "y" };

bubbleSort(array, std::size(array),
    [&swapCount](const std::string& s1, const std::string& s2) {
        if (s1.length() < s2.length()) {
            ++swapCount;
            return true;
        }
        return false;
    }
);

std::cout << swapCount << '\n';
```

При этом захват по ссылке в лямбде не продлевает время жизни захваченного объекта. 

Поэтому возникают dangling references (висячие ссылки). 
Если вызвать лямбду, которая захватила висячую ссылку, то будет UB.

В этом примере мы захватываем ссылку на переменную, которая будет уничтожена при выходе из области видимости makeCounter(). И в итоге лямбда захватит висячую ссылку.
```C++
auto makeCounter() {
    int count = 0;
    return [&count]() { return count++; };
}

auto counter = makeCounter();
std::cout << counter() << '\n'; //UB
```
### Решение = mutable + захват по значению
Чтобы избежать dangling references в лямбдах, можно сделать захват по значению с ключевым словом mutable.

Без ключевого слова mutable параметры захвата по значению - константы.
```c++
auto makeCounter() {
    int count = 0;
    return [count]() mutable { return count++; };
}
```

### Частичное применение функций 
Частичное применение функций - создание новой функции путем фиксирования части аргументов исходной.

Пример из JavaScript:
```js
function bindFirst(func, firstArg) {
    return (a) => func(firstArg, a);
}

var add = (a, b) => a + b;

var add100 = bindFirst(add, 100);

alert(add100(50)); 
```

Раньше в с++03 были шаблоны для частичного применения функций. Но теперь они deprecated.

В с++11 появился std::bind.

std::bind позволяет получить функтор из:
- указателя на метод класса, зафиксировав объект, от которого вызывается метод.
- указателя на функцию, зафиксировав часть ее параметров.

### std::bind для фиксации части параметров функции
```c++
int add(int a, int b) {
    return a + b;
}

int main() {
    auto functorAdd = std::bind(add, 10, 20);
    std::cout << functorAdd() << '\n'; //30
    return 0;
}
```

### placeholders для std::bind
Если в std::bind мы хотим связать не все аргументы, а лишь часть, то для несвязанных надо дать плейсхолдеры из  std::placeholders
```c++
int add(int a, int b) {
    return a + b;
}

int main() {
    auto add1 = bind(add, 1, std::placeholders::_1);

    cout << add1(42) << '\n'; //43
}