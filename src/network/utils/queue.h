/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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
 */

// The queue base class has a limit on its size, in terms of number of
// packets or number of bytes depending on the operating mode.
// The base class implements tracing and basic statistics calculations.

#ifndef QUEUE_H
#define QUEUE_H

#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/traced-callback.h"
#include "ns3/traced-value.h"
#include "ns3/log.h"
#include "ns3/queue-size.h"
#include "ns3/queue-item.h"
#include "ns3/queue-fwd.h"
#include <string>
#include <sstream>

namespace ns3 {

/**
 * \ingroup network
 * \defgroup queue Queue
 */

/**
 * \ingroup queue
 * \brief Abstract base class for packet Queues
 *
 * This class defines the subset of the base APIs for packet queues in the ns-3 system
 * that is independent of the type of enqueued objects
 */
class QueueBase : public Object
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  QueueBase ();
  virtual ~QueueBase ();

  /**
   * \brief Append the item type to the provided type ID if the latter does not
   *        end with '>'
   *
   * \param typeId the type ID
   * \param itemType the item type
   *
   * This method is meant to be invoked by helpers to save users from
   * specifying the type of items stored in a queue. For instance,
   * PointToPointHelper::SetQueue calls
   *
   * \code
   *   QueueBase::AppendItemTypeIfNotPresent (type, "Packet");
   * \endcode
   *
   * where type specifies the queue type (e.g., "ns3::DropTailQueue").
   * This allows users to call SetQueue ("ns3::DropTailQueue")
   * instead of SetQueue ("ns3::DropTailQueue<Packet>")
   */
  static void AppendItemTypeIfNotPresent (std::string& typeId, const std::string& itemType);

  /**
   * \return true if the queue is empty; false otherwise
   */
  bool IsEmpty (void) const;

  /**
   * \return The number of packets currently stored in the Queue
   */
  uint32_t GetNPackets (void) const;

  /**
   * \return The number of bytes currently occupied by the packets in the Queue
   */
  uint32_t GetNBytes (void) const;

  /**
   * \return The current size of the Queue in terms of packets, if the maximum
   *         size is specified in packets, or bytes, otherwise
   */
  QueueSize GetCurrentSize (void) const;

  /**
   * \return The total number of bytes received by this Queue since the
   * simulation began, or since ResetStatistics was called, according to
   * whichever happened more recently
   */
  uint32_t GetTotalReceivedBytes (void) const;

  /**
   * \return The total number of packets received by this Queue since the
   * simulation began, or since ResetStatistics was called, according to
   * whichever happened more recently
   */
  uint32_t GetTotalReceivedPackets (void) const;

  /**
   * \return The total number of bytes dropped by this Queue since the
   * simulation began, or since ResetStatistics was called, according to
   * whichever happened more recently
   */
  uint32_t GetTotalDroppedBytes (void) const;

  /**
   * \return The total number of bytes dropped before enqueue by this Queue
   * since the simulation began, or since ResetStatistics was called, according
   * to whichever happened more recently
   */
  uint32_t GetTotalDroppedBytesBeforeEnqueue (void) const;

  /**
   * \return The total number of bytes dropped after dequeue by this Queue
   * since the simulation began, or since ResetStatistics was called, according
   * to whichever happened more recently
   */
  uint32_t GetTotalDroppedBytesAfterDequeue (void) const;

  /**
   * \return The total number of packets dropped by this Queue since the
   * simulation began, or since ResetStatistics was called, according to
   * whichever happened more recently
   */
  uint32_t GetTotalDroppedPackets (void) const;

  /**
   * \return The total number of packets dropped before enqueue by this Queue
   * since the simulation began, or since ResetStatistics was called, according
   * to whichever happened more recently
   */
  uint32_t GetTotalDroppedPacketsBeforeEnqueue (void) const;

  /**
   * \return The total number of packets dropped after dequeue by this Queue
   * since the simulation began, or since ResetStatistics was called, according
   * to whichever happened more recently
   */
  uint32_t GetTotalDroppedPacketsAfterDequeue (void) const;

  /**
   * Resets the counts for dropped packets, dropped bytes, received packets, and
   * received bytes.
   */
  void ResetStatistics (void);

  /**
   * \brief Set the maximum size of this queue
   *
   * Trying to set a null size has no effect.
   *
   * \param size the maximum size
   */
  void SetMaxSize (QueueSize size);

