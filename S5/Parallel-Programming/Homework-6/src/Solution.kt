/**
 * В теле класса решения разрешено использовать только переменные делегированные в класс RegularInt.
 * Нельзя volatile, нельзя другие типы, нельзя блокировки, нельзя лазить в глобальные переменные.
 *
 * @author Eremin Vladimir
 */
class Solution : MonotonicClock {
    private var c1 by RegularInt(0)
    private var c2 by RegularInt(0)
    private var c3 by RegularInt(0)

    private var r1 by RegularInt(0)
    private var r2 by RegularInt(0)

    override fun write(time: Time) {
        // write left-to-right
        c1 = time.d1
        c2 = time.d2
        c3 = time.d3

        // write right-to-left
        r2 = time.d2
        r1 = time.d1
    }

    override fun read(): Time {
        // read left-to-right
        val r1V = r1
        val r2V = r2

        // read right-to-left
        val c3V = c3
        val c2V = c2
        val c1V = c1

        return Time(
            c1V,
            if (c1V == r1V) c2V else 0,
            if (c1V == r1V && c2V == r2V) c3V else 0
        )
    }
}