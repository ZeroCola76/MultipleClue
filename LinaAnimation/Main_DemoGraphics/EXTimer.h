#pragma once
/// <summary>
/// 김주환교수님 타이머를 가져왔다 어차피 
/// </summary>
class EXTimer
{
private:
	double mSecondsPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;
public:
	EXTimer();
	~EXTimer();

	float TotalTime()const;  // in seconds
	float DeltaTime()const; // in seconds

	void Reset(); // Call before message loop.
	void Start(); // Call when unpaused.
	void Stop();  // Call when paused.
	void Tick();  // Call every frame.
};

