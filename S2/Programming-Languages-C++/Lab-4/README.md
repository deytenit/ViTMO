## Least frequent recently used

Этот алгоритм применяется в областях, где требуется сбалансировать сохранение в кеше как часто используемых, так и недавно использованных элементов. Существуют разные вариации, здесь предложена одна из простых.

Кеш поделён на две области: привилегированная и непривилегированная. Привилегированная моделируется, как LRU список, а непривилегированная - как очередь FIFO.

Поиск производится сначала в привилегированной очереди (если элемент найден - он перемещается в её начало), затем - в непривилегированной (если элемент найден - он перемещается в привилегированную очередь).
Если элемент не найден, он добавляется в непривилегированную очередь.

Вытеснение из привилегированной очереди происходит так:
* удаляется самый редко используемый (последний) элемент из очереди
* он помещается в начало непривилегированной очереди (возможно, вызывая вытеснение там)

Для непривилегированной очереди новые элементы добавляются в начало, вытесненные удаляются с конца.

### Допущение в реализации
В реализации кеша допустимо предполагать, что все хранимые там объекты имеют в иерархии наследования предка, задаваемого шаблонным параметром KeyProvider,
который, в свою очередь, имеет оператор равенства с ключом (задаваемым шаблонным параметром Key).

## Модификация pool аллокатора, реализующая алгоритм "двойников"

Требуется расширить реализацию pool аллокатора возможностью размещать объекты произвольных размеров.
В качестве модели реализации возьмём известную схему "двойников" ([buddy](https://en.wikipedia.org/wiki/Buddy_memory_allocation)), детально описанную в книге
Дональда Кнута "Искусство программирования, т.1".

При создании аллокатора задаётся два параметра:
* минимальная степень двойки `N`
* максимальная степень двойки `M`

Максимальная степень двойки определяет размер пула - `2 ^ M`. Минимальная степень двойки определяет размер минимально выделяемого блока - `2 ^ N`.

К расходу памяти предъявляются следующие требования:
* пусть пул полностью заполнен и в нём находится `K = 2 ^ (M - N)`
* тогда максимальный расход памяти на пул должен составлять `S <= 2 ^ M + K * 7 * sizeof(void *)`

Изначально пул состоит из одного блока максимального размера (`2 ^ M`), который считается свободным.

При запросе на выделение памяти происходит следующее:
* определяется минимальная степень двойки `K` такая, что запрошенный размер `X <= 2 ^ K`
* в пуле ищется свободный блок соответствующего размера (`2 ^ K`)
* если такой блок найден, он помечается, как занятый, и возвращается в качестве выделенной памяти
* иначе ищется наименьший свободный блок большего размера
* если такого блока не найдено, то бросается исключение `std::bad_alloc`
* иначе найденный блок делится пополам, порождая два новых свободных блока вместо себя, до тех пор, пока не будет получена пара блоков нужного размера (`2 ^ K`)
* из пары новых блоков выбирается первый - как на очередной итерации рекурсивного деления блоков, так и в конце, когда достигнута нужная гранулярность
* полученный блок нужного размера помечается, как занятый, и возвращается в качестве выделенной памяти

При запросе на освобождение памяти происходит следующее:
* освобождённый блок помечается, как свободный
* если его блок-"двойник" тоже свободен, происходит слияние блоков в блок следующего размера и операция повторяется

Таким образом, все блоки, кроме блока максимального размера, существуют в парах и как только оба блока пары оказываются свободными, происходит слияние блоков, что
позволяет бороться с фрагментацией пула. При этом процедура поиска свободного блока тоже достаточно проста и её можно реализовать достаточно эффективно.

Недостатком такой модели является неполное использование выделенной памяти в случаях, когда размеры размещаемых объектов не равны степеням двойки, а также когда
размеры размещаемых объектов меньше размера минимального блока.

Подумайте, как можно оптимизировать служебные расходы памяти для такого пула.