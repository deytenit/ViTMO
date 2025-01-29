import java.util.concurrent.atomic.*

/**
 * @author Eremin Vladimir
 */
class MSQueue<E> : Queue<E> {
    private val headRef: AtomicReference<Node<E>>
    private val tailRef: AtomicReference<Node<E>>

    init {
        val dummy = Node<E>(null)
        headRef = AtomicReference(dummy)
        tailRef = AtomicReference(dummy)
    }

    override fun enqueue(element: E) {
        val node = Node(element)
        while (true) {
            val tail = tailRef.get()
            if (tail.next.compareAndSet(null, node)) {
                tailRef.compareAndSet(tail, node)
                break
            } else {
                tailRef.compareAndSet(tail, tail.next.get())
            }
        }
    }

    override fun dequeue(): E? {
        while (true) {
            val head = headRef.get()
            val headNext = head.next.get() ?: return null
            if (headRef.compareAndSet(head, headNext)) {
                val result = headNext.element
                headNext.element = null
                return result
            }
        }
    }

    // FOR TEST PURPOSE, DO NOT CHANGE IT.
    override fun validate() {
        check(tailRef.get().next.get() == null) {
            "At the end of the execution, `tail.next` must be `null`"
        }
        check(headRef.get().element == null) {
            "At the end of the execution, the dummy node shouldn't store an element"
        }
    }

    private class Node<E>(
        var element: E?
    ) {
        val next = AtomicReference<Node<E>?>(null)
    }
}
