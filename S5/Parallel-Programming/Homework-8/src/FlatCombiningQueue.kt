import java.util.concurrent.ThreadLocalRandom
import java.util.concurrent.atomic.AtomicBoolean
import java.util.concurrent.atomic.AtomicReferenceArray

class FlatCombiningQueue<E> : Queue<E> {
    private val queue = ArrayDeque<E>() // sequential queue
    private val combinerLock = AtomicBoolean(false) // unlocked initially
    private val tasksForCombiner = AtomicReferenceArray<Any?>(TASKS_FOR_COMBINER_SIZE)

    override fun enqueue(element: E) {
        if (combinerLock.compareAndSet(false, true)) {
            try {
                performAnnouncedTasks()
                queue.addLast(element)
            } finally {
                combinerLock.set(false)
            }
        } else {
            addTask(element)
        }
    }

    override fun dequeue(): E? {
        if (combinerLock.compareAndSet(false, true)) {
            try {
                performAnnouncedTasks()
                return queue.removeFirstOrNull()
            } finally {
                combinerLock.set(false)
            }
        } else {
            val res = addTask(Dequeue)
            return res.value
        }
    }

    private fun randomCellIndex(): Int =
        ThreadLocalRandom.current().nextInt(tasksForCombiner.length())

    private fun performAnnouncedTasks() {
        for (i in 0 until TASKS_FOR_COMBINER_SIZE) {
            when (val task = tasksForCombiner.get(i)) {
                is Dequeue -> {
                    val res = queue.removeFirstOrNull()
                    tasksForCombiner.set(i, Result(res))
                }

                is Result<*>, null -> continue

                else -> {
                    queue.addLast(task as E)
                    tasksForCombiner.set(i, Result(null))
                }
            }
        }
    }

    private fun addTask(task: Any?): Result<E?> {
        while (true) {
            val index = randomCellIndex()
            if (tasksForCombiner.compareAndSet(index, null, task)) {
                return waitForResult(index)
            }
        }
    }

    private fun waitForResult(i: Int): Result<E?> {
        while (true) {
            val res = tasksForCombiner.get(i)

            if (res is Result<*>) {
                tasksForCombiner.set(i, null)
                return res as Result<E?>
            }

            if (combinerLock.compareAndSet(false, true)) {
                try {
                    performAnnouncedTasks()
                    val lostRes = tasksForCombiner.get(i) as Result<*>
                    tasksForCombiner.set(i, null)
                    return lostRes as Result<E?>
                } finally {
                    combinerLock.set(false)
                }
            }
        }
    }
}

private const val TASKS_FOR_COMBINER_SIZE = 3 // Do not change this constant!

private object Dequeue

private class Result<V>(
    val value: V
)