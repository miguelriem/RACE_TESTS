#include <pluginlib/class_list_macros.h>                                                                                                               
#include <nodelet/nodelet.h>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <stdio.h>

#include "leveldb/db.h"
#include "PointerAddressUtils.h"
#include "test_nodelets/get_db_address.h"

namespace test_nodelet2_ns
{

	class test_nodelet2 : public nodelet::Nodelet, public PointerAddressUtils<leveldb::DB>
	{
		public:
			test_nodelet2(): value_(0){} //empty constructor  

		private:
			virtual void onInit() //mandatory method for nodelet manager to call
			{
				//Declare a node handle
				ros::NodeHandle& private_nh = getPrivateNodeHandle();

				//Initial config of publish subscribers
				//pub = private_nh.advertise<sensor_msgs::PointCloud2>("/tn2_pc_out", 1);
				//sub = private_nh.subscribe("/tn1_pc_out", 1, &test_nodelet2::callback, this);
				sub = private_nh.subscribe("/camera/depth_registered/points", 1, &test_nodelet2::callback, this);

				//Config service clients
				ros::ServiceClient client = private_nh.serviceClient<test_nodelets::get_db_address>("/get_db_address");

				//get db address by calling /get_db_address service
				test_nodelets::get_db_address srv;
				if (client.call(srv))
				{
					convertAddressStringToPointer(srv.response.mem_address, &ptr_db);
					ROS_INFO("Address to db is %s. Creating local pointer to db (pointed to address %p)", srv.response.mem_address.c_str(),ptr_db);
				}
				else
				{
					ROS_ERROR("Nodelet2: Failed to call service get_db_address");
				}

			}

			void callback(const sensor_msgs::PointCloud2::ConstPtr& input)
			{
				static int i=77;
				//sensor_msgs::PointCloud2::Ptr output(new sensor_msgs::PointCloud2());
				//ROS_INFO("Nodelet2: Received a msg");
				////NODELET_DEBUG("Adding %f to get %f", value_, output->data);
				//pub.publish(output);

				//Define a new value for key 3	
				i++;
				std::string ss = "value " + boost::lexical_cast<std::string>(i);
				ROS_INFO("Adding (from nodelet2) another key 3 = %s", ss.c_str());

				//putting new key 3 value on the db
				leveldb::Status status;
				status = ptr_db->Put(leveldb::WriteOptions(), "key3", ss); //Put some info on the database

				//putting value on key 1
				status = ptr_db->Put(leveldb::WriteOptions(), "key1", ss); //Put some info on the database

			}

			ros::Publisher pub;
			ros::Subscriber sub;
			double value_;
			leveldb::DB* ptr_db;
	};

	PLUGINLIB_DECLARE_CLASS(test_nodelet2_ns, test_nodelet2, test_nodelet2_ns::test_nodelet2, nodelet::Nodelet);
}
