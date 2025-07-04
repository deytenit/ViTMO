## 6.2 Вычислительная масс-спектрометрия

Задача: для данного пептида сгенерировать его теоретический спектр.
Вход: строка в алфавите аминокислот (Peptide).
Выход: набор целых чисел через пробел – теоретический спектр Peptide.

Примечание 1: здесь и далее мы будем считать, что массы аминокислот целочисленные, а заряд спектра +1. В своей программе вы можете использовать следующие вектора (массивы), хранящие однобуквенные обозначения аминокислот и их целочисленные массы, соответственно:

alphabet = ['A','C','D','E','F','G','H','I','K','L','M','N','P','Q','R','S','T','V','W','Y'] 
masses = [71, 103, 115, 129, 147, 57, 137, 113, 128, 113, 131, 114, 97, 128, 156, 87, 101, 99, 186, 163]

Обратите внимание, что массы 'I' и 'L' (113), а также массы 'K' и 'Q' (128) совпадают.
Примечание 2: не забудьте включить в теоретический спектр массу 0 и массу всего пептида. Кроме того, теоретический спектр не должен содержать масс-дубликатов и должен быть отсортирован по возрастанию массы.

Sample Input:
REDCA

Sample Output:
0 71 156 174 285 289 400 418 503 574

## 6.2 Вычислительная масс-спектрометрия

Задача: по данному идеальному спектру Spectrum найти пептид Peptide, который его мог породить.
Вход: набор целых чисел через пробел – теоретический спектр Peptide.
Выход: строка в алфавите аминокислот (Peptide).

Примечание 1: первая масса в Spectrum всегда 0, последняя масса всегда обозначает массу всего спектра, т.е. всего Peptide.
Примечание 2: если у задачи больше одного решения, то выведите одно любое.

Sample Input:
0 71 156 174 285 289 400 418 503 574

Sample Output:
ACDER

## 6.3 Вычислительная масс-спектрометрия

Задача: в протеоме найти пептид, который наилучшим образом соответствует заданному спектру, т.е. имеет максимальную оценку среди всех (подходящих по массе) пептидов из протеома. 
Вход: спектральный вектор Spectrum' (набор целых чисел через пробел) и строка в алфавите аминокислот Proteome.
Выход: подстрока Proteome с максимальной оценкой относительно Spectrum'.

Примечание 1: на вход подается спектральный вектор Spectrum' = s1 ... sm, который не включает нулевой элемент; при написании программы учитывайте, что s0 = 0. Масса спектра, а также подходящая масса пептида, равны m (число символов во входном спектральном векторе).

Примечание 2: в примере ниже для простоты используется алфавит из двух несуществующих аминокислот: 'X' массы 4 и 'Z' массы 5. В реальных заданиях протеом будет записан в алфавите из 20 стандартных аминокислот и может достигать в длину несколько тысяч символов. К сожалению, из-за особенностей Stepik, если мы показываем какой-то пример как тестовый, то ваша программа должна уметь его решать. Таким образом, вам надо предусмотреть в программе режим работы с алфавитом {'X', 'Z'} , например, как-то так:

def get_AA_letters_masses():
    letters = ['A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'Y']
    masses = [71, 103, 115, 129, 147, 57, 137, 113, 128, 113, 131, 114, 97, 128, 156, 87, 101, 99, 186, 163]
    return letters, masses

def get_XZ_letters_masses():
    letters = ['X', 'Z']
    masses = [4, 5]
    return letters, masses

...

if len(spectral_vector) > 30: # ключевое место для различия между "тестовым примером" и "реальным"
     letters, masses = get_AA_letters_masses()
 else:
     letters, masses = get_XZ_letters_masses()

print(best_protein(proteome, spectral_vector, letters, masses))

Sample Input:
0 0 0 4 -2 -3 -1 -7 6 5 3 2 1 9 3 -8 0 3 1 2 1 8
XZZXZXXXZXZZXZXXZ

Sample Output:
ZXZXX