  /**
   * \return the maximum size of this queue
   */
  QueueSize GetMaxSize (void) const;

  /**
   * \brief Check if the queue would overflow with additional bytes or packets
   * Note: the check is performed according to the queue's operating mode (bytes or packets).
   * \param nPackets number of additional packets
   * \param nBytes number of additional bytes
   * \return true if the queue should overflow, false otherwise.
   */
  bool WouldOverflow (uint32_t nPackets, uint32_t nBytes) const;

#if 0
  // average calculation requires keeping around
  // a buffer with the date of arrival of past received packets
  // which are within the average window
  // so, it is quite costly to do it all the time.
  // Hence, it is disabled by default and must be explicitly
  // enabled with this method which specifies the size
  // of the average window in time units.
  void EnableRunningAverage (Time averageWindow);
  void DisableRunningAverage (void);
  // average
  double GetQueueSizeAverage (void);
  double GetReceivedBytesPerSecondAverage (void);
  double GetReceivedPacketsPerSecondAverage (void);
  double GetDroppedBytesPerSecondAverage (void);
  double GetDroppedPacketsPerSecondAverage (void);
  // variance
  double GetQueueSizeVariance (void);
  double GetReceivedBytesPerSecondVariance (void);
  double GetReceivedPacketsPerSecondVariance (void);
  double GetDroppedBytesPerSecondVariance (void);
  double GetDroppedPacketsPerSecondVariance (void);
#endif

protected:
  TracedValue<uint32_t> m_nBytes;               //!< Number of bytes in the queue
  uint32_t m_nTotalReceivedBytes;               //!< Total received bytes
  TracedValue<uint32_t> m_nPackets;             //!< Number of packets in the queue
  uint32_t m_nTotalReceivedPackets;             //!< Total received packets
  uint32_t m_nTotalDroppedBytes;                //!< Total dropped bytes
  uint32_t m_nTotalDroppedBytesBeforeEnqueue;   //!< Total dropped bytes before enqueue
  uint32_t m_nTotalDroppedBytesAfterDequeue;    //!< Total dropped bytes after dequeue
  uint32_t m_nTotalDroppedPackets;              //!< Total dropped packets
  uint32_t m_nTotalDroppedPacketsBeforeEnqueue; //!< Total dropped packets before enqueue
  uint32_t m_nTotalDroppedPacketsAfterDequeue;  //!< Total dropped packets after dequeue

  QueueSize m_maxSize;                //!< max queue size
};


/**
 * \ingroup queue
 * \brief Template class for packet Queues
 *
 * This class defines the subset of the base APIs for packet queues in the ns-3 system
 * that is dependent on the type of enqueued objects.
 *
 * Queue is a template class. The type of the objects stored within the queue
 * is specified by the type parameter, which can be any class providing a
 * GetSize () method (e.g., Packet, QueueDiscItem, etc.). Subclasses need to
 * implement the Enqueue, Dequeue, Remove and Peek methods, and are
 * encouraged to leverage the DoEnqueue, DoDequeue, DoRemove, and DoPeek
 * methods in doing so, to ensure that appropriate trace sources are called
 * and statistics are maintained. The template parameter specifies the type of
 * container used internally to store queue items. The container type must provide
 * the methods insert(), erase() and clear() following the usual syntax of C++
 * containers. The default container type is std::list (as defined in queue-fwd.h).
 *
 * Users of the Queue template class usually hold a queue through a smart pointer,
 * hence forward declaration is recommended to avoid pulling the implementation
 * of the templates included in this file. Thus, include queue-fwd.h, which
 * provides a forward declaration for the Queue class that defines the default
 * value for the template template parameter, instead of queue.h in your .h file.
 * Then, include queue.h in the corresponding .cc file.
 *
 * \tparam Item \explicit Type of the objects stored within the queue
 * \tparam Container \explicit Type of the container that stores queue items
 */
