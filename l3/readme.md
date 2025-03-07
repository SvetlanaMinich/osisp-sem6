**To run the programm:**
1. make test



**MAKEFILE DESCRIPTION**
Давайте разберём, как пишется `Makefile` и что означает каждая строка в контексте приведённого примера. `Makefile` — это сценарий для утилиты `make`, которая автоматизирует процесс компиляции, сборки и других задач в проекте. Он состоит из правил, переменных и команд. Я объясню построчно наш `Makefile` из предыдущего ответа и дам общий контекст.

---

### Содержимое `Makefile`
```makefile
CC = gcc
CFLAGS = -Wall -g
TARGET = censor
OBJECTS = main.o censor.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

main.o: main.c censor.h
	$(CC) $(CFLAGS) -c main.c

censor.o: censor.c censor.h
	$(CC) $(CFLAGS) -c censor.c

clean:
	rm -f $(OBJECTS) $(TARGET)

test: $(TARGET)
	@echo "Running test..."
	@echo "This is a bad word and good text" | ./$(TARGET) -d dict.txt > output.txt
	@cat output.txt
	@echo "\nTest completed."

.PHONY: all clean test
```

---

### Построчный разбор

#### 1. Определение переменных
```makefile
CC = gcc
```
- **Что это**: Определяет переменную `CC` (сокращение от "C Compiler"), которая указывает, какой компилятор использовать.
- **Значение**: `gcc` — это GNU C Compiler, стандартный компилятор для C в Unix-подобных системах.
- **Зачем**: Позволяет легко менять компилятор (например, на `clang`), просто изменив эту строку.

```makefile
CFLAGS = -Wall -g
```
- **Что это**: Переменная `CFLAGS` (C Flags) задаёт флаги для компилятора.
- **Значение**:
  - `-Wall`: Включает все основные предупреждения (warnings) о потенциальных проблемах в коде.
  - `-g`: Добавляет отладочную информацию в скомпилированный файл, полезно для работы с отладчиками вроде `gdb`.
- **Зачем**: Упрощает настройку параметров компиляции для всех файлов.

```makefile
TARGET = censor
```
- **Что это**: Переменная `TARGET` задаёт имя конечного исполняемого файла.
- **Значение**: `censor` — имя программы, которая будет создана.
- **Зачем**: Удобно менять имя программы в одном месте, если потребуется.

```makefile
OBJECTS = main.o censor.o
```
- **Что это**: Переменная `OBJECTS` перечисляет объектные файлы (`.o`), которые нужно скомпилировать из исходных файлов.
- **Значение**: `main.o` и `censor.o` — это объектные файлы, соответствующие `main.c` и `censor.c`.
- **Зачем**: Указывает, какие модули участвуют в сборке программы.

---

#### 2. Правило для сборки всей программы
```makefile
all: $(TARGET)
```
- **Что это**: Правило `all` — это цель (target), которая является точкой входа по умолчанию, если просто выполнить `make`.
- **Зависимости**: `$(TARGET)` (т.е. `censor`) — программа должна быть собрана.
- **Команды**: Здесь нет явных команд, но правило зависит от следующего, где сборка описана.
- **Зачем**: Определяет основную цель проекта — собрать исполняемый файл.

```makefile
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)
```
- **Что это**: Правило для создания `censor` (значение `$(TARGET)`).
- **Зависимости**: `$(OBJECTS)` (т.е. `main.o censor.o`) — перед сборкой программы нужно скомпилировать эти объектные файлы.
- **Команда**:
  - `$(CC)`: Вызывает компилятор (`gcc`).
  - `$(OBJECTS)`: Указывает объектные файлы (`main.o censor.o`).
  - `-o $(TARGET)`: Задаёт имя выходного файла (`censor`).
- **Зачем**: Связывает объектные файлы в один исполняемый файл. Это стадия линковки.

---

