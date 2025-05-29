### Лекция 6-2. Алгоритмы STL
Алгоритмы stl
- находятся в модуле algorithm
- шаблонные функции
- работают с итераторами
### std::for_each
Производит действия над каждым элементом контейнера.

Добавлять или удалять элементы не рекомендуется, ибо это может привести к инвалидации итераторов.
```c++
std::vector<int> vec{1, 2, 3, 4, 5};

std::for_each(v.begin(), v.end(),
    [](int& e) { e *= 2; }
);

std::for_each(v.begin(), v.end(), 
    [](int& e) { std::cout << e << " "; } 
);

//2 4 6 8 10
```
std::for_each может вернуть копию использованного функтора. Например, в копии функтора может храниться результат всех преобразований
```c++
struct Sum {
    int sum = 0;

    void operator()(int n) {
        sum += n;
    }
};

std::vector<int> vec{1, 2, 3, 4};

Sum s = std::for_each(vec.begin(), vec.end(), Sum());

std::cout << s.sum << '\n'; //10
```
### std::accumulate
Выполняет свертку элементов контейнера. По умолчанию - сложение элементов. 
```c++
std::list<int> l{ 1, 2, 3, 4, 5 };
int sum = std::accumulate(l.cbegin(), l.cend(), 0); // 15

int prod = std::accumulate(l.cbegin(), l.cend(), 1, // 120

std::multiplies<int>());
```
### std::find, std::find_if, std::find_if_not
Возвращают либо итератор на найденный элемент, либо итератор на конец диапазона (end()).

Вариации std::find_if и std::find_if_not принимают лямбду - условие поиска.
```c++
std::vector<int> v{ 1, 2, 3, 4, 5 };

auto i = std::find(v.cbegin(), v.cend(), 3);
std::cout << (i != v.cend() ? "yes\n" : "no\n"); // yes
```

Пример с find_if:
```c++
std::vector<Person> persons;
//...
auto i = std::find_if(persons.cbegin(), persons.cend(),
[](const Person &p){ return p.lastName == "Ivanov"; });
```

### std::copy
Копирует данные из одного диапазона в другой. Но нужно заранее позаботиться, что принимающий диапазон имеет достаточно места для копирования.

Например, можно вывести на экран значения при помощи std::copy
```c++
std::vector<int> v{ 1, 2, 3, 4, 5 };

std::copy(v.cbegin(), v.cend(), std::ostream_iterator<int>(std::cout, " "));
```

### std::transform
Преобразует каждый элемент контейнера.

3-й-параметр - адрес, куда вставится результат.

Например, преобразование строки в верхний регистр:
```c++
std::string s = "hello";

std::transform(s.cbegin(), s.cend(), s.begin(), std::toupper);
```

### std::sort 
Сортирует контейнер. По умолчанию в порядке возрастания при помощи оператора <.

Но можно задать свой вариант сортировки функтором.

Сортировка неустойчивая, для устойчивой есть std::stable_sort()
```c++
std::vector<Person> persons;
//...
std::sort(persons.begin(), persons.end(),
    [](const Person &p1, const Person &p2) {
        return p1.lastName < p2.lastName;
    }
);
```

