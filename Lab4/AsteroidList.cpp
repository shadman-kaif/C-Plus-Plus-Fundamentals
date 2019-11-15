#include <galaxy-explorer/AsteroidList.hpp>

AsteroidListItem::AsteroidListItem() {
    this->next = nullptr;
    this->data = nullptr;
}

AsteroidListItem::AsteroidListItem(Asteroid a) {
    this->next = nullptr;
    // Dynamic allocation
    this->data = new Asteroid(a);
}

AsteroidListItem::~AsteroidListItem() {
    // Destructor deletes data
    delete data;
}

// Empty the list
AsteroidList::AsteroidList() {
    head.setNext(nullptr);
}

AsteroidList::AsteroidList(const AsteroidList& src) {
    insertAfter(&head, src);
}

AsteroidList::~AsteroidList() {
    clear();
}

void AsteroidList::pushFront(Asteroid e) {
    insertAfter(&head, e);
}

Asteroid& AsteroidList::front() {
    if (isEmpty() == false) {
        return head.getNext()->getData();
    }
    else {
        return *(Asteroid*)nullptr;
    }
}

const Asteroid& AsteroidList::front() const {
    if (isEmpty() == false){
        return head.getNext()->getData();
    }
    else {
        return *(const Asteroid*)nullptr;
    }
}

bool AsteroidList::isEmpty() const {
    return !head.hasNext();
}

int AsteroidList::size() const {
    int temp = 0;
    const AsteroidListItem* ptr = beforeBegin();
    while(ptr->hasNext()) {
        temp++;
        ptr= ptr->getNext();
    }
    return temp;
}

// Returns the address of the head of the list
AsteroidListItem* AsteroidList::beforeBegin() {
    return &head;
}

// Returns the address of the head of the list
const AsteroidListItem* AsteroidList::beforeBegin() const {
    return &head;
}

AsteroidListItem* AsteroidList::begin() {
    return head.getNext();
}

const AsteroidListItem* AsteroidList::begin() const {
    return head.getNext();
}

AsteroidListItem* AsteroidList::beforeEnd() {
    AsteroidListItem* ptr = beforeBegin();
    while (ptr->hasNext()) {
        ptr = ptr->getNext();
    }
    return ptr;
}

const AsteroidListItem* AsteroidList::beforeEnd() const {
    const AsteroidListItem* ptr = beforeBegin();
    while (ptr->hasNext()) {
        ptr = ptr->getNext();
    }
    return ptr;
}

AsteroidListItem* AsteroidList::end() {
    return nullptr;
}

const AsteroidListItem* AsteroidList::end() const {
    return nullptr;
}

AsteroidListItem* AsteroidList::insertAfter(AsteroidListItem* prev, Asteroid e) {
    if (prev == nullptr) {
        return nullptr;
    }
    AsteroidListItem* ptr = new AsteroidListItem(e);
    ptr->setNext(prev->setNext(ptr));
    return ptr;
}

AsteroidListItem* AsteroidList::insertAfter(AsteroidListItem* prev, const AsteroidList& others) {
    if(!others.head.hasNext()) {
        return prev;
    }
    const AsteroidListItem* ptr = others.head.getNext();
    AsteroidListItem* temp = prev->getNext();
    while (ptr != nullptr) {
        prev->setNext(new AsteroidListItem(ptr->getData()));
        prev = prev->getNext();
        ptr = ptr->getNext();
    }
    prev->setNext(temp);
    return prev;
}

AsteroidListItem* AsteroidList::eraseAfter(AsteroidListItem* prev) {
    if (!prev->hasNext()) {
        return nullptr;
    }
    delete prev->setNext(prev->getNext()->getNext());
    return prev->getNext();
}

void AsteroidList::clear() {
    while (isEmpty() == false) {
        eraseAfter(&head);
    }
}

AsteroidList& AsteroidList::operator=(const AsteroidList& src) {
    clear();
    insertAfter(&head, src);
    return *this;
}