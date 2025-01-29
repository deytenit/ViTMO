import java.util.concurrent.atomic.*

/**
 * @author Eremin Vladimir
 */
class FAABasedQueue<E> : Queue<E> {
    private val enqIdx = AtomicLong(0)
    private val deqIdx = AtomicLong(0)
    private val segmentsQueue = SegmentQueue()

    override fun enqueue(element: E) {
        while (true) {
            val tail = segmentsQueue.tail.get()

            val index = enqIdx.getAndIncrement()

            val segment = segmentsQueue.find(tail, index / SEGMENT_SIZE)
            segmentsQueue.moveTailTo(segment)

            if (segment.cells.compareAndSet((index % SEGMENT_SIZE).toInt(), null, element)) {
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

            val head = segmentsQueue.head.get()

            val index = deqIdx.getAndIncrement()

            val segment = segmentsQueue.find(head, index / SEGMENT_SIZE)
            segmentsQueue.moveHeadTo(segment)

            if (segment.cells.compareAndSet((index % SEGMENT_SIZE).toInt(), null, POISONED)) {
                continue
            }

            val value = segment.cells.get((index % SEGMENT_SIZE).toInt()) as E
            if (segment.cells.compareAndSet((index % SEGMENT_SIZE).toInt(), value, null)) {
                return value
            }
        }
    }

    private class SegmentQueue {
        class Segment(val id: Long) {
            val next = AtomicReference<Segment?>(null)
            val cells = AtomicReferenceArray<Any?>(SEGMENT_SIZE)
        }

        val head: AtomicReference<Segment>
        val tail: AtomicReference<Segment>

        init {
            val dummy = Segment(0)
            head = AtomicReference(dummy)
            tail = AtomicReference(dummy)
        }

        fun find(from: Segment, id: Long): Segment {
            var current = from
            while (current.id != id) {
                val nextRef = current.next
                if (nextRef.get() == null) {
                    nextRef.compareAndSet(null, Segment(current.id + 1))
                }
                current = nextRef.get()!!
            }

            return current
        }

        fun moveTailTo(s: Segment) {
            val tailV = tail.get()
            if (s.id > tailV.id) {
                tail.compareAndSet(tailV, s)
            }
        }

        fun moveHeadTo(s: Segment) {
            val headV = head.get()
            if (s.id > headV.id) {
                head.compareAndSet(headV, s)
            }
        }
    }
}

// DO NOT CHANGE THIS CONSTANT
private const val SEGMENT_SIZE = 2

private val POISONED = Any()