#### 3. Правила для компиляции объектных файлов
```makefile
main.o: main.c censor.h
	$(CC) $(CFLAGS) -c main.c
```
- **Что это**: Правило для создания `main.o`.
- **Зависимости**: `main.c` и `censor.h` — если эти файлы изменятся, `main.o` будет перекомпилирован.
- **Команда**:
  - `$(CC)`: Компилятор (`gcc`).
  - `$(CFLAGS)`: Флаги компиляции (`-Wall -g`).
  - `-c`: Указывает компилятору создать объектный файл, а не связывать его сразу в программу.
  - `main.c`: Исходный файл для компиляции.
- **Зачем**: Компилирует `main.c` в `main.o`, учитывая зависимости от заголовочного файла.

```makefile
censor.o: censor.c censor.h
	$(CC) $(CFLAGS) -c censor.c
```
- **Что это**: Правило для создания `censor.o`.
- **Зависимости**: `censor.c` и `censor.h`.
- **Команда**: Аналогична предыдущей, но компилирует `censor.c`.
- **Зачем**: Создаёт объектный файл для модуля `censor.c`.

---

#### 4. Правило очистки
```makefile
clean:
	rm -f $(OBJECTS) $(TARGET)
```
- **Что это**: Правило `clean` для удаления сгенерированных файлов.
- **Зависимости**: Нет (пусто после двоеточия).
- **Команда**:
  - `rm`: Утилита для удаления файлов.
  - `-f`: Флаг "force", чтобы не запрашивать подтверждение и игнорировать ошибки, если файлов нет.
  - `$(OBJECTS) $(TARGET)`: Удаляет `main.o`, `censor.o` и `censor`.
- **Зачем**: Выполняет "очистку" проекта, как требует задание.

---

#### 5. Правило тестирования
```makefile
test: $(TARGET)
	@echo "Running test..."
	@echo "This is a bad word and good text" | ./$(TARGET) -d dict.txt > output.txt
	@cat output.txt
	@echo "\nTest completed."
```
- **Что это**: Правило `test` для проверки работы программы.
- **Зависимости**: `$(TARGET)` (т.е. `censor`) — тест запускается только после сборки программы.
- **Команды**:
  1. `@echo "Running test..."`:
     - `echo`: Выводит текст в консоль.
     - `@`: Подавляет вывод самой команды в терминале (виден только результат).
  2. `@echo "This is a bad word and good text" | ./$(TARGET) -d dict.txt > output.txt`:
     - `echo "..."`: Генерирует тестовый ввод.
     - `|`: Перенаправляет вывод `echo` в stdin программы.
     - `./$(TARGET)`: Запускает программу `censor`.
     - `-d dict.txt`: Указывает словарь.
     - `> output.txt`: Перенаправляет вывод программы в файл `output.txt`.
  3. `@cat output.txt`:
     - `cat`: Выводит содержимое `output.txt` в консоль.
  4. `@echo "\nTest completed."`:
     - Выводит завершающее сообщение с новой строкой (`\n`).
- **Зачем**: Выполняет тестирование с заранее заготовленными данными, как требует задание.

---

#### 6. Указание фиктивных целей
```makefile
.PHONY: all clean test
```
- **Что это**: Директива `.PHONY` указывает, что `all`, `clean` и `test` — это не имена файлов, а абстрактные цели.
- **Зачем**: Если в директории случайно появятся файлы с именами `all`, `clean` или `test`, `make` не будет думать, что они уже "созданы", и выполнит команды.

---

### Как это работает вместе
1. **`make` или `make all`**:
   - Проверяет зависимости `all` → `censor` → `main.o` и `censor.o`.
   - Компилирует `main.c` в `main.o`, если нужно.
   - Компилирует `censor.c` в `censor.o`, если нужно.
   - Линкует `main.o` и `censor.o` в `censor`.

2. **`make clean`**:
   - Удаляет все сгенерированные файлы.

3. **`make test`**:
   - Убеждается, что `censor` собран, затем запускает тест.

---

### Общие принципы `Makefile`
- **Правила**: `цель: зависимости` + команды (с отступом табуляцией).
- **Переменные**: `имя = значение`, используются как `$(имя)`.
- **Автоматизация**: `make` проверяет временные метки файлов и пересобирает только изменённое.
- **Флаги**: Например, `-c` для компиляции, `-o` для указания имени вывода.

Если что-то осталось непонятным или нужно пример посложнее, дайте знать!