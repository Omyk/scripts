#include "Navio/MPU9250.h"
#include "Navio/LSM9DS1.h"
#include "Navio/Util.h"
#include <unistd.h>

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "common_msgs/sensor_msgs"
#include <sstream>

void init_msg(sensor_msgs::Imu* imu_msg)
{
	imu_msg->orientation.x = 0.0f;
	imu_msg->orientation.y = 0.0f;
	imu_msg->orientation.z = 0.0f;
	imu_msg->orientation.w = 0.0f;

	imu_msg->orientation_covariance =      {1, 0, 0,
				 		0, 1, 0,
				 		0, 0, 1};

	imu_msg->angular_velocity.x = 0.0f;
	imu_msg->angular_velocity.y = 0.0f;
	imu_msg->angular_velocity.z = 0.0f;

	imu_msg->angular_velocity_covariance = {1, 0, 0,
				 		0, 1, 0,
				 		0, 0, 1};

	imu_msg->linear_acceleration.x = 0.0f;
	imu_msg->linear_acceleration.y = 0.0f;
	imu_msg->linear_acceleration.z = 0.0f;

	imu_msg->linear_acceleration_covariance =      {1, 0, 0,
				 			0, 1, 0,
				 			0, 0, 1};

}

void update_msg(sensor_msgs::Imu* imu_msg, InertialSensor* sensor)
{

	float ax, ay, az, gx, gy, gz, mx, my, mz;

	sensor->update();

	sensor->read_accelerometer(&ax, &ay, &az);
        sensor->read_gyroscope(&gx, &gy, &gz);
        //sensor->read_magnetometer(&mx, &my, &mz);

	//imu_msg->orientation.x = ax;
	//imu_msg->orientation.y = ay;
	//imu_msg->orientation.z = az;

	imu_msg->angular_velocity.x = gx;
	imu_msg->angular_velocity.y = gy;
	imu_msg->angular_velocity.z = gz;

	imu_msg->linear_acceleration.x = ax;
	imu_msg->linear_acceleration.y = ay;
	imu_msg->linear_acceleration.z = az;

	ROS_INFO("Accelerometer : X = %+7.3f, Y = %+7.3f, Z = %+7.3f", ax, ay, az);
	ROS_INFO("Gyroscope : X = %+7.3f, Y = %+7.3f, Z = %+7.3f", gx, gy, gz);
	
}

int main(int argc, char **argv)
{
 	/***********************/
	/* Initialize The Node */
	/***********************/
	ros::init(argc, argv, "imu_readings");
	ros::NodeHandle n;
	ros::Publisher imu_pub = n.advertise<sensor_msgs::Imu>("imu_adv", 1000);

	//running rate = 2 Hz
	ros::Rate loop_rate(2);

	/*************************/
	/* Initialize the Sensor */
	/*************************/

	printf("Selected: MPU9250\n");
	InertialSensor* sensor = new MPU9250();

	/***************/
	/* Test Sensor */
	/***************/
	if (!sensor->probe()) 
	{
		printf("Sensor not enabled\n");
		return EXIT_FAILURE;
	}

	sensor->initialize();

	while (ros::ok())
	{

		sensor_msgs::Imu imu_msg;

		init_msg(&imu_msg);
		
		update_msg(&imu_msg, sensor);

		imu_pub.publish(sensor_msgs);

		ros::spinOnce();

		loop_rate.sleep();

	}


  	return 0;
}

