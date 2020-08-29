#ifndef PX4_APPLICATION_UAV_COLLABORATION_H_
#define PX4_APPLICATION_UAV_COLLABORATION_H_

#include "ros_base.h"
#include "status_subscriber.h"
#include "px4_application/UavCommand.h"
#include "pid_controller.h"

class States
{
public:
    States();
    virtual ~States();
    void StateMachineSchedule(const StatusSubscriber& _current_info,
                               const ros::Publisher& _uav_command_pub,
                                px4_application::UavCommand* _command_deliver,
                                 States** _State);    //注：使用指针的指针，确保能访问到对象指针本身，因为状态转移需delete与new操作
private:
    virtual void Run(const StatusSubscriber& _current_info,
                      const ros::Publisher& _uav_command_pub,
                       px4_application::UavCommand* _command_deliver,
                        States** _State) = 0;
protected:
    geometry_msgs::Vector3 reach_point_range_;
};

class Prepare : public States
{
public:
    Prepare();
    ~Prepare();
private:
    virtual void Run(const StatusSubscriber& _current_info,
                      const ros::Publisher& _uav_command_pub,
                       px4_application::UavCommand* _command_deliver,
                        States** _State);
};

class TakeOff : public States
{
public:
    TakeOff();
    ~TakeOff();
private:
    virtual void Run(const StatusSubscriber& _current_info,
                      const ros::Publisher& _uav_command_pub,
                       px4_application::UavCommand* _command_deliver,
                        States** _State);

    geometry_msgs::Vector3 takeoff_position_;
    geometry_msgs::Vector3 takeoff_absolute_position_param_;    //绝对起飞位置
    double takeoff_relative_height_param_;    //相对起飞高度
    bool takeoff_id_;    //false绝对起飞 true相对起飞
};

class Assemble : public States
{
public:
    Assemble();
    ~Assemble();
private:
    virtual void Run(const StatusSubscriber& _current_info,
                      const ros::Publisher& _uav_command_pub,
                       px4_application::UavCommand* _command_deliver,
                        States** _State);
    geometry_msgs::Vector3 assemble_position_;
};

class Tracking : public States
{
public:
    Tracking();
    ~Tracking();
private:
    virtual void Run(const StatusSubscriber& _current_info,
                      const ros::Publisher& _uav_command_pub,
                       px4_application::UavCommand* _command_deliver,
                        States** _State);
    geometry_msgs::Vector3 tracking_position_;    //相机坐标系下的追踪
    double tracking_yaw_;
    geometry_msgs::Vector3 tracking_threshold_;
    PidController TrackingX;
    PidController TrackingY;
    PidController TrackingZ;
};

class ReturnHome : public States
{
public:
    ReturnHome();
    ~ReturnHome();
private:
    virtual void Run(const StatusSubscriber& _current_info,
                      const ros::Publisher& _uav_command_pub,
                       px4_application::UavCommand* _command_deliver,
                        States** _State);
    geometry_msgs::Vector3 home_position_;
};

class Landing : public States
{
public:
    Landing();
    ~Landing();
private:
    virtual void Run(const StatusSubscriber& _current_info,
                      const ros::Publisher& _uav_command_pub,
                       px4_application::UavCommand* _command_deliver,
                        States** _State);
    geometry_msgs::Vector3 landing_pos_vel_;
    
};

class Finished : public States
{
public:
    Finished();
    ~Finished();
private:
    virtual void Run(const StatusSubscriber& _current_info,
                      const ros::Publisher& _uav_command_pub,
                       px4_application::UavCommand* _command_deliver,
                        States** _State);
};

class UavCollaboration : public RosBase 
{
public:
    UavCollaboration(const ros::NodeHandle& _nh, double _period);
    ~UavCollaboration();
    
private:

    ros::Publisher uav_command_pub_;

    px4_application::UavCommand command_deliver_;
    StatusSubscriber current_info_;    //无人机和目标状态
    States* UavState_;

    void Initialize(void);

    virtual void LoopTask(void);
};

#endif