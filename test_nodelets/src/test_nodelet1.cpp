#include <pluginlib/class_list_macros.h>                                                                                                               
#include <nodelet/nodelet.h>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <stdio.h>

#include <boost/lexical_cast.hpp>

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <iostream>
#include "leveldb/db.h"
#include "PointerAddressUtils.h"
#include "test_nodelets/get_db_address.h"

using namespace std;

namespace test_nodelet1_ns
{

	class test_nodelet1 : public nodelet::Nodelet, public PointerAddressUtils<leveldb::DB>
	{
		public:
			test_nodelet1(): value_(0){} //empty constructor  

			bool get_db_address(test_nodelets::get_db_address::Request  &req, test_nodelets::get_db_address::Response &res)
			{
				//Dont care about the request, just pass the db pointers address
				//to a string in response
				res.mem_address = convertPointerToStringAddress(db);
				return true;
			}


			/**
			 * @brief Is called when a point cloud is received
			 *
			 * @param input a ptr to the pc
			 */
			void callback(const sensor_msgs::PointCloud2::ConstPtr& input)
			{
				static int i=0;

				//Compute the new value of key1
				i++;
				std::string ss = "value " + boost::lexical_cast<std::string>(i);
				ROS_INFO("This is the new value for key1=%s",ss.c_str());

				//Set the new value for key1
				leveldb::Status status;
				status = db->Put(leveldb::WriteOptions(), "key1", ss);

				//print everything on the db
				ROS_INFO("from nodelet 1: full db keys");
				leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
				for (it->SeekToFirst(); it->Valid(); it->Next()) {
					ROS_INFO("%s:%s",it->key().ToString().c_str(), it->value().ToString().c_str());
				}
				assert(it->status().ok());  // Check for any errors found during the scan
				delete it;

			}


			virtual void onInit() //mandatory method for nodelet manager to call
			{

				//DB stuff
				options.create_if_missing = true;
				leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db); // open the databaser
				assert(status.ok());

				if (!status.ok()) std::cout << status.ToString() << std::endl;
				ROS_INFO("DB created. Status is %s address is %p\n Initial DB status is", status.ToString().c_str(), db);

				//print everything on the db. before adding anything else
				leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
				for (it->SeekToFirst(); it->Valid(); it->Next()) {
					ROS_INFO("%s:%s",it->key().ToString().c_str(), it->value().ToString().c_str());
				}
				assert(it->status().ok());  // Check for any errors found during the scan
				delete it;

				//Add some stuff to the db
				status = db->Put(leveldb::WriteOptions(), "key1", "value1"); //Put some info on the database


				// do some ros speficif initial configuration
				ros::NodeHandle& private_nh = getNodeHandle(); //get a pointer to the node handle
				pub = private_nh.advertise<sensor_msgs::PointCloud2>("/tn1_pc_out", 1); //advertive pc_out
				sub = private_nh.subscribe("/camera/depth_registered/points", 1, &test_nodelet1::callback, this);
				service = private_nh.advertiseService("/get_db_address", &test_nodelet1::get_db_address, this);

				//Nodelet is prepared to run:
				ROS_INFO("Starting nodelet1");

			}

/* _________________________________
  |                                 |
  |           VARIABLES             |
  |_________________________________| */

			ros::Publisher pub;
			ros::Subscriber sub;
			ros::ServiceServer service;
			double value_;

			leveldb::DB* db;
			leveldb::Options options;
	};

	PLUGINLIB_DECLARE_CLASS(test_nodelet1_ns, test_nodelet1, test_nodelet1_ns::test_nodelet1, nodelet::Nodelet);
}


