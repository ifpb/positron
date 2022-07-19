/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 Georgia Tech Research Corporation, INRIA
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
 * Authors: George F. Riley<riley@ece.gatech.edu>
 *          Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include <cmath>
#include "node.h"
#include "node-list.h"
#include "net-device.h"
#include "application.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/object-vector.h"
#include "ns3/uinteger.h"
#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/global-value.h"
#include "ns3/boolean.h"
#include "ns3/string.h"
#include "ns3/double.h"
#include "ns3/object.h"

#include "ns3/control-layer-helper.h"

namespace ns3
{

  NS_LOG_COMPONENT_DEFINE("Node");

  NS_OBJECT_ENSURE_REGISTERED(Node);

  /**
 * \relates Node
 * \anchor GlobalValueChecksumEnabled
 * \brief A global switch to enable all checksums for all protocols.
 */
  static GlobalValue g_checksumEnabled = GlobalValue("ChecksumEnabled",
                                                     "A global switch to enable all checksums for all protocols",
                                                     BooleanValue(false),
                                                     MakeBooleanChecker());

  TypeId
  Node::GetTypeId(void)
  {
    static TypeId tid = TypeId("ns3::Node")
                            .SetParent<Object>()
                            .SetGroupName("Network")
                            .AddConstructor<Node>()
                            .AddAttribute("DeviceList", "The list of devices associated to this Node.",
                                          ObjectVectorValue(),
                                          MakeObjectVectorAccessor(&Node::m_devices),
                                          MakeObjectVectorChecker<NetDevice>())
                            .AddAttribute("ApplicationList", "The list of applications associated to this Node.",
                                          ObjectVectorValue(),
                                          MakeObjectVectorAccessor(&Node::m_applications),
                                          MakeObjectVectorChecker<Application>())
                            .AddAttribute("Id", "The id (unique integer) of this Node.",
                                          TypeId::ATTR_GET, // allow only getting it.
                                          UintegerValue(0),
                                          MakeUintegerAccessor(&Node::m_id),
                                          MakeUintegerChecker<uint32_t>())
                            .AddAttribute("SystemId", "The systemId of this node: a unique integer used for parallel simulations.",
                                          TypeId::ATTR_GET | TypeId::ATTR_SET,
                                          UintegerValue(0),
                                          MakeUintegerAccessor(&Node::m_sid),
                                          MakeUintegerChecker<uint32_t>())
                            .AddAttribute("Power", "Carga atual da bateria. (%)",
                                          DoubleValue(100.0),
                                          MakeDoubleAccessor(&Node::power),
                                          MakeDoubleChecker<double>(0.0, 100.0))
                            .AddAttribute("InitialConsumption", "Consumo inicial da bateria. (%/s)",
                                          DoubleValue(0),
                                          MakeDoubleAccessor(&Node::initialConsumption),
                                          MakeDoubleChecker<double>(0.0, 100.0))
                            .AddAttribute("CurrentConsumption", "Consumo autal da bateria. (%/s)",
                                          DoubleValue(0),
                                          MakeDoubleAccessor(&Node::currentConsumption),
                                          MakeDoubleChecker<double>())
                            .AddAttribute("CPU", "Informações sobre o processador. (Gb)",
                                          DoubleValue(3.0),
                                          MakeDoubleAccessor(&Node::cpu),
                                          MakeDoubleChecker<double>())
                            .AddAttribute("Memory", "Informações sobre a memória. (Gb)",
                                          DoubleValue(8.0),
                                          MakeDoubleAccessor(&Node::memory),
                                          MakeDoubleChecker<double>())
                            .AddAttribute("Transmission", "Capacidade de transmissão. (Mb/s)",
                                          DoubleValue(100.0),
                                          MakeDoubleAccessor(&Node::transmission),
                                          MakeDoubleChecker<double>())
                            .AddAttribute("Storage", "Informação sobre armazenamento. (Gb)",
                                          DoubleValue(5.0),
                                          MakeDoubleAccessor(&Node::storage),
                                          MakeDoubleChecker<double>());
    return tid;
  }

