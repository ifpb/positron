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

<<<<<<< HEAD
<<<<<<< HEAD
#include <cmath>
=======
>>>>>>> b6a5ee815 (Run utils/trim-trailing-whitespace.py on codebase)
=======
#include <cmath>
>>>>>>> 27288fce1 (POSITRON: initial commit)
#include "node.h"
#include "node-list.h"
#include "net-device.h"
#include "application.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/object-vector.h"
#include "ns3/uinteger.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 27288fce1 (POSITRON: initial commit)
#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/global-value.h"
#include "ns3/boolean.h"
#include "ns3/string.h"
#include "ns3/double.h"
#include "ns3/object.h"

#include "ns3/control-layer-helper.h"
<<<<<<< HEAD

namespace ns3
{

  NS_LOG_COMPONENT_DEFINE("Node");
=======
=======
>>>>>>> 27288fce1 (POSITRON: initial commit)

namespace ns3
{

  NS_LOG_COMPONENT_DEFINE("Node");

<<<<<<< HEAD
NS_OBJECT_ENSURE_REGISTERED(Node);
>>>>>>> e25ff966f (Apply clang-format to codebase)

  NS_OBJECT_ENSURE_REGISTERED(Node);

=======
  NS_OBJECT_ENSURE_REGISTERED(Node);

>>>>>>> 27288fce1 (POSITRON: initial commit)
  /**
 * \relates Node
 * \anchor GlobalValueChecksumEnabled
 * \brief A global switch to enable all checksums for all protocols.
 */
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 27288fce1 (POSITRON: initial commit)
  static GlobalValue g_checksumEnabled = GlobalValue("ChecksumEnabled",
                                                     "A global switch to enable all checksums for all protocols",
                                                     BooleanValue(false),
                                                     MakeBooleanChecker());
<<<<<<< HEAD
=======
static GlobalValue g_checksumEnabled =
    GlobalValue("ChecksumEnabled",
                "A global switch to enable all checksums for all protocols",
                BooleanValue(false),
                MakeBooleanChecker());
>>>>>>> e25ff966f (Apply clang-format to codebase)

<<<<<<< HEAD
=======

>>>>>>> 27288fce1 (POSITRON: initial commit)
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
<<<<<<< HEAD
    return tid;
  }
=======
TypeId
Node::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::Node")
            .SetParent<Object>()
            .SetGroupName("Network")
            .AddConstructor<Node>()
            .AddAttribute("DeviceList",
                          "The list of devices associated to this Node.",
                          ObjectVectorValue(),
                          MakeObjectVectorAccessor(&Node::m_devices),
                          MakeObjectVectorChecker<NetDevice>())
            .AddAttribute("ApplicationList",
                          "The list of applications associated to this Node.",
                          ObjectVectorValue(),
                          MakeObjectVectorAccessor(&Node::m_applications),
                          MakeObjectVectorChecker<Application>())
            .AddAttribute("Id",
                          "The id (unique integer) of this Node.",
                          TypeId::ATTR_GET, // allow only getting it.
                          UintegerValue(0),
                          MakeUintegerAccessor(&Node::m_id),
                          MakeUintegerChecker<uint32_t>())
            .AddAttribute(
                "SystemId",
                "The systemId of this node: a unique integer used for parallel simulations.",
                TypeId::ATTR_GET | TypeId::ATTR_SET,
                UintegerValue(0),
                MakeUintegerAccessor(&Node::m_sid),
                MakeUintegerChecker<uint32_t>());
    return tid;
}
>>>>>>> b6a5ee815 (Run utils/trim-trailing-whitespace.py on codebase)

<<<<<<< HEAD
=======
    return tid;
  }

>>>>>>> 27288fce1 (POSITRON: initial commit)
  Node::Node()
      : m_id(0),
        m_sid(0)
  {
<<<<<<< HEAD
    NS_LOG_FUNCTION(this);
    Construct();
  }
=======
Node::Node()
    : m_id(0),
      m_sid(0)
{
    NS_LOG_FUNCTION(this);
    Construct();
}
>>>>>>> e25ff966f (Apply clang-format to codebase)

