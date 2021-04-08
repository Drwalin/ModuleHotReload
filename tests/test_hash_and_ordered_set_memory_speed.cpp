
#include <string>
#include <memory>
#include <unordered_set>
#include <set>
#include <iostream>
#include <vector>
#include <cinttypes>
#include <cstdlib>
#include <atomic>
#include <ctime>
#include <random>
#include <cstring>

std::atomic<size_t> allocated;

template < typename T >
class Destructor
{
public:
	void Destroy( T*object ) { object->~T(); }
};

template < typename T >
class Destructor<T*>
{
public:
	void Destroy( T**object ) { *object = NULL; }
};

template < typename T >
class array
{
public:
	
	int64_t _capacity;
	int64_t _size;
	T * _data;
	
	T & operator[]( const long long id )
	{
#ifdef SAVE_ARRAY_ACCESS_CHECK
		if( id < 0 )
			return this->_data[0];
		if( id >= this->_size )
			return this->_data[this->_size?this->_size-1:0];
#endif
		return this->_data[id];
	}
	
	const T & operator[]( const long long id ) const
	{
#ifdef SAVE_ARRAY_ACCESS_CHECK
		if( id < 0 )
			return this->_data[0];
		if( id >= this->_size )
			return this->_data[this->_size?this->_size-1:0];
#endif
		return this->_data[id];
	}
	
	bool operator == ( const array<T> & other ) const
	{
		if( this->_size == other._size )
		{
			for( int64_t i=0; i<this->_size; ++i )
			{
				if( !(this->_data[i] == other._data[i]) )
					return false;
			}
			return true;
		}
		return false;
	}
	
	bool operator != ( const array<T> & other ) const
	{
		return !(*this==other);
	}
	
	void reserve( const int64_t capacity )
	{
		if( capacity > this->_capacity && capacity < (int64_t(1)<<((sizeof(int64_t)<<3)-5)) )
		{
			int64_t c = this->_capacity;
			while( this->_capacity < capacity )
				this->_capacity <<= 1;
			this->_data = (T*)realloc( this->_data, this->_capacity*sizeof(T) );
			memset( &(this->_data[c]), 0, (this->_capacity-c)*sizeof(T) );
		}
	}
	
	void resize( const int64_t __size )
	{
		this->reserve( __size );
		if( this->_size < __size )
		{
			for( int i=this->_size; i<__size; ++i )
				new( &(this->_data[i]) ) T;
		}
		else
		{
			for( int i=__size; i<this->_size; ++i )
				this->_data[i].~T();
		}
		this->_size = __size;
	}
	
	void resize( const int64_t __size, const T & __default )
	{
		this->reserve( __size );
		if( this->_size < __size )
		{
			for( int i=this->_size; i<__size; ++i )
				new( &(this->_data[i]) ) T(__default);
		}
		else
		{
			for( int i=__size; i<this->_size; ++i )
				this->_data[i].~T();
		}
		this->_size = __size;
	}
	
	array<T> & operator = ( const array<T> & other )
	{
		this->reserve( other._size );
		this->clear();
		this->insert( 0, other.begin(), other.end() );
		return *this;
	}
	
	array<T> & operator += ( const array<T> & other )
	{
		this->insert( this->_size, other.begin(), other.end() );
		return *this;
	}
	
	void insert( long long pos, const T & value ) { this->insert( pos, &value, (&value)+1 ); }
	
	void insert( long long pos, const T * b_value, const T * e_value )
	{
		if( pos < 0 )
			pos = 0;
		const int64_t ns = e_value - b_value;
		this->reserve( this->_size+ns );
		
		int i;
		for( i=this->_size; i<this->_size+ns; ++i )
			new (&(this->_data[i])) T;
		for( i=this->_size-1; i>=pos; --i )
			this->_data[i+ns] = this->_data[i];
		for( i=0; i<ns; ++i )
			this->_data[pos+i] = b_value[i];
		this->_size += ns;
	}
	
	void erase( long long pos ) { this->erase( pos, 1 ); }
	
	void erase( long long pos, long long elements )
	{
		if( pos+elements > this->_size )
			elements = this->_size - pos;
		if( elements <= 0 )
			return;
		int i;
		for( i=pos; i+elements<this->_size; ++i )
			this->_data[i] = this->_data[i+elements];
		for( ; i<this->_size; ++i )
			Destructor<T>().Destroy( &(this->_data[i]) );
		this->_size -= elements;
	}
	
