/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
#include "control-layer-helper.h"
#include "ns3/control-layer-server.h"
#include "ns3/control-layer-client.h"
#include "ns3/uinteger.h"
#include "ns3/names.h"

#include <yaml-cpp/yaml.h>
#include <sstream>
#include <string>
#include <vector>

#include "ns3/internet-module.h"

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <iostream>

NS_LOG_COMPONENT_DEFINE("control-layer-helper");

ns3::NodeContainer controlNodes;
ns3::Ipv6InterfaceContainer controlInterfaces;
bool balanced = 1;
bool validworker = 0;

// std::vector<ns3::EventId> evento;

ns3::EventId finishIDApp[100000];
uint32_t qntDeApp = 0;

struct Worker
{
  int ID;
  float POWER;
  float INITIAL_CONSUMPTION;
  float CURRENT_CONSUMPTION;
  float CPU;
  float MEMORY;
  float TRANSMISSION;
  float STORAGE;
};

struct ApplicationStruct
{
  int ID;
  float START;
  float DURATION;
  bool FINISH;
  float CPU;
  float MEMORY;
  float STORAGE;
  char POLICY[20];
};

struct Worker worker;
struct ApplicationStruct application;

static int database_query(const char *query_sql, int (*callback)(void *, int, char **, char **))
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  double currentTime = ns3::Simulator::Now().GetSeconds();
  std::string data = "At time " +  std::to_string(currentTime).substr(0, std::to_string(currentTime).find(".") + 2) + "s";
  //std::string data = "At time " + std::to_string(ns3::Simulator::Now().GetSeconds()) + "s";

  /* Open database */
  rc = sqlite3_open("scratch/database.db", &db);

  if (rc)
  {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return (0);
  }
  else
  {
    NS_LOG_INFO("Opened database successfully");
  }

  /* Execute SQL statement */
  rc = sqlite3_exec(db, query_sql, callback, (void *)data.c_str(), &zErrMsg);

  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  else
  {
    NS_LOG_INFO("Opened database successfully");
  }
  sqlite3_close(db);
  return 0;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
  int i;
  for (i = 0; i < argc; i++)
  {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

static int callback_worker(void *data, int argc, char **argv, char **azColName)
{
  fprintf(stderr, "%s: Callback_Worker(NodeId: %s)\n", (const char *)data, argv[0]);

  worker.ID = atoi(argv[0]);
  worker.POWER = controlNodes.Get(worker.ID)->GetPower();
  worker.INITIAL_CONSUMPTION = atof(argv[2]);
  worker.CURRENT_CONSUMPTION = atof(argv[3]);
  worker.CPU = atof(argv[4]);
  worker.MEMORY = atof(argv[5]);
  worker.TRANSMISSION = atof(argv[6]);
  worker.STORAGE = atof(argv[7]);
  validworker = 1;

  return 0;
}

static int callback_workers_applications_count(void *data, int argc, char **argv, char **azColName)
{
  qntDeApp = atoi(argv[0]);
  return 0;
}

static int callback_application(void *data, int argc, char **argv, char **azColName)
{
  fprintf(stderr, "%s: Callback_Application(AppId: %s)\n", (const char *)data, argv[0]);

  application.ID = atoi(argv[0]);
  application.START = atof(argv[1]);
  application.DURATION = atof(argv[2]);
  application.FINISH = atoi(argv[3]);
  application.CPU = atof(argv[4]);
  application.MEMORY = atof(argv[5]);
  application.STORAGE = atof(argv[6]);
  strcpy(application.POLICY, argv[7]);

  return 0;
}

static int callback_apps_id(void *data, int argc, char **argv, char **azColName)
{
  fprintf(stderr, "%s: Callback_apps_id(AppId: %s)\n", (const char *)data, argv[0]);

  ns3::Simulator::Cancel(finishIDApp[atoi(argv[0])]);

  return 0;
}

static bool select_worker(float cpu, float memory, float storage, char *policy, bool balanced)
{
  //  const std::string load_balanced = balanced ? "DESC" : "ASC";
  char sql[10000] = "SELECT\
                        WORKERS.ID AS worker,\
                        WORKERS.CPU AS cpu,\
                        WORKERS.CPU - COALESCE(SUM(APPLICATIONS.CPU), 0) AS cpu_remaining,\
                        WORKERS.MEMORY AS memory,\
                        WORKERS.MEMORY - COALESCE(SUM(APPLICATIONS.MEMORY), 0) AS memory_remaining,\
                        WORKERS.STORAGE AS storage,\
                        WORKERS.STORAGE - COALESCE(SUM(APPLICATIONS.STORAGE), 0) AS storage_remaining,\
                        WORKERS.POWER AS battery,\
                        WORKERS.TRANSMISSION AS transmission,\
                        WORKERS.INITIAL_CONSUMPTION AS initial_consumption,\
                        WORKERS.CURRENT_CONSUMPTION + (COUNT(APPLICATIONS.ID) * WORKERS.INITIAL_CONSUMPTION) AS current_consumption,\
                        COUNT(APPLICATIONS.ID) AS application_quantity\
                    FROM WORKERS\
                    LEFT JOIN WORKERS_APPLICATIONS ON WORKERS_APPLICATIONS.ID_WORKER == WORKERS.ID\
                    LEFT JOIN APPLICATIONS ON APPLICATIONS.ID == WORKERS_APPLICATIONS.ID_APPLICATION AND APPLICATIONS.FINISH == 0\
                    GROUP BY\
                        WORKERS.ID\
                    ORDER BY\
                        ";

  std::ostringstream query;
  query << sql;

  if (balanced)
  {
    query << "application_quantity ASC, ";
  }

  /* Create SQL statement */

  if (strcmp(policy, "performance") == 0)
  {
    query << "cpu_remaining DESC ";
  }
  else if (strcmp(policy, "storage") == 0)
  {
    query << "storage_remaining DESC ";
  }
  else if (strcmp(policy, "transmission") == 0)
  {
    query << "transmission DESC ";
  }
  else
  {
    std::cout << "Error defining application POLICY type" << std::endl;
  }

  const std::string query2 = query.str();
  const char *query3 = query2.c_str();

  std::ostringstream oss;
  oss << "SELECT\
              worker,\
              battery,\
              initial_consumption,\
              current_consumption,\
              cpu_remaining,\
              memory_remaining,\
              transmission,\
              storage_remaining\
          FROM ("
      << query3 << ")\
          WHERE\
              cpu_remaining >="
      << cpu << " AND\
              memory_remaining >="
      << memory << " AND\
              storage_remaining >="
      << storage << " AND\
              battery > 0\
          LIMIT 1;";

  const std::string sql2 = oss.str();
  const char *sql3 = sql2.c_str();

  // std::cout << sql3 << std::endl;

  validworker = 0;
  database_query(sql3, callback_worker);

  if (validworker)
  {
    return 1;
  }
  return 0;
}

static ApplicationStruct select_application_by_id(int id)
{
  /* Create SQL statement */
  char sql[] = "SELECT * from APPLICATIONS WHERE ID = ";
  std::ostringstream oss;
  oss << sql << id;

  const std::string sql2 = oss.str();
  const char *sql3 = sql2.c_str();

  database_query(sql3, callback_application);

  return application;
}

static int insert_worker_application(int id_worker, int id_application, float performed_at)
{
  std::ostringstream oss;
  oss << "INSERT INTO WORKERS_APPLICATIONS (ID_WORKER,ID_APPLICATION,PERFORMED_AT,FINISHED_AT) VALUES ";
  oss << "(" << id_worker << ", " << id_application << ", " << performed_at << ", 0 ); ";

  const std::string sql2 = oss.str();
  const char *sql3 = sql2.c_str();

  database_query(sql3, callback);
  return 0;
}

static Worker get_worker_by_application_id(int id, float start)
{
  /* Create SQL statement */
  std::ostringstream oss;
  oss << "SELECT * from WORKERS WHERE ID = (SELECT ID_WORKER from WORKERS_APPLICATIONS WHERE ID_APPLICATION = ";
  oss << id << " AND FINISHED_AT = 0" << ");";

  const std::string sql2 = oss.str();
  const char *sql3 = sql2.c_str();

  database_query(sql3, callback_worker);

  return worker;
}

namespace ns3
{
  static void deallocate_worker_application(ApplicationStruct application, std::string finish)
  {
 
    float finished_at = Simulator::Now().GetSeconds();
    Worker worker = get_worker_by_application_id(application.ID, application.START);

    std::cout << "At time " << std::to_string(finished_at).substr(0, std::to_string(finished_at).find(".") + 2) << "s: ";
    std::cout << "deallocate_worker_application called in worker " << worker.ID << " and application " << application.ID << " finish: " << finish << std::endl;

    Ptr<Node> node = controlNodes.Get(worker.ID);
    node->RemoveApplication();

    // update relational table
    // std::ostringstream oss;
    // oss << "UPDATE WORKERS_APPLICATIONS SET";
    // oss << " FINISHED_AT= " << finished_at << " WHERE (ID_WORKER= " << worker.ID << " AND ID_APPLICATION= " << application.ID << " AND PERFORMED_AT= " << application.START << ");"
    //     << "UPDATE APPLICATIONS SET FINISH = " << finish << " WHERE ID = " << application.ID << ";";
    std::ostringstream oss;
    oss << "UPDATE WORKERS_APPLICATIONS SET FINISHED_AT = " << finished_at;
    oss << " WHERE (ID_WORKER= " << worker.ID << " AND ID_APPLICATION= " << application.ID << " AND FINISHED_AT = 0);"
        << " UPDATE APPLICATIONS SET FINISH = " << finish << " WHERE ID = " << application.ID << ";";

    const std::string sql2 = oss.str();
    const char *sql3 = sql2.c_str();
    database_query(sql3, callback_worker);
  }

  static void allocate_worker_application(ApplicationStruct application)
  {

    // select node throught application's parameters
    bool workerfound = select_worker(application.CPU,
                                     application.MEMORY,
                                     application.STORAGE,
                                     application.POLICY,
                                     balanced);
    if (workerfound)
    {
      double currentTime = ns3::Simulator::Now().GetSeconds();
      std::cout << "At time " << std::to_string(currentTime).substr(0, std::to_string(currentTime).find(".") + 2) << "s: ";
      std::cout << "allocate_worker_application called in worker " << worker.ID << " and application " << application.ID << std::endl;
      // std::cout << "Application Start: " << application.START << " - Application Duration: " << application.DURATION << std::endl;

      // insert_worker_application(worker.ID, application.ID, application.START);
      insert_worker_application(worker.ID, application.ID, currentTime);
      NS_LOG_UNCOND("\n");
      NS_LOG_UNCOND("APPLICATION" << "\t \t" << "NODE");
      NS_LOG_UNCOND("ID = " << application.ID << "\t \t" << "ID = " << worker.ID);
      NS_LOG_UNCOND("START =  " << currentTime << "\t" << "INITIAL CONSUMPTION = " << worker.INITIAL_CONSUMPTION);
      NS_LOG_UNCOND("POLICY = " << application.POLICY << "\t" << "POWER = " << worker.POWER);
      NS_LOG_UNCOND("DURATION = " << application.DURATION << "\t" << "CURRENT CONSUMPTION = " << worker.CURRENT_CONSUMPTION);
      NS_LOG_UNCOND("CPU = " << application.CPU << "\t \t" << "CPU = " << worker.CPU);
      NS_LOG_UNCOND("MEMORY = " << application.MEMORY << "\t \t" << "MEMORY = " << worker.MEMORY);
      NS_LOG_UNCOND("STORAGE = " << application.STORAGE << "\t \t" << "STORAGE = " << worker.STORAGE);
      NS_LOG_UNCOND("FINISH = " << application.FINISH << "\t \t" << "TRANSMISSION = " << worker.TRANSMISSION);
      NS_LOG_UNCOND("\n");

      Ptr<Node> node = controlNodes.Get(worker.ID);
      node->AddApplication();

      finishIDApp[application.ID] = Simulator::Schedule(
          Seconds(application.DURATION),
          &deallocate_worker_application,
          application, "1");

    }
    else
    {

      std::cout << "It was not possible to allocate application : " << application.ID << std::endl;
      // TODO:
      // PRECISAMOS DEFINIR COMO INDICAR SE NAO TIVER WORKER PARA A APP NESSE MOMENTO
    }

    // int sourceID = 0;
    // int destinationID = 0;

    //get IPs
    // Ipv6Address sourceIP = controlInterfaces.GetAddress(sourceID, 1);

    // Ipv6Address destinationIP = controlInterfaces.GetAddress(destinationID, 1);
    // ControlLayerClientHelper controlLayer(controlInterfaces.GetAddress(sourceID, 1), 9);
    // controlLayer.SetAttribute("MaxPackets", UintegerValue(1));
    // controlLayer.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    // controlLayer.SetAttribute("PacketSize", UintegerValue(1024));

    // //control node is 0
    // ApplicationContainer controlContainer = controlLayer.Install(node);

    // controlContainer.Start(Seconds(1.0));
    // controlContainer.Stop(Seconds(20.0));

    // std::ostringstream oss;
    // oss << "\"name\":\"" << application.POLICY << "\"\"time\":\"" << application.START << "\"\"sourceIP\":\"" << sourceIP << "\"\"destinationIP\":\"" << destinationIP << "\"";
    // std::string data = oss.str();

    // controlLayer.SetFill(controlContainer.Get(0), data);
  }

  ControlLayerServerHelper::ControlLayerServerHelper(uint16_t port)
  {
    m_factory.SetTypeId(ControlLayerServer::GetTypeId());
    SetAttribute("Port", UintegerValue(port));
  }

  void
  ControlLayerServerHelper::SetAttribute(
      std::string name,
      const AttributeValue &value)
  {
    m_factory.Set(name, value);
  }

  ApplicationContainer
  ControlLayerServerHelper::Install(Ptr<Node> node) const
  {
    return ApplicationContainer(InstallPriv(node));
  }

  ApplicationContainer
  ControlLayerServerHelper::Install(std::string nodeName) const
  {
    Ptr<Node> node = Names::Find<Node>(nodeName);
    return ApplicationContainer(InstallPriv(node));
  }

  ApplicationContainer
  ControlLayerServerHelper::Install(NodeContainer c) const
  {
    ApplicationContainer apps;
    for (NodeContainer::Iterator i = c.Begin(); i != c.End(); ++i)
    {
      apps.Add(InstallPriv(*i));
    }

    return apps;
  }

  Ptr<Application>
  ControlLayerServerHelper::InstallPriv(Ptr<Node> node) const
  {
    Ptr<Application> app = m_factory.Create<ControlLayerServer>();
    node->AddApplication(app);

    return app;
  }

  ControlLayerClientHelper::ControlLayerClientHelper(Address address, uint16_t port)
  {
    m_factory.SetTypeId(ControlLayerClient::GetTypeId());
    SetAttribute("RemoteAddress", AddressValue(address));
    SetAttribute("RemotePort", UintegerValue(port));
  }

  ControlLayerClientHelper::ControlLayerClientHelper(Address address)
  {
    m_factory.SetTypeId(ControlLayerClient::GetTypeId());
    SetAttribute("RemoteAddress", AddressValue(address));
  }

  void
  ControlLayerClientHelper::SetAttribute(
      std::string name,
      const AttributeValue &value)
  {
    m_factory.Set(name, value);
  }

  void
  ControlLayerClientHelper::SetFill(Ptr<Application> app, std::string fill)
  {
    app->GetObject<ControlLayerClient>()->SetFill(fill);
  }

  void
  ControlLayerClientHelper::SetFill(Ptr<Application> app, uint8_t fill, uint32_t dataLength)
  {
    app->GetObject<ControlLayerClient>()->SetFill(fill, dataLength);
  }

  void
  ControlLayerClientHelper::SetFill(Ptr<Application> app, uint8_t *fill, uint32_t fillLength, uint32_t dataLength)
  {
    app->GetObject<ControlLayerClient>()->SetFill(fill, fillLength, dataLength);
  }

  ApplicationContainer
  ControlLayerClientHelper::Install(Ptr<Node> node) const
  {
    return ApplicationContainer(InstallPriv(node));
  }

  ApplicationContainer
  ControlLayerClientHelper::Install(std::string nodeName) const
  {
    Ptr<Node> node = Names::Find<Node>(nodeName);
    return ApplicationContainer(InstallPriv(node));
  }

  ApplicationContainer
  ControlLayerClientHelper::Install(NodeContainer c) const
  {
    ApplicationContainer apps;
    for (NodeContainer::Iterator i = c.Begin(); i != c.End(); ++i)
    {
      apps.Add(InstallPriv(*i));
    }

    return apps;
  }

  Ptr<Application>
  ControlLayerClientHelper::InstallPriv(Ptr<Node> node) const
  {
    Ptr<Application> app = m_factory.Create<ControlLayerClient>();
    node->AddApplication(app);

    return app;
  }

  void
  sendIPMsg(int nodeID, float timeEv, Ipv4Address nodeIP)
  {
    std::cout << "At time +" << timeEv << "s IP of node " << nodeID << " was requested, which is " << nodeIP << std::endl;
  }

  void ControlLayerClientHelper::CreateDatabase()
  {

    /* Create SQL statement */
    const char sql_workers[] = "CREATE TABLE WORKERS("
                               "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                               "POWER  FLOAT   NOT NULL,"
                               "INITIAL_CONSUMPTION  FLOAT   NOT NULL,"
                               "CURRENT_CONSUMPTION    FLOAT   NOT NULL,"
                               "CPU            FLOAT   NOT NULL,"
                               "MEMORY         FLOAT   NOT NULL,"
                               "TRANSMISSION   FLOAT   NOT NULL,"
                               "STORAGE        FLOAT   NOT NULL,"
                               "NAME           TEXT    NOT NULL );";

    /* Execute SQL statement */
    database_query(sql_workers, callback);

    /* Create SQL statement */
    const char sql_applications[] = "CREATE TABLE APPLICATIONS("
                                    "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                    "START      FLOAT   NOT NULL,"
                                    "DURATION   FLOAT   NOT NULL,"
                                    "FINISH   INTEGER   NOT NULL,"
                                    "CPU        FLOAT   NOT NULL,"
                                    "MEMORY     FLOAT   NOT NULL,"
                                    "STORAGE    FLOAT   NOT NULL,"
                                    "POLICY     TEXT    NOT NULL );";

    database_query(sql_applications, callback);

    /* Create SQL statement */
    const char sql_workers_applications[] = "CREATE TABLE WORKERS_APPLICATIONS("
                                            "ID_WORKER      INTEGER   NOT NULL,"
                                            "ID_APPLICATION INTEGER   NOT NULL,"
                                            "PERFORMED_AT   FLOAT     NOT NULL,"
                                            "FINISHED_AT    FLOAT     NOT NULL,"
                                            "FOREIGN KEY(ID_WORKER) REFERENCES WORKERS(ID),"
                                            "FOREIGN KEY(ID_APPLICATION) REFERENCES APPLICATIONS(ID),"
                                            "UNIQUE(ID_WORKER, ID_APPLICATION, PERFORMED_AT) );";

    database_query(sql_workers_applications, callback);
  }

  void ControlLayerClientHelper::AddNodeToDatabase(double power, double initial_consumption, double current_consumption, double cpu, double memory, double transmission, double storage, std::string node_name)
  {
    std::cout << "Inserting Node " << node_name << " to database with:\n"
              << "  > Power = " << power << ";\n"
              << "  > Initial Consumption = " << initial_consumption << ";\n"
              << "  > Current Consumption = " << current_consumption << ";\n"
              << "  > CPU = " << cpu << ";\n"
              << "  > Memory = " << memory << ";\n"
              << "  > Transmission = " << transmission << ";\n"
              << "  > Storage = " << storage << ";\n"
              << std::endl;

    char sql[] = "INSERT INTO WORKERS (POWER,INITIAL_CONSUMPTION,CURRENT_CONSUMPTION,CPU,MEMORY,TRANSMISSION,STORAGE,NAME) VALUES ";
    std::ostringstream oss;
    oss << sql << "(" << power << ", " << initial_consumption << ", " << current_consumption << ", " << cpu << ", " << memory << ", " << transmission << ", " << storage << ", "
        << "\"" << node_name << "\""
        << " ); ";

    const std::string sql2 = oss.str();
    const char *sql3 = sql2.c_str();

    database_query(sql3, callback);
  }

  void ControlLayerClientHelper::AddAppToDatabase(std::string policy, float start, float duration, double cpu, double memory, double storage)
  {
    NS_LOG_INFO("App scheduled at: " << Simulator::Now().GetSeconds() << "s");

    double currentTime = ns3::Simulator::Now().GetSeconds();
    std::cout << "At time " << std::to_string(currentTime).substr(0, std::to_string(currentTime).find(".") + 2) << "s: ";
    std::cout << "Inserting Application " << policy << " to database with:\n"
              << "  > Start = " << start << ";\n"
              << "  > Duration = " << duration << ";\n"
              << "  > CPU = " << cpu << ";\n"
              << "  > Memory = " << memory << ";\n"
              << "  > Storage = " << storage << ";\n"
              << std::endl;

    char sql[] = "INSERT INTO APPLICATIONS (START,DURATION,FINISH,CPU,MEMORY,STORAGE,POLICY) VALUES ";

    std::ostringstream oss;
    oss << sql << "(" << start << ", " << duration << ", 0 , " << cpu << ", " << memory << ", " << storage << ", "
        << "\"" << policy << "\""
        << " ); ";

    const std::string sql2 = oss.str();
    const char *sql3 = sql2.c_str();

    database_query(sql3, callback);
  }

  void ControlLayerClientHelper::DropDatabase()
  {

    const char sql[] = "DROP TABLE WORKERS;"
                       "DROP TABLE APPLICATIONS;"
                       "DROP TABLE WORKERS_APPLICATIONS;";

    /* Execute SQL statement */
    database_query(sql, callback);
  }

  void ControlLayerClientHelper::initialize(NodeContainer tcontrolNodes, Ipv6InterfaceContainer tcontrolInterfaces, bool tbalanced)
  {
    controlNodes = tcontrolNodes;
    controlInterfaces = tcontrolInterfaces;
    balanced = tbalanced;
  }

  int ControlLayerClientHelper::lost_power(Ptr<Node> worker)
  {
    double currentTime = ns3::Simulator::Now().GetSeconds();
    std::cout << "At time " << std::to_string(currentTime).substr(0, std::to_string(currentTime).find(".") + 2) << " - Node: " << worker->GetId() << " --- controllayerhelper::lost_power" << std::endl;
    // std::cout << "At time " << ns3::Simulator::Now().GetSeconds() << " - Node: " << worker->GetId() << " --- controllayerhelper::lost_power" << std::endl;

    std::string query;

    // coloca a bateria do nó para zero
    query = "UPDATE WORKERS SET POWER = 0 WHERE ID = " + std::to_string(worker->GetId()) + ";";
    database_query(query.c_str(), callback_worker);

    // ler todas aplicaçoes rodando no nó e cancela os eventos
    query = "SELECT WORKERS_APPLICATIONS.ID_APPLICATION FROM WORKERS_APPLICATIONS WHERE EXISTS (SELECT APPLICATIONS.ID FROM APPLICATIONS WHERE WORKERS_APPLICATIONS.ID_APPLICATION = APPLICATIONS.ID AND APPLICATIONS.FINISH == 0) AND WORKERS_APPLICATIONS.ID_WORKER == " + std::to_string(worker->GetId()) + " GROUP BY WORKERS_APPLICATIONS.ID_APPLICATION";
    database_query(query.c_str(), callback_apps_id);

    // conta as aplicações que estão rodando neste nó
    query = "SELECT COUNT(*) FROM (" + query + ");";
    database_query(query.c_str(), callback_workers_applications_count);
      
    //obter a quantidade real das aplicações
    for (uint32_t i = 0; i < qntDeApp; i++)
    {
      query = "SELECT * FROM APPLICATIONS WHERE EXISTS ( SELECT WORKERS_APPLICATIONS.ID_APPLICATION FROM WORKERS_APPLICATIONS WHERE WORKERS_APPLICATIONS.ID_APPLICATION == APPLICATIONS.ID AND WORKERS_APPLICATIONS.ID_WORKER == " + std::to_string(worker->GetId()) + " ) AND APPLICATIONS.FINISH == 0 GROUP BY APPLICATIONS.ID LIMIT 1 OFFSET " + std::to_string(i) + ";";
      database_query(query.c_str(), callback_application);

      // std::cout << "id da aplicacao atual: " << application.ID << std::endl;

      deallocate_worker_application(application, "0");
      allocate_worker_application(application);
    }
    qntDeApp = 0;

    return 0;
  }

  // PAULO 26/10/21
  // FALTA IMPLEMENTAR A SELECAO DAS APPs NO METODO ABAIXO
  // QQ DUVIDA, VERIFICAR O VIDEO DA REUNIAO DESSE DIA.
  // ALEM DISSO, FALTA VERIFICAR O CONFLITO DO ENVIO DE PACOTES,
  // QUANDO OS NOHS PERDEM BATERIA AO MESMO TEMPO.

  int ControlLayerClientHelper::node_recharge(Ptr<Node> worker)
  {
      double currentTime = ns3::Simulator::Now().GetSeconds();
    std::cout << "At time " << std::to_string(currentTime).substr(0, std::to_string(currentTime).find(".") + 2) << " --- controllayerhelper::node_recharge" << std::endl;
    // std::cout << "At time " << ns3::Simulator::Now().GetSeconds() << " - Node: " << worker->GetId() << " --- controllayerhelper::node_recharge" << std::endl;

    std::string query;

    // coloca a bateria do nó para 100
    query = "UPDATE WORKERS SET POWER = 100 WHERE ID = " + std::to_string(worker->GetId()) + ";";
    database_query(query.c_str(), callback_worker);

    // query = "SELECT WORKERS_APPLICATIONS.ID_APPLICATION FROM WORKERS_APPLICATIONS LEFT JOIN APPLICATIONS ON APPLICATIONS.FINISH == 0;";
    // database_query(query.c_str(), callback_apps_id);

    // //obter a quantidade real das aplicações
    // for (uint32_t i = 0; i < qntDeApp; i++)
    // {
    //   query = "SELECT * FROM APPLICATIONS WHERE ID=(SELECT ID_APPLICATION FROM WORKERS_APPLICATIONS WHERE ID_WORKER=" + std::to_string(worker->GetId()) + " LIMIT 1 OFFSET " + std::to_string(i) + ");";
    //   database_query(query.c_str(), callback_application);

    //   allocate_worker_application(application);
    // }
    // qntDeApp = 0;

    // std::cout << "fim recharge\n";
    return 0;
  }

  void
  ControlLayerClientHelper::AllocateAPP(int id_app)
  {
    ApplicationStruct app_allocate;

    app_allocate = select_application_by_id(id_app);

    // allocate_worker_application(app_allocate, controlNodes, controlInterfaces, balanced);

    // start application and allocate required resources into a node
    allocate_worker_application(app_allocate);
    //    Simulator::Schedule(
    //        Simulator::Now(),
    //        &allocate_worker_application,
    //        app_allocate);
  }

  //  void
  //  ControlLayerClientHelper::Manager(std::size_t num_apps)
  //  {
  //    ApplicationStruct application_struct_array[num_apps];
  //
  //    for (std::size_t id_app = 1; id_app <= num_apps; id_app++)
  //    {
  //
  //      application_struct_array[id_app - 1] = select_application_by_id(id_app);
  //
  //      // free node's resources after application finished
  //      Simulator::Schedule(
  //          Seconds(application_struct_array[id_app - 1].START + application_struct_array[id_app - 1].DURATION),
  //          &deallocate_worker_application,
  //          application_struct_array[id_app - 1]);
  //    }
  //  }

} // namespace ns3