  Node::Node()
      : m_id(0),
        m_sid(0)
  {
    NS_LOG_FUNCTION(this);
    Construct();
  }

  Node::Node(uint32_t sid)
      : m_id(0),
        m_sid(sid)
  {
    NS_LOG_FUNCTION(this << sid);
    Construct();
  }

  void
  Node::Construct(void)
  {
    NS_LOG_FUNCTION(this);
    m_id = NodeList::Add(this);
    lastSet = Seconds(0); // Tempo da ultima medicao do CurrentPower.
    Simulator::Schedule(Seconds(0), &Node::UpdateEvent, this); // Inicializa o evento de fim de bateria
  }

  Node::~Node()
  {
    NS_LOG_FUNCTION(this);
  }

  uint32_t
  Node::GetId(void) const
  {
    NS_LOG_FUNCTION(this);
    return m_id;
  }

  // -----------------------------------------------

  double
  Node::GetPower(void)
  {
    NS_LOG_FUNCTION(this);
    AttPower();
    return power;
  }

  double
  Node::GetInitialConsumption(void) const
  {
    NS_LOG_FUNCTION(this);
    return initialConsumption;
  }

  double
  Node::GetCurrentConsumption(void) const
  {
    NS_LOG_FUNCTION(this);
    return currentConsumption;
  }

  double
  Node::GetCpu(void) const
  {
    NS_LOG_FUNCTION(this);
    return cpu;
  }

  double
  Node::GetMemory(void) const
  {
    NS_LOG_FUNCTION(this);
    return memory;
  }

  double
  Node::GetTransmission(void) const
  {
    NS_LOG_FUNCTION(this);
    return transmission;
  }

  double
  Node::GetStorage(void) const
  {
    NS_LOG_FUNCTION(this);
    return storage;
  }

  // Cria um evento que avisa ao controller quando o nó está sem bateria
  void
  Node::UpdateEvent(void)
  {
    Simulator::Cancel(outOfPowerEvent);
    if (this->GetCurrentConsumption () > 0.0)
    {
      // std::cout <<"Node: " << this->GetId() << 
      // " - Agora: " << Simulator::Now() << 
      // " - Escalonamento para: " << Simulator::Now() + Seconds(power / currentConsumption) << 
      // " - Power: " << this->GetPower() << 
      // " - CurrentConsumption: " << this->GetCurrentConsumption() << 
      // std::endl;
      outOfPowerEvent = Simulator::Schedule(Seconds(power / currentConsumption), &Node::OutOfPower, this);
    }
  }

  // Método temporário para o término da bateria (será trocado pelo método do controller)
  void
  Node::OutOfPower(void)
  {
    NS_LOG_FUNCTION(this);
    double currentTime = ns3::Simulator::Now().GetSeconds();
    std::cout << "At time " << std::to_string(currentTime).substr(0, std::to_string(currentTime).find(".") + 2) << "s: ";
    std::cout << "Node " << m_id << " out of power." << std::endl;

    std::ostringstream oss;
    oss << "\"name\":\"" << "OutOfPower" << "\"\"time\":\"" << Simulator::Now().GetSeconds() << "\"\"sourceID\":\"" << m_id << "\"\"destinationID\":\"" << 0 << "\"";
    std::string data = oss.str();

    //criando pacote escrevendo conteúdo
    Ptr<Packet> p = Create<Packet>(reinterpret_cast<const uint8_t *>(data.c_str()), data.size());

     //enviando pacote
    TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
    Ptr<Socket> m_socket = Socket::CreateSocket(this, tid);
    Inet6SocketAddress local = Inet6SocketAddress(Ipv6Address::GetAny(), 9999);
    if (m_socket->Bind(local) == -1)
    {
      NS_FATAL_ERROR("Failed to bind socket");
    }
    Inet6SocketAddress remote = Inet6SocketAddress(Ipv6Address("2001:1::ff:fe00:1"), 9);
    m_socket->Connect(remote);
    m_socket->Send(p);
    m_socket->Close();

    // Simulator::Schedule(Seconds(600) , &Node::Recharge, this);
  }

