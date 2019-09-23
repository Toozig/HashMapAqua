/////////////////////////////////////////////////////////////
/// cpp_ex3                                               ///
/// written by toozig 204096556                           ///
/// file name: HashMap.cpp                                ///
///Description: This file contains the template  for the  ///
///              HashMap container and it's iterator.     ///
/////////////////////////////////////////////////////////////


/////////////////// includes ///////////////////

#include <algorithm>
#include <vector>
#include <set>
#include <cmath>
#include <stdexcept>
#include <cmath>
#include <iostream>

/////////////////// template ///////////////////

template<typename keyT, typename valueT>
/**
 * Template for HashMap container
 * @tparam keyT types for keys
 * @tparam valueT types for values
 */
class HashMap
{
    typedef __gnu_cxx::__normal_iterator<std::pair<keyT, valueT> *, std::vector<std::pair<keyT, valueT>>> iterator;
    typedef std::vector<std::pair<keyT, valueT>> bucket;

    /**
    * result if a key wasnt found in the hashmap
    */
    static const int NOT_FOUND = -1;

    /**
     * The default size factor for capacity
     */
    static const int DEFAULT_SIZE = 4;

    /**
     * The base of the capacity power
     */
    static const int SIZE_BASE = 2;

    /**
     * points if to calculate the hash code  according to it's size or if it's for resizing
     */
    static const int DEFAULT = -1;

    /**
    * Enum for the resize functions
    */
    enum reSizeFactors
    {
        shrink,
        enlarg
    };

    /**
 * The default value for lower bound
 */
    constexpr static double DEFAULT_LOW_FACTOR = 0.25;

    /**
     * The default value for upper bound
     */
    constexpr static double DEFAULT_UP_FACOTR = 0.75;
public:

    /**
     * Default Ctor
     * @param lowerBound
     */
    explicit HashMap(double lowerBound = DEFAULT_LOW_FACTOR, double upBound = DEFAULT_UP_FACOTR) :
            _size(DEFAULT_SIZE),
            _counter(0),
            _lowerBound(lowerBound),
            _upperBound(upBound)
    {
        if (_lowerBound > _upperBound || _upperBound < 0 ||
            _upperBound > 1 || _lowerBound > 1 || _lowerBound < 0)
        {
            throw std::invalid_argument("Invalid parameters");
        }
        _map = new (std::nothrow) bucket[(int) _mapSize(DEFAULT_SIZE)];
        if( _map == nullptr)
        {
            throw std::bad_alloc();
        }//todo check if this is how it's need to be done
    }

    /**
     * Ctor that gets 2 vectors and creates a hashmap
     * @param keys  vectors of keys
     * @param values vectors of values
     */
    explicit HashMap(const std::vector<keyT> &keys, const std::vector<valueT> &values) :
              _size(DEFAULT_SIZE),
              _counter(0),
             _lowerBound(DEFAULT_LOW_FACTOR),
              _upperBound(DEFAULT_UP_FACOTR)
    {
        size_t keyNums = keys.size();
        if (keys.size() != values.size())
        {
            throw std::invalid_argument("Vectors not in the same size");
        }

        while ((keyNums / _mapSize(_size)) > _upperBound)
        {
            ++_size;
        }
        _map = new (std::nothrow) bucket[(int) _mapSize(_size)];
        if( _map == nullptr)
        {
            throw std::bad_alloc();
        }//todo check if this is how it's need to be done
        long hash;
        for (size_t i = 0; i < keyNums; ++i)
        {
            hash = _hashKey(keys[i]);

            int idx = _getIdx(keys[i], hash);
            switch (idx)
            {
                case NOT_FOUND:
                    _map[hash].push_back(std::make_pair(keys[i], values[i]));
                    ++_counter;
                    break;
                default:
                    _map[hash].at(idx).second = values[i];
            }
        }
    }

