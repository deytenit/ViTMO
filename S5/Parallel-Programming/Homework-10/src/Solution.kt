import java.util.concurrent.atomic.*

/**
 * @author Eremin Vladimir
 */
class Solution(private val env: Environment) : Lock<Solution.Node> {
    private val tail = AtomicReference<Node?>(null)

    override fun lock(): Node {
        val my = Node() // сделали узел
        my.locked.value = true

        val prev = tail.getAndSet(my)
        if (prev != null) {
            prev.next.value = my

            while (my.locked.value) {
                env.park()
            }
        }

        return my // вернули узел
    }

    override fun unlock(node: Node) {
        val next = node.next.value
        if (next == null) {
            if (tail.compareAndSet(node, null)) {
                return
            }

            while (node.next.value == null) {
                continue
            }
        }

        next?.let {
            it.locked.value = false
            env.unpark(it.thread)
        }
    }

    class Node {
        val thread: Thread = Thread.currentThread() // запоминаем поток, которые создал узел
        val locked = AtomicReference(true) // поддерживаем состояние блокировки
        val next = AtomicReference<Node?>(null) // ссылаемся на следующий узел
    }
}