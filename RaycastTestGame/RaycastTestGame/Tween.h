#pragma once
template<typename val>
struct Tween
{
public:
	Tween(val start, val goal, val& target, float time, bool withNext); 

public:
	void RunTween(float delta);
	bool IsFinished();
	bool IsContinuous();

private:
	val Lerp(val start, val goal, float influence);

private:
	val m_start;
	val m_goal;
	val& m_target;
	float m_time;
	float m_timeLimit;
	bool m_withNext;
};

template<typename val>
Tween<val>::Tween(val start, val goal, val& target, float time, bool withNext)
	: m_start{ start }, m_goal{ goal }, m_target{ target },
	m_timeLimit{ time }, m_withNext{ withNext }, m_time{ 0.f }
{
}

template<typename val>
void Tween<val>::RunTween(float delta)
{
	// Add time to timer
	m_time += delta;

	if (m_time > m_timeLimit)
	{
		m_time = m_timeLimit;
		m_target = m_goal;
		return;
	}

	// Calculate influence
	float influence = m_time / m_timeLimit;

	m_target = Lerp(m_start, m_goal, influence);

}

template<typename val>
bool Tween<val>::IsFinished()
{
	if (m_time >= m_timeLimit)
	{
		return true;
	}

	return false;
}



template<typename val>
bool Tween<val>::IsContinuous()
{
	return m_withNext;
}

template<typename val>
val Tween<val>::Lerp(val start, val goal, float influence)
{
	return (start * (1.f - influence) + goal * influence);
}