<<<<<<< HEAD
=======
    NS_LOG_FUNCTION(this);
    Construct();
  }

>>>>>>> 27288fce1 (POSITRON: initial commit)
  Node::Node(uint32_t sid)
      : m_id(0),
        m_sid(sid)
  {
<<<<<<< HEAD
    NS_LOG_FUNCTION(this << sid);
    Construct();
  }
=======
Node::Node(uint32_t sid)
    : m_id(0),
      m_sid(sid)
{
    NS_LOG_FUNCTION(this << sid);
    Construct();
}
>>>>>>> b6a5ee815 (Run utils/trim-trailing-whitespace.py on codebase)

<<<<<<< HEAD
  void
  Node::Construct(void)
  {
    NS_LOG_FUNCTION(this);
    m_id = NodeList::Add(this);
    lastSet = Seconds(0); // Tempo da ultima medicao do CurrentPower.
    Simulator::Schedule(Seconds(0), &Node::UpdateEvent, this); // Inicializa o evento de fim de bateria
  }
=======
void
Node::Construct()
{
    NS_LOG_FUNCTION(this);
    m_id = NodeList::Add(this);
}
>>>>>>> 6bb638356 (Fix clang-tidy modernize-redundant-void-arg warnings)

<<<<<<< HEAD
  Node::~Node()
  {
    NS_LOG_FUNCTION(this);
  }
=======
Node::~Node()
{
    NS_LOG_FUNCTION(this);
}
>>>>>>> e25ff966f (Apply clang-format to codebase)

<<<<<<< HEAD
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
=======
uint32_t
Node::GetId() const
{
=======
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
>>>>>>> 27288fce1 (POSITRON: initial commit)
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

    Simulator::Schedule(Seconds(600) , &Node::Recharge, this);
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
<<<<<<< HEAD
}
>>>>>>> 6bb638356 (Fix clang-tidy modernize-redundant-void-arg warnings)

