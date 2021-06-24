#ifndef __POOL_H__
#define __POOL_H__

template <typename T>
class Pool {
    private:
        struct PoolItem {
            T item;
            PoolItem* next;
            bool active;
        };
        short _max;
        PoolItem* _pool;
        PoolItem* _firstAvailable;
    public:
        // Constructor.
        // maxItems: The maximum number of items in the pool.
        Pool(short maxItems) {
            _max = maxItems;
            _pool = new PoolItem[_max];

            for (int i = 0; i < _max - 1; i++) {
                _pool[i].next = &_pool[i + 1];
            }

            _pool[_max - 1].next = nullptr;
            _firstAvailable = &_pool[0];
        }
        ~Pool() {
            delete _pool;
        }

        // Grab the next available item from the pool.
        T* next() {
            if (_firstAvailable) {
                PoolItem* item = _firstAvailable;
                item->active = true;
                _firstAvailable = item->next;
                return &item->item;
            }
            return nullptr;
        }

        // Release the item back to the pool.
        // item: The item to release.
        void release(T* item) {
            for (int i = 0; i < _max; i++) {
                if (&_pool[i].item == item) {
                    _pool[i].active = false;
                    _pool[i].next = _firstAvailable;
                    _firstAvailable = &_pool[i];
                    return;
                }
            }
        }

        // Get the size of the pool.
        short size() { return _max; }

        // Check if an item in the pool is alive.
        bool isActive(int index) {
            if (index >= 0 && index < _max) {
                return _pool[index].active;
            }
            return false;
        }
        
        // Get an item from the pool but don't treat it like it's alive.
        T* item(int index) {
            if (index >= 0 && index < _max) {
                return &_pool[index].item;
            }
            return nullptr;
        }
};

#endif //__POOL_H__