    /**
     * Copy Ctor
     */
    HashMap(const HashMap &other) :
            _size(other._size),
            _counter(other._counter),
            _lowerBound(other._lowerBound),
            _upperBound(other._upperBound),
            _map(new (std::nothrow) bucket[(int) _mapSize(other._size)])
    {
        if( _map == nullptr)
        {
            throw std::bad_alloc();
        }//todo check if this is how it's need to be done
        for (int i = 0; i < _mapSize(other._size); ++i)
        {

            _map[i] = other._map[i];
        }
    }

    /**
     * Assigment Ctor
     */
    HashMap & operator=(HashMap other)
    {
        swap(*this, other);
        return *this;
    }


    /**
     * move Ctor
     */
    HashMap(HashMap &&  other) noexcept
            :  _size(other._size),
              _counter(other._counter),
             _lowerBound(other._lowerBound),
              _upperBound(other._upperBound),
              _map(other._map)
    {
        other._counter = 0;
        other._map = nullptr;
    }

    /**
     * Dtor
     */
    ~HashMap()
    {
        delete[] _map;
        _map = nullptr;
    }

    /**
     * Iterator for the HashMap
     */
    class const_iterator
    {
    public:

        typedef const_iterator self_type;
        typedef std::pair<keyT, valueT> value_type;
        typedef std::pair<keyT, valueT> &reference;
        typedef std::pair<keyT, valueT> * pointer;
        typedef int difference_type;
        typedef std::forward_iterator_tag iterator_category;

        explicit const_iterator(const HashMap &map, size_t outIdx = 0, size_t inIdx = 0):
         _arrSize((size_t) _mapSize(map._size)),
         _arr(map._map),
         _outIdx(outIdx),
         _inIdx(inIdx)
        {
            _forward();
        };

        /**
         * prefix ++ operator
         */
        self_type operator++()
        {
            if (_outIdx < _arrSize)
            {
                ++_inIdx;
            }
            _forward();
            return *this;
        }

        /**
       * prefix ++ operator
       */
        self_type operator++(int)
        {
            self_type i = *this;
            if (_outIdx < _arrSize)
            {
                ++_inIdx;
            }
            _forward();
            return i;
        }

        /**
         *  '*' operator for the Iterator
         * @return refrence to the cur pair the map is at
         */
        const value_type &operator*()
        {
            return _arr[_outIdx].at(_inIdx);
        }


        /**
         *  '->' operator for the Iterator
         * @return pointer to the cur pair the map is at
         */
        const value_type * operator->()
        {
            return &_arr[_outIdx].at(_inIdx);
        }

        /**
         *
         * @param rhs other iterator
         * @return  true if both iterators points on the same obj
         */
        bool operator==(const const_iterator &rhs)
        {

            return _arr == rhs._arr && _outIdx == rhs._outIdx && _inIdx == rhs._inIdx;
        }

        /**
         *
         * @param rhs other iterator
         * @return  false if both iterators points on the same obj
         */
        bool operator!=(const self_type &rhs)
        {
            return !(*this == rhs);
        }

    private:

        /**
         * Set the index of the iterator to the next spot
         */
        void _forward()
        {
            if (_outIdx == _arrSize || _inIdx != _arr[_outIdx].size())
            {
                return;
            }
            _inIdx = 0;
            for (++_outIdx; _outIdx < _arrSize; ++_outIdx)
            {
                if (_arr[_outIdx].size())
                {
                    break;
                }
            }
        }

        /**
         * The size of the array of buckets
         */
        size_t _arrSize;

        /**
         * Refrence to the array of buckets
         */
        bucket * _arr;

        /**
         * Index that follows the array of buckets
         */
        size_t _outIdx;

        /**
         * Index that follows the buckets
         */
        size_t _inIdx;

    };

    /**
     *
     * @return iterator that can iterate on all the obj inside the map
     */
    const_iterator begin() const
    {
        return const_iterator(*this);
    }

    /**
     *
     * @return iterator that can iterate on all the obj inside the map (const)
     */
    const_iterator cbegin() const
    {
        return const_iterator(*this);
    }