<<<<<<< HEAD
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

    Simulator::Schedule(Seconds(600) , &Node::Recharge, this);
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
=======
uint32_t
Node::AddDevice(Ptr<NetDevice> device)
{
=======
  }

  uint32_t
  Node::AddDevice(Ptr<NetDevice> device)
  {
>>>>>>> 27288fce1 (POSITRON: initial commit)
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

<<<<<<< HEAD
void
Node::DoDispose()
{
<<<<<<< HEAD
  NS_LOG_FUNCTION (this);
  m_deviceAdditionListeners.clear ();
  m_handlers.clear ();
  for (std::vector<Ptr<NetDevice> >::iterator i = m_devices.begin ();
       i != m_devices.end (); i++)
>>>>>>> b6a5ee815 (Run utils/trim-trailing-whitespace.py on codebase)
    {
      Ptr<NetDevice> device = *i;
<<<<<<< HEAD
      device->Dispose();
      *i = 0;
=======
      device->Dispose ();
      *i = nullptr;
>>>>>>> 4af0fe4b8 (Fix clang-tidy modernize-use-nullptr warnings)
    }
    m_devices.clear();
    for (std::vector<Ptr<Application>>::iterator i = m_applications.begin();
         i != m_applications.end(); i++)
    {
      Ptr<Application> application = *i;
<<<<<<< HEAD
      application->Dispose();
      *i = 0;
=======
      application->Dispose ();
      *i = nullptr;
>>>>>>> 4af0fe4b8 (Fix clang-tidy modernize-use-nullptr warnings)
    }
<<<<<<< HEAD
    m_applications.clear();
    Object::DoDispose();
  }
  void
  Node::DoInitialize(void)
  {
    NS_LOG_FUNCTION(this);
    for (std::vector<Ptr<NetDevice>>::iterator i = m_devices.begin();
         i != m_devices.end(); i++)
=======
  m_applications.clear ();
  Object::DoDispose ();
}
void
Node::DoInitialize ()
{
  NS_LOG_FUNCTION (this);
  for (std::vector<Ptr<NetDevice> >::iterator i = m_devices.begin ();
       i != m_devices.end (); i++)
>>>>>>> b6a5ee815 (Run utils/trim-trailing-whitespace.py on codebase)
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
=======
=======
  void
  Node::DoDispose()
  {
>>>>>>> 27288fce1 (POSITRON: initial commit)
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
<<<<<<< HEAD
}
>>>>>>> e25ff966f (Apply clang-format to codebase)

<<<<<<< HEAD
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
=======
void
Node::DoInitialize()
{
<<<<<<< HEAD
  NS_LOG_FUNCTION (this << &handler << protocolType << device << promiscuous);
  struct Node::ProtocolHandlerEntry entry;
  entry.handler = handler;
  entry.protocol = protocolType;
  entry.device = device;
  entry.promiscuous = promiscuous;
>>>>>>> b6a5ee815 (Run utils/trim-trailing-whitespace.py on codebase)

    // On demand enable promiscuous mode in netdevices
    if (promiscuous)
    {
<<<<<<< HEAD
      if (device == 0)
      {
        for (std::vector<Ptr<NetDevice>>::iterator i = m_devices.begin();
             i != m_devices.end(); i++)
=======
      if (!device)
>>>>>>> e461e54d5 (Update Ptr validity checks to new Ptr<>::operator bool)
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
=======
=======
  }
  void
  Node::DoInitialize(void)
  {
>>>>>>> 27288fce1 (POSITRON: initial commit)
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
<<<<<<< HEAD
        if (i->handler.IsEqual(handler))
        {
            m_handlers.erase(i);
            break;
        }
>>>>>>> e25ff966f (Apply clang-format to codebase)
    }
  }

<<<<<<< HEAD
  bool
  Node::ChecksumEnabled(void)
  {
    NS_LOG_FUNCTION_NOARGS();
    BooleanValue val;
    g_checksumEnabled.GetValue(val);
    return val.Get();
  }
=======
bool
Node::ChecksumEnabled()
{
=======
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
>>>>>>> 27288fce1 (POSITRON: initial commit)
    NS_LOG_FUNCTION_NOARGS();
    BooleanValue val;
    g_checksumEnabled.GetValue(val);
    return val.Get();
<<<<<<< HEAD
}
>>>>>>> 6bb638356 (Fix clang-tidy modernize-redundant-void-arg warnings)

<<<<<<< HEAD
=======
  }

>>>>>>> 27288fce1 (POSITRON: initial commit)
  bool
  Node::PromiscReceiveFromDevice(Ptr<NetDevice> device, Ptr<const Packet> packet, uint16_t protocol,
                                 const Address &from, const Address &to, NetDevice::PacketType packetType)
  {
<<<<<<< HEAD
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
<<<<<<< HEAD
      if (i->device == 0 ||
          (i->device != 0 && i->device == device))
      {
        if (i->protocol == 0 ||
            i->protocol == protocol)
=======
      if (!i->device ||
          (i->device == device))
>>>>>>> e461e54d5 (Update Ptr validity checks to new Ptr<>::operator bool)
        {
<<<<<<< HEAD
          if (promiscuous == i->promiscuous)
          {
            i->handler(device, packet, protocol, from, to, packetType);
            found = true;
          }
=======
          if (i->protocol == 0 ||
              i->protocol == protocol)
=======
bool
Node::PromiscReceiveFromDevice(Ptr<NetDevice> device,
                               Ptr<const Packet> packet,
                               uint16_t protocol,
                               const Address& from,
                               const Address& to,
                               NetDevice::PacketType packetType)
{
=======
>>>>>>> 27288fce1 (POSITRON: initial commit)
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
<<<<<<< HEAD
            if (i->protocol == 0 || i->protocol == protocol)
>>>>>>> e25ff966f (Apply clang-format to codebase)
            {
                if (promiscuous == i->promiscuous)
                {
                    i->handler(device, packet, protocol, from, to, packetType);
                    found = true;
                }
            }
>>>>>>> b6a5ee815 (Run utils/trim-trailing-whitespace.py on codebase)
=======
          if (promiscuous == i->promiscuous)
          {
            i->handler(device, packet, protocol, from, to, packetType);
            found = true;
          }
>>>>>>> 27288fce1 (POSITRON: initial commit)
        }
      }
    }
<<<<<<< HEAD
<<<<<<< HEAD
    return found;
  }
  void
  Node::RegisterDeviceAdditionListener(DeviceAdditionListener listener)
  {
<<<<<<< HEAD
    NS_LOG_FUNCTION(this << &listener);
    m_deviceAdditionListeners.push_back(listener);
    // and, then, notify the new listener about all existing devices.
    for (std::vector<Ptr<NetDevice>>::const_iterator i = m_devices.begin();
         i != m_devices.end(); ++i)
=======
  return found;
=======
    return found;
>>>>>>> e25ff966f (Apply clang-format to codebase)
}

void
Node::RegisterDeviceAdditionListener(DeviceAdditionListener listener)
{
<<<<<<< HEAD
  NS_LOG_FUNCTION (this << &listener);
  m_deviceAdditionListeners.push_back (listener);
  // and, then, notify the new listener about all existing devices.
  for (std::vector<Ptr<NetDevice> >::const_iterator i = m_devices.begin ();
       i != m_devices.end (); ++i)
>>>>>>> b6a5ee815 (Run utils/trim-trailing-whitespace.py on codebase)
    {
      listener(*i);
=======
=======
>>>>>>> 27288fce1 (POSITRON: initial commit)
    NS_LOG_FUNCTION(this << &listener);
    m_deviceAdditionListeners.push_back(listener);
    // and, then, notify the new listener about all existing devices.
    for (std::vector<Ptr<NetDevice>>::const_iterator i = m_devices.begin();
         i != m_devices.end(); ++i)
    {
<<<<<<< HEAD
        listener(*i);
>>>>>>> e25ff966f (Apply clang-format to codebase)
    }
<<<<<<< HEAD
=======
      listener(*i);
    }
>>>>>>> 27288fce1 (POSITRON: initial commit)
  }
  void
  Node::UnregisterDeviceAdditionListener(DeviceAdditionListener listener)
  {
<<<<<<< HEAD
    NS_LOG_FUNCTION(this << &listener);
    for (DeviceAdditionListenerList::iterator i = m_deviceAdditionListeners.begin();
         i != m_deviceAdditionListeners.end(); i++)
=======
}

void
Node::UnregisterDeviceAdditionListener(DeviceAdditionListener listener)
{
<<<<<<< HEAD
  NS_LOG_FUNCTION (this << &listener);
  for (DeviceAdditionListenerList::iterator i = m_deviceAdditionListeners.begin ();
       i != m_deviceAdditionListeners.end (); i++)
>>>>>>> b6a5ee815 (Run utils/trim-trailing-whitespace.py on codebase)
    {
      if ((*i).IsEqual(listener))
      {
        m_deviceAdditionListeners.erase(i);
        break;
      }
=======
=======
>>>>>>> 27288fce1 (POSITRON: initial commit)
    NS_LOG_FUNCTION(this << &listener);
    for (DeviceAdditionListenerList::iterator i = m_deviceAdditionListeners.begin();
         i != m_deviceAdditionListeners.end(); i++)
    {
<<<<<<< HEAD
        if ((*i).IsEqual(listener))
        {
            m_deviceAdditionListeners.erase(i);
            break;
        }
>>>>>>> e25ff966f (Apply clang-format to codebase)
    }
<<<<<<< HEAD
  }
=======
}
=======
      if ((*i).IsEqual(listener))
      {
        m_deviceAdditionListeners.erase(i);
        break;
      }
    }
  }
>>>>>>> 27288fce1 (POSITRON: initial commit)

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

<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> b6a5ee815 (Run utils/trim-trailing-whitespace.py on codebase)

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
=======
} // namespace ns3
>>>>>>> e25ff966f (Apply clang-format to codebase)
=======
} // namespace ns3
>>>>>>> 27288fce1 (POSITRON: initial commit)
