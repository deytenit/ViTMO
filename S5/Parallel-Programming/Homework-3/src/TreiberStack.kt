import java.util.concurrent.atomic.AtomicReference

/**
 * @author Eremin Vladimir
 */
class TreiberStack<E> : Stack<E> {
    // Initially, the stack is empty.
    private val topRef = AtomicReference<Node<E>?>(null)

    override fun push(element: E) {
        while (true) {
            val top = topRef.get()
            val newTop = Node(element, top)
            if (topRef.compareAndSet(top, newTop)) {
                break
            }
        }
    }

    override fun pop(): E? {
        while (true) {
            val top = topRef.get() ?: return null
            if (topRef.compareAndSet(top, top.next)) {
                return top.element
            }
        }
    }

    private class Node<E>(
        val element: E,
        val next: Node<E>?
    )
}
