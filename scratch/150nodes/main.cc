/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <fstream>
#include "ns3/core-module.h"
#include "ns3/sixlowpan-module.h"
#include "ns3/lr-wpan-module.h"
#include "ns3/mobility-model.h"
#include "ns3/mobility-helper.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
// #include "ns3/random-variable-stream.h"
#include <yaml-cpp/yaml.h>

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE("POSITRON");

int main(int argc, char *argv[])
{

  bool logging = false;
  bool tracing = false;
  bool balanced = false;
  bool powerless = false;
  uint32_t seed = 42;

  CommandLine cmd(__FILE__);
  cmd.AddValue("logging", "Tell control applications to logging if true", logging);
  cmd.AddValue("tracing", "Tell control applications to tracing if true", tracing);
  cmd.AddValue("balanced", "Tell control whether is a balanced policy", balanced);
  cmd.AddValue("powerless", "Set simulation scenario with battery loss", powerless);
  cmd.AddValue("seed", "Set seed as an input parameter", seed);

  cmd.Parse(argc, argv);

  SeedManager::SetSeed(seed);

  if (logging)
  {
    LogComponentEnable("ControlLayerClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("ControlLayerServerApplication", LOG_LEVEL_INFO);
    // LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    // LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
  }

  YAML::Node input = YAML::LoadFile("./scratch/input.yaml");
  YAML::Node nodes = input["nodes"];
  YAML::Node apps = input["applications"];

  u_int32_t simulationTime = input["configs"][1]["simulationTime"].as<int>();

  NodeContainer controlNodes;
  controlNodes.Create(1);
  Names::Add("Controller", controlNodes.Get(0));

  NodeContainer workerNodes;

  for (auto attr : nodes)
  {
    int nodeQtd = attr["nNodes"].as<int>();
    if (nodeQtd > 0)
    {
      // Defining nodes' attributes
      Config::SetDefault("ns3::Node::Power", DoubleValue(attr["power"].as<double>()));
      Config::SetDefault("ns3::Node::InitialConsumption", DoubleValue(attr["initialConsumption"].as<double>()));
      Config::SetDefault("ns3::Node::CurrentConsumption", DoubleValue(attr["currentConsumption"].as<double>()));
      Config::SetDefault("ns3::Node::CPU", DoubleValue(attr["cpu"].as<double>()));
      Config::SetDefault("ns3::Node::Memory", DoubleValue(attr["memory"].as<double>()));
      Config::SetDefault("ns3::Node::Transmission", DoubleValue(attr["transmission"].as<double>()));
      Config::SetDefault("ns3::Node::Storage", DoubleValue(attr["storage"].as<double>()));

      // Creating workers
      workerNodes.Create(nodeQtd);

      // Naming workers
      for (int i = 0; i < nodeQtd; i++)
      {
        int nodeId = workerNodes.GetN() - 1 - i;
        string auxWorkerID = attr["name"].as<string>() + to_string(nodeQtd - i);
        Names::Add(auxWorkerID, workerNodes.Get(nodeId));
      }

      if (powerless) {
        int tenpercent = nodeQtd/10;
        int count = 0;

        for (int i = 0; i < nodeQtd; i++) {
          int nodeId = workerNodes.GetN() - (nodeQtd) + i;
          count += 1;
          if (count <= tenpercent) {
            workerNodes.Get (nodeId)->SetAttribute ("InitialConsumption", DoubleValue(0.001157407));
            workerNodes.Get (nodeId)->SetAttribute ("CurrentConsumption", DoubleValue(0.001157407));
          }
        } 
      }
    }
  }
  controlNodes.Add(workerNodes);

  MobilityHelper mobility;
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

  Ptr<ListPositionAllocator> nodesPositionAlloc = CreateObject<ListPositionAllocator>();
  nodesPositionAlloc->Add(Vector(0.0, 0.0, 0.0));
  nodesPositionAlloc->Add(Vector(50.0, 0.0, 0.0));
  mobility.SetPositionAllocator(nodesPositionAlloc);
  mobility.Install(controlNodes);

  LrWpanHelper ethernet;
  //lrWpanHelper.EnableLogComponents();
  NetDeviceContainer controlDevices = ethernet.Install(controlNodes);
  ethernet.CreateAssociatedPan(controlDevices, 10);

  InternetStackHelper stack;
  stack.SetIpv4StackInstall(false);
  stack.Install(controlNodes);

  SixLowPanHelper sixlowpan;
  NetDeviceContainer sixlpDevices = sixlowpan.Install(controlDevices);

  Ipv6AddressHelper address;
  address.SetBase(Ipv6Address("2001:1::"), Ipv6Prefix(64));
  Ipv6InterfaceContainer controlInterfaces = address.Assign(sixlpDevices);

  ControlLayerServerHelper controlServer(9);
  UdpEchoServerHelper workerServer(7);

  ApplicationContainer controlApps = controlServer.Install(controlNodes);
  controlApps.Start(Seconds(1.0));
  controlApps.Stop(Seconds(2*simulationTime));

  ApplicationContainer workerApps = workerServer.Install(workerNodes);
  workerApps.Start(Seconds(1.0));
  workerApps.Stop(Seconds(2*simulationTime));

  ControlLayerClientHelper controller(Ipv6Address("2001:1::2"), 9);
  ApplicationContainer controlClient = controller.Install("/Names/Controller");
  controlClient.Start(Seconds(1.0));
  controlClient.Stop(Seconds(2*simulationTime));

  controller.DropDatabase();
  controller.CreateDatabase();
  controller.initialize(controlNodes, controlInterfaces, balanced);

  cout << "List of nodes:" << endl;
  for (NodeList::Iterator node = NodeList::Begin(); node != NodeList::End(); node++)
  {
    cout << "Node ID: " << (*node)->GetId() << "; Address: " << controlInterfaces.GetAddress ((*node)->GetId(), 0) << "; Name: " << Names::FindName(*node) << ";" << endl;
  }
  cout << "" << endl;

  // Adding nodes to database
  for (uint32_t i = 0; i < workerNodes.GetN(); i++)
  {
    Ptr<Node> node = workerNodes.Get(i);

    controller.AddNodeToDatabase(
        node->GetPower(),
        node->GetInitialConsumption(),
        node->GetCurrentConsumption(),
        node->GetCpu(),
        node->GetMemory(),
        node->GetTransmission(),
        node->GetStorage(),
        Names::FindName(node));
  }

  // Adding applications to database
  for (std::size_t i = 0; i < apps.size(); i++)
  {

    std::string policy = apps[i]["policy"].as<std::string>();
    float cpu = apps[i]["cpu"].as<float>();
    float memory = apps[i]["memory"].as<float>();
    float storage = apps[i]["storage"].as<float>();

    float start = 0.0;
    float startAux = apps[i]["start"].as<float>();
    float startMin = startAux + 10; // plus 10s to avoid starting at the very beginning
    float startMax = startAux + 3600; // Apps arrive up to 1h shift
    Ptr<UniformRandomVariable> rndStart = CreateObject<UniformRandomVariable> ();
    rndStart->SetAttribute ("Min", DoubleValue (startMin));
    rndStart->SetAttribute ("Max", DoubleValue (startMax));
    start = rndStart->GetValue ();
    

    float duration = 0.0;
    float durationMean = apps[i]["duration"].as<float>();
    float durationVar = pow((durationMean * 0.20), 2); // 20% std deviation to the mean
    while(duration <= 0.0) {
      Ptr<NormalRandomVariable> rndDuration = CreateObject<NormalRandomVariable> ();
      rndDuration->SetAttribute ("Mean", DoubleValue (durationMean));
      rndDuration->SetAttribute ("Variance", DoubleValue (durationVar));
      duration = rndDuration->GetValue ();
    }

    int app_id = i + 1;

    float startNow = 0.0;
    // Simulator::Schedule(Seconds(start), &ControlLayerClientHelper::AddAppToDatabase, &controller, policy, start, duration, cpu, memory, storage);
    Simulator::Schedule(Seconds(startNow), &ControlLayerClientHelper::AddAppToDatabase, &controller, policy, start, duration, cpu, memory, storage);
    Simulator::Schedule(Seconds(start), &ControlLayerClientHelper::AllocateAPP, &controller, app_id);

  }

  if (tracing) {
    std::string scenarioName = input["configs"][0]["scenarioName"].as<std::string>();
    ethernet.EnablePcap (scenarioName, controlDevices.Get (0), true);
    //ethernet.EnablePcapAll (scenarioName);
  }

  //Simulator::Stop(Seconds (simulationTime));
  Simulator::Run();
  Simulator::Destroy();
  return 0;

}
