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
#include <test_nodelets/StructSerialization.h>

using namespace std;

template <typename T>
class LevelDBUtils
{
	public:
//	LevelDBUtils<T>{};
//	~LevelDBUtils{};

};

static int const arr_size=1*1024*1024;
static char const seperate= ',';

#include <ros/message_traits.h>
#include <ros/serialization.h>


namespace my_rclass
{
template <class ContainerAllocator>
struct myClass_
{
    typedef myClass_<ContainerAllocator> Type;

    myClass_()
    : arrSize()
    , arrName()
    , data()  {
    } 

    myClass_(const ContainerAllocator& _alloc)
    : arrSize(arr_size)
    , arrName()
    , data(_alloc)  {
    } 

    typedef uint32_t _arrary_size;
    _arrary_size arrSize;

    typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _arrary_name;
    _arrary_name arrName;

    typedef std::vector<uint8_t, typename ContainerAllocator::template rebind<uint8_t>::other >  _data_type;
    _data_type data;

    typedef boost::shared_ptr< ::my_rclass::myClass_<ContainerAllocator> > Ptr;
    typedef boost::shared_ptr< ::my_rclass::myClass_<ContainerAllocator> const> ConstPtr;
    boost::shared_ptr<std::map<std::string, std::string> > __connection_header;

};

typedef ::my_rclass::myClass_<std::allocator<void> > myClass;

typedef boost::shared_ptr< ::my_rclass::myClass > myClassPtr;
typedef boost::shared_ptr< ::my_rclass::myClass const> myClassConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::my_rclass::myClass_<ContainerAllocator> & v)
{
    ros::message_operations::Printer< ::my_rclass::myClass_<ContainerAllocator> >::stream(s, "", v);
    return s;
}

} // namespace my_rclass


namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::my_rclass::myClass_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.arrSize);
      stream.next(m.arrName);
      stream.next(m.data);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct JointTrajectory_

} // namespace serialization
} // namespace ros



#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/array.hpp> 

#ifndef PFLN
#define PFLN printf("L=%d of file: %s \n", __LINE__,__FILE__);
#endif

class myBClass
{
    friend class boost::serialization::access;
    friend std::ostream & operator<<(std::ostream &os, const myBClass &mc);

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {   
        ROS_INFO("start serialize func !!");
        ar & arrSize;
        ar & arrName;
        ar & arrName2;
//        ar & data;
        ROS_INFO("start array serialization !!");
//        for (int i =0; i < arrSize ; i++)
//            ar & data[i];
    
        ROS_INFO("end serialize func !!");
    }   

public:
    int arrSize;
    std::string arrName;
    std::string arrName2;
//    int data[arr_size];
    myBClass() : 
        arrSize(arr_size)
    {}; 
};