	void clear()
	{
		for( int i=0; i<this->_size; ++i )
			Destructor<T>().Destroy( &(this->_data[i]) );
		this->_size = 0;
	}
	
	void push_back( const T & value ) { this->insert( this->_size, value ); }
	void pop_back( const T & value ) { this->resize( this->size-1 ); }
	T & back() { return this->_data[ this->_size ? this->_size-1 : 0 ]; }
	T & front() { return this->_data[0]; }	
	const T & back() const { return this->_data[ this->_size ? this->_size-1 : 0 ]; }
	const T & front() const { return this->_data[0]; }
	int64_t size() const { return this->_size; }
	const T * begin() const { return this->_data; }
	const T * end() const { return this->_data+this->_size; }
	T * begin() { return this->_data; }
	T * end() { return this->_data+this->_size; }
	
	array(const array<T>& other) {
		this->_size = 0;
		this->_capacity = 32;
		this->_data = (T*)malloc( this->_capacity*sizeof(T) );
		memset( this->_data, 0, this->_capacity*sizeof(T) );
		(*this) = other;
	}
	
	array()
	{
		this->_size = 0;
		this->_capacity = 32;
		this->_data = (T*)malloc( this->_capacity*sizeof(T) );
		memset( this->_data, 0, this->_capacity*sizeof(T) );
	}
	
	~array()
	{
		this->clear();
		free( this->_data );
		this->_size = 0;
		this->_capacity = 0;
		this->_data = NULL;
	}
};

struct Pa {
	void* ptr;
	uint64_t size;
	Pa* next;
};
//array<Pa> map;
Pa* first = NULL;

void* operator new(size_t size) {
//	map.reserve(200000000);
	void* ptr = malloc(size);
	if(ptr) {
		//map.push_back({ptr, size});
		Pa* n = (Pa*)malloc(sizeof(Pa));
		n->ptr = ptr;
		n->size = size;
		n->next = first;
		first = n;
		allocated += size;
	}
	return ptr;
}

void operator delete(void*p) {
	if(p) {
		Pa* a=first, *b=NULL;
		for(;a!=b && a; b=a, a=a->next) {
			if(a->ptr == p) {
				allocated -= a->size;
				if(b) {
					b->next = a->next;
				} else {
					first = a->next;
				}
				free(a);
				break;
			}
		}
		/*
		for(int i=0; i<map.size(); ++i) {
			if(map[i].ptr == p) {
				allocated -= map[i].size;
				map.erase(i);
				break;
			}
		}
		*/
		free(p);
	}
}

void ClearList() {
	for(;first;) {
		Pa* n = first->next;
		allocated -= first->size;
		free(first);
		first = n;
	}
}

template<typename T>
void TimeAndSizeStatistics(std::vector<uint64_t>& elements) {
	size_t currentSize = allocated;
	T* set = new T;
	int beg = clock();
	for(size_t i=0; i<elements.size(); ++i) {
		set->insert(elements[i]);
	}
	int end = clock();
	size_t size = allocated - currentSize;
	printf(", %12llu, %12llu, %f, %f",
			elements.size(), size, ((float)(end-beg))/(float)CLOCKS_PER_SEC,
			((double)size)/((double)elements.size()));
//	map.clear();
	ClearList();
	beg = clock();
	for(size_t i=0; i<elements.size(); ++i) {
		set->erase(elements[i]);
	}
	end = clock();
	printf(", %f", ((float)(end-beg))/(float)CLOCKS_PER_SEC);
	delete set;
}

std::default_random_engine generator;
std::uniform_int_distribution<uint64_t> distribution(0,-1ll);
void random(std::vector<uint64_t>& el, size_t count) {
	el.resize(count);
	size_t i=0;
	while(i<count) {
		uint64_t state = distribution(generator);
		for(size_t j=0; j<100000 && i<count; ++j, ++i) {
			el[i] = state;
			state ^= i^j;
			state += (state << 1) + (state << 4) + (state << 5) + (state << 7) +
				(state << 8) + (state << 40);
		}
	}
}

void std_random(std::vector<uint64_t>& el, size_t count) {
	el.resize(count);
	for(uint64_t&e : el)
		e = distribution(generator);
}

