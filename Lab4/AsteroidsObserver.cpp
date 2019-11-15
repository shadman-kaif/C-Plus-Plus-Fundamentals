#include <galaxy-explorer/AsteroidsObserver.hpp>

void AsteroidsObserver::onAsteroidInRange(Asteroid asteroid) {
    asteroid_list.pushFront(asteroid);
}

void AsteroidsObserver::onAsteroidOutOfRange(Asteroid asteroid) {
    onAsteroidDestroy(asteroid);
}

void AsteroidsObserver::onAsteroidUpdate(Asteroid asteroid) {
    for (AsteroidListItem* ptr = asteroid_list.beforeBegin(); ptr != asteroid_list.end(); ptr = ptr->getNext()) {
        if (!ptr->hasNext()) {
            continue;
        }
        if (ptr->getNext()->getData().getID() != asteroid.getID()) {
            continue;
        }
        AsteroidListItem* temp = asteroid_list.insertAfter(ptr, asteroid);
        asteroid_list.eraseAfter(temp);
        break;
    }
}

void AsteroidsObserver::onAsteroidDestroy(Asteroid asteroid) {
    for (AsteroidListItem* temp = asteroid_list.beforeBegin(); temp != asteroid_list.end(); temp = temp->getNext()) {
        if (temp->hasNext() && temp->getNext()->getData().getID() == asteroid.getID()) {
           asteroid_list.eraseAfter(temp);
           break;
        }
    }
}
