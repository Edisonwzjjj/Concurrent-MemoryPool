#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
using std::cout;
using std::endl;
static const size_t MAX_BYTES = 256 * 1024;
static const size_t NFREELIST = 208;
void*& NextObj(void* obj)
{
    return *(void**)obj;
}
//管理切分好的小对象
class FreeList {
public:
    void Push(void* obj)
    {
        assert(obj);
        NextObj(obj) = _freelist;
        _freelist = obj; 
    }

    void Pop()
    {
        void* obj = _freelist;
        _freelist = NextObj(obj);


    }
    //10%的碎片浪费 
    //[1,128]  8byte对齐 freelist[0， 16)
    size_t RoundUp(size_t size)
    {
        if (size < 128)
        {

        }
    }

private:
    void* _freelist;
};

class SizeClass {
public:
    static inline size_t _RoundUp(size_t bytes, size_t alignnum)
    {
        return ((bytes + alignnum - 1) & ~(alignnum - 1));
    }

    static inline size_t RoundUp(size_t size)
    {
        if (size <= 128)
		{
			return _RoundUp(size, 8);
		}
		else if (size <= 1024)
		{
			return _RoundUp(size, 16);
		}
		else if (size <= 8*1024)
		{
			return _RoundUp(size, 128);
		}
		else if (size <= 64*1024)
		{
			return _RoundUp(size, 1024);
		}
		else if (size <= 256 * 1024)
		{
			return _RoundUp(size, 8*1024);
		}
		else
		{
			assert(false);
			return -1;
		}
    }


    static inline size_t _Index(size_t bytes, size_t align_shift)
    {
        return ((bytes + (1 << align_shift) - 1) >> align_shift) - 1;
    }

    static inline size_t Index(size_t bytes)
    {
        assert(bytes <= MAX_BYTES);
        static int group_array[4] = { 16, 56, 56, 56 };
        if (bytes <= 128){
			return _Index(bytes, 3);
		}
		else if (bytes <= 1024){
			return _Index(bytes - 128, 4) + group_array[0];
		}
		else if (bytes <= 8 * 1024){
			return _Index(bytes - 1024, 7) + group_array[1] + group_array[0];
		}
		else if (bytes <= 64 * 1024){
			return _Index(bytes - 8 * 1024, 10) + group_array[2] + group_array[1] + group_array[0];
		}
		else if (bytes <= 256 * 1024){
			return _Index(bytes - 64 * 1024, 13) + group_array[3] + group_array[2] + group_array[1] + group_array[0];
		}
		else{
			assert(false);
		}

		return -1;
    }
};