    /**
   *
   * @return iterator to the one after last place
   */
    const_iterator end() const
    {
        size_t outIdx = (size_t) _mapSize(_size);
        return const_iterator(*this, outIdx);
    }

    /**
    *
    * @return iterator to the one after last place (const)
    */
    const_iterator cend() const
    {
        return end();
    }


    /**
     * Checks if two maps are the same (capacity, size, and objects)
     */
    bool operator==(const HashMap &other) const
    {
        if (_counter != other._counter || _size != other._size)
        {
            return false;
        }
        for (auto &pair : other)
        {
            long hash = _hashKey(pair.first);
            long idx = _getIdx(pair, hash);
            if (idx == _map[hash].size() || idx == NOT_FOUND)
            {
                return false;
            }
        }
        return true;
    }

    /**
     * Check if two hashmap are not equal
     */
    bool operator!=(const HashMap &other) const
    {
        return !(*this == other);
    }


    /**
     * returns the amount of keys inside the map
     */
    int size()
    { return _counter; }

    /**
     * returns the current capacity of themap
     * @return
     */
    int capacity()
    { return (int) _mapSize(_size); }

    /**
     *
     * @return the lower load factor
     */
    double getLoadFactor()
    { return _counter / _mapSize(_size); }

    /**
     * returns true if the map is empty
     */
    bool empty()
    { return !_counter; }

    /**
     * Insert a key and a value to the map.
     * if the key exist, it replace it's value
     * @return true if insert correctly
     */
    bool insert(const keyT &key, const valueT &value)
    {
        long hash = _hashKey(key);
        long idx = _getIdx(key, hash);
        //checks if the key already exist
        if (0 <= idx)
        {
            return false;
        }
        ++_counter;
        if (getLoadFactor() > _upperBound)
        {
            _resize(enlarg);
            hash = _hashKey(key);
        }
        _map[hash].push_back(std::make_pair(key, value));
        return true;
    }

    /**
     * Checks if the map contains a key
     */
    bool containsKey(const keyT &key) const
    {
        long hash = _hashKey(key);
        long idx = _getIdx(key, hash);
        return idx != NOT_FOUND;
    }

    /**
     *
     * @param key the key which the buckets belongs to
     * @return the bucket size of a given key
     */
    int bucketSize(const keyT &key) const
    {
        long hash = _hashKey(key);
        long idx = _getIdx(key, hash);
        return idx == NOT_FOUND ? throw std::invalid_argument("Key does not exist") : (int) _map[hash].size();
    }

    /**
     * clear the map
     */
    void clear()
    {
        for (int i = 0; i < _mapSize(_size); ++i)
        {
            _map[i].clear();
        }
        _counter = 0;
    }

    /**
     *  returns the value of the given key.
     *  creates one if does not exist.
     */
    valueT &operator[](const keyT &key)
    {
        long hash = _hashKey(key);
        long idx = _getIdx(key, hash);
        if (idx != NOT_FOUND)
        {
            return _map[hash].at(idx).second;
        }
        std::pair<keyT, valueT> pair = std::make_pair(key, valueT());
        _map[hash].push_back(pair);
        ++_counter;
        if (getLoadFactor() > _upperBound)
        {
            _resize(enlarg);
            hash = _hashKey(key);
            idx = _getIdx(key, hash);
            return _map[hash].at(idx).second;
        }
        return (--_map[hash].end())->second;
    }

    /**
     * if exist, returns th value of a given key, if not throws exception
     */
    valueT operator[](const keyT &key) const
    {
        long hash = _hashKey(key);
        long idx = _getIdx(key, hash);
        if (idx != NOT_FOUND)
        {
            return _map[hash].at(idx).second;
        }
        throw std::invalid_argument("key does not exist");
    }

