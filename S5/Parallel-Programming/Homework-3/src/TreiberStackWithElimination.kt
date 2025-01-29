import java.util.concurrent.ThreadLocalRandom
import java.util.concurrent.atomic.AtomicReferenceArray

/**
 * @author Eremin Vladimir
 */
open class TreiberStackWithElimination<E> : Stack<E> {
    private val stack = TreiberStack<E>()

    private val eliminationArray = AtomicReferenceArray<Any?>(ELIMINATION_ARRAY_SIZE)

    override fun push(element: E) {
        if (tryPushElimination(element)) {
            return
        }
        stack.push(element)
    }

    protected open fun tryPushElimination(element: E): Boolean {
        val index = randomCellIndex()
        for (i in 0 until ELIMINATION_WAIT_CYCLES) {
            if (eliminationArray.compareAndSet(index, CELL_STATE_EMPTY, element)) {
                while (true) {
                    val cell = eliminationArray.get(index)
                    if (cell === CELL_STATE_RETRIEVED) {
                        if (eliminationArray.compareAndSet(index, CELL_STATE_RETRIEVED, CELL_STATE_EMPTY)) {
                            return true
                        }
                    } else if (eliminationArray.compareAndSet(index, cell, CELL_STATE_EMPTY)) {
                        return false
                    }
                }
            }
        }
        return false
    }

    override fun pop(): E? = tryPopElimination() ?: stack.pop()

    private fun tryPopElimination(): E? {
        val index = randomCellIndex()
        for (i in 0 until ELIMINATION_WAIT_CYCLES) {
            val value = eliminationArray[index]
            if (value !== CELL_STATE_EMPTY && value !== CELL_STATE_RETRIEVED) {
                if (eliminationArray.compareAndSet(index, value, CELL_STATE_RETRIEVED)) {
                    return value as E
                }
            }
        }
        return null
    }

    private fun randomCellIndex(): Int =
        ThreadLocalRandom.current().nextInt(eliminationArray.length())

    companion object {
        private const val ELIMINATION_ARRAY_SIZE = 2 // Do not change!
        private const val ELIMINATION_WAIT_CYCLES = 1 // Do not change!

        // Initially, all cells are in EMPTY state.
        private val CELL_STATE_EMPTY = null

        // `tryPopElimination()` moves the cell state
        // to `RETRIEVED` if the cell contains element.
        private val CELL_STATE_RETRIEVED = Any()
    }
}
