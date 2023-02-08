#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-monitor-module.h"
#include <vector>

/*
 * UTA NAME : sadhan pawar vadegher
 * UTA ID : 1002023295
 * */

/* Network flow 
 * 0		1
 * A ---------- |
 * 		|
 * 2	 3	|
 * B ----D------E
 *	 |
 * 4	 |
 * C -----
 *
 *
 * */

using namespace ns3;
using namespace std;

void CalculateThroughput();
void averageThroughput();

NS_LOG_COMPONENT_DEFINE ("CN_Project2_Sadhan");

Ptr<FlowMonitor> flowMonitor;
FlowMonitorHelper flwHelper;
vector<double> vec(1000,0);
vector<double> vec2(1000,0);
ofstream flow1_file("flow1.txt");
ofstream flow2_file("flow2.txt");
ofstream flow5_file("flow5.txt");
ofstream flow7_file("flow7.txt");


int main(int argc, char* argv[])
{
	CommandLine cmd(__FILE__);
	cmd.Parse(argc,argv);
	
	/* create 5 nodes/computer objects */
	NodeContainer nodes;
	nodes.Create(5);
	
	/* create a node container which is concatenation of two containers */	
	NodeContainer nae = NodeContainer(nodes.Get(0), nodes.Get(1));	
	NodeContainer nbd = NodeContainer(nodes.Get(2), nodes.Get(3));	
	NodeContainer nde = NodeContainer(nodes.Get(3), nodes.Get(1));	
	NodeContainer ncd = NodeContainer(nodes.Get(4), nodes.Get(3));	
	
	/* create a point to point channel link with below attributes among nodes */
	PointToPointHelper p2p;
	p2p.SetDeviceAttribute("DataRate", StringValue("100Mbps"));
	p2p.SetChannelAttribute("Delay", StringValue("0.5ms"));
	
	/* Install the p2p connection between two nodes for all nodes as specified in the project document */	
	NetDeviceContainer ndae = p2p.Install(nae);
	NetDeviceContainer ndbd = p2p.Install(nbd);
        NetDeviceContainer ndde = p2p.Install(nde);
	NetDeviceContainer ndcd = p2p.Install(ncd);
	
	/* enable the internet stack on all nodes which follows 
	 * protocol rules */
	InternetStackHelper stack;
	stack.Install(nodes);
	
	/* Assigning IP addresses for all the nodes */	
	Ipv4AddressHelper address1;
	Ipv4AddressHelper address2;
	Ipv4AddressHelper address3;
	Ipv4AddressHelper address4;
	
	/* this sets the base network and network mask */	
	address1.SetBase("10.1.1.0","255.255.255.0");
	address2.SetBase("10.1.2.0","255.255.255.0");
	address3.SetBase("10.1.3.0","255.255.255.0");
	address4.SetBase("10.1.4.0","255.255.255.0");
	
	/* This interfaces are installed on devices using ipv4 adress */	
	Ipv4InterfaceContainer interface1;
	Ipv4InterfaceContainer interface2;
	Ipv4InterfaceContainer interface3;
	Ipv4InterfaceContainer interface4;
	
	/* assign method takes NetDeviceContainer which will find associated node and ipv4 address */
	interface1 = address1.Assign(ndae);
	interface2 = address2.Assign(ndbd);
	interface3 = address3.Assign(ndde);
	interface4 = address4.Assign(ndcd);
	
	/* populate the routing tables */	
	ns3::Ipv4GlobalRoutingHelper::PopulateRoutingTables();
	
	/* This helps in enabling Ipv4 flow monitoring on all the nodes */	
	flowMonitor = flwHelper.InstallAll();
	
	/* we could visualize the nodes through animation application where test.xml is given
	 * as input */	
	AnimationInterface anime("test.xml");
	anime.SetConstantPosition(nodes.Get(4),5.0,100.0); /* set the position points of all nodes in netanime application */
	anime.SetConstantPosition(nodes.Get(1),100.0,50.0);
	anime.SetConstantPosition(nodes.Get(2),5.0,50.0);
	anime.SetConstantPosition(nodes.Get(3),50.0,50.0);
	anime.SetConstantPosition(nodes.Get(0),5.0,25.0);
	anime.UpdateNodeSize(0,5.0,5.0);	/* increase the nodes size */
	anime.UpdateNodeSize(1,5.0,5.0);
	anime.UpdateNodeSize(2,5.0,5.0);
	anime.UpdateNodeSize(3,5.0,5.0);
	anime.UpdateNodeSize(4,5.0,5.0);
	anime.SetMaxPktsPerTraceFile(500000000000); /* max size allocated to avoid max packet exceeded warning */	
	
	/* this is to instantiate bulk send application with tcp protocol on nodes */
	BulkSendHelper source1("ns3::TcpSocketFactory",
				InetSocketAddress(interface1.GetAddress(1),  8888));
	
	source1.SetAttribute("MaxBytes", UintegerValue(0)); /* send the max bytes of data */
	source1.SetAttribute("SendSize", UintegerValue(100));
	
	/* installing the send application on nodes */	
	auto cliapps = source1.Install(nodes.Get(0));
	cliapps.Start(Seconds(0.0));
	cliapps.Stop(Seconds(15.0));
	
	/* this is to instantiate packet sink application with tcp protocol connection
	 * on nodes
	 * */
	PacketSinkHelper sinkhelper("ns3::TcpSocketFactory", 
			InetSocketAddress(Ipv4Address::GetAny(),8888));
	
	/* install the sink application on single nodes */	
 	auto serverapp = sinkhelper.Install(nodes.Get(1));
	serverapp.Start(Seconds(0.0));
	serverapp.Stop(Seconds(15.0));
	
	/* this is to instantiate bulk send application with tcp protocol on nodes */
	BulkSendHelper source2("ns3::TcpSocketFactory",
				InetSocketAddress(interface1.GetAddress(1),  8888));
	
	source2.SetAttribute("MaxBytes", UintegerValue(0));
	source2.SetAttribute("SendSize", UintegerValue(100));

	/* installing the send application on nodes */	
	auto cliapps1 = source2.Install(nodes.Get(2));
	cliapps1.Start(Seconds(0.0));
	cliapps1.Stop(Seconds(15.0));
	
	/* this is to instantiate bulk send application with tcp protocol on nodes */
	BulkSendHelper source3("ns3::TcpSocketFactory",
				InetSocketAddress(interface1.GetAddress(1),  8888));
	
	source3.SetAttribute("MaxBytes", UintegerValue(0));
	source3.SetAttribute("SendSize", UintegerValue(100));
	
		
	/* installing the send application on nodes */	
	auto cliapps3 = source3.Install(nodes.Get(4));
	cliapps3.Start(Seconds(3.0));
	cliapps3.Stop(Seconds(15.0));
	
	/* this is to instantiate bulk send application with tcp protocol on nodes */
	BulkSendHelper source4("ns3::TcpSocketFactory",
				InetSocketAddress(interface1.GetAddress(1),  8888));
	
	source4.SetAttribute("MaxBytes", UintegerValue(0));
	source4.SetAttribute("SendSize", UintegerValue(100));
	
	/* installing the send application on nodes */	
	auto cliapps4 = source4.Install(nodes.Get(4));
	cliapps4.Start(Seconds(7.0));
	cliapps4.Stop(Seconds(12.0));
	
	/* Run the simulation for 60 secs*/
	Simulator::Stop(Seconds(60));

	/* schedule the throughputs to calculate throughput and avg throughput */
	Simulator::Schedule(Seconds(0.0),&CalculateThroughput);
	Simulator::Schedule(Seconds(15.0),&averageThroughput);

	Simulator::Run();

	Simulator::Destroy();

	flowMonitor->SerializeToXmlFile("throughput.xml",true,true);

	return 0;
}

