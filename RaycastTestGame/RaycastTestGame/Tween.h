#pragma once
template<typename VAL>
struct Tween
{
public:
	Tween(VAL start, VAL goal, VAL& target, float time, bool withNext);

public:
	void RunTween(float delta);
	bool IsFinished();
	bool IsContinuous();

private:
	VAL Lerp(VAL start, VAL goal, float influence);

private:
	VAL m_start;
	VAL m_goal;
	VAL& m_target;
	float m_time;
	float m_timeLimit;
	bool m_withNext;
};

template<typename VAL>
Tween<VAL>::Tween(VAL start, VAL goal, VAL& target, float time, bool withNext)
	: m_start{ start }, m_goal{ goal }, m_target{ target },
	m_timeLimit{ time }, m_withNext{ withNext }, m_time{ 0.f }
{
}

template<typename VAL>
void Tween<VAL>::RunTween(float delta)
{
	// Add time to timer
	m_time += delta;

	// If timer is above limit
	if (m_time > m_timeLimit)
	{
		// Set data to goal and exit
		m_time = m_timeLimit;
		m_target = m_goal;
		return;
	}

	// Calculate influence
	float influence = m_time / m_timeLimit;

	m_target = Lerp(m_start, m_goal, influence);

}

template<typename VAL>
bool Tween<VAL>::IsFinished()
{
	// If timer is finished
	if (m_time >= m_timeLimit)
	{
		return true;
	}

	return false;
}



template<typename VAL>
bool Tween<VAL>::IsContinuous()
{
	return m_withNext;
}

template<typename VAL>
VAL Tween<VAL>::Lerp(VAL start, VAL goal, float influence)
{
	return (start * (1.f - influence) + goal * influence);
}