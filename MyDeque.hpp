#ifndef MY_DEQUE_HPP
#define MY_DEQUE_HPP

#include <initializer_list>
#include <array>
#include<vector>

template<typename T>
class Deque
{
public:
    Deque();
    
    Deque( size_t _size, T _fillValue );

    template <typename U>
    Deque( std::initializer_list<U> _list );
    
    ~Deque() = default;


    void pushBack(const T &  _t);

    void pushFront(const T & _t);

    void popBack();

    void popFront();

    typename T back();

    typename T front();

    size_t size();

private:
    /****************************************************|m_frontInternalIndex
    *****************|m_frontBlockIndex->[ ][ ][ ][ ][ ][1][2][3]
    *m_directorySize |*******************[4][5][6][7][8][9][8][7]
    *****************|m_backBlockIndex-> [6][5][4][3][ ][ ][ ][ ]
    ***********************************************|m_backInternalIndex
    */
    
    size_t m_directorySize;

    static constexpr  int m_blockSize = 4;
    
    size_t m_frontBlockIndex, m_backBlockIndex;

    int m_frontInternalIndex, m_backInternalIndex;

    enum class RequestType:bool {Front, Back};

    using BlockType = std::array<T, m_blockSize>;

    std::vector< BlockType > m_pDirectory;



    typename BlockType &  getBackValidBlock();

    typename BlockType & getFrontValidBlock();

    //Memory work

    void requestMemory( size_t  _amount , RequestType _request);

    bool isOneElementInDeque();

    void resetIndexes();
    //Back work
    size_t getBackIndexInBlock(); //return valid position for push element;
    
    void expandBack();

    void incInternalBack();

    void incBackBlockIndex();

    void decInternalBack();


    //Front work
    size_t getFrontIndexInBlock();

    void expandFront();

    void incInternalFront();

    void incFrontBlockIndex();

    void decInternalFront();

};


template<typename T>
Deque<T>::Deque()
{
    m_frontBlockIndex = 0;
    m_backBlockIndex = 1;

    m_frontInternalIndex = m_blockSize;
    m_backInternalIndex = -1;
 
    m_directorySize = 0;
}

template<typename T>
inline Deque<T>::Deque(size_t _size, T _fillValue)
{
    for (size_t i = 0; i < _size; i++) 
    {
        pushBack(_fillValue);
    }
}


template<typename T>
template<typename U>
inline Deque<T>::Deque(std::initializer_list<U> _list)
{
    for (auto const & x : _list)
    {
        pushBack(x);
    }
}


template<typename T>
inline void Deque<T>::pushBack(const T & _t)
{
    incInternalBack();
    requestMemory(1, Deque<T>::RequestType::Back);
    getBackValidBlock()[getBackIndexInBlock()] = _t;
}

template<typename T>
inline void Deque<T>::pushFront(const T & _t)
{
    incInternalFront();
    requestMemory(1, Deque<T>::RequestType::Front);
    getFrontValidBlock()[getFrontIndexInBlock()] = _t;
    
}

template<typename T>
inline void Deque<T>::popBack()
{
    getBackValidBlock()[getBackIndexInBlock()] = 0;
    decInternalBack();

}

template<typename T>
inline void Deque<T>::popFront()
{
  
    getFrontValidBlock()[getFrontIndexInBlock()] = 0;

    decInternalFront();
}

template<typename T>
inline typename T Deque<T>::back()
{
    return getBackValidBlock()[getBackIndexInBlock()];
}

template<typename T>
inline typename T Deque<T>::front()
{
    return getFrontValidBlock()[getFrontIndexInBlock()];
}

template<typename T>
inline size_t Deque<T>::size()
{
    size_t dequeSize{};

    if (m_directorySize == 0) 
    {
        return 0;
    }

    if (m_backBlockIndex == m_frontBlockIndex) 
    {
        if (isOneElementInDeque()) 
        {
            return 1;
        }
        return m_frontInternalIndex - m_backInternalIndex;
    }

    //this state:
    //*-empty cell
    //                           |<-m_frontInternalIndex
    //m_frontBlockIndex ->[*][*][1][2]
    //m_backBlockIndex -> [2][4][5][*]
    //                           |<-backInternalIndex
    if ((m_backBlockIndex - m_frontBlockIndex) == 1) 
    {
        dequeSize = (m_blockSize - m_frontInternalIndex) + (m_backInternalIndex+1);
    }
    else 
    {
        dequeSize = (m_blockSize - m_frontInternalIndex) + (m_backInternalIndex + 1) + (m_backBlockIndex  - m_frontBlockIndex - 1)*m_blockSize;
    }
    
    return dequeSize;
}



