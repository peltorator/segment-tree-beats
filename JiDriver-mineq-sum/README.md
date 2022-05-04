# Задача Ji Driver Segment Tree с запросами запросы `min=` и `sum`

## Решения:

- `main-ji-driver-segment-tree.cpp` — основное чистое решение, реализующее в точности идеи из статьи

- `ji-driver-alternative-implementation.cpp` — то же самое решение, но иначе реализованное

- `ji-driver-segment-tree-tagbreakcondition-combined.cpp` — решение, аналогичное основному, однако _tagCondition_ и _breakCondition_ скомбинированы в одно условие

- `brute-force.cpp` — "тупое" решение за`O(nq)` (получает TL для генераторов `gen-random.cpp` и `gen-smart-test.cpp`)

- `ji-driver-segment-tree-int.cpp` — почти правильное решение, использующее 32-битный тип целых чисел вместо 64-битного (получает WA для генераторов `gen-random.cpp` и `gen-smart-test.cpp`)

- `ji-driver-segment-tree-with-wrong-tag-condition.cpp` — решение, в котором используется неправильный _tagCondition_: мы проверяем, что все числа на отрезке равны (получает TL для генератора `gen-smart-test.cpp`)

- `ji-driver-segment-tree-without-break-condition.cpp` — решение, в котором пропущено _breakCondition_ (получает WA для генераторов `gen-random.cpp` и `gen-smart-test.cpp`)

- `ji-driver-segment-tree-without-tag-condition.cpp` — решение, в котором пропущено _tagCondition_ (получает TL для генератора `gen-smart-test.cpp`)


## Генераторы тестов:

- `gen-random.cpp` — генерирует случайные тесты при заданных параметрах (или ограничениях на параметры) `n` и `q`

- `gen-smart-test.cpp` — генерирует "умные" тесты при заданных параметрах (или ограничениях на параметры) `n` и `q`, которые воспроизводят ситуацию, которая является контртестом для неправильных _tagCondition_

## Скрипт тестов:

- `test-script.txt` — скрипт для генерации тестов

