#include "ros/ros.h"
#include "std_msgs/String.h"
#include <boost/lexical_cast.hpp>
#include <boost/thread/thread.hpp>
#include <sstream>

#include "graph_wrapper.h"



///Xgtk external graphviz library/plugin (only plugin, there's no need to create a library since it can be linked internally). 
extern gvplugin_library_t gvplugin_xgtk_LTX_library;

///Additional thread to run the graphviz plug-in
pthread_t graph_thread;

extern "C" 
{
	/** 
	  \brief External function used to install a gv plugin from a library
	  */
	extern void
		gvconfig_plugin_install_from_library(GVC_t*,char*,gvplugin_library_t*);
}

///Graph context pointer, used in the xgtk plug-in
GVGraph*graph_context;

///Draw mutex, used in the xgtk plug-in
pthread_mutex_t _draw_mutex;

void*GraphicalThread(void*data)
{

	graph_context=new GVGraph("graph 123");
	gvconfig_plugin_install_from_library(graph_context->getGVCcontext(), NULL, &gvplugin_xgtk_LTX_library);

	//graph_context->addNode("node1");
	//graph_context->setRootNode("node1");
	//graph_context->addNode("node2");
	//graph_context->addNode("node3");
	//graph_context->addEdge("node1", "node3");

	graph_context->applyLayout();

	ROS_INFO("in thread");
	graph_context->startRender();

	ROS_INFO("in thread");
	delete graph_context;

	return NULL;
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "talker");
	ros::NodeHandle n;
	ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
	ros::Rate loop_rate(1);


	pthread_create( &graph_thread, NULL, GraphicalThread,NULL);

	ros::Duration(1).sleep();
	pthread_mutex_lock(&(_draw_mutex));
	graph_context->addNode("node 1");
	graph_context->setRootNode("node 1");

	graph_context->addNode("node 76");

	graph_context->addNode("node 761");
	graph_context->addNode("node 6751");
	graph_context->addNode("node 451");

	graph_context->addEdge("node 76", "node 6751");

	graph_context->setNodeAttribute("node 76", "image","house.png");
	graph_context->setNodeAttribute("node 76", "imagescale","true");
	graph_context->setNodeAttribute("node 76", "fixedsize","true");
	//graph_context->setNodeAttribute("node 76", "label","uvas safisjkdoifjsidofjoij\nsfsdf\nsdofkspofdk");

	//graph_context->setNodeAttribute("node 76", "label","<<TABLE BORDER=0 CELLBORDER=1 CELLSPACING=0><TR><TD>left</TD><TD PORT=f1>mid dle</TD><TD PORT=f2>right</TD></TR></TABLE>>");

	graph_context->setNodeAttribute("node 76", "labelloc","b");
	graph_context->setNodeAttribute("node 76", "shape","box");
	graph_context->setNodeAttribute("node 76", "height","2.5");
	graph_context->setNodeAttribute("node 76", "width","1.5");

	graph_context->addEdge("node 1", "node 6751");
	graph_context->addEdge("node 451", "node 6751");
	graph_context->addEdge("node 1", "node 451");
	graph_context->addEdge("node 76", "node 451");

	graph_context->setEdgeAttribute("node 76", "node 6751","label","on_top");
	pthread_mutex_unlock(&(_draw_mutex));

	int count = 1;
	while (ros::ok())
	{
		std_msgs::String msg;

		std::stringstream ss;
		ss << "hello world " << count;
		msg.data = ss.str();

		ROS_INFO("%s", msg.data.c_str());
		chatter_pub.publish(msg);

		ros::spinOnce();

		loop_rate.sleep();
		++count;


		//graph_context->clearNodes();

		//graph_context=new GVGraph("graph 123");
		//gvconfig_plugin_install_from_library(graph_context->getGVCcontext(), NULL, &gvplugin_xgtk_LTX_library);


		pthread_mutex_lock(&(_draw_mutex));
		std::string prev_node_name = "node " + boost::lexical_cast<std::string>(count-1);
		std::string node_name = "node " + boost::lexical_cast<std::string>(count);
		ROS_INFO("Adding a new node named %s",node_name.c_str());
		graph_context->addNode(node_name);
		graph_context->setNodeAttribute(node_name,"shape","record");
		graph_context->setNodeAttribute(node_name,"label","sdf | dsf | dsf | ewr");
		graph_context->addEdge(prev_node_name, node_name);
		graph_context->setRootNode(node_name);

		pthread_mutex_unlock(&(_draw_mutex));
		//graph_context->addNode("node3");
		//graph_context->addEdge("node1", "node3");

	}


	return 0;
}