void mangle(std::vector<uint64_t>& el) {
	uint64_t state = distribution(generator) ^ 14695981039346656037llu;
	for(size_t i=0; i<el.size(); ++i) {
		state ^= el[i]^i;
		state += (state << 1) + (state << 4) + (state << 5) + (state << 7) +
			(state << 8) + (state << 40);
		el[i] = state;
	}
}

void Test(std::vector<uint64_t>& el, size_t count) {
	random(el, count);
	printf("\n           sdt::set");
	TimeAndSizeStatistics<std::set<uint64_t>>(el);
	printf("\n sdt::unordered_set");
	TimeAndSizeStatistics<std::unordered_set<uint64_t>>(el);
	fflush(stdout);
}

void random_mod(std::vector<uint64_t>& el, size_t count) {
	uint64_t state = distribution(generator) ^ 14695981039346656037llu;
	if(el.size() < count) {
		el.resize(count);
		mangle(el);
	} else {
		mangle(el);
		uint64_t s = el.size();
		el.resize(count);
		for(uint64_t i=s; s<count; ++s) {
			state ^= el[i]^i;
			state += (state << 1) + (state << 4) + (state << 5) + (state << 7) +
				(state << 8) + (state << 40);
			el[i] = state;
			
		}
	}
}

template<typename T>
void TestMultiAverage(std::vector<uint64_t>& el, size_t count, size_t countmin, size_t countmax) {
	//random(el, size);
	el.reserve(countmin+countmax+count);
	printf("\n started TestMultiAverage");
	fflush(stdout);
	uint64_t beg_size = allocated;
	std::vector<T> sets;
	sets.resize(count);
	int beg = clock();
	uint64_t sum = 0;
	uint64_t I=0;
	for(auto&s : sets) {
		++I;
		//random_mod(el, (distribution(generator)%(countmax-countmin)+countmin);
		std_random(el, (distribution(generator)%(countmax-countmin)+countmin));
		sum += el.size();
		for(uint64_t e : el) {
			s.insert(e);
		}
		uint64_t bytes = allocated - beg_size;
		printf("\n %12llu, %12llu, %12llu, %f", I, sum, bytes,
				(double)bytes/(double)sum);
	}
	int end = clock();
	uint64_t bytes = allocated - beg_size;
	printf("\n %llu -> %llu gives %llu bytes in %fs", count, sum, bytes,
			((float)(end-beg))/(float)CLOCKS_PER_SEC);
	fflush(stdout);
//	map.clear();
	ClearList();
}

int main() {
	srand(time(NULL));
	std::vector<uint64_t> elements;
	
	printf("\n sizeof(std::string) = %llu", sizeof(std::string));
	printf("\n sizeof(std::vector<uint64_t>) = %llu", sizeof(std::vector<uint64_t>));
	printf("\n sizeof(std::set<uint64_t>) = %llu", sizeof(std::set<uint64_t>));
	printf("\n sizeof(std::unordered_set<uint64_t>) = %llu", sizeof(std::unordered_set<uint64_t>));
	
	printf("\n\n sdt::set:");
	fflush(stdout);
	TestMultiAverage<std::set<uint64_t>>(elements,           10000, 1233, 3001);
	printf("\n sdt::unordered_set:");
	fflush(stdout);
	TestMultiAverage<std::unordered_set<uint64_t>>(elements, 10000, 1233, 3001);
	
	for(uint64_t i=0; i<200000000; i+=1+(i/70)) {
		Test(elements, i);
	}
	
	/*
	random(elements, 10000);
	printf("\n\n sdt::set:");
	TimeAndSizeStatistics<std::set<uint64_t>>(elements);
	printf("\n\n sdt::unordered_set:");
	TimeAndSizeStatistics<std::unordered_set<uint64_t>>(elements);
	
	random(elements, 1000000);
	printf("\n\n sdt::set:");
	TimeAndSizeStatistics<std::set<uint64_t>>(elements);
	printf("\n\n sdt::unordered_set:");
	TimeAndSizeStatistics<std::unordered_set<uint64_t>>(elements);
	
	random(elements, 10000000);
	printf("\n\n sdt::set:");
	TimeAndSizeStatistics<std::set<uint64_t>>(elements);
	printf("\n\n sdt::unordered_set:");
	TimeAndSizeStatistics<std::unordered_set<uint64_t>>(elements);
	*/
	return 0;
}

