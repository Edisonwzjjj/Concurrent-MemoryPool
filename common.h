#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
using std::cout;
using std::endl;

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

private:
    void* _freelist;
};