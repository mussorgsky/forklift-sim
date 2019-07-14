class PID
{
  private:
    float kP, kI, kD;
    float p, i, d;
    float lastError;

  public:
    PID(float kp, float ki, float kd);
    float update(float error, float dT);
};