std::ostream & operator<<(std::ostream &os, const myBClass &mc)
{
    ROS_INFO("start operator << !!");
    os << ' ' << mc.arrSize << (unsigned char)186 << mc.arrName << (unsigned char)186 << mc.arrName2 << '"';

//    for (int i =0; i < mc.arrSize ; i++)
//        os << '\n' << mc.data[i];

    ROS_INFO("end operator << !!");
    return os;
}


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


            void testBoostStr( int i)
            {
				leveldb::Status status;
                
                myBClass _mstr, _mstr2;
                std::stringstream _stm;

                std::string key = "key";
                std::string value;
                std::stringstream ss;
                ss << i;
                key += std::string(ss.str());

                _mstr.arrName = key;
                _mstr.arrSize = arr_size;
                _mstr.arrName2 = "arrName2";
//                _mstr.data[1] = 10+i;
//                _mstr.data[arr_size-2] = 2+i;

                ROS_INFO("The value of _mstr.arrName is =%s", _mstr.arrName.c_str());
                std::stringbuf _sb;
                std::ostream _os(&_sb);
                boost::archive::binary_oarchive _oa(_os);
                _oa << _mstr;

//                std::stringstream os(std::ios_base::binary| std::ios_base::out| std::ios_base::in);
//                {
//                    boost::archive::binary_oarchive oa(os, boost::archive::no_header);
//                    oa << _mstr;
//                }

//                ROS_INFO("The value of _mstr.data[arr_size-2] is =%d", _mstr.data[arr_size-2]);

//std::stringstream is(std::string(buf, buf+os.str().length()), flags);
                std::istream _is(&_sb);
                boost::archive::binary_iarchive _ia(_is);
//                ROS_INFO("The value of _mstr.data[arr_size-2] is =%d", _mstr.data[arr_size-2]);
                _ia >> _mstr2;
//                ROS_INFO("The value of _mstr.data[arr_size-2] is =%d", _mstr.data[arr_size-2]);

                ROS_INFO("The value of _mstr2.arrName is =%s", _mstr2.arrName.c_str());
                ROS_INFO("The value of _mstr2.arrName2 is =%s", _mstr2.arrName2.c_str());
//                ROS_INFO("The value of _mstr2.data[1] is =%d", _mstr2.data[1]);
//
                leveldb::ReadOptions ro;
//                wo.sync = true;
				status = db->Get(ro, key, &value);
//                if (status.ok()) {
//				    ROS_INFO("from nodelet 1: key: %s, size: %d", key.c_str(), value.size());
//                    memcpy(tmp, value.data(), value.size());
//                } else {
//				    ROS_INFO("from nodelet 1i, status is not ok: key: %s", key.c_str());
////                    *vallen = 0;
////                    if (!status.IsNotFound()) {
////                        SaveError(errptr, status);
////                    }
//                }
//				ROS_INFO("from nodelet 1 of tmp[arr_size-2]: %d", tmp[arr_size-2]);

                return;
            }

			/**
			 * @brief Is called when a point cloud is received
			 *
			 * @param input a ptr to the pc
			 */
			void callback(const sensor_msgs::PointCloud2::ConstPtr& input)
			{
				static int i=0;

				PFLN
				//Declare msg
				test_nodelets::StructSerialization our_msg;

				PFLN
				//Add stuff to msg
				our_msg.string1 = "hello";
				our_msg.vec_int1.push_back(100);
				our_msg.vec_int1.push_back(101);
				our_msg.vec_int1.push_back(102);

				PFLN
				//Serialize
				uint32_t serial_size = ros::serialization::serializationLength(our_msg);
				boost::shared_array<uint8_t> buffer(new uint8_t[serial_size]);
				ros::serialization::OStream stream(buffer.get(), serial_size);
				ros::serialization::serialize(stream, our_msg);	

				PFLN
				//WRITE Buffer TO DB


				//Deserialization
				uint32_t serial_size_des = ros::serialization::serializationLength(our_msg); //compute the msg size

				PFLN
				//Fill buffer with a serialized UInt32
				ros::serialization::IStream stream_des(buffer.get(), serial_size_des); //Get a istream from buffer

				PFLN
				test_nodelets::StructSerialization our_msg_des; //declare a new msg to be filled by the deserialization procedure
				ros::serialization::deserialize(stream_des, our_msg_des); //deserialize

				PFLN
					//print array values
				ROS_INFO("string1=%s\nsize vec_int1=%ld", our_msg_des.string1.c_str(), our_msg_des.vec_int1.size());
				for (size_t i=0; i<our_msg_des.vec_int1.size(); i++)
				{
					ROS_INFO("size vec_int1[%ld]=%ld", i,our_msg_des.vec_int1.at(i));
				}

				PFLN
				return;
//				ROS_INFO("The size of current point cloud is =%d", sizeof *input);
//				ROS_INFO("The size of current point cloud data:height is =%d", input->height);
//				ROS_INFO("The size of current point cloud data:row_step is =%d", input->row_step );
//				ROS_INFO("The size of current point cloud data is =%d", input->row_step * input->height);

				//Compute the new value of key1
//				i++;
//				std::string ss = "value " + boost::lexical_cast<std::string>(i);
//				ROS_INFO("This is the new value for key1=%s",ss.c_str());

				//Set the new value for key1
				leveldb::Status status;
                

				testPutIntArr(i);
				testGetIntArr(i);
                testBoostStr(i);
                //i++;

				//print everything on the db
				ROS_INFO("from nodelet 1: full db keys");
				leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
                std::string str;
				for (it->SeekToFirst(); it->Valid(); it->Next()) {
                    str += it->key().ToString();
                    str += " ";
				}
//				ROS_INFO("keys: %s", str.c_str());
				assert(it->status().ok());  // Check for any errors found during the scan
				delete it;

			}

            void testPutIntArr( int i)
            {
				leveldb::Status status;

                int tmp[arr_size]; 
                leveldb::Slice s((char *)tmp, sizeof(int)*arr_size);
                //tmp[1] = 10+i;
                tmp[1] = 77;
                //tmp[arr_size-2] = 2+i;
                tmp[arr_size-2] = 88;
                std::string key = "key";
                std::stringstream ss;
                ss << i;
                //key += std::string(ss.str());

                leveldb::WriteOptions wo;
//                wo.sync = true;
				status = db->Put(wo, key, s);

				ROS_INFO("Put: from nodelet 1: key: %s", key.c_str());
                return;
            }
    
            void testGetIntArr( int i)
            {
				leveldb::Status status;
                int tmp[arr_size]; 

                std::string key = "key";
                std::string value;
                std::stringstream ss;
                ss << i;
                //key += std::string(ss.str());

                leveldb::ReadOptions ro;
//                wo.sync = true;
				status = db->Get(ro, key, &value);
                if (status.ok()) {
				    ROS_INFO("from nodelet 1: key: %s, size: %d", key.c_str(), (int)value.size());
                    memcpy(tmp, value.data(), value.size());
                } else {
				    ROS_INFO("from nodelet 1i, status is not ok: key: %s", key.c_str());
//                    *vallen = 0;
//                    if (!s.IsNotFound()) {
//                    SaveError(errptr, s);
//                    }
                }
				ROS_INFO("Get: from nodelet 1 of tmp[1]: %d", tmp[1]);
				ROS_INFO("Get: from nodelet 1 of tmp[arr_size-2]: %d", tmp[arr_size-2]);

                return;
            }