  void
  Node::Recharge(void){
    NS_LOG_FUNCTION(this);
    double currentTime = ns3::Simulator::Now().GetSeconds();
    std::cout << "At time " << std::to_string(currentTime).substr(0, std::to_string(currentTime).find(".") + 2) << "s: ";
    std::cout << "Node " << m_id << " recharge for 100\% power." << std::endl;
    // std::cout << "At time " << Simulator::Now().GetSeconds() << "s: ";
    // std::cout << "Node " << m_id << " recharge for 100\% power." << std::endl;

    std::ostringstream oss;
    oss << "\"name\":\"" << "Recharge" << "\"\"time\":\"" << Simulator::Now().GetSeconds() << "\"\"sourceID\":\"" << m_id << "\"\"destinationID\":\"" << 0 << "\"";
    std::string data = oss.str();

    //criando pacote escrevendo conteúdo
    Ptr<Packet> p = Create<Packet>(reinterpret_cast<const uint8_t *>(data.c_str()), data.size());

     //enviando pacote
    TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
    Ptr<Socket> m_socket_recharge = Socket::CreateSocket(this, tid);
    Inet6SocketAddress local_recharge = Inet6SocketAddress(Ipv6Address::GetAny(), 9999);
    if (m_socket_recharge->Bind(local_recharge) == -1)
    {
      NS_FATAL_ERROR("Failed to bind socket");
    }
    Inet6SocketAddress remote_recharge = Inet6SocketAddress(Ipv6Address("2001:1::ff:fe00:1"), 9);
    m_socket_recharge->Connect(remote_recharge);
    m_socket_recharge->Send(p);
    m_socket_recharge->Close();

    power = 100;
  }

  // Atualiza a quantidade de carga da bateria
  void
  Node::AttPower(void)
  {
    NS_LOG_FUNCTION(this);
    double toSubtract = (Simulator::Now() - lastSet).GetSeconds() * currentConsumption;
    power -= toSubtract > power ? power : toSubtract;
    lastSet = Simulator::Now();
  }

  // Atualiza a bateria, modifica o consumo e atualiza quando a bateria irá acabar
  // Para entrada de novas aplicações
  void
  Node::AddApplication(void)
  {
    NS_LOG_FUNCTION(this);
    double currentTime = ns3::Simulator::Now().GetSeconds();
    std::cout << "At time " << std::to_string(currentTime).substr(0, std::to_string(currentTime).find(".") + 2) << "s: ";
    std::cout << "AddApplication called at node " << this->GetId() << std::endl;
    AttPower();
    currentConsumption += initialConsumption;
    UpdateEvent();
  }

  // Atualiza a bateria, modifica o consumo e atualiza quando a bateria irá acabar
  // Para finalização de aplicações
  void
  Node::RemoveApplication(void)
  {
    if (this->GetPower () > 0.0) {
      NS_LOG_FUNCTION(this);
      std::cout << "At time " << Simulator::Now().GetSeconds() << "s: ";
      std::cout << "RemoveApplication called at node " << this->GetId() << std::endl;
      AttPower();
      currentConsumption -= initialConsumption;
      UpdateEvent();
    }
    else {
      currentConsumption = initialConsumption;
    }
  }

  // -----------------------------------------------

  Time
  Node::GetLocalTime(void) const
  {
    NS_LOG_FUNCTION(this);
    return Simulator::Now();
  }

  uint32_t
  Node::GetSystemId(void) const
  {
    NS_LOG_FUNCTION(this);
    return m_sid;
  }

