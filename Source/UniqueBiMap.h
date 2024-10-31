// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

#include <unordered_map>



namespace Project001
{
    // Bi-Directional Map
    // 
    // Both keys and values have to be unique. No two keys can have the same
    // value and no two values can have the same key.
    template <class Key, class Value>
    class UniqueBiMap
    {
    public:
        using iterator = typename std::unordered_map<Key, Value>::iterator;
        using const_iterator = typename std::unordered_map<Key, Value>::const_iterator;

        iterator Add(const Key& key, const Value& value)
        {
            std::unordered_map<Key, Value>::iterator keyToValueIterator = keyToValueMap_.find(key);
            if (keyToValueIterator != keyToValueMap_.end())
            {
                valueToKeyMap_.erase(keyToValueIterator->second);
                keyToValueMap_.erase(keyToValueIterator);
            }

            std::unordered_map<Value, Key>::iterator valueToKeyIterator = valueToKeyMap_.find(value);
            if (valueToKeyIterator != valueToKeyMap_.end())
            {
                keyToValueMap_.erase(valueToKeyIterator->second);
                valueToKeyMap_.erase(valueToKeyIterator);
            }

            valueToKeyMap_.insert({ value, key });
            return keyToValueMap_.insert({ key, value }).first;
        }

        iterator FindKey(const Key& key)
        {
            return keyToValueMap_.find(key);
        }

        const_iterator FindKey(const Key& key) const
        {
            return keyToValueMap_.find(key);
        }

        iterator FindValue(const Value& value)
        {
            std::unordered_map<Value, Key>::iterator valueToKeyIterator = valueToKeyMap_.find(value);
            if (valueToKeyIterator != valueToKeyMap_.end())
            {
                return keyToValueMap_.find(valueToKeyIterator->second);
            }
            return keyToValueMap_.end();
        }

        const_iterator FindValue(const Value& value) const
        {
            std::unordered_map<Value, Key>::const_iterator valueToKeyIterator = valueToKeyMap_.find(value);
            if (valueToKeyIterator != valueToKeyMap_.end())
            {
                return keyToValueMap_.find(valueToKeyIterator->second);
            }
            return keyToValueMap_.end();
        }

        iterator Erase(iterator iter)
        {
            if (iter != keyToValueMap_.end())
            {
                valueToKeyMap_.erase(iter->second);
                return keyToValueMap_.erase(iter);
            }
            return keyToValueMap_.end();
        }

        iterator EraseKey(const Key& key)
        {
            std::unordered_map<Key, Value>::iterator keyToValueIterator = keyToValueMap_.find(key);
            if (keyToValueIterator != keyToValueMap_.end())
            {
                valueToKeyMap_.erase(keyToValueIterator->second);
                return keyToValueMap_.erase(keyToValueIterator);
            }
            return keyToValueIterator;
        }

        iterator EraseValue(const Value& value)
        {
            std::unordered_map<Value, Key>::iterator valueToKeyIterator = valueToKeyMap_.find(value);
            if (valueToKeyIterator != valueToKeyMap_.end())
            {
                std::unordered_map<Key, Value>::iterator keyToValueIterator = keyToValueMap_.find(valueToKeyIterator->second);
                keyToValueMap_.erase(keyToValueIterator);
                valueToKeyMap_.erase(valueToKeyIterator);
                return keyToValueIterator;
            }
            return keyToValueMap_.end();
        }

        void Clear()
        {
            keyToValueMap_.clear();
            valueToKeyMap_.clear();
        }

        size_t Size() const
        {
            return keyToValueMap_.size();
        }

        iterator IteratorAtBeginning()
        {
            return keyToValueMap_.begin();
        }

        const_iterator IteratorAtBeginning() const
        {
            return keyToValueMap_.begin();
        }

        iterator IteratorPastTheEnd()
        {
            return keyToValueMap_.end();
        }

        const_iterator IteratorPastTheEnd() const
        {
            return keyToValueMap_.end();
        }

    protected:
        std::unordered_map<Key, Value> keyToValueMap_;
        std::unordered_map<Value, Key> valueToKeyMap_;
    };
}