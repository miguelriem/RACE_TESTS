#include "ros/ros.h"
#include "std_msgs/String.h"
#include <boost/lexical_cast.hpp>
#include <boost/thread/thread.hpp>
#include <boost/regex.hpp>
#include <boost/graph/graphviz.hpp>
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

//#include <graphviz/gvc.h>
//#include <graphviz/graph.h>

//struct graph_t
      //: boost::adjacency_list<boost::vecS, // OutEdgeList...
                              //boost::vecS, // VertexList
                              //boost::bidirectionalS, // Directed
                              //vertex_ptr, // vertex property
                              //edge_ptr> // edge property

//#define STRINGIZE(A) #A
  ////see http://www.graphviz.org/content/node-shapes for reference.
  //const char* table_str = STRINGIZE(
      //<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0" CELLPADDING="4">
      //%s
      //<TR>
      //%s
      //%s
      //</TR>
      //%s
      //%s
      //</TABLE>
  //);

  //const char* input_str = STRINGIZE(
      //<TD PORT="i_%s" BGCOLOR="springgreen">%s</TD>
  //);

  //const char* cell_str = STRINGIZE(<TD ROWSPAN="%d" COLSPAN="%d" BGCOLOR="khaki">%s</TD>
  //);

  //const char* param_str_1st = STRINGIZE(
      //<TD PORT="p_%s" BGCOLOR="lightblue">%s</TD>
  //);

  //const char* param_str_N = STRINGIZE(
      //<TR>
      //<TD PORT="p_%s" BGCOLOR="lightblue">%s</TD>
      //</TR>
  //);

  //const char* output_str = STRINGIZE(
      //<TD PORT="o_%s" BGCOLOR="indianred1">%s</TD>
  //);

  //struct vertex_writer
  //{
    //graph_t* g;

    //vertex_writer(graph_t* g_) : g(g_) { }

    //std::string
    //htmlescape(const std::string& in)
    //{
      //const boost::regex esc_lt("[<]");
      //const std::string rep_lt("&lt;");
      //const boost::regex esc_gt("[>]");
      //const std::string rep_gt("&gt;");

      //std::string htmlescaped_name = in;
      //htmlescaped_name = boost::regex_replace(htmlescaped_name, esc_lt, rep_lt, boost::match_default);
      //htmlescaped_name = boost::regex_replace(htmlescaped_name, esc_gt, rep_gt, boost::match_default);
      //return htmlescaped_name;
    //}

    //void
    //operator()(std::ostream& out, graph_t::vertex_descriptor vd)
    //{

      //cell_ptr c = (*g)[vd]->cell();
      //int n_inputs = c->inputs.size();
      //int n_outputs = c->outputs.size();
      //int n_params = c->parameters.size();
      //std::string htmlescaped_name = htmlescape(c->name());

      //std::string inputs;
      //BOOST_FOREACH(const tendrils::value_type& x, c->inputs)
          //{
            //std::string key = x.first;
            //if (inputs.empty())
              //inputs = "<TR>\n";
            //inputs += boost::str(boost::format(input_str) % key % key) + "\n";
          //}
      //if (!inputs.empty())
        //inputs += "</TR>";

      //std::string outputs;
      //BOOST_FOREACH(const tendrils::value_type& x, c->outputs)
          //{
            //std::string key = x.first;
            //if (outputs.empty())
              //outputs = "<TR>\n";
            //outputs += boost::str(boost::format(output_str) % key % key) + "\n";
          //}
      //if (!outputs.empty())
        //outputs += "</TR>";

      //std::string cellrow = boost::str(
          //boost::format(cell_str) % std::max(1, n_params) % int(std::max(1, std::max(n_inputs, n_outputs)))
          //% htmlescaped_name);
      //std::string p1, pN;
      //BOOST_FOREACH(const tendrils::value_type& x, c->parameters)
          //{
            //std::string key = x.first;
            //if (p1.empty())
              //p1 = boost::str(boost::format(param_str_1st) % key % key) + "\n";
            //else
              //pN += boost::str(boost::format(param_str_N) % key % key) + "\n";
          //}

      //std::string table = boost::str(boost::format(table_str) % inputs % cellrow % p1 % pN % outputs);
      //out << "[label=<" << table << ">]";
    //}
  //};

  //struct edge_writer
  //{
    //graph_t* g;

    //edge_writer(graph_t* g_) : g(g_) { }

    //void
    //operator()(std::ostream& out, graph_t::edge_descriptor ed)
    //{
      //out << "[headport=\"i_" << (*g)[ed]->to_port()
          //<< "\" tailport=\"o_" << (*g)[ed]->from_port() << "\"]";
    //}
  //};

  //struct graph_writer
  //{
    //void
    //operator()(std::ostream& out) const
    //{
      //out << "graph [rankdir=TB, ranksep=1]" << std::endl;
      //out << "edge [labelfontsize=8]" << std::endl;
      //out << "node [shape=plaintext]" << std::endl;
    //}
  //};



