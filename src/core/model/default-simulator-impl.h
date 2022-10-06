/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#ifndef DEFAULT_SIMULATOR_IMPL_H
#define DEFAULT_SIMULATOR_IMPL_H

#include "simulator-impl.h"
#include <list>
#include <mutex>
#include <thread>

/**
 * \file
 * \ingroup simulator
 * ns3::DefaultSimulatorImpl declaration.
 */

namespace ns3 {

// Forward
class Scheduler;

/**
 * \ingroup simulator
 *
 * The default single process simulator implementation.
 */
class DefaultSimulatorImpl : public SimulatorImpl
{
public:
  /**
   *  Register this type.
   *  \return The object TypeId.
   */
  static TypeId GetTypeId ();

  /** Constructor. */
  DefaultSimulatorImpl ();
  /** Destructor. */
  ~DefaultSimulatorImpl ();

  // Inherited
  virtual void Destroy ();
  virtual bool IsFinished () const;
  virtual void Stop ();
  virtual void Stop (const Time &delay);
  virtual EventId Schedule (const Time &delay, EventImpl *event);
  virtual void ScheduleWithContext (uint32_t context, const Time &delay, EventImpl *event);
  virtual EventId ScheduleNow (EventImpl *event);
  virtual EventId ScheduleDestroy (EventImpl *event);
  virtual void Remove (const EventId &id);
  virtual void Cancel (const EventId &id);
  virtual bool IsExpired (const EventId &id) const;
  virtual void Run ();
  virtual Time Now () const;
  virtual Time GetDelayLeft (const EventId &id) const;
  virtual Time GetMaximumSimulationTime () const;
  virtual void SetScheduler (ObjectFactory schedulerFactory);
  virtual uint32_t GetSystemId () const;
  virtual uint32_t GetContext () const;
  virtual uint64_t GetEventCount () const;

private:
  virtual void DoDispose ();

  /** Process the next event. */
  void ProcessOneEvent ();
  /** Move events from a different context into the main event queue. */
  void ProcessEventsWithContext ();

  /** Wrap an event with its execution context. */
  struct EventWithContext
  {
    /** The event context. */
    uint32_t context;
    /** Event timestamp. */
    uint64_t timestamp;
    /** The event implementation. */
    EventImpl *event;
  };
  /** Container type for the events from a different context. */
  typedef std::list<struct EventWithContext> EventsWithContext;
  /** The container of events from a different context. */
  EventsWithContext m_eventsWithContext;
  /**
   * Flag \c true if all events with context have been moved to the
   * primary event queue.
   */
  bool m_eventsWithContextEmpty;
  /** Mutex to control access to the list of events with context. */
  std::mutex m_eventsWithContextMutex;

  /** Container type for the events to run at Simulator::Destroy() */
  typedef std::list<EventId> DestroyEvents;
  /** The container of events to run at Destroy. */
  DestroyEvents m_destroyEvents;
  /** Flag calling for the end of the simulation. */
  bool m_stop;
  /** The event priority queue. */
  Ptr<Scheduler> m_events;

  /** Next event unique id. */
  uint32_t m_uid;
  /** Unique id of the current event. */
  uint32_t m_currentUid;
  /** Timestamp of the current event. */
  uint64_t m_currentTs;
  /** Execution context of the current event. */
  uint32_t m_currentContext;
  /** The event count. */
  uint64_t m_eventCount;
  /**
   * Number of events that have been inserted but not yet scheduled,
   *  not counting the Destroy events; this is used for validation
   */
  int m_unscheduledEvents;

  /** Main execution thread. */
  std::thread::id m_mainThreadId;
};

} // namespace ns3

#endif /* DEFAULT_SIMULATOR_IMPL_H */