template <typename Item, typename Container>
class Queue : public QueueBase
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  Queue ();
  virtual ~Queue ();

  /**
   * Place an item into the Queue (each subclass defines the position)
   * \param item item to enqueue
   * \return True if the operation was successful; false otherwise
   */
  virtual bool Enqueue (Ptr<Item> item) = 0;

  /**
   * Remove an item from the Queue (each subclass defines the position),
   * counting it and tracing it as dequeued
   * \return 0 if the operation was not successful; the item otherwise.
   */
  virtual Ptr<Item> Dequeue (void) = 0;

  /**
   * Remove an item from the Queue (each subclass defines the position),
   * counting it and tracing it as both dequeued and dropped
   * \return 0 if the operation was not successful; the item otherwise.
   */
  virtual Ptr<Item>  Remove (void) = 0;

  /**
   * Get a copy of an item in the queue (each subclass defines the position)
   * without removing it
   * \return 0 if the operation was not successful; the item otherwise.
   */
  virtual Ptr<const Item> Peek (void) const = 0;

  /**
   * Flush the queue by calling Remove() on each item enqueued.  Note that
   * this operation will cause dequeue and drop counts to be incremented and
   * traces to be triggered for each Remove() action.
   */
  void Flush (void);

  /// Define ItemType as the type of the stored elements
  typedef Item ItemType;

protected:
  /// Const iterator.
  typedef typename Container::const_iterator ConstIterator;
  /// Iterator.
  typedef typename Container::iterator Iterator;

  /**
   * \brief Get a const iterator which refers to the first item in the queue.
   *
   * Subclasses can browse the items in the queue by using a const iterator
   *
   * \code
   *   for (auto i = begin (); i != end (); ++i)
   *     {
   *       (*i)->method ();  // some const method of the Item class
   *     }
   * \endcode
   *
   * \returns a const iterator which refers to the first item in the queue.
   */
  ConstIterator begin (void) const;

  /**
   * \brief Get an iterator which refers to the first item in the queue.
   *
   * Subclasses can browse the items in the queue by using an iterator
   *
   * \code
   *   for (auto i = begin (); i != end (); ++i)
   *     {
   *       (*i)->method ();  // some method of the Item class
   *     }
   * \endcode
   *
   * \returns an iterator which refers to the first item in the queue.
   */
  Iterator begin (void);

  /**
   * \brief Get a const iterator which indicates past-the-last item in the queue.
   *
   * Subclasses can browse the items in the queue by using a const iterator
   *
   * \code
   *   for (auto i = begin (); i != end (); ++i)
   *     {
   *       (*i)->method ();  // some const method of the Item class
   *     }
   * \endcode
   *
   * \returns a const iterator which indicates past-the-last item in the queue.
   */
  ConstIterator end (void) const;

  /**
   * \brief Get an iterator which indicates past-the-last item in the queue.
   *
   * Subclasses can browse the items in the queue by using an iterator
   *
   * \code
   *   for (auto i = begin (); i != end (); ++i)
   *     {
   *       (*i)->method ();  // some method of the Item class
   *     }
   * \endcode
   *
   * \returns an iterator which indicates past-the-last item in the queue.
   */
  Iterator end (void);

  /**
   * Push an item in the queue
   * \param pos the position before which the item will be inserted
   * \param item the item to enqueue
   * \return true if success, false if the packet has been dropped.
   */
  bool DoEnqueue (ConstIterator pos, Ptr<Item> item);

  /**
   * Push an item in the queue
   * \param pos the position before which the item will be inserted
   * \param item the item to enqueue
   * \param[out] ret an iterator pointing to the inserted value
   * \return true if success, false if the packet has been dropped.
   */
  bool DoEnqueue (ConstIterator pos, Ptr<Item> item, Iterator& ret);

  /**
   * Pull the item to dequeue from the queue
   * \param pos the position of the item to dequeue
   * \return the item.
   */
  Ptr<Item> DoDequeue (ConstIterator pos);

  /**
   * Pull the item to drop from the queue
   * \param pos the position of the item to remove
   * \return the item.
   */
  Ptr<Item> DoRemove (ConstIterator pos);

  /**
   * Peek the front item in the queue
   * \param pos the position of the item to peek
   * \return the item.
   */
  Ptr<const Item> DoPeek (ConstIterator pos) const;

  /**
   * \brief Drop a packet before enqueue
   * \param item item that was dropped
   *
   * This method is called by the base class when a packet is dropped because
   * the queue is full and by the subclasses to notify parent (this class) that
   * a packet has been dropped for other reasons before being enqueued.
   */
  void DropBeforeEnqueue (Ptr<Item> item);

  /**
   * \brief Drop a packet after dequeue
   * \param item item that was dropped
   *
   * This method is called by the base class when a Remove operation is requested
   * and by the subclasses to notify parent (this class) that a packet has been
   * dropped for other reasons after being dequeued.
   */
  void DropAfterDequeue (Ptr<Item> item);

  /** \copydoc ns3::Object::DoDispose */
  void DoDispose (void) override;