int main(int argc, char **argv)
{
	//ros::init(argc, argv, "talker");
	//ros::NodeHandle n;
	//ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
	//ros::Rate loop_rate(1);

	std::ostream out;
	//Agraph_t *g;
	graph_t *g;

	//boost::write_graphviz(out, g, vertex_writer(&g), edge_writer(&g), graph_writer());

	//Agraph_t *g;

	//Agnode_t *n, *m;

	//Agedge_t *e;

	//Agsym_t *a;

	//GVC_t *gvc;

	//[> set up a graphviz context <]

	//gvc = gvContext();


	//[> parse command line args - minimally argv[0] sets layout engine <]
	//char* args[] = {
		//"dot",
		//"-Tpng", [> gif output <]
		//"-oabc.png" [> output to file abc.gif <]
	//};
	//gvParseArgs (gvc, sizeof(args)/sizeof(char*), args);
	////gvParseArgs(gvc, argc, argv);

	//[> Create a simple digraph <]
	//g = agopen("g", AGGRAPH);
	//n = agnode(g, "n");
	//m = agnode(g, "m1");
	//e = agedge(g, n, m);

	//[> Set an attribute - in this case one that affects the visible rendering <]

	//agsafeset(n, "color", "red", "");

	////char* l = agstrdup_html("< <TABLE>  </TABLE> >");
	////agset (n,"label",l);
	////std::string ht	=htmlescape(table_str);
	//std::string ht	=htmlescape(cell_str);
	//printf("table_str=%s\n",table_str);
	//printf("ht=%s\n",ht.c_str());

	//agset (n,"shape",(char*)"none");
	//agset (n,"label",(char*)ht.c_str());

	//[> Compute a layout using layout engine from command line args <]
	//gvLayoutJobs(gvc, g);
	//[> Write the graph according to -T and -o options <]
	//gvRenderJobs(gvc, g);
	//[> Free layout data <]
	//gvFreeLayout(gvc, g);
	//[> Free graph structures <]

	//agclose(g);

	/* close output file, free context, and return number of errors */

	return (gvFreeContext(gvc));
	//pthread_create( &graph_thread, NULL, GraphicalThread,NULL);

	//ros::Duration(1).sleep();
	//pthread_mutex_lock(&(_draw_mutex));


	////graph_context->addNode("LDB");
	//graph_context->addNode("Scene Perception");

	////Object Detection Node
	//graph_context->addNode("Object Detection");
	////graph_context->setNodeAttribute("Object Detection", "shape","record");
	//graph_context->setNodeAttribute("Object Detection", "shape","none");
	////graph_context->setNodeAttribute("Object Detection", "label","{ {Obj_X_TID |asd } |Object Detection| f}");
	////graph_context->setNodeAttribute("Object Detection", "label","<	<table cellpadding=\"0\" cellborder=\"0\" cellspacing=\"0\" border=\"0\"> <tr> <td bgcolor=\"orange\">abc</td> <td bgcolor=\"yellow\">def</td> </tr> </table> >");
	//graph_context->setNodeAttribute("Object Detection", "label","[label=<</TABLE> >]");
	////char* l;
	////l = (char*) malloc(500);   
	////l= agstrdup_html("< <TABLE>  </TABLE> >");

	//////agset (n,"label",l);
	////ROS_INFO("line %d",__LINE__);
	//////graph_context->setNodeAttribute("Object Detection", "label",agstrdup_html("<<BR> Isto é Bold </BR>>"));
	////ROS_INFO("line %d",__LINE__);


	////graph_context->setNodeAttribute("Object Detection", "color","{ {brown1 | cadetblue1 } |brown1| brown1}");
	////graph_context->setNodeAttribute("Object Detection", "color","cadetblue1");
	////graph_context->setNodeAttribute("Object Detection", "bgcolor","brown1");
	////graph_context->setNodeAttribute("Object Detection", "style","filled");

	////a[shape="none", label=<
	////<table cellpadding="0" cellborder="0" cellspacing="0" border="0">
	////<tr>
	////<td bgcolor="orange">abc</td>
	////<td bgcolor="yellow">def</td>
	////</tr>
	////</table>
	////>]

	////graph_context->addNode("Object Tracking");
	////graph_context->addNode("Feature Extraction");
	////graph_context->addNode("Object Recognition");
	////graph_context->addNode("Object Anchoring");
	////graph_context->addNode("Object Conceptualizer");


	//graph_context->setRootNode("Object Detection");

	////graph_context->addEdge("Scene Perception", "Object Detection");
	////graph_context->setEdgeAttribute("Scene Perception", "Object Detection","label","point cloud");

	////graph_context->addEdge("Object Tracking", "Object Detection");
	////graph_context->setEdgeAttribute("Object Tracking", "Object Detection","label","TID; position; boundingbox");

	////graph_context->addEdge("Object Detection", "Object Tracking");
	////graph_context->setEdgeAttribute("Object Detection", "Object Tracking","label","Launch OT for new objects");
	////graph_context->setEdgeAttribute("Object Detection", "Object Tracking","color","brown1");


	////graph_context->addEdge("Object Detection", "Scene Perception");
	////graph_context->addNode("node 1");
	////graph_context->setRootNode("node 1");

	////graph_context->addNode("node 76");

	////graph_context->addNode("node 761");
	////graph_context->addNode("node 6751");
	////graph_context->addNode("node 451");

	////graph_context->addEdge("node 76", "node 6751");

	////graph_context->setNodeAttribute("node 76", "image","house.png");
	////graph_context->setNodeAttribute("node 76", "imagescale","true");
	////graph_context->setNodeAttribute("node 76", "fixedsize","true");
	////graph_context->setNodeAttribute("node 76", "label","uvas safisjkdoifjsidofjoij\nsfsdf\nsdofkspofdk");

	////graph_context->setNodeAttribute("node 76", "label","<<TABLE BORDER=0 CELLBORDER=1 CELLSPACING=0><TR><TD>left</TD><TD PORT=f1>mid dle</TD><TD PORT=f2>right</TD></TR></TABLE>>");

	////graph_context->setNodeAttribute("node 76", "labelloc","b");
	////graph_context->setNodeAttribute("node 76", "shape","box");
	////graph_context->setNodeAttribute("node 76", "height","2.5");
	////graph_context->setNodeAttribute("node 76", "width","1.5");

	////graph_context->addEdge("node 1", "node 6751");
	////graph_context->addEdge("node 451", "node 6751");
	////graph_context->addEdge("node 1", "node 451");
	////graph_context->addEdge("node 76", "node 451");

	////graph_context->setEdgeAttribute("node 76", "node 6751","label","on_top");
	//pthread_mutex_unlock(&(_draw_mutex));

	//int count = 1;
	//while (ros::ok())
	//{
	//std_msgs::String msg;

	//std::stringstream ss;
	//ss << "hello world " << count;
	//msg.data = ss.str();

	//ROS_INFO("%s", msg.data.c_str());
	//chatter_pub.publish(msg);

	//ros::spinOnce();

	//loop_rate.sleep();
	//++count;


	////graph_context->clearNodes();

	////graph_context=new GVGraph("graph 123");
	////gvconfig_plugin_install_from_library(graph_context->getGVCcontext(), NULL, &gvplugin_xgtk_LTX_library);


	////pthread_mutex_lock(&(_draw_mutex));
	////std::string prev_node_name = "node " + boost::lexical_cast<std::string>(count-1);
	////std::string node_name = "node " + boost::lexical_cast<std::string>(count);
	////ROS_INFO("Adding a new node named %s",node_name.c_str());
	////graph_context->addNode(node_name);
	////graph_context->setNodeAttribute(node_name,"shape","record");
	////graph_context->setNodeAttribute(node_name,"label","sdf | dsf | dsf | ewr");
	////graph_context->addEdge(prev_node_name, node_name);
	////graph_context->setRootNode(node_name);

	////pthread_mutex_unlock(&(_draw_mutex));
	////graph_context->addNode("node3");
	////graph_context->addEdge("node1", "node3");

	//}


	return 0;
}