void CalculateThroughput()
{
	double val ;
	vector<pair<double,double> > log[2000];
	
	/* An object that monitors and reports back packet flows observed during a simulation.*/
	std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats();
	
	Time now = Simulator::Now();	
	
	/* Iterate through all the bytes */
	for(std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin(); i != stats.end(); ++i)
	{
		val = (i->second.rxBytes - vec2[i->first - 1])*4.0 / 1000/ 1000;
		vec2[i->first - 1] = i->second.rxBytes;
		
		/* calculate for four flows */
		if( (i->first == 1) ||
			(i->first == 2) ||
			(i->first == 5) ||
			(i->first == 7)
		)
		{
			log[i->first - 1].push_back(make_pair(now.GetSeconds(),val));
			cout << i->first<< " " << now.GetSeconds() << " " << val << endl;
			
			/* redirect all the data to respective files according to the flow which will be the input to gnuplot */	
			if(i->first == 1) {
				flow1_file << i->first << " " << now.GetSeconds() << " " << val << "\n" << std::flush;
			} else if (i->first == 2) {
				flow2_file << i->first << " " << now.GetSeconds() << " " << val << "\n" << std::flush;
			} else if ( i->first == 5) {
				flow5_file << i->first << " " << now.GetSeconds() << " " << val << "\n" << std::flush;
			} else if ( i->first == 7) {
				flow7_file << i->first << " " << now.GetSeconds() << " " << val << "\n" << std::flush;
			} else {
				/* do nothing */
			}
		}
	}
	
	/* schedule at every 100ms */	
	Simulator::Schedule(MilliSeconds(100), &CalculateThroughput);
}

void averageThroughput()
{
	float totalthroughput = 0;
	float avgthroughput = 0;
	int total = 0;
	
	/* provides a method to translate raw packet data into abstract ``flow identifier'' and ``packet identifier'' parameters. */
	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flwHelper.GetClassifier());
	std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats();
	
	/* calculate the average of each flow's throughput */
	for( std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin(); i!= stats.end(); ++i)
	{
		/* calculate for four flows */
		if( (i->first == 1) ||
			(i->first == 2) ||
			(i->first == 5) ||
			(i->first == 7)
		  )
		{
			std::cout << "------------------------------------------------------------------" << std::endl;
			/* tuple as source, destination address, protocol, src port, dst port*/
			ns3::Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);

			std::cout << " Flow" << i->first << "(" << t.sourceAddress << "->" << t.destinationAddress << ")\n";
			std::cout << " Tx Packets: " << i->second.txPackets << "\n";
			std::cout << " Tx Bytes: " << i->second.txBytes << endl;
			std::cout << " TxOffered: " << i->second.txBytes * 8.0/(i->second.timeLastTxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1000/1000 << "Mbps\n";
			std::cout << " Rx Packets: " << i->second.rxPackets << "\n";
			std::cout << " Rx Bytes: " << i->second.rxBytes << "\n";
			std::cout << " Throughput: " << i->second.rxBytes*8.0/(i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1000/1000 << " Mbps\n";
			totalthroughput = totalthroughput + ( i->second.rxBytes * 8.0/(i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1000/1000);
			total++;
		}
	}
	
	avgthroughput = totalthroughput/total;

	std::cout << "------------------------------------------------------------------" << std::endl;
	std::cout << "avg throughput: " << avgthroughput << endl;
}

