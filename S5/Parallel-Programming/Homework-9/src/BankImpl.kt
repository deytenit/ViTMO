import java.util.concurrent.locks.ReentrantReadWriteLock
import kotlin.concurrent.read
import kotlin.concurrent.write
import kotlin.math.max
import kotlin.math.min

/**
 * Bank implementation.
 *
 * @author Eremin Vladimir
 */
class BankImpl(n: Int) : Bank {
    private val accounts: Array<Account> = Array(n) { Account() }

    override val numberOfAccounts: Int
        get() = accounts.size

    override fun getAmount(index: Int): Long {
        val account = accounts[index]
        account.lock.read {
            return account.amount
        }
    }

    override val totalAmount: Long
        get() {
            accounts.forEach { it.lock.readLock().lock() }
            return try {
                accounts.sumOf { it.amount }
            } finally {
                accounts.forEach { it.lock.readLock().unlock() }
            }
        }

    override fun deposit(index: Int, amount: Long): Long {
        require(amount > 0) { "Invalid amount: $amount" }

        val account = accounts[index]
        account.lock.write {
            check(amount <= Bank.MAX_AMOUNT && account.amount + amount <= Bank.MAX_AMOUNT) { "Overflow" }
            account.amount += amount
            return account.amount
        }
    }

    override fun withdraw(index: Int, amount: Long): Long {
        require(amount > 0) { "Invalid amount: $amount" }

        val account = accounts[index]
        account.lock.write {
            check(account.amount - amount >= 0) { "Underflow" }
            account.amount -= amount
            return account.amount
        }
    }

    override fun transfer(fromIndex: Int, toIndex: Int, amount: Long) {
        require(amount > 0) { "Invalid amount: $amount" }
        require(fromIndex != toIndex) { "fromIndex == toIndex" }

        val firstLock = accounts[min(fromIndex, toIndex)].lock
        val secondLock = accounts[max(fromIndex, toIndex)].lock

        firstLock.write { secondLock.write {
            val from = accounts[fromIndex]
            val to = accounts[toIndex]
            check(amount <= accounts[fromIndex].amount) { "Underflow" }
            check(amount <= Bank.MAX_AMOUNT && to.amount + amount <= Bank.MAX_AMOUNT) { "Overflow" }

            from.amount -= amount
            to.amount += amount
        }}
    }

    override fun consolidate(fromIndices: List<Int>, toIndex: Int) {
        require(fromIndices.isNotEmpty()) { "empty fromIndices" }
        require(fromIndices.distinct() == fromIndices) { "duplicates in fromIndices" }
        require(toIndex !in fromIndices) { "toIndex in fromIndices" }

        val indices = (fromIndices + toIndex).sorted()
        indices.forEach { accounts[it].lock.writeLock().lock() }
        try {
            val amount = fromIndices.sumOf { accounts[it].amount }
            val to = accounts[toIndex]
            check(to.amount + amount <= Bank.MAX_AMOUNT) { "Overflow" }

            fromIndices.forEach { accounts[it].amount = 0 }
            to.amount += amount
        } finally {
            indices.forEach { accounts[it].lock.writeLock().unlock() }
        }

    }

    /**
     * Private account data structure.
     */
    class Account {
        /**
         * Amount of funds in this account.
         */
        var amount: Long = 0

        /**
         * Lock for synchronized access to this specific account.
         */
        val lock = ReentrantReadWriteLock()
    }
}