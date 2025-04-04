/***********************************************************************
 * Header:
 *    HASH
 * Summary:
 *    Our custom implementation of std::unordered_Set
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        unordered_set           : A class that represents a hash
 *        unordered_set::iterator : An interator through hash
 * Author
 *    Nathan Bird, Brock Hoskins
 ************************************************************************/

#pragma once
#pragma warning(disable : 4244) // disable warning for conversion from 'size_t' to 'float', possible loss of data

#include "list.h"     // because this->buckets[0] is a list
#include "vector.h"   // because this->buckets is a vector
#include <memory>     // for std::allocator
#include <functional> // for std::hash
#include <cmath>      // for std::ceil


class TestHash;             // forward declaration for Hash unit tests

namespace custom
{


/************************************************
 * UNORDERED SET
 * A set implemented as a hash
 ************************************************/
template <typename T,
   typename Hash = std::hash<T>,
   typename EqPred = std::equal_to<T>,
   typename A = std::allocator<T> >
class unordered_set
{
   friend class ::TestHash;   // give unit tests access to the privates
   template <typename TT, typename HH, typename EE, typename AA>
   friend void swap(unordered_set<TT,HH,EE,AA>& lhs, unordered_set<TT,HH,EE,AA>& rhs);
public:
   //
   // Construct
   //
   unordered_set() : numElements(0), maxLoadFactor(1.0), buckets(8)
   {}
   unordered_set(size_t numBuckets) : numElements(0), maxLoadFactor(1.0), buckets(numBuckets)
   {}
   unordered_set(const unordered_set& rhs)
   {
      (*this) = rhs;
   }
   unordered_set(unordered_set&& rhs)
   {
      (*this) = std::move(rhs);
   }
   template <class Iterator>
   unordered_set(Iterator first, Iterator last) : unordered_set()
   {
      reserve(last - first);
      for (Iterator it = first; it != last; ++it)
         insert(*it);
   }

   //
   // Assign
   //
   unordered_set& operator=(const unordered_set& rhs)
   {
      numElements = rhs.numElements;
      maxLoadFactor = rhs.maxLoadFactor;
      buckets = rhs.buckets;
      return *this;
   }
   unordered_set& operator=(unordered_set&& rhs)
   {
      numElements =   std::move(rhs.numElements);
      maxLoadFactor = std::move(rhs.maxLoadFactor);
      buckets =       std::move(rhs.buckets);
      
      // Return rhs to default state
      rhs.numElements = 0;
      rhs.maxLoadFactor = 1.0;
      rhs.rehash(8);

      return *this;
   }
   unordered_set& operator=(const std::initializer_list<T>& il)
   {
      insert(il);
      return *this;
   }
   void swap(unordered_set& rhs)
   {
      std::swap(numElements,     rhs.numElements);
      std::swap(maxLoadFactor,   rhs.maxLoadFactor);
      std::swap(buckets,         rhs.buckets);
   }

   // 
   // Iterator
   //
   class iterator;
   class local_iterator;
   iterator begin()
   {
      for (auto itBucket = buckets.begin(); itBucket != buckets.end(); ++itBucket)
      {
         if (!(*itBucket).empty())
            return iterator(buckets.end(), itBucket, (*itBucket).begin());
      }
      return end();
   }
   iterator end()
   {
      return iterator(buckets.end(), buckets.end(), buckets[0].end());
   }
   local_iterator begin(size_t iBucket)
   {
      return local_iterator(buckets[iBucket].begin());
   }
   local_iterator end(size_t iBucket)
   {
      return local_iterator(buckets[iBucket].end());
   }

   //
   // Access
   //
   size_t bucket(const T& t)
   {
      if (bucket_count() == 0) return 0;
      return Hash()(t) % bucket_count();
   }
   iterator find(const T& t);

   //   
   // Insert
   //
   custom::pair<iterator, bool> insert(const T& t);
   void insert(const std::initializer_list<T>& il);
   void rehash(size_t numBuckets);
   void reserve(size_t num)
   {
      rehash(num / maxLoadFactor);
   }

   // 
   // Remove
   //
   void clear() noexcept
   {
      for (size_t i = 0; i < buckets.size(); i++)
      {
         buckets[i].clear();
      }
      numElements = 0;
   }
   iterator erase(const T& t);

   //
   // Status
   //
   size_t size() const
   {
      return numElements;
   }
   bool empty() const
   {
      return numElements == 0;
   }
   size_t bucket_count() const
   {
      return buckets.size();
   }
   size_t bucket_size(size_t i) const
   {
      return buckets[i].size();
   }
   float load_factor() const noexcept
   {
      return numElements / bucket_count();
   }
   float max_load_factor() const noexcept
   {
      return maxLoadFactor;
   }
   void  max_load_factor(float m)
   {
      maxLoadFactor = m;
   }

private:

