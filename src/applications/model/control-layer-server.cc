/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2007 University of Washington
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

#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv6-address.h"
#include "ns3/address-utils.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/udp-socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"

#include "control-layer-server.h"

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/node-list.h"

#include <iostream>
using namespace std;

string extractInformation(size_t p, string key, const string &theEntireString)
{
  string s = "\"" + key + "\":\"";
  auto p1 = theEntireString.find(s);
  if (string::npos != p1)
    p1 += s.size();
  auto p2 = theEntireString.find_first_of('\"', p1);
  if (string::npos != p2)
    return theEntireString.substr(p1, p2 - p1);
  return "";
}

namespace ns3
{

  static void sendIPMsg(Ptr<Node> nodeID, float timeEv, Ipv6Address nodeIP)
  {
    std::cout << "At time +" << timeEv << "s IP of node " << nodeID << " was requested, which is " << nodeIP << std::endl;
  }

  static void SchedUdpEchoMulticastApp(string name, Ptr<Node> sourceNode, uint16_t port)
  {

    double stime = Simulator::Now().GetSeconds();

    std::cout << "At time +" << stime << "s " << name << " of node "
              << sourceNode->GetDevice(1)->GetMulticast(Ipv6Address("2001:1::a:ff:fe00:1"))
              << " was requested." << std::endl;

    // UdpEchoClientHelper echoClient(Ipv6Address("ff02::1"), port);
    // echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    // echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.5)));
    // echoClient.SetAttribute("PacketSize", UintegerValue(pckSize));

    // ApplicationContainer app_multicast = echoClient.Install(sourceNode);

    // app_multicast.Start(Time(stime));
    // app_multicast.Stop(Seconds(59.0));
    // app_multicast.Stop (Time (stime+10.0));
  }

  static void SchedUdpEchoUnicastApp(string name, Ptr<Node> sourceNode, uint16_t port, Ipv6Address destinationIP)
  {

    double stime = Simulator::Now().GetSeconds();

    // std::cout << "At time +" << stime << "s " << name << " of node "
    //           << "nodeID"
    //           << " was requested." << std::endl;

    UdpEchoClientHelper echoClient(destinationIP, port);
    echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    // echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.5)));
    // echoClient.SetAttribute("PacketSize", UintegerValue(pckSize));

    ApplicationContainer app_unicast = echoClient.Install(sourceNode);

    app_unicast.Start(Time(stime));
    app_unicast.Stop(Seconds(59.0));
    // app_unicast.Stop (Time (stime+10.0));
  }

} // namespace ns3

namespace ns3
{

  NS_LOG_COMPONENT_DEFINE("ControlLayerServerApplication");

  NS_OBJECT_ENSURE_REGISTERED(ControlLayerServer);

  TypeId
  ControlLayerServer::GetTypeId(void)
  {
    static TypeId tid = TypeId("ns3::ControlLayerServer")
                            .SetParent<Application>()
                            .SetGroupName("Applications")
                            .AddConstructor<ControlLayerServer>()
                            .AddAttribute("Port", "Port on which we listen for incoming packets.",
                                          UintegerValue(9),
                                          MakeUintegerAccessor(&ControlLayerServer::m_port),
                                          MakeUintegerChecker<uint16_t>())
                            .AddTraceSource("Rx", "A packet has been received",
                                            MakeTraceSourceAccessor(&ControlLayerServer::m_rxTrace),
                                            "ns3::Packet::TracedCallback")
                            .AddTraceSource("RxWithAddresses", "A packet has been received",
                                            MakeTraceSourceAccessor(&ControlLayerServer::m_rxTraceWithAddresses),
                                            "ns3::Packet::TwoAddressTracedCallback");
    return tid;
  }

  ControlLayerServer::ControlLayerServer()
  {
    NS_LOG_FUNCTION(this);
  }

  ControlLayerServer::~ControlLayerServer()
  {
    NS_LOG_FUNCTION(this);
    m_socket = 0;
    m_socket6 = 0;
  }

  void
  ControlLayerServer::DoDispose(void)
  {
    NS_LOG_FUNCTION(this);
    Application::DoDispose();
  }

  void
  ControlLayerServer::StartApplication(void)
  {
    NS_LOG_FUNCTION(this);

    if (m_socket == 0)
    {
      TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
      m_socket = Socket::CreateSocket(GetNode(), tid);
      Inet6SocketAddress local = Inet6SocketAddress(Ipv6Address::GetAny(), m_port);
      if (m_socket->Bind(local) == -1)
      {
        NS_FATAL_ERROR("Failed to bind socket");
      }
      if (addressUtils::IsMulticast(m_local))
      {
        Ptr<UdpSocket> udpSocket = DynamicCast<UdpSocket>(m_socket);
        if (udpSocket)
        {
          // equivalent to setsockopt (MCAST_JOIN_GROUP)
          udpSocket->MulticastJoinGroup(0, m_local);
        }
        else
        {
          NS_FATAL_ERROR("Error: Failed to join multicast group");
        }
      }
    }

    if (m_socket6 == 0)
    {
      TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
      m_socket6 = Socket::CreateSocket(GetNode(), tid);
      Inet6SocketAddress local6 = Inet6SocketAddress(Ipv6Address::GetAny(), m_port);
      if (m_socket6->Bind6() == -1)
      {
        NS_FATAL_ERROR("Failed to bind socket");
      }
      if (addressUtils::IsMulticast(local6))
      {
        Ptr<UdpSocket> udpSocket = DynamicCast<UdpSocket>(m_socket6);
        if (udpSocket)
        {
          // equivalent to setsockopt (MCAST_JOIN_GROUP)
          udpSocket->MulticastJoinGroup(0, local6);
        }
        else
        {
          NS_FATAL_ERROR("Error: Failed to join multicast group");
        }
      }
    }

    m_socket->SetRecvCallback(MakeCallback(&ControlLayerServer::HandleRead, this));
    m_socket6->SetRecvCallback(MakeCallback(&ControlLayerServer::HandleRead, this));
  }

