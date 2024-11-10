// Name: Dylan Phan
// CWID: 885582536
// Email: DylanP@csu.fullerton.edu

#ifndef MY_HASH_TABLE_HPP
#define MY_HASH_TABLE_HPP


/**
 * TODO: Complete this class!
 */


/// Your welcome
#include <forward_list>
#include <functional>
#include <memory>
#include <string>
#include <stdexcept>


//	More includes ... ?

#include <iostream>
//#include <>


///	Begin our class namespace
namespace CPSC131::MyHashTable
{
	//
	template<class VTYPE>
	class MyHashTable
	{
		//
		public:
			
			//
			static constexpr size_t DEFAULT_CAPACITY = 1024;
			static constexpr size_t ULLONG_WRAP_AT = 4294967295;
			
			/**
			 * Constructor
			 * 
			 * Probably easiest to do the following:
			 * 
			 *   1. Call on setCapacity() to set the table's capacity
			 */
			MyHashTable(size_t capacity = MyHashTable::DEFAULT_CAPACITY)
			{
				setCapacity(capacity);
				size_ = 0;
				n_collisions_ = 0;
			}
			
			/**
			 * Copy constructor
			 */
			MyHashTable(const MyHashTable& other)
			{
				operator=(other);
			}
			
			/**
			 * Destructor
			 * 
			 * Deallocate the table (array of std::forward_list 's), and set the table to nullptr
			 * Don't double-deallocate if the table is already a null pointer
			 */
			~MyHashTable()
			{
				//work on this
				if (table_ != nullptr) {
					for (size_t i = 0; i < capacity_; i++) {
						if (!table_[i].empty()) {
							table_[i].clear();
						}
					}
					delete[] table_;
				}
				size_ = 0;
				capacity_ = 0;
				n_collisions_ = 0;
			}
			
			/**
			 * Should return the capacity_ of this hash table
			 * Meaning, the total number of rows in this hash table
			 * Doesn't discern between used/unused/collided rows
			 */
			size_t capacity() const
			{
				return capacity_;
			}
			
			/**
			 * Should return the number of items currently in the hash table
			 */
			size_t size() const
			{
				return size_;
			}
			
			/**
			 * Return true if the table is empty, false otherwise
			 */
			bool empty() const
			{
				return (size_ == 0);
			}
			
			/**
			 * Should return the number of collisions currently present in the hash table
			 */
			size_t n_collisions() const
			{
				return n_collisions_;
			}
			
			/**
			 * Set the capacity for this table.
			 * Changes the total number of hash table rows available.
			 * 
			 * Existing entries should survive the change,
			 * 	so you probably want to do the following:
			 * 
			 *  1. Keep the existing table array pointer in a temporary pointer
			 *  
			 *  2. Re-initialize 'this' to contain a new empty table of the right capacity
			 *  
			 *  3. The temporary pointer should now contain the OLD data (old table),
			 *     while 'this' contains the brand new table of the new size.
			 * 
			 *  4. Iterate through all key/value pairs in the old data (temp pointer).
			 * 
			 *  5. For each key/value pair in the old data, call this->add() to insert
			 *     properly into the new table.
			 *  
			 *  6. Deallocate the old table by calling `delete []` on the temporary pointer.
			 * 
			 * Don't forget to check whether the current table is a nullptr before you
			 *  decide to do iterations!
			 */
			void setCapacity(size_t c)
			{
				if (c == 0) {
					throw std::runtime_error("Capacity cannot be zero");;
				}
				std::forward_list<std::pair<std::string, VTYPE>>* oldTable = table_;
				size_t oldCapacity = capacity_;

				std::forward_list<std::pair<std::string, VTYPE>>* newTable = new std::forward_list<std::pair<std::string, VTYPE>>[c];
				capacity_ = c;
				size_ = 0;
				n_collisions_ = 0;

				table_ = newTable;

				for (size_t i = 0; i < oldCapacity; i++) {
					for (const auto& entry : oldTable[i]) {
						this->add(entry.first, entry.second);
					}
				}

				delete[] oldTable;
				//oldTable = nullptr;
			}
			
			///	Your welcome
			unsigned long long int hash(std::string key) const
			{
				return this->midSquareHash(key);
			}
			
			/**
			 * Implement the "modified mid square" hash function,
			 * as described in the instructions.
			 */
			unsigned long long int midSquareHash(std::string key) const
			{
        		unsigned long long int sum = 1;
        		for (char c : key) {
            		unsigned long long int charValue = static_cast<unsigned long long int>(c);
            		sum = (sum * charValue) % ULLONG_WRAP_AT;
					//sum = sum % ULLONG_WRAP_AT;
       			}
        		unsigned long long int hashCodeSquared = (sum*sum); 
        		std::string hashCodeSquaredString = std::to_string(hashCodeSquared);
				int stringsize = (hashCodeSquaredString.length());
				std::string midHashCodeSquaredString = hashCodeSquaredString.substr((stringsize / 4), (stringsize / 2));
				unsigned long long int hashCodeMiddle = stoull(midHashCodeSquaredString, nullptr, 10) % capacity_;
				return hashCodeMiddle;
			}
			