private:
  Container m_packets;                      //!< the items in the queue
  NS_LOG_TEMPLATE_DECLARE;                  //!< the log component

  /// Traced callback: fired when a packet is enqueued
  TracedCallback<Ptr<const Item> > m_traceEnqueue;
  /// Traced callback: fired when a packet is dequeued
  TracedCallback<Ptr<const Item> > m_traceDequeue;
  /// Traced callback: fired when a packet is dropped
  TracedCallback<Ptr<const Item> > m_traceDrop;
  /// Traced callback: fired when a packet is dropped before enqueue
  TracedCallback<Ptr<const Item> > m_traceDropBeforeEnqueue;
  /// Traced callback: fired when a packet is dropped after dequeue
  TracedCallback<Ptr<const Item> > m_traceDropAfterDequeue;
};


/**
 * Implementation of the templates declared above.
 */

template <typename Item, typename Container>
TypeId
Queue<Item, Container>::GetTypeId (void)
{
  std::string name = GetTemplateClassName<Queue<Item, Container>> ();
  auto startPos = name.find ('<') + 1;
  auto endPos = name.find_first_of (",>", startPos);
  std::string tcbName = "ns3::" + name.substr (startPos, endPos - startPos) + "::TracedCallback";

  static TypeId tid = TypeId (name)
    .SetParent<QueueBase> ()
    .SetGroupName ("Network")
    .AddTraceSource ("Enqueue", "Enqueue a packet in the queue.",
                     MakeTraceSourceAccessor (&Queue<Item, Container>::m_traceEnqueue),
                     tcbName)
    .AddTraceSource ("Dequeue", "Dequeue a packet from the queue.",
                     MakeTraceSourceAccessor (&Queue<Item, Container>::m_traceDequeue),
                     tcbName)
    .AddTraceSource ("Drop", "Drop a packet (for whatever reason).",
                     MakeTraceSourceAccessor (&Queue<Item, Container>::m_traceDrop),
                     tcbName)
    .AddTraceSource ("DropBeforeEnqueue", "Drop a packet before enqueue.",
                     MakeTraceSourceAccessor (&Queue<Item, Container>::m_traceDropBeforeEnqueue),
                     tcbName)
    .AddTraceSource ("DropAfterDequeue", "Drop a packet after dequeue.",
                     MakeTraceSourceAccessor (&Queue<Item, Container>::m_traceDropAfterDequeue),
                     tcbName)
  ;
  return tid;
}

template <typename Item, typename Container>
Queue<Item, Container>::Queue ()
  : NS_LOG_TEMPLATE_DEFINE ("Queue")
{
}

template <typename Item, typename Container>
Queue<Item, Container>::~Queue ()
{
}

template <typename Item, typename Container>
bool
Queue<Item, Container>::DoEnqueue (ConstIterator pos, Ptr<Item> item)
{
  Iterator ret;
  return DoEnqueue (pos, item, ret);
}

template <typename Item, typename Container>
bool
Queue<Item, Container>::DoEnqueue (ConstIterator pos, Ptr<Item> item, Iterator& ret)
{
  NS_LOG_FUNCTION (this << item);

  if (GetCurrentSize () + item > GetMaxSize ())
    {
      NS_LOG_LOGIC ("Queue full -- dropping pkt");
      DropBeforeEnqueue (item);
      return false;
    }

  ret = m_packets.insert (pos, item);

  uint32_t size = item->GetSize ();
  m_nBytes += size;
  m_nTotalReceivedBytes += size;

  m_nPackets++;
  m_nTotalReceivedPackets++;

  NS_LOG_LOGIC ("m_traceEnqueue (p)");
  m_traceEnqueue (item);

  return true;
}

