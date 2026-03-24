#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
protected:
    class node {
    public:
        T *data;
        node *prev;
        node *next;

        node() : data(nullptr), prev(nullptr), next(nullptr) {}

        node(const T &value, node *p = nullptr, node *n = nullptr)
            : prev(p), next(n) {
            data = new T(value);
        }

        ~node() {
            if (data != nullptr) {
                delete data;
            }
        }
    };

protected:
    node *head;
    node *tail;
    size_t len;

    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        cur->prev = pos->prev;
        cur->next = pos;
        pos->prev->next = cur;
        pos->prev = cur;
        len++;
        return cur;
    }

    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node *erase(node *pos) {
        node *ret = pos->next;
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        len--;
        return ret;
    }

public:
    class const_iterator;
    class iterator {
    private:
        list *container;
        node *ptr;

        iterator(list *c, node *p) : container(c), ptr(p) {}

    public:
        iterator() : container(nullptr), ptr(nullptr) {}

        /**
         * iter++
         */
        iterator operator++(int) {
            if (ptr == container->tail) {
                throw invalid_iterator();
            }
            iterator temp = *this;
            ptr = ptr->next;
            return temp;
        }

        /**
         * ++iter
         */
        iterator & operator++() {
            if (ptr == container->tail) {
                throw invalid_iterator();
            }
            ptr = ptr->next;
            return *this;
        }

        /**
         * iter--
         */
        iterator operator--(int) {
            if (ptr == container->head->next) {
                throw invalid_iterator();
            }
            iterator temp = *this;
            ptr = ptr->prev;
            return temp;
        }

        /**
         * --iter
         */
        iterator & operator--() {
            if (ptr == container->head->next) {
                throw invalid_iterator();
            }
            ptr = ptr->prev;
            return *this;
        }

        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (ptr == container->tail || ptr == container->head) {
                throw invalid_iterator();
            }
            return *(ptr->data);
        }

        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (ptr == container->tail || ptr == container->head) {
                throw invalid_iterator();
            }
            return ptr->data;
        }

        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        friend class list;
        friend class const_iterator;
    };

    /**
     * TODO
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
    private:
        const list *container;
        node *ptr;

        const_iterator(const list *c, node *p) : container(c), ptr(p) {}

    public:
        const_iterator() : container(nullptr), ptr(nullptr) {}

        const_iterator(const iterator &other) : container(other.container), ptr(other.ptr) {}

        /**
         * iter++
         */
        const_iterator operator++(int) {
            if (ptr == container->tail) {
                throw invalid_iterator();
            }
            const_iterator temp = *this;
            ptr = ptr->next;
            return temp;
        }

        /**
         * ++iter
         */
        const_iterator & operator++() {
            if (ptr == container->tail) {
                throw invalid_iterator();
            }
            ptr = ptr->next;
            return *this;
        }

        /**
         * iter--
         */
        const_iterator operator--(int) {
            if (ptr == container->head->next) {
                throw invalid_iterator();
            }
            const_iterator temp = *this;
            ptr = ptr->prev;
            return temp;
        }

        /**
         * --iter
         */
        const_iterator & operator--() {
            if (ptr == container->head->next) {
                throw invalid_iterator();
            }
            ptr = ptr->prev;
            return *this;
        }

        /**
         * *it
         */
        const T & operator *() const {
            if (ptr == container->tail || ptr == container->head) {
                throw invalid_iterator();
            }
            return *(ptr->data);
        }

        /**
         * it->field
         */
        const T * operator ->() const {
            if (ptr == container->tail || ptr == container->head) {
                throw invalid_iterator();
            }
            return ptr->data;
        }

        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        friend class list;
        friend class iterator;
    };

    /**
     * TODO Constructs
     * Atleast two: default constructor, copy constructor
     */
    list() {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
        len = 0;
    }

    list(const list &other) {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
        len = 0;

        for (node *p = other.head->next; p != other.tail; p = p->next) {
            push_back(*(p->data));
        }
    }

    /**
     * TODO Destructor
     */
    virtual ~list() {
        clear();
        delete head;
        delete tail;
    }

    /**
     * TODO Assignment operator
     */
    list &operator=(const list &other) {
        if (this == &other) {
            return *this;
        }

        clear();

        for (node *p = other.head->next; p != other.tail; p = p->next) {
            push_back(*(p->data));
        }

        return *this;
    }

    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if (empty()) {
            throw container_is_empty();
        }
        return *(head->next->data);
    }

    const T & back() const {
        if (empty()) {
            throw container_is_empty();
        }
        return *(tail->prev->data);
    }

    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(this, head->next);
    }

    const_iterator cbegin() const {
        return const_iterator(this, head->next);
    }

    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(this, tail);
    }

    const_iterator cend() const {
        return const_iterator(this, tail);
    }

    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const {
        return len == 0;
    }

    /**
     * returns the number of elements
     */
    virtual size_t size() const {
        return len;
    }

    /**
     * clears the contents
     */
    virtual void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        if (pos.container != this || pos.ptr == nullptr) {
            throw invalid_iterator();
        }

        node *newNode = new node(value);
        node *inserted = insert(pos.ptr, newNode);
        return iterator(this, inserted);
    }

    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
    virtual iterator erase(iterator pos) {
        if (pos.container != this || pos.ptr == nullptr) {
            throw invalid_iterator();
        }
        if (pos.ptr == head || pos.ptr == tail) {
            throw invalid_iterator();
        }
        if (empty()) {
            throw container_is_empty();
        }

        node *nextNode = erase(pos.ptr);
        delete pos.ptr;
        return iterator(this, nextNode);
    }

    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        node *newNode = new node(value);
        insert(tail, newNode);
    }

    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (empty()) {
            throw container_is_empty();
        }
        node *toDelete = tail->prev;
        erase(toDelete);
        delete toDelete;
    }

    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        node *newNode = new node(value);
        insert(head->next, newNode);
    }

    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (empty()) {
            throw container_is_empty();
        }
        node *toDelete = head->next;
        erase(toDelete);
        delete toDelete;
    }

    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {
        if (len <= 1) return;

        T **arr = new T*[len];
        node *p = head->next;
        for (size_t i = 0; i < len; i++) {
            arr[i] = p->data;
            p = p->next;
        }

        sjtu::sort<T*>(arr, arr + len, [](const T *a, const T *b) { return *a < *b; });

        p = head->next;
        for (size_t i = 0; i < len; i++) {
            p->data = arr[i];
            p = p->next;
        }

        delete[] arr;
    }

    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        if (this == &other || other.empty()) return;

        node *p1 = head->next;
        node *p2 = other.head->next;

        while (p1 != tail && p2 != other.tail) {
            if (*(p2->data) < *(p1->data)) {
                node *next2 = p2->next;

                other.erase(p2);

                p2->prev = p1->prev;
                p2->next = p1;
                p1->prev->next = p2;
                p1->prev = p2;
                len++;

                p2 = next2;
            } else {
                p1 = p1->next;
            }
        }

        while (p2 != other.tail) {
            node *next2 = p2->next;

            other.erase(p2);

            p2->prev = tail->prev;
            p2->next = tail;
            tail->prev->next = p2;
            tail->prev = p2;
            len++;

            p2 = next2;
        }
    }

    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        node *p = head;
        while (p != nullptr) {
            node *temp = p->prev;
            p->prev = p->next;
            p->next = temp;
            p = p->prev;
        }

        node *temp = head;
        head = tail;
        tail = temp;
    }

    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (len <= 1) return;

        node *p = head->next;
        while (p != tail && p->next != tail) {
            if (*(p->data) == *(p->next->data)) {
                node *toDelete = p->next;
                erase(toDelete);
                delete toDelete;
            } else {
                p = p->next;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP
