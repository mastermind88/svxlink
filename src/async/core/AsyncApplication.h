/**
@file	 AsyncApplication.h
@brief   The core class for writing asyncronous applications
@author  Tobias Blomberg
@date	 2003-03-16

This file contains the AsyncApplication class which is the core of an
application that use the Async classes.

\verbatim
Async - A library for programming event driven applications
Copyright (C) 2003-2019 Tobias Blomberg

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
\endverbatim
*/

#ifndef ASYNC_APPLICATION_INCLUDED
#define ASYNC_APPLICATION_INCLUDED


/****************************************************************************
 *
 * System Includes
 *
 ****************************************************************************/

#include <sigc++/sigc++.h>

#include <string>
#include <deque>
#include <thread>
#include <mutex>


/****************************************************************************
 *
 * Project Includes
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Local Includes
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Forward declarations
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Namespace
 *
 ****************************************************************************/

namespace Async
{

/****************************************************************************
 *
 * Forward declarations of classes inside of the declared namespace
 *
 ****************************************************************************/

class Timer;
class FdWatch;
class DnsLookupWorker;


/****************************************************************************
 *
 * Defines & typedefs
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Exported Global Variables
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Class definitions
 *
 ****************************************************************************/

/**
 * @brief The base class for asynchronous applications
 *
 * This is the base class for all asynchronous applications. It is an abstract
 * class and so it must be inherited from to create a class that can be
 * instantiated.
 */   
class Application : public sigc::trackable
{
  public:
    /**
     * @brief 	Get the one and only application instance
     *
     * Use this static member function to get the one and only instance of the
     * application object. If an application object has not been previously
     * created, the application will crash with a failed assertion.
     * @return	Returns a reference to the applicaton instance
     */
    static Application &app(void);
    
    /**
     * @brief Default constructor
     */
    Application(void);
    
    /**
     * @brief Destructor
     */
    virtual ~Application(void);
    
    /**
     * @brief Execute the application main loop
     *
     * When this member function is called the application core will enter the
     * core main loop. It will not exit from this loop until the
     * Async::Application::quit method is called.
     */
    virtual void exec(void) = 0;
    
    /**
     * @brief Exit the application main loop
     *
     * This function should be called to exit the application core main loop.
     */
    virtual void quit(void) = 0;

    /**
     * @brief Run a task from the Async main loop
     * @param task The task (SigC++ slot) to run
     *
     * This function can be used to delay a function call until the call chain
     * have returned to the Async main loop. This may be required in some
     * cases where one otherwise would get into trouble with complex callback
     * chains causing strange errors.
     *
     * Use it something like this to call a function taking no arguments:
     *
     *   runTask(sigc::mem_fun(*this, &MyClass::func));
     *
     * If the function need arguments passed to it, they must be bound to the
     * slot using the sigc::bind function:
     *
     *   runTask(sigc::bind(sigc::mem_fun(*this, &MyClass::func), true, 42));
     *
     * In this case the function take two arguments where the first is a bool
     * and the second is an integer.
     *
     * This function is thread safe so a separate thread can add tasks which
     * are then executed on the main Async thread.
     */
    void runTask(sigc::slot<void> task);

    /**
     * @brief Get the thread ID for the main Async thread
     */
    std::thread::id threadId(void) const { return m_main_thread_id; }

    /**
     * @brief   Signal that is emitted when the exec function exit
     *
     * This signal is emitted at the end of the exec function, just before
     * it exits.
     */
    sigc::signal<void> execDone;

    /**
     * @brief   Signal that is emitted just before destruction
     *
     * This signal is emitted right after the Application object is constructed.
     */
    sigc::signal<void> construct;

    /**
     * @brief   Signal that is emitted just before destruction
     *
     * This signal is emitted right before the Application object is destroyed.
     */
    sigc::signal<void> destroy;

  private:
    friend class FdWatch;
    friend class Timer;
    friend class DnsLookup;

    typedef std::deque<sigc::slot<void>> SlotList;

    static Application *app_ptr;

    const std::thread::id m_main_thread_id;
    SlotList              m_task_queue;
    Timer*                m_task_timer;
    std::mutex            m_task_mu;
    Async::FdWatch*       m_task_rd_watch = 0;
    int                   m_task_wr_pipe = -1;

    void processTaskQueue(void);
    void handleTaskWatch(Async::FdWatch* w);
    void clearTasks(void);
    virtual void addFdWatch(FdWatch *fd_watch) = 0;
    virtual void delFdWatch(FdWatch *fd_watch) = 0;
    virtual void addTimer(Timer *timer) = 0;
    virtual void delTimer(Timer *timer) = 0;
    virtual DnsLookupWorker *newDnsLookupWorker(const std::string& label) = 0;
    
};  /* class Application */


} /* namespace */

#endif /* ASYNC_APPLICATION_INCLUDED */



/*
 * This file has not been truncated
 */

