#include <Arduino.h>
#include <ros2arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <iModBot.h>

#define SSID "nome da rede wifi"
#define SSID_PW "pass da rede wifi"
#define AGENT_IP "ip do agente"
#define AGENT_PORT 2018 //AGENT port number

#define SENSORS_UPDATE_FREQ 20 //hz

iModBot robot;
float Linear, Angular;

void US_sensor_dist(std_msgs::Int16 *msg, void *arg)
{
  (void)(arg);

  msg->data = robot.distance();
}

void IR_board_sensors(std_msgs::String *msg, void *arg)
{
  (void)(arg);

  sprintf(msg->data, "%d,%d,%d,%d,%d,%d,%d", robot.readS1(), robot.readS2(), robot.readS3(), robot.readS4(), robot.readS5(), robot.readCLP(), robot.readNear());
}

void Enc_count_RL(std_msgs::String *msg, void *arg)
{
  (void)(arg);

  sprintf(msg->data, "%d,%d", robot.getRightEncoderCount(), robot.getLeftEncoderCount());
}

class PubSensors : public ros2::Node
{
public:
  ros2::Publisher<std_msgs::Bool> *on_col;
  PubSensors()
      : Node("PubSensors_node")
  {
    on_col = this->createPublisher<std_msgs::Bool>("col_flag");

    ros2::Publisher<std_msgs::Int16> *US_sensor = this->createPublisher<std_msgs::Int16>("US_sensor_dist");
    this->createWallFreq(SENSORS_UPDATE_FREQ, (ros2::CallbackFunc)US_sensor_dist, nullptr, US_sensor);

    ros2::Publisher<std_msgs::String> *IR_board = this->createPublisher<std_msgs::String>("IR_board_sensors");
    this->createWallFreq(SENSORS_UPDATE_FREQ, (ros2::CallbackFunc)IR_board_sensors, nullptr, IR_board);

    ros2::Publisher<std_msgs::String> *Enc_count = this->createPublisher<std_msgs::String>("Enc_count_RL");
    this->createWallFreq(SENSORS_UPDATE_FREQ, (ros2::CallbackFunc)Enc_count_RL, nullptr, Enc_count);
  }
};

void subscribeCMD_Vel(geometry_msgs::Twist *cmd_vel, void *arg)
{
  (void)(arg);

  Linear = cmd_vel->linear.x;
  Angular = cmd_vel->angular.z;
}

class SubCMD_Vel : public ros2::Node
{
public:
  SubCMD_Vel()
      : Node("CMD_Vel_sub_node")
  {
    this->createSubscriber<geometry_msgs::Twist>("cmd_vel_SOC", (ros2::CallbackFunc)subscribeCMD_Vel, nullptr);
  }
};

WiFiUDP udp;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(SSID, SSID_PW);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println(".");
    delay(100);
  }
  Serial.print("Connected");
  ros2::init(&udp, AGENT_IP, AGENT_PORT);

  robot.begin();
}

void loop()
{
  static std_msgs::Bool col_flag;

  static PubSensors pubNode;
  static SubCMD_Vel CMD_Vel_Sub_Node;

  ros2::spin(&CMD_Vel_Sub_Node);
  ros2::spin(&pubNode);

  //-------------robotmovment-------------
  int botspeed_fb = 180;
  int botspeed_rl = 160;
  if (Linear > 0)
  {
    robot.forward(botspeed_fb);
  }

  if (Linear < 0)
  {
    robot.reverse(botspeed_fb);
  }

  if (Angular > 0)
  {
    robot.rotateRight(botspeed_rl);
  }

  if (Angular < 0)
  {
    robot.rotateLeft(botspeed_rl);
  }

  if (Linear == 0 && Angular == 0)
  {
    robot.stopMotors();
  }

  //----------------coldetect----------------
  static bool lastS = 0;
  if (robot.readCLP() == 1 && lastS == 0)
  {
    lastS = 1;
    col_flag.data = true;
    pubNode.on_col->publish(&col_flag);
  }

  if (robot.readCLP() == 0 && lastS == 1)
  {
    lastS = 0;
  }
}