  uint32_t
  Node::AddDevice(Ptr<NetDevice> device)
  {
    NS_LOG_FUNCTION(this << device);
    uint32_t index = m_devices.size();
    m_devices.push_back(device);
    device->SetNode(this);
    device->SetIfIndex(index);
    device->SetReceiveCallback(MakeCallback(&Node::NonPromiscReceiveFromDevice, this));
    Simulator::ScheduleWithContext(GetId(), Seconds(0.0),
                                   &NetDevice::Initialize, device);
    NotifyDeviceAdded(device);
    return index;
  }
  Ptr<NetDevice>
  Node::GetDevice(uint32_t index) const
  {
    NS_LOG_FUNCTION(this << index);
    NS_ASSERT_MSG(index < m_devices.size(), "Device index " << index << " is out of range (only have " << m_devices.size() << " devices).");
    return m_devices[index];
  }
  uint32_t
  Node::GetNDevices(void) const
  {
    NS_LOG_FUNCTION(this);
    return m_devices.size();
  }

  uint32_t
  Node::AddApplication(Ptr<Application> application)
  {
    NS_LOG_FUNCTION(this << application);
    uint32_t index = m_applications.size();
    m_applications.push_back(application);
    application->SetNode(this);
    Simulator::ScheduleWithContext(GetId(), Seconds(0.0),
                                   &Application::Initialize, application);
    return index;
  }
  Ptr<Application>
  Node::GetApplication(uint32_t index) const
  {
    NS_LOG_FUNCTION(this << index);
    NS_ASSERT_MSG(index < m_applications.size(), "Application index " << index << " is out of range (only have " << m_applications.size() << " applications).");
    return m_applications[index];
  }
  uint32_t
  Node::GetNApplications(void) const
  {
    NS_LOG_FUNCTION(this);
    return m_applications.size();
  }

  void
  Node::DoDispose()
  {
    NS_LOG_FUNCTION(this);
    m_deviceAdditionListeners.clear();
    m_handlers.clear();
    for (std::vector<Ptr<NetDevice>>::iterator i = m_devices.begin();
         i != m_devices.end(); i++)
    {
      Ptr<NetDevice> device = *i;
      device->Dispose();
      *i = 0;
    }
    m_devices.clear();
    for (std::vector<Ptr<Application>>::iterator i = m_applications.begin();
         i != m_applications.end(); i++)
    {
      Ptr<Application> application = *i;
      application->Dispose();
      *i = 0;
    }
    m_applications.clear();
    Object::DoDispose();
  }
  void
  Node::DoInitialize(void)
  {
    NS_LOG_FUNCTION(this);
    for (std::vector<Ptr<NetDevice>>::iterator i = m_devices.begin();
         i != m_devices.end(); i++)
    {
      Ptr<NetDevice> device = *i;
      device->Initialize();
    }
    for (std::vector<Ptr<Application>>::iterator i = m_applications.begin();
         i != m_applications.end(); i++)
    {
      Ptr<Application> application = *i;
      application->Initialize();
    }

    Object::DoInitialize();
  }

  void
  Node::RegisterProtocolHandler(ProtocolHandler handler,
                                uint16_t protocolType,
                                Ptr<NetDevice> device,
                                bool promiscuous)
  {
    NS_LOG_FUNCTION(this << &handler << protocolType << device << promiscuous);
    struct Node::ProtocolHandlerEntry entry;
    entry.handler = handler;
    entry.protocol = protocolType;
    entry.device = device;
    entry.promiscuous = promiscuous;

    // On demand enable promiscuous mode in netdevices
    if (promiscuous)
    {
      if (device == 0)
      {
        for (std::vector<Ptr<NetDevice>>::iterator i = m_devices.begin();
             i != m_devices.end(); i++)
        {
          Ptr<NetDevice> dev = *i;
          dev->SetPromiscReceiveCallback(MakeCallback(&Node::PromiscReceiveFromDevice, this));
        }
      }
      else
      {
        device->SetPromiscReceiveCallback(MakeCallback(&Node::PromiscReceiveFromDevice, this));
      }
    }

    m_handlers.push_back(entry);
  }