//            void testRosBoostStr()
//            {
//				leveldb::Status status;
//
//                int tmp[arr_size]; 
//                ::my_rclass::myClass _mstr();
////                ::my_rclass::myClass _mstr(tmp);
//                ::my_rclass::myClass _mstr2;
//                std::stringstream _stm;
//
//                _mstr.arrName = "key1";
//                _mstr.arrSize = arr_size;
//                _mstr.data[1] = 10;
//                _mstr.data[arr_size-2] = 2;
//
//				ROS_INFO("The value of _mstr.arrName is =%s", _mstr.arrName.c_str());
//
//
//                uint32_t serial_size = ros::serialization::serializationLength(_mstr);
//                boost::shared_array<uint8_t> buffer(new uint8_t[serial_size]);
//
//				ROS_INFO("The size of serial is =%d", serial_size);
//
//                ros::serialization::OStream stream(buffer.get(), serial_size);
//                ros::serialization::serialize(stream, _mstr);
//
//                std::stringbuf _sb;
//
//                std::ostream _os(&_sb);
//                boost::archive::binary_oarchive _oa(_os);
//                _oa << _mstr;
//
//
//                std::istream _is(&_sb);
//                boost::archive::binary_iarchive _ia(_is);
//                _ia >> _mstr2;
//
//
//				ROS_INFO("The value of _mstr2.arrName is =%s", _mstr2.arrName.c_str());
//				ROS_INFO("The value of _mstr2.data[1] is =%d", _mstr2.data[1]);
//
//                _mstr2.arrName = "kkkkkkey22222222";
//                _mstr2.arrSize = arr_size;
//                _mstr2.data[1] = 11;
//                _mstr2.data[arr_size-2] = 3;
//
//				ROS_INFO("The size of _mstr is =%d", sizeof(_mstr));
//				ROS_INFO("The size of _mstr2 is =%d", sizeof(_mstr2));
//            }
//

			virtual void onInit() //mandatory method for nodelet manager to call
			{

				//DB stuff
				options.create_if_missing = true;
				//leveldb::Status status = leveldb::DB::Open(options, "/home/hmetal/tmp/testdb", &db); // open the databaser
				leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db); // open the databaser
				assert(status.ok());

				if (!status.ok()) std::cout << status.ToString() << std::endl;
				ROS_INFO("DB created. Status is %s address is %p\n Initial DB status is", status.ToString().c_str(), db);

				//print everything on the db. before adding anything else
//				leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
//				for (it->SeekToFirst(); it->Valid(); it->Next()) {
//					ROS_INFO("%s:%s",it->key().ToString().c_str(), it->value().ToString().c_str());
//				}
//				assert(it->status().ok());  // Check for any errors found during the scan
//				delete it;

//				//Add some stuff to the db
//				status = db->Put(leveldb::WriteOptions(), "key1", "value1"); //Put some info on the database


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


