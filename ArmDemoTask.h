#ifndef ARMDEMOTASK_H
#define ARMDEMOTASK_H

#include "Aria.h"
#include "ArNetworking.h"
#include "RemoteArnlTask.h"
#include "ArClientHandlerRobotUpdate.h"

namespace Kinova {
#include "Kinova.API.CommLayerUbuntu.h"
#include "Kinova.API.UsbCommandLayerUbuntu.h"
#include "KinovaTypes.h"
};

typedef enum {
  CartesianVel,
  CartesianPos,
  Reactive,
  Idle
} DemoMode;

#define MAX_ARMS 2
#define LEFT 0
#define RIGHT 1

/** Call init_arms() to connect to arms */
class ArmDemoTask: public virtual RemoteArnlTask
{
public:
  ArmDemoTask(ArClientBase *client, ArPTZ *ptu = NULL);


private:
  const DemoMode DEFAULT_MODE;
  bool demoDone;
  bool demoWaitingToFinish;
  ArTime demoTime;


  // These are initialized in init_demo():
  DemoMode demoMode;
  Kinova::CartesianInfo demoCartesianVelocities[12];
  int numDemoCartesianVelocities;
  Kinova::TrajectoryPoint demoTrajectoryCommand;
  Kinova::CartesianInfo demoCartesianPositions[12];
  int numDemoCartesianPositions;
  Kinova::TrajectoryPoint demoPositionCommand;


  Kinova::KinovaDevice armList[MAX_ARMS];
  int armCount;

  typedef struct {
    float x;
    float y;
    float z;
  } PosData;
  PosData armOffset[MAX_ARMS]; // in arm coordinate system but relative to PTU

  Kinova::CartesianPosition currentArmPositions[MAX_ARMS];
  ArMutex currentArmPositionMutex[MAX_ARMS];

  ArPTZ *ptu;


public:
  bool init_arms();
  void set_demo_mode(DemoMode newMode);
  void rehome_all_arms();
  void park_arms();
  void ptu_look_at(float x, float y, float z);
  virtual ~ArmDemoTask();
  void armEENetDrawingCallback(ArServerClient *client, ArNetPacket *pkt);

private:
  void init_demo();
  void clear_all_arm_trajectories();
  void set_pose(Kinova::CartesianInfo& pos, float px, float py, float pz, float ox, float oy, float oz);
  void print_user_position(Kinova::UserPosition& p);
  void set_fingers(Kinova::FingersPosition& f, float f1, float f2, float f3);
  void set_fingers_open(Kinova::FingersPosition& f);
  void set_fingers_closed(Kinova::FingersPosition& f);
  void setup_torso_protection_zone_for_left_arm();
  void setup_torso_protection_zone_for_right_arm();
  void run_demo();
  void arm_demo_done();
  virtual void goalReached(const GoalInfo& g);
  virtual void touringToGoal(const GoalInfo& g);
};

#endif
