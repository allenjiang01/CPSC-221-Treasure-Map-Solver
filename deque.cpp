/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */


template <class T>
Deque<T>::Deque(){
    n1 = 0; //index L
    n2 = 0; //index R
}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T newItem)
{
    data.push_back(newItem);
    n2 = data.size();
    
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes. 
 *
 * @return The item that used to be at the left of the Deque.
 */
template <class T>
T Deque<T>::popL()
{
    T value = data[n1];
    n1++;
    n2--;

     if (n2 <= n1) {
        data.erase(data.begin(), data.begin()+n1);
        data.shrink_to_fit();
        n1 = 0;
        n2 = data.size();
     }

     return value;
}
/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Deque.
 */
template <class T>
T Deque<T>::popR()
{
    T value = data[n2-1];
    data.pop_back();
    n2--;

//data.size() >= 2*(n2-n1+1)
    if (n2 <= n1) {
        data.erase(data.begin(), data.begin()+n1);
        data.shrink_to_fit();
        n1 = 0;
        n2 = data.size();
    }

    return value;
}

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the front of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    return data[n1];
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
    return data[n2-1];
}

/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    return data.empty();
}
