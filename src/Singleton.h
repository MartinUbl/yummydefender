#ifndef YD_SINGLETON_H
#define YD_SINGLETON_H

/*
 * Singleton template class used for all singletons in application
 */
template<class T>
class Singleton
{
    public:
        // retrieve singleton instance
        static T* getInstance()
        {
            // if does not exist, create new
            if (!m_instance)
                m_instance = new T;

            return m_instance;
        }

    private:
        // only instance
        static T* m_instance;
};

template<class T> T* Singleton<T>::m_instance = nullptr;

#endif