    /**
     * @return true if a given key deleted
     */
    bool erase(const keyT &key)
    {
        long hash = _hashKey(key);
        long idx = _getIdx(key, hash);
        if (idx == NOT_FOUND)
        {
            return false;
        } //key is not in the map
        _map[hash].erase(_map[hash].begin() + idx);
        --_counter;
        if (getLoadFactor() < _lowerBound)
        {
            _resize(shrink);
        }
        return true;
    }

    /**
     * returns the value of a given key (const)
     */
    valueT at(const keyT &key) const
    {
        long hash = _hashKey(key);
        long idx = _getIdx(key, hash);
        if (0 <= _getIdx(key, hash))
        {
            return _map[hash].at(idx).second;
        }
        else
        {
            throw std::invalid_argument("The key does not exist");
        }
    }


    /**
     * returns the value of a given key
     */
    valueT &at(const keyT &key)
    {
        long hash = _hashKey(key);
        long idx = _getIdx(key, hash);
        if (0 <= _getIdx(key, hash))
        {
            return _map[hash].at(idx).second;
        }
        else
        {
            throw std::invalid_argument("The key does not exist");
        }
    }

    /**
   * Swap method for the Hashmap
   */
    friend void swap(HashMap &first, HashMap &second)
    {
        using std::swap;
        swap(first._map, second._map);
        swap(first._counter, second._counter);
        swap(first._size, second._size);
        swap(first._lowerBound, second._lowerBound);
    }

private:

    /**
     * gets the index of a key inside the bucket.
     * @param pair
     * @return  the idx, -1 if bucket is empty
     */
    int _getIdx(const std::pair<keyT, valueT> &pair, const long hash) const
    {
        if (!_counter || !_map[hash].size())
        {
            return NOT_FOUND;
        }
        bucket list = _map[hash];
        iterator item = (std::find(list.begin(), list.end(), pair));
        int idx = (int) std::distance(list.begin(), (std::find(list.begin(), list.end(), pair)));
        return idx == _map[hash].size() ? NOT_FOUND : idx;
    }

    /**
     * gets the index of a key inside the bucket.
     * @param pair
     * @return  the idx, -1 if wasnt found
     */
    int _getIdx(const keyT &key, const long hash) const
    {
        if (!_counter || !_map[hash].size())
        {
            return NOT_FOUND;
        }
        std::pair<keyT, valueT> result;
        for (auto i = _map[hash].begin(); i != _map[hash].end(); i++)
        {
            if (i->first == key)
            {
                result = *i;
                break;
            }
        }
        int idx = (int) std::distance(_map[hash].begin(), (std::find(_map[hash].begin(), _map[hash].end(), result)));
        return idx == (int)_map[hash].size() ? NOT_FOUND : idx;
    }

    /**
     * Resize the hashmap
     */
    void _resize(reSizeFactors factor)
    {
        if (!_size)
        {
            return;
        }
        int addition = factor == enlarg ? 1 : -1;
        auto * tmp = new bucket[(int) _mapSize(_size + factor)];
        for (std::pair<keyT, valueT> pair: *this)
        {
            int hash = _hashKey(pair.first, _size + factor);
            tmp[hash].push_back(pair);
        }
        delete[] _map;
        _size += addition;
        _map = tmp;
    }


    /**
    * returns the hash value of a given key
    */
    int _hashKey(const keyT &key, int size = DEFAULT) const
    {
        size = size == DEFAULT ? _size : size;
        long hash;
        hash = std::hash<keyT>{}(key);
        return (int) (hash & ((long) _mapSize(size) - 1));
    }

    /**
     * returns the current map size
     */
    static double _mapSize(const int size)
    {
        return pow(SIZE_BASE, size);
    }

    /**
     * the sizefactor to calculate the map size
     */
    int _size;

    /**
     * The amount of obj inside the HashMap
     */
    int _counter;

    /**
     * The lower bound of the map
     */
    double _lowerBound;

    /**
     * The upper bound of the map.
     */
    double _upperBound;

    /**
     * Array wich contains the buckets
     */
    bucket * _map;
};