			/**
			 * Implement your own custom hash functions to compete on the Gradescope Leaderboard!
			 * Four stubs are provided to make experimentation a little easier.
			 * Only the best hash function will count towards the leaderboard.
			 */
			unsigned long long int myCustomHashFunction1(std::string key) const
			{
				return 0;
			}
			unsigned long long int myCustomHashFunction2(std::string key) const
			{
				return 0;
			}
			unsigned long long int myCustomHashFunction3(std::string key) const
			{
				return 0;
			}
			unsigned long long int myCustomHashFunction4(std::string key) const
			{
				return 0;
			}
			
			/**
			 * Return true if a key exists in the table, or false otherwise.
			 */
			bool exists(std::string key) const
			{
				const auto& sll = table_[hash(key)];
				for (const auto& [k, v] : sll) {
					if (k == key) {
						return true;
					}
				}
				return false;
			}
			
			/**
			 * Add a key/value pair to this table.
			 * If the key already exists, throw a runtime_error.
			 */
			void add(std::string key, VTYPE value)
			{

				if (exists(key)) {
					throw std::runtime_error("Key already exists in the table.");
				}
				auto& sll = table_[hash(key)];
					if (!sll.empty()) {
						n_collisions_++;
					}
					sll.emplace_front(key, value);
					size_++;
			}
			
			/**
			 * Should return a reference to the VTYPE value associated to the provided key.
			 * If the key/value pair isn't in the table, a runtime_error should be thrown.
			 */
			VTYPE& get(std::string key) const
			{
				size_t index = hash(key);
				std::forward_list<std::pair<std::string, VTYPE>>& sll = table_[index];
				for (auto it = sll.begin(); it != sll.end(); it++) {
					if (it->first == key) {
						return it->second;
					}
				}
				throw std::runtime_error("Cannot get value for key because it doesn't exist: " + key);
			}
			
			/**
			 * In a single std::forward_list, return every key currently present in the table
			 * 
			 * If no keys exist (empty table), just return an empty std::forward_list.
			 * 
			 * It's okay that when sorted == false, std::forward_list::push_front will
			 *  return your keys in backwards-order, as there is no guarantee
			 *  of properly-ordered keys anyway.
			 * 
			 * However, if the caller passes sorted=true, you should sort the keys
			 * 	as a last step before returning them.
			 * 	Try using the built-in method std::forward_list::sort
			 */
			std::forward_list<std::string> getAllKeys(bool sorted = false) const
			{
				std::forward_list<std::string> keys;
				for (size_t i = 0; i < capacity_; i++) {
					for (const auto& entry : table_[i]) {
						keys.push_front(entry.first);
					}
				}
				if (sorted) {
					keys.sort();
				}
				return keys;
			}
			
			/**
			 * Remove a key/value pair that corresponds to the provided key.
			 * If no such key exists, throw a runtime_error.
			 */
			void remove(std::string key)
			{
				if (!exists(key)) {
					throw std::runtime_error("Cannot remove value for key because it doesn't exist: " + key);
				}
					std::forward_list<std::pair<std::string, VTYPE>>& sll = table_[hash(key)];
					sll.remove_if([&key](const std::pair<std::string, VTYPE>& p) { return p.first == key;});
					size_--;
					if (!sll.empty() && std::distance(sll.begin(), sll.end()) > 1) {
						n_collisions_--;
					}
				
			}
			
			/**
			 * Remove all entries in this table
			 * Iterate over each table row and call clear on the row's list
			 */
			void clear()
			{
				for (size_t i = 0; i < capacity_; i++) {
					auto& sll = table_[i];
					sll.clear();
				}
				n_collisions_ = 0;
				size_ = 0;
			}
			
			/**
			 * Assignment Operator
			 */
			MyHashTable<VTYPE>& operator=(const MyHashTable<VTYPE>& other)
			{
				if (this != &other) {
					this->clear();
					setCapacity(other.capacity_);
					for (const auto& entry : other.getAllKeys()) {
						this->add(entry, other.get(entry));
					}
				}
				return *this;
			}
			
		//
		private:
			
			/**
			 * Keeps track of the capacity of this hash table
			 * (total number of rows in the table, whether used or not)
			 * (does not care about collisions)
			 */
			size_t capacity_ = 0;
			
			/**
			 * Keeps track of the total number of items currently in
			 * the hash table
			 */
			size_t size_ = 0;
			
			/**
			 * Keeps track of the number of collisions currently present in the table
			 */
			size_t n_collisions_ = 0;
			
			/**
			 * The actual hash table.
			 * We'll make this a fixed-sized array,
			 *   but dynamically allocated so we can resize whenever we need.
			 * 
			 * Try to imagine each table row is actually a complete forward_list.
			 * In a perfect situation where a table has no collisions, each list
			 *   will simply contain one item.
			 * 
			 * One "item" in this case refers to an std::pair<std::string, VTYPE>,
			 *   where the first part of the pair is the key and the second
			 *   is the value (of type VTYPE).
			 */
			std::forward_list<std::pair<std::string, VTYPE>> * table_ = nullptr;
	};
}













#endif
