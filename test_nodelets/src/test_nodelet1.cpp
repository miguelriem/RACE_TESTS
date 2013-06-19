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


			void callback(const sensor_msgs::PointCloud2::ConstPtr& input)
			{
				//ROS_INFO("Nodelet1: Received a msg");

				//sensor_msgs::PointCloud2::Ptr output(new sensor_msgs::PointCloud2());
				//pub.publish(output);

				static int i=0;
				i++;
				std::string ss = "value " + boost::lexical_cast<std::string>(i);
				ROS_INFO("This is the new value for key1=%s",ss.c_str());
				leveldb::Status status;
				status = db->Put(leveldb::WriteOptions(), "key1", ss);

				sensor_msgs::PointCloud2 output_noptr;
				pub.publish(output_noptr);

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
				leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
				assert(status.ok());
				if (!status.ok()) std::cout << status.ToString() << std::endl;
				ROS_INFO("DB created. Status is %s address is %p\n Initial DB status is", status.ToString().c_str(), db);

				//print everything on the db
				leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
				for (it->SeekToFirst(); it->Valid(); it->Next()) {
					ROS_INFO("%s:%s",it->key().ToString().c_str(), it->value().ToString().c_str());
				}
				assert(it->status().ok());  // Check for any errors found during the scan
				delete it;

				//Add some stuff
				status = db->Put(leveldb::WriteOptions(), "key1", "value1"); //Put some info on the database

				//std::string ptr_address = convertPointerToStringAddress(db);
				//ROS_INFO("Passed address to string: %s", ptr_address.c_str());

				//leveldb::DB* ptr_db;
				//convertAddressStringToPointer(ptr_address, &ptr_db);

				//ROS_INFO("DB pointer created. address is %p",ptr_db);


				//ROS STUFF, publishers, subscribers and services
				//ros::NodeHandle& private_nh = getPrivateNodeHandle();
				ros::NodeHandle& private_nh = getNodeHandle();

				pub = private_nh.advertise<sensor_msgs::PointCloud2>("/tn1_pc_out", 1);
				sub = private_nh.subscribe("/camera/depth_registered/points", 1, &test_nodelet1::callback, this);
				service = private_nh.advertiseService("/get_db_address", &test_nodelet1::get_db_address, this);

				ROS_INFO("Starting nodelet1");

				/* _________________________________
				   |                                 |
				   |     USING ANEESH functions      |
				   |_________________________________| */


				//ROS_INFO("STEP 1: allocate space");
				//char* s;
				//ROS_INFO("STEP 2: convert to char*");
				//s = convert_leveldb_ptr2str(&db);
				////Aneesh :- donit forget to remove the string later on from
				////memory :P
				//ROS_INFO("STEP 3: put in a std::string");
				//std::string str_address = s;
				//ROS_INFO("Address in str =%s", str_address.c_str());
				////ROS_INFO("Address in str = %s", str_address.c_str());

				//leveldb::DB* ptr_db=NULL;

				//ROS_INFO("before DB pointer adress %p",ptr_db);
				//ROS_INFO("STEP 4: Get back the ptr");
				//convert_str2leveldb_ptr(s, &ptr_db);

				//ROS_INFO("after DB pointer adress %p",ptr_db);

				//ROS_INFO("STEP 5: try to put something in the database using the pointer");


				//status = ptr_db->Put(leveldb::WriteOptions(), "key2", "value1");

				//int x;   
				//std::stringstream ss;
				//ss << std::hex << "fffefffe";
				//ss >> x;
				//try
				//{
				//int x = boost::lexical_cast<int>(str_address.c_str());
				//}
				//catch( boost::bad_lexical_cast const& ) {
				//std::cout << "Error: input string was not valid" << std::endl;
				//}
				//int int_address;
				//s >> int_address;

				//leveldb::DB* ptr_db = reinterpret_cast<leveldb::DB*>(str_address.c_str());
			}

			char *convert_leveldb_ptr2str(leveldb::DB **p)
			{

				ROS_INFO("1");
				//ROS_INFO("inside DB pointer adress %p",p);
				//printf("inside DB pointer adress %p\n",p);
				fflush(stdout);
				//int size_of_leveldb_ptr = sizeof(leveldb::DB *) +1;

				//Aneesh: The reason I add 3 is because it seems the sizeof(pointer)
				//does not include "0x" at the beginning of the printed pointer
				//value. So we need to accommodate these two extra characters 
				//+ 1 more for the empty space afterwards
				int size_of_leveldb_ptr = sizeof(*p) +3;
				printf("size is %d\n", size_of_leveldb_ptr);
				printf("pointer val =  %p\n", *p);
				ROS_INFO("2");
				char *str_p = (char *)malloc(size_of_leveldb_ptr);
				//char *str_p = (char *)malloc(8);
				ROS_INFO("3");
				sprintf(str_p, "%p", *p); 
				ROS_INFO("4");

				return str_p;
			}

			void convert_str2leveldb_ptr(char *str_p, leveldb::DB **ptr)
			{
				ROS_INFO("1");
				leveldb::DB *new_ptr;
				ROS_INFO("2");
				sscanf(str_p,"%p", &new_ptr);
				ROS_INFO("3");
				*ptr = new_ptr;
				ROS_INFO("4");
			}

			ros::Publisher pub;
			ros::Subscriber sub;
			ros::ServiceServer service;
			double value_;

			leveldb::DB* db;
			leveldb::Options options;
	};

	PLUGINLIB_DECLARE_CLASS(test_nodelet1_ns, test_nodelet1, test_nodelet1_ns::test_nodelet1, nodelet::Nodelet);
}