   /**
    * Return the minimum number of buckets required to hold num elements.
    * 
    * load_factor = numElements / numBuckets => numBuckets = numElements / load_factor
    */
   size_t min_buckets_required(size_t num) const
   {
      return num / maxLoadFactor;
   }

   custom::vector<custom::list<T, A>> buckets;  // each bucket in the hash
   int numElements;                            // number of elements in the Hash
   float maxLoadFactor;                        // the ratio of elements to buckets signifying a rehash
};


/************************************************
 * UNORDERED SET ITERATOR
 * Iterator for an unordered set
 ************************************************/
template <typename T, typename H, typename E, typename A>
class unordered_set <T, H, E, A> ::iterator
{
   friend class ::TestHash;   // give unit tests access to the privates
   template <typename TT, typename HH, typename EE, typename AA>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   iterator()
   {}
   iterator(const typename custom::vector<custom::list<T> >::iterator& itVectorEnd,
            const typename custom::vector<custom::list<T> >::iterator& itVector,
            const typename custom::list<T>::iterator& itList)
      : itVectorEnd(itVectorEnd), itVector(itVector), itList(itList)
   {}
   iterator(const iterator& rhs)
   {
      (*this) = rhs;
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
      itVectorEnd = rhs.itVectorEnd;
      itVector = rhs.itVector;
      itList = rhs.itList;
      return *this;
   }

   //
   // Compare
   //
   bool operator != (const iterator& rhs) const
   {
      return !(*this == rhs);
   }
   bool operator == (const iterator& rhs) const
   {
      return itVector == rhs.itVector
         && itVectorEnd == rhs.itVectorEnd
         && itList == rhs.itList;
   }

   // 
   // Access
   //
   T& operator * ()
   {
      return *itList;
   }

   //
   // Arithmetic
   //
   iterator& operator ++ ();
   iterator operator ++ (int postfix)
   {
      iterator temp(*this);
      ++(*this);
      return temp;
   }

private:
   typename vector<list<T>>::iterator itVectorEnd;
   typename list<T>::iterator itList;
   typename vector<list<T>>::iterator itVector;
};


/************************************************
 * UNORDERED SET LOCAL ITERATOR
 * Iterator for a single bucket in an unordered set
 ************************************************/
template <typename T, typename H, typename E, typename A>
class unordered_set <T, H, E, A> ::local_iterator
{
   friend class ::TestHash;   // give unit tests access to the privates

   template <typename TT, typename HH, typename EE, typename AA>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   //
   local_iterator()
   {}
   local_iterator(const typename custom::list<T>::iterator& itList)
      : itList(itList)
   {}
   local_iterator(const local_iterator& rhs)
   {
      (*this) = rhs;
   }

   //
   // Assign
   //
   local_iterator& operator = (const local_iterator& rhs)
   {
      itList = rhs.itList;
      return *this;
   }

   // 
   // Compare
   //
   bool operator != (const local_iterator& rhs) const
   {
      return !(*this == rhs);
   }
   bool operator == (const local_iterator& rhs) const
   {
      return itList == rhs.itList;
   }

   // 
   // Access
   //
   T& operator * ()
   {
      return *itList;
   }