template <typename Item, typename Container>
Ptr<Item>
Queue<Item, Container>::DoDequeue (ConstIterator pos)
{
  NS_LOG_FUNCTION (this);

  if (m_nPackets.Get () == 0)
    {
      NS_LOG_LOGIC ("Queue empty");
      return 0;
    }

  Ptr<Item> item = *pos;
  m_packets.erase (pos);

  if (item != 0)
    {
      NS_ASSERT (m_nBytes.Get () >= item->GetSize ());
      NS_ASSERT (m_nPackets.Get () > 0);

      m_nBytes -= item->GetSize ();
      m_nPackets--;

      NS_LOG_LOGIC ("m_traceDequeue (p)");
      m_traceDequeue (item);
    }
  return item;
}

template <typename Item, typename Container>
Ptr<Item>
Queue<Item, Container>::DoRemove (ConstIterator pos)
{
  NS_LOG_FUNCTION (this);

  if (m_nPackets.Get () == 0)
    {
      NS_LOG_LOGIC ("Queue empty");
      return 0;
    }

  Ptr<Item> item = *pos;
  m_packets.erase (pos);

  if (item != 0)
    {
      NS_ASSERT (m_nBytes.Get () >= item->GetSize ());
      NS_ASSERT (m_nPackets.Get () > 0);

      m_nBytes -= item->GetSize ();
      m_nPackets--;

      // packets are first dequeued and then dropped
      NS_LOG_LOGIC ("m_traceDequeue (p)");
      m_traceDequeue (item);

      DropAfterDequeue (item);
    }
  return item;
}

template <typename Item, typename Container>
void
Queue<Item, Container>::Flush (void)
{
  NS_LOG_FUNCTION (this);
  while (!IsEmpty ())
    {
      Remove ();
    }
}

template <typename Item, typename Container>
void
Queue<Item, Container>::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_packets.clear ();
  Object::DoDispose ();
}

template <typename Item, typename Container>
Ptr<const Item>
Queue<Item, Container>::DoPeek (ConstIterator pos) const
{
  NS_LOG_FUNCTION (this);

  if (m_nPackets.Get () == 0)
    {
      NS_LOG_LOGIC ("Queue empty");
      return 0;
    }

  return *pos;
}

template <typename Item, typename Container>
typename Queue<Item, Container>::ConstIterator Queue<Item, Container>::begin (void) const
{
  return m_packets.cbegin ();
}

template <typename Item, typename Container>
typename Queue<Item, Container>::Iterator Queue<Item, Container>::begin (void)
{
  return m_packets.begin ();
}

template <typename Item, typename Container>
typename Queue<Item, Container>::ConstIterator Queue<Item, Container>::end (void) const
{
  return m_packets.cend ();
}

template <typename Item, typename Container>
typename Queue<Item, Container>::Iterator Queue<Item, Container>::end (void)
{
  return m_packets.end ();
}

template <typename Item, typename Container>
void
Queue<Item, Container>::DropBeforeEnqueue (Ptr<Item> item)
{
  NS_LOG_FUNCTION (this << item);

  m_nTotalDroppedPackets++;
  m_nTotalDroppedPacketsBeforeEnqueue++;
  m_nTotalDroppedBytes += item->GetSize ();
  m_nTotalDroppedBytesBeforeEnqueue += item->GetSize ();

  NS_LOG_LOGIC ("m_traceDropBeforeEnqueue (p)");
  m_traceDrop (item);
  m_traceDropBeforeEnqueue (item);
}

template <typename Item, typename Container>
void
Queue<Item, Container>::DropAfterDequeue (Ptr<Item> item)
{
  NS_LOG_FUNCTION (this << item);

  m_nTotalDroppedPackets++;
  m_nTotalDroppedPacketsAfterDequeue++;
  m_nTotalDroppedBytes += item->GetSize ();
  m_nTotalDroppedBytesAfterDequeue += item->GetSize ();

  NS_LOG_LOGIC ("m_traceDropAfterDequeue (p)");
  m_traceDrop (item);
  m_traceDropAfterDequeue (item);
}

// The following explicit template instantiation declarations prevent all the
// translation units including this header file to implicitly instantiate the
// Queue<Packet> class and the Queue<QueueDiscItem> class. The unique instances
// of these classes are explicitly created through the macros
// NS_OBJECT_TEMPLATE_CLASS_DEFINE (Queue,Packet) and
// NS_OBJECT_TEMPLATE_CLASS_DEFINE (Queue,QueueDiscItem), which are included in queue.cc
extern template class Queue<Packet>;
extern template class Queue<QueueDiscItem>;

} // namespace ns3

#endif /* QUEUE_H */