//Internal Functions block
template<typename T>
inline typename Deque<T>::BlockType & Deque<T>::getBackValidBlock()
{
    return m_pDirectory[m_backBlockIndex];
}

template<typename T>
inline typename Deque<T>::BlockType & Deque<T>::getFrontValidBlock()
{
    return m_pDirectory[m_frontBlockIndex];
}



//Memory work
template<typename T>
inline void Deque<T>::requestMemory(size_t _amount, RequestType _request)
{

    if (m_pDirectory.empty())
    {
        m_pDirectory.emplace_back(std::array<T, Deque<T>::m_blockSize>{});
        m_pDirectory.emplace_back(std::array<T, Deque<T>::m_blockSize>{});
        this->m_directorySize = m_pDirectory.size();
    }

    if (_request == Deque::RequestType::Back)
    {
        if (m_backInternalIndex == m_blockSize)
        {
            incBackBlockIndex();
            if (m_backBlockIndex == m_directorySize)
            {
                expandBack();
            }
            m_backInternalIndex = 0;
        }
    }

    if (_request == Deque::RequestType::Front)
    {
        if (m_frontInternalIndex == -1) 
        {
            incFrontBlockIndex();
            if (m_frontBlockIndex == -1) 
            {
                expandFront();
            }
            m_frontInternalIndex = m_blockSize - 1;
        }
    }
}

template<typename T>
inline bool Deque<T>::isOneElementInDeque()
{
    if (m_backBlockIndex == m_frontBlockIndex) 
    {
        return m_backInternalIndex == m_frontInternalIndex;
    }
    return false;
}

template<typename T>
inline void Deque<T>::resetIndexes()
{
    m_backInternalIndex = -1;
    m_frontInternalIndex = 16;
    m_backBlockIndex++;
}

template<typename T>
inline size_t Deque<T>::getBackIndexInBlock()
{
    return m_backInternalIndex;
}


template<typename T>
inline void Deque<T>::expandBack()
{
    size_t l_prevSize = m_pDirectory.size();

    for (size_t i = 0; i < l_prevSize; i++)
    {
        m_pDirectory.push_back(std::array<T, m_blockSize>{});
    }
    m_directorySize = m_pDirectory.size();
}

template<typename T>
inline void Deque<T>::incInternalBack()
{
    m_backInternalIndex++;
}

template<typename T>
inline void Deque<T>::incBackBlockIndex()
{
    m_backBlockIndex++;
}

template<typename T>
inline void Deque<T>::decInternalBack()
{
    if (isOneElementInDeque()) 
    {
        resetIndexes();
        return;
    }

    m_backInternalIndex--;
 
    if (m_backInternalIndex == -1) 
    {
        m_backInternalIndex = m_blockSize - 1;
        m_backBlockIndex--;
    }
}

template<typename T>
inline size_t Deque<T>::getFrontIndexInBlock()
{
    if (m_frontInternalIndex == m_blockSize) 
    {
        m_frontInternalIndex = 0;
        m_frontBlockIndex++;
    }
    return m_frontInternalIndex;
}

template<typename T>
inline void Deque<T>::expandFront()
{
    size_t l_prevSize = m_pDirectory.size();
    size_t realUsed = m_backBlockIndex - m_frontBlockIndex;

    if (getBackIndexInBlock() == m_directorySize) 
    {
        expandBack();
    }

    if (m_backBlockIndex + realUsed <= m_directorySize) 
    {
        //dosomeShit with copy elements 
    }
}

template<typename T>
inline void Deque<T>::incInternalFront()
{
    m_frontInternalIndex--;
}

template<typename T>
inline void Deque<T>::incFrontBlockIndex()
{
    m_frontBlockIndex--;
}

template<typename T>
inline void Deque<T>::decInternalFront()
{
    if (isOneElementInDeque())
    {
        resetIndexes();
        return;
    }

    m_frontInternalIndex++;

    if (m_frontInternalIndex == m_blockSize) 
    {
        m_frontInternalIndex = 0;
        m_frontBlockIndex++;
    }
}


#endif // !MY_DEQUE_HPP


