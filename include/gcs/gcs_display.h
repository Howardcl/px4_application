#ifndef PX4_APPLICATION_GCS_DISPLAY_H_
#define PX4_APPLICATION_GCS_DISPLAY_H_

#include <nav_msgs/Path.h>
#include "ros_base/ros_base.h"
#include "subscriber/status_subscriber.h"
#include "px4_application/UavCommand.h"

class GcsDisplay : public RosBase
{
public:
    GcsDisplay(const ros::NodeHandle& _nh, double _period);
    ~GcsDisplay();

private:
    ros::Time begin_time;
    ros::Subscriber uav_command_sub;
    ros::Publisher matlab_pub;
    ros::Publisher rviz_pub;

    int own_id;
    char c_id;

    StatusSubscriber current_info;    //无人机与目标状态
    px4_application::UavCommand command_reception;

    geometry_msgs::PoseStamped matlab_display_data;
    nav_msgs::Path rviz_display_data;
    const int MAX_RVIZ_SIZE;

    float GetTimePassSec(void);
    void CommandCallback(const px4_application::UavCommand::ConstPtr& _msg);

    void Initialize(void);
    void UavStateDisplay(void); // 终端地面站信息显示
    void RvizStateDisplay(void); // rviz轨迹显示
    void MatlabStateDisplay(void); // matlab轨迹显示
    void CommandUpdateReset(void);

    virtual void LoopTask(void);
};


#endif