  void
  Node::UnregisterProtocolHandler(ProtocolHandler handler)
  {
    NS_LOG_FUNCTION(this << &handler);
    for (ProtocolHandlerList::iterator i = m_handlers.begin();
         i != m_handlers.end(); i++)
    {
      if (i->handler.IsEqual(handler))
      {
        m_handlers.erase(i);
        break;
      }
    }
  }

  bool
  Node::ChecksumEnabled(void)
  {
    NS_LOG_FUNCTION_NOARGS();
    BooleanValue val;
    g_checksumEnabled.GetValue(val);
    return val.Get();
  }

  bool
  Node::PromiscReceiveFromDevice(Ptr<NetDevice> device, Ptr<const Packet> packet, uint16_t protocol,
                                 const Address &from, const Address &to, NetDevice::PacketType packetType)
  {
    NS_LOG_FUNCTION(this << device << packet << protocol << &from << &to << packetType);
    return ReceiveFromDevice(device, packet, protocol, from, to, packetType, true);
  }

  bool
  Node::NonPromiscReceiveFromDevice(Ptr<NetDevice> device, Ptr<const Packet> packet, uint16_t protocol,
                                    const Address &from)
  {
    NS_LOG_FUNCTION(this << device << packet << protocol << &from);
    return ReceiveFromDevice(device, packet, protocol, from, device->GetAddress(), NetDevice::PacketType(0), false);
  }

  bool
  Node::ReceiveFromDevice(Ptr<NetDevice> device, Ptr<const Packet> packet, uint16_t protocol,
                          const Address &from, const Address &to, NetDevice::PacketType packetType, bool promiscuous)
  {
    NS_LOG_FUNCTION(this << device << packet << protocol << &from << &to << packetType << promiscuous);
    NS_ASSERT_MSG(Simulator::GetContext() == GetId(), "Received packet with erroneous context ; "
                                                          << "make sure the channels in use are correctly updating events context "
                                                          << "when transferring events from one node to another.");
    NS_LOG_DEBUG("Node " << GetId() << " ReceiveFromDevice:  dev "
                         << device->GetIfIndex() << " (type=" << device->GetInstanceTypeId().GetName()
                         << ") Packet UID " << packet->GetUid());
    bool found = false;

    for (ProtocolHandlerList::iterator i = m_handlers.begin();
         i != m_handlers.end(); i++)
    {
      if (i->device == 0 ||
          (i->device != 0 && i->device == device))
      {
        if (i->protocol == 0 ||
            i->protocol == protocol)
        {
          if (promiscuous == i->promiscuous)
          {
            i->handler(device, packet, protocol, from, to, packetType);
            found = true;
          }
        }
      }
    }
    return found;
  }
  void
  Node::RegisterDeviceAdditionListener(DeviceAdditionListener listener)
  {
    NS_LOG_FUNCTION(this << &listener);
    m_deviceAdditionListeners.push_back(listener);
    // and, then, notify the new listener about all existing devices.
    for (std::vector<Ptr<NetDevice>>::const_iterator i = m_devices.begin();
         i != m_devices.end(); ++i)
    {
      listener(*i);
    }
  }
  void
  Node::UnregisterDeviceAdditionListener(DeviceAdditionListener listener)
  {
    NS_LOG_FUNCTION(this << &listener);
    for (DeviceAdditionListenerList::iterator i = m_deviceAdditionListeners.begin();
         i != m_deviceAdditionListeners.end(); i++)
    {
      if ((*i).IsEqual(listener))
      {
        m_deviceAdditionListeners.erase(i);
        break;
      }
    }
  }

  void
  Node::NotifyDeviceAdded(Ptr<NetDevice> device)
  {
    NS_LOG_FUNCTION(this << device);
    for (DeviceAdditionListenerList::iterator i = m_deviceAdditionListeners.begin();
         i != m_deviceAdditionListeners.end(); i++)
    {
      (*i)(device);
    }
  }

} // namespace ns3