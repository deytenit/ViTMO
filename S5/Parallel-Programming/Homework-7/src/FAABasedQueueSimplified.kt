import java.util.concurrent.atomic.*
import kotlin.math.*

/**
 * @author Eremin Vladimir
 */
class FAABasedQueueSimplified<E> : Queue<E> {
    private val infiniteArray = AtomicReferenceArray<Any?>(64) // conceptually infinite array
    private val enqIdx = AtomicLong(0)
    private val deqIdx = AtomicLong(0)

    override fun enqueue(element: E) {
        while (true) {
            val index = enqIdx.getAndIncrement()
            if (infiniteArray.compareAndSet((index % infiniteArray.length()).toInt(), null, element)) {
                return
            }
        }
    }

    @Suppress("UNCHECKED_CAST")
    override fun dequeue(): E? {
        while (true) {
            if (deqIdx.get() >= enqIdx.get()) {
                return null
            }

            val index = deqIdx.getAndIncrement()
            if (infiniteArray.compareAndSet((index % infiniteArray.length()).toInt(), null, POISONED)) {
                continue
            }

            val result = infiniteArray.get((index % infiniteArray.length()).toInt()) as E
            if (infiniteArray.compareAndSet((index % infiniteArray.length()).toInt(), result, null)) {
                return result
            }
        }
    }

    override fun validate() {
        for (i in 0 until min(deqIdx.get().toInt(), enqIdx.get().toInt())) {
            check(infiniteArray[i] == null || infiniteArray[i] == POISONED) {
                "`infiniteArray[$i]` must be `null` or `POISONED` with `deqIdx = ${deqIdx.get()}` at the end of the execution"
            }
        }
        for (i in max(deqIdx.get().toInt(), enqIdx.get().toInt()) until infiniteArray.length()) {
            check(infiniteArray[i] == null || infiniteArray[i] == POISONED) {
                "`infiniteArray[$i]` must be `null` or `POISONED` with `enqIdx = ${enqIdx.get()}` at the end of the execution"
            }
        }
    }
}

private val POISONED = Any()