   // 
   // Arithmetic
   //
   local_iterator& operator ++ ()
   {
      ++itList;
      return *this;
   }
   local_iterator operator ++ (int postfix)
   {
      local_iterator temp(*this);
      ++(*this);
      return temp;
   }

private:
   typename list<T>::iterator itList;
};


/*****************************************
 * UNORDERED SET :: ERASE
 * Remove one element from the unordered set
 ****************************************/
template <typename T, typename Hash, typename E, typename A>
typename unordered_set <T, Hash, E, A> ::iterator unordered_set<T, Hash, E, A>::erase(const T& t)
{
   iterator itErase = find(t);
   if (itErase == end())
      return itErase;

   iterator itReturn = itErase;
   itReturn++;

   (*itErase.itVector).erase(itErase.itList);
   numElements--;
   return itReturn;
}

/*****************************************
 * UNORDERED SET :: INSERT
 * Insert one element into the hash
 ****************************************/
template <typename T, typename H, typename E, typename A>
custom::pair<typename custom::unordered_set<T, H, E, A>::iterator, bool> unordered_set<T, H, E, A>::insert(const T& t)
{
   // 1. Find the bucket where the new element is to reside.
   size_t iBucket = bucket(t);

   // 2. If the bucket is empty, add the new element.
   for (typename list<T>::iterator it = buckets[iBucket].begin(); it != buckets[iBucket].end(); ++it)
   {
      if (*it == t)
         return custom::pair<custom::unordered_set<T, H, E, A>::iterator, bool>(iterator(buckets.end(), vector<list<T>>::iterator(iBucket, buckets), it), false);
   }

   // 3. Reserve more space if we are already at the limit.
   if (min_buckets_required(numElements + 1) > bucket_count())
   {
      reserve(numElements * 2);
      iBucket = bucket(t);
   }

   // 4. Insert the new element on the back of the bucket if it's not there already.
   if (list_find(buckets[iBucket], t) != buckets[iBucket].end())
      return custom::pair<custom::unordered_set<T, H, E, A>::iterator, bool>(end(), false);

   buckets[iBucket].push_back(t);
   numElements++;

   // 5. Return the iterator to the new element.
   iterator itReturn(buckets.end(), typename vector<list<T>>::iterator(iBucket, buckets), list_find(buckets[iBucket], t));
   return custom::pair<custom::unordered_set<T, H, E, A>::iterator, bool>(itReturn, true);
}

template <typename T, typename H, typename E, typename A>
void unordered_set<T, H, E, A>::insert(const std::initializer_list<T>& il)
{
   for (auto it = il.begin(); it != il.end(); ++it)
      insert(*it);
}

/*****************************************
 * UNORDERED SET :: REHASH
 * Re-Hash the unordered set by numBuckets
 ****************************************/
template <typename T, typename Hash, typename E, typename A>
void unordered_set<T, Hash, E, A>::rehash(size_t numBuckets)
{
   // If the current bucket count is sufficient, nothing to do.
   if (numBuckets <= bucket_count())
      return;

   // Create a new vector with the new number of buckets
   vector<list<T, A>> newBuckets(numBuckets);

   // Move all elements from the old buckets to the new buckets.
   for (auto itBucket = buckets.begin(); itBucket != buckets.end(); ++itBucket)
   {
      for (auto itList = (*itBucket).begin(); itList != (*itBucket).end(); ++itList)
      {
         size_t iBucket = Hash()(*itList) % numBuckets;
         newBuckets[iBucket].push_back(std::move(*itList));
      }
   }

   // Swap the new buckets with the old buckets.
   std::swap(buckets, newBuckets);
}

/*****************************************
 * ITERATOR :: LIST FIND
 * Find an element in a list
 ****************************************/
template <typename T>
typename list<T>::iterator list_find(list<T>& list, const T& t)
{
   for (auto it = list.begin(); it != list.end(); ++it)
      if (*it == t) return it;
   return list.end();
}

/*****************************************
 * UNORDERED SET :: FIND
 * Find an element in an unordered set
 ****************************************/
template <typename T, typename H, typename E, typename A>
typename unordered_set <T, H, E, A> ::iterator unordered_set<T, H, E, A>::find(const T& t)
{
   size_t iBucket = bucket(t);

   typename list<T>::iterator itList = list_find(buckets[iBucket], t);
   
   if (itList != buckets[iBucket].end())
     return iterator(buckets.end(), vector<list<T>>::iterator(iBucket, buckets), itList);
   else
     return end();
   return end();
}

/*****************************************
 * UNORDERED SET :: ITERATOR :: INCREMENT
 * Advance by one element in an unordered set
 ****************************************/
template <typename T, typename H, typename E, typename A>
typename unordered_set <T, H, E, A> ::iterator& unordered_set<T, H, E, A>::iterator::operator ++ ()
{
   // 1. Only advance if not already at end
   if (itVector == itVectorEnd)
      return *this;

   // 2. Advance the list iterator
   // If still not at end, we are done
   ++itList;
   if (itList != (*itVector).end())
      return *this;

   // 3. We are at the end of the list. Find next non-empty bucket
   ++itVector;
   while (itVector != itVectorEnd && (*itVector).empty())
      ++itVector;

   if (itVector != itVectorEnd)
      itList = (*itVector).begin();

   return *this;
}

/*****************************************
 * SWAP
 * Stand-alone unordered set swap
 ****************************************/
template <typename T, typename H, typename E, typename A>
void swap(unordered_set<T, H, E, A>& lhs, unordered_set<T, H, E, A>& rhs)
{
   std::swap(lhs.numElements, rhs.numElements);
   std::swap(lhs.maxLoadFactor, rhs.maxLoadFactor);
   std::swap(lhs.buckets, rhs.buckets);
}


} // namespace custom
