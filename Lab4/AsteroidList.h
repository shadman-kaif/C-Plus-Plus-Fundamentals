/*
 * This file was developed for the Fall 2018 instance of ECE244 at the University of Toronto.
 * Creator: Matthew J. P. Walker
 * Supervised by: Tarek Abdelrahman
 */

#ifndef ECE244_ASTEROID_LIST_HPP
#define ECE244_ASTEROID_LIST_HPP

#include <galaxy-explorer/Asteroid.hpp>

/**
 * An "item" within an `AsteroidList`. Contains things like the next pointer, and the Asteroid that is in the list.
 */
class AsteroidListItem {
public:
        AsteroidListItem();

        AsteroidListItem(Asteroid a);

        ~AsteroidListItem();

        /**
         * Get the Asteroid that this list item
         */
              Asteroid& getData()       { return *data; }
        const Asteroid& getData() const { return *data; }

        /**
         * Get the list item that comes after this one
         */
              AsteroidListItem* getNext()       { return next; }
        const AsteroidListItem* getNext() const { return next; }

        /**
         * Set `next` to `elli`, and return the old value of `next`.
         */
        AsteroidListItem* setNext(AsteroidListItem* elli) { AsteroidListItem* old = next; next = elli; return old; }
        bool hasNext() const { return next != nullptr; }
private:
        AsteroidListItem(const AsteroidListItem& src) { *this = src; }
        AsteroidListItem& operator=(const AsteroidListItem& rhs) {
                next = nullptr;
                delete data;
                data = new Asteroid(*rhs.data);
                return *this;
        }

        AsteroidListItem* next;
