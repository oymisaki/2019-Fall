#ifndef WRAPPER_H
#define WRAPPER_H

// ! since we need to repeatedly use class like PayOffBridge, this file is to templatize it.

template< class T>

class Wrapper
{
private:
    T* DataPtr;
public:
    Wrapper() { DataPtr = 0; }  

    // ! this is for implicit conversion from t to Wrapper
    // ! Note that when T is a pure abstract base class, can't use "T t" here
    // ! Because abstract class can't be initialized
    Wrapper(const T& t)
    {
        DataPtr = t.clone();
    }
    ~Wrapper() 
    {
        if (DataPtr != 0)
        {
            delete DataPtr;
        }
    }

    Wrapper(const Wrapper<T>& original)
    {
        if(original.DataPtr != 0)
        {
            DataPtr = original.DataPtr->clone();
        }
        else
        {
            DataPtr = 0;
        }
    }

    Wrapper& operator=(const Wrapper<T>& original)
    {
        if(this != &original)
        {
            if (DataPtr != 0)
                delete DataPtr;
            
            DataPtr = (original.DataPtr != 0) ? original.DataPtr->clone() : 0;
        }

        return *this;
    }

    T& operator*()
    {
        return *DataPtr;
    }

    // ! 2 types of deferencing, ensure it could dereference const object
    const T& operator*() const
    {
        return *DataPtr;
    }

    T* operator->()
    {
        return DataPtr;
    }

    const T* operator->() const
    {
        return DataPtr;
    }
};

#endif // !WRAPPER_H
