/*
*为使C++全局变量按照一定顺序构造及析构，将有关联的全局变量放入一个文件中.
*一定要将被依赖的组件的全局变量放在依赖其的组件的全局变量的前面。
*/

//HCPPObjectPool
#include "HCPPObjectPool.h"
#include "map"
#include <mutex>

namespace  HCPPObjectPoolGlobal
{
std::mutex HCPPObjectPoolLock;
std::map<std::string,HCPPObject *> HCPPObjectPool;
std::map<void *,std::string> HCPPObjectPoolDeleteHelper;
}

//HCPPObject
#include "HCPPObject.h"
#include <map>
#include <mutex>
#include <algorithm>

namespace HCPPObjectGlobal
{
class HCPPObjectInfo
{
    std::thread::id _tid;
public:
    HCPPObjectInfo();
    HCPPObjectInfo(HCPPObjectInfo&& other);
    HCPPObjectInfo& operator =(HCPPObjectInfo&& other);
    HCPPObjectInfo(HCPPObjectInfo& other);
    HCPPObjectInfo& operator =(HCPPObjectInfo& other);
    std::thread::id& GetThreadId();
    bool SetThreadId(std::thread::id _id, bool force_update = false);
};

HCPPObjectInfo::HCPPObjectInfo()
{
    _tid = std::this_thread::get_id();
}
HCPPObjectInfo::HCPPObjectInfo(HCPPObjectInfo&& other) :_tid(other._tid)
{

}
HCPPObjectInfo& HCPPObjectInfo::operator =(HCPPObjectInfo&& other)
{
    if (this == &other)
    {
        return *this;
    }

    _tid = other._tid;

    return *this;
}
HCPPObjectInfo::HCPPObjectInfo(HCPPObjectInfo& other) :_tid(other._tid)
{

}
HCPPObjectInfo& HCPPObjectInfo::operator =(HCPPObjectInfo& other)
{
    if (this == &other)
    {
        return *this;
    }

    _tid = other._tid;

    return *this;
}
std::thread::id& HCPPObjectInfo::GetThreadId()
{
    return _tid;
}

bool HCPPObjectInfo::SetThreadId(std::thread::id _id, bool force_update)
{
    if (force_update)
    {
        _tid = _id;
        return true;
    }
    if (_id == std::this_thread::get_id())
    {
        _tid = _id;
        return true;
    }
    return false;
}

std::map<void *,HCPPObjectInfo> CPPHeapObjPool;
std::recursive_mutex CPPHeapObjPoolLock;
}

//HCPPThread


//HCPPTimer
#include "HCPPTimer.h"
#include <mutex>
namespace HCPPTimerGlobal
{
class TimerThread
{
    HCPPThread *m_thread;
    std::recursive_mutex m_lock;
public:
    TimerThread();
    ~TimerThread();
    HCPPThread* GetThread();
} g_timerthread;

TimerThread::TimerThread() :m_thread(NULL)
{

}
TimerThread::~TimerThread()
{
    std::lock_guard<std::recursive_mutex> lock(m_lock);
    if (m_thread != NULL)
    {
        m_thread->DeleteThread();
        m_thread = NULL;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
HCPPThread* TimerThread::GetThread()
{
    std::lock_guard<std::recursive_mutex> lock(m_lock);
    if (m_thread == NULL)
    {
        m_thread = HCPPThread::New();
    }
    return m_thread;
}
}
