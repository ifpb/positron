/*
 * Copyright (c) 2009 CTTC
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */

#include "wifi-spectrum-phy-interface.h"

#include "ns3/log.h"
#include "ns3/mobility-model.h"
#include "ns3/net-device.h"
#include "ns3/spectrum-channel.h"
#include "ns3/spectrum-value.h"

NS_LOG_COMPONENT_DEFINE("WifiSpectrumPhyInterface");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED(WifiSpectrumPhyInterface);

TypeId
WifiSpectrumPhyInterface::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::WifiSpectrumPhyInterface").SetParent<SpectrumPhy>().SetGroupName("Wifi");
    return tid;
}

WifiSpectrumPhyInterface::WifiSpectrumPhyInterface(FrequencyRange range)
    : m_range{range}
{
    NS_LOG_FUNCTION(this << range);
}

void
WifiSpectrumPhyInterface::DoDispose()
{
    NS_LOG_FUNCTION(this);
    m_rxSpectrumModel = nullptr;
    m_spectrumWifiPhy = nullptr;
    m_netDevice = nullptr;
    m_channel = nullptr;
}

void
WifiSpectrumPhyInterface::SetSpectrumWifiPhy(const Ptr<SpectrumWifiPhy> spectrumWifiPhy)
{
    m_spectrumWifiPhy = spectrumWifiPhy;
}

Ptr<NetDevice>
WifiSpectrumPhyInterface::GetDevice() const
{
    return m_netDevice;
}

Ptr<MobilityModel>
WifiSpectrumPhyInterface::GetMobility() const
{
    return m_spectrumWifiPhy->GetMobility();
}

void
WifiSpectrumPhyInterface::SetDevice(const Ptr<NetDevice> d)
{
    m_netDevice = d;
}

void
WifiSpectrumPhyInterface::SetMobility(const Ptr<MobilityModel> m)
{
    m_spectrumWifiPhy->SetMobility(m);
}

void
WifiSpectrumPhyInterface::SetChannel(const Ptr<SpectrumChannel> c)
{
    NS_LOG_FUNCTION(this << c);
    NS_ASSERT_MSG(!m_rxSpectrumModel, "Spectrum channel shall be set before RX spectrum model");
    m_channel = c;
}

void
WifiSpectrumPhyInterface::SetRxSpectrumModel(Ptr<const SpectrumModel> rxSpectrumModel)
{
    NS_LOG_FUNCTION(this << rxSpectrumModel);
    m_rxSpectrumModel = rxSpectrumModel;
}

Ptr<SpectrumChannel>
WifiSpectrumPhyInterface::GetChannel() const
{
    return m_channel;
}

Ptr<const SpectrumModel>
WifiSpectrumPhyInterface::GetRxSpectrumModel() const
{
    return m_rxSpectrumModel;
}

Ptr<Object>
WifiSpectrumPhyInterface::GetAntenna() const
{
    return m_spectrumWifiPhy->GetAntenna();
}

const FrequencyRange&
WifiSpectrumPhyInterface::GetFrequencyRange() const
{
    return m_range;
}

void
WifiSpectrumPhyInterface::StartRx(Ptr<SpectrumSignalParameters> params)
{
    m_spectrumWifiPhy->StartRx(params);
}

void
WifiSpectrumPhyInterface::StartTx(Ptr<SpectrumSignalParameters> params)
{
    params->txPhy = Ptr<SpectrumPhy>(this);
    params->txAntenna = m_spectrumWifiPhy->GetAntenna();
    m_channel->StartTx(params);
}

} // namespace ns3