  void
  ControlLayerServer::StopApplication()
  {
    NS_LOG_FUNCTION(this);

    if (m_socket != 0)
    {
      m_socket->Close();
      m_socket->SetRecvCallback(MakeNullCallback<void, Ptr<Socket>>());
    }
    if (m_socket6 != 0)
    {
      m_socket6->Close();
      m_socket6->SetRecvCallback(MakeNullCallback<void, Ptr<Socket>>());
    }
  }

  void
  ControlLayerServer::HandleRead(Ptr<Socket> socket)
  {
    NS_LOG_FUNCTION(this << socket);

    Ptr<Packet> packet;
    Address from;
    Address localAddress;
    while ((packet = socket->RecvFrom(from)))
    {
      socket->GetSockName(localAddress);
      m_rxTrace(packet);
      m_rxTraceWithAddresses(packet, from, localAddress);
      if (InetSocketAddress::IsMatchingType(from))
      {
        NS_LOG_INFO("At time " << Simulator::Now().As(Time::S) << " (control layer) server received " << packet->GetSize() << " bytes from " << InetSocketAddress::ConvertFrom(from).GetIpv4() << " port " << InetSocketAddress::ConvertFrom(from).GetPort());
      }
      else if (Inet6SocketAddress::IsMatchingType(from))
      {
        NS_LOG_INFO("At time " << Simulator::Now().As(Time::S) << " (control layer) server received " << packet->GetSize() << " bytes from " << Inet6SocketAddress::ConvertFrom(from).GetIpv6() << " port " << Inet6SocketAddress::ConvertFrom(from).GetPort());
      }

      packet->RemoveAllPacketTags();
      packet->RemoveAllByteTags();

      NS_LOG_LOGIC("Echoing packet");
      socket->SendTo(packet, 0, from);

      uint8_t *buffer = new uint8_t[packet->GetSize()];
      packet->CopyData(buffer, packet->GetSize());

      string data = (char *)buffer;

      string name = extractInformation(0, "name", data);
      double time = atof(extractInformation(0, "time", data).c_str());
      Ipv6Address sourceIP;
      sourceIP.Set(extractInformation(0, "sourceIP", data).c_str());
      Ipv6Address destinationIP;
      destinationIP.Set(extractInformation(0, "destinationIP", data).c_str());

      Ptr<Node> sourceNode = GetNode();

      uint16_t dest_port = 7;

      if (!name.compare("OutOfPower"))
      {
        double sourceID = atof(extractInformation(0, "sourceID", data).c_str());
        sourceNode = NodeList::GetNode(sourceID);

        Ptr<Application> auxPtr1 = sourceNode->GetApplication(0);
        Application *auxPtr2 = PeekPointer(auxPtr1);
        ControlLayerClientHelper *auxPtr3 = (ControlLayerClientHelper *)auxPtr2;
        auxPtr3->lost_power(sourceNode);
      }
      else if(!name.compare("Recharge")){
        double sourceID = atof(extractInformation(0, "sourceID", data).c_str());
        sourceNode = NodeList::GetNode(sourceID);

        Ptr<Application> auxPtr1 = sourceNode->GetApplication(0);
        Application *auxPtr2 = PeekPointer(auxPtr1);
        ControlLayerClientHelper *auxPtr3 = (ControlLayerClientHelper *)auxPtr2;
        auxPtr3->node_recharge(sourceNode);
      }
      else if (!name.compare("multicast"))
      {
        Simulator::Schedule(Seconds(time), &SchedUdpEchoMulticastApp, name, sourceNode, dest_port);
      }
      else if (!name.compare("unicast"))
      {
        Simulator::Schedule(Seconds(time), &SchedUdpEchoUnicastApp, name, sourceNode, dest_port, destinationIP);
      }
      else if (!name.compare("ip_info"))
      {
        // int sourceID_int = 1;
        Simulator::Schedule(Seconds(time), &sendIPMsg, sourceNode, time, sourceIP);
      }
      else
      {
        // cout << "invalid option" << endl;
      }

      if (InetSocketAddress::IsMatchingType(from))
      {
        NS_LOG_INFO("At time " << Simulator::Now().As(Time::S) << " (control layer) server sent " << packet->GetSize() << " bytes to " << InetSocketAddress::ConvertFrom(from).GetIpv4() << " port " << InetSocketAddress::ConvertFrom(from).GetPort());
      }
      else if (Inet6SocketAddress::IsMatchingType(from))
      {
        NS_LOG_INFO("At time " << Simulator::Now().As(Time::S) << " (control layer) server sent " << packet->GetSize() << " bytes to " << Inet6SocketAddress::ConvertFrom(from).GetIpv6() << " port " << Inet6SocketAddress::ConvertFrom(from).GetPort());
      }
    }
  }

} // Namespace ns3
