#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"

/* Network flow 
 * 0		1
 * A ---------- E
 * 		|
 * 2	 3	|
 * B ----D-------
 *
 * 4	 |
 * C -----
 *
 *
 * */

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Project2_Sadhan");

int main(int argc, char* argv[])
{
	NodeContainer nodes;
	nodes.Create(5);
	
	/*
	NodeContainer nAnE;
	NodeContainer nBnD;
	NodeContainer nDnE;
	NodeContainer nCnD;
	NodeContainer nDnE;

	
	 nodes.Add(nAnE);		
	nodes.Add(nBnD);		
	nodes.Add(nDnE);		
	nodes.Add(nCnD);		
	nodes.Add(nDnE);		
	*/

	PointToPointHelper p2p;
	p2p.SetDeviceAttribute("DataRate", StringValue("100Mbps"));
	p2p.SetChannelAttribute("Delay", StringValue("0.5ms"));

	NetDeviceContainer p2pDev1;
	NetDeviceContainer p2pDev2;
	NetDeviceContainer p2pDev3;
	NetDeviceContainer p2pDev4;
	p2pDev1 = p2p.Install(nodes.Get(0),nodes.Get(1));	
	p2pDev2 = p2p.Install(nodes.Get(2),nodes.Get(3));
	p2pDev3 = p2p.Install(nodes.Get(3), nodes.Get(1));
	p2pDev4 = p2p.Install(nodes.Get(4), nodes.Get(3));

	InternetStackHelper IntStack;
	IntStack.Install(nodes);
	
	Ipv4AddressHelper address1;
	Ipv4AddressHelper address2;
	Ipv4AddressHelper address3;
	Ipv4AddressHelper address4;
	//Ipv4AddressHelper address5;
	address1.SetBase("10.1.1.0","255.255.255.0");
	address2.SetBase("10.1.1.1","255.255.255.0");
	address3.SetBase("10.1.1.2","255.255.255.0");
	address4.SetBase("10.1.1.3","255.255.255.0");
	
	Ipv4InterfaceContainer interfaces1 = address1.Assign(p2pDev1);	
	Ipv4InterfaceContainer interfaces2 = address2.Assign(p2pDev2);	
	Ipv4InterfaceContainer interfaces3 = address3.Assign(p2pDev3);	
	Ipv4InterfaceContainer interfaces4 = address4.Assign(p2pDev4);

	//Ipv4GlobalRoutingHelper::PopulateRoutingTable();
	
	UdpEchoServerHelper serverA(10);
	UdpEchoServerHelper serverB(11);
	UdpEchoServerHelper serverC(12);
	
	ApplicationContainer appA = serverA.Install(nodes.Get(0));
	ApplicationContainer appB = serverB.Install(nodes.Get(1));
	ApplicationContainer appC = serverC.Install(nodes.Get(2));
	
	appA.Start(Seconds(1.0));
	appA.Stop(Seconds(15.0));

	appB.Start(Seconds(1.0));
	appB.Stop(Seconds(15.0));

	appC.Start(Seconds(3.0));
	appC.Start(Seconds(15.0));
	
	UdpEchoClientHelper echoClient(interfaces1.GetAddress(1),10);
	echoClient.SetAttribute("MaxPackets",UintegerValue(1));
	echoClient.SetAttribute("Interval",TimeValue(Seconds(1.0)));
	echoClient.SetAttribute("PacketSize",UintegerValue(1024));

	UdpEchoClientHelper echoClient1(interfaces2.GetAddress(1),11);
	echoClient1.SetAttribute("MaxPackets",UintegerValue(1));
	echoClient1.SetAttribute("Interval",TimeValue(Seconds(1.0)));
	echoClient1.SetAttribute("PacketSize",UintegerValue(1024));
	
	ApplicationContainer clientAppD = echoClient.Install(nodes.Get(3));
	clientAppD.Start(Seconds(2.0));
	clientAppD.Stop(Seconds(6.0));

	ApplicationContainer clientAppE = echoClient1.Install(nodes.Get(4));
	clientAppD.Start(Seconds(7.0));
	clientAppD.Stop(Seconds(11.0));
	
	AnimationInterface anime("project2.xml");
	anime.SetConstantPosition(nodes.Get(0),1.0,2.0);
	anime.SetConstantPosition(nodes.Get(1),2.0,3.0);
	anime.SetConstantPosition(nodes.Get(2),3.0,4.0);
	anime.SetConstantPosition(nodes.Get(3),4.0,5.0);
	anime.SetConstantPosition(nodes.Get(4),5.0,6.0);

	Simulator::Run();
	Simulator::Destroy();

	return 0;
}
