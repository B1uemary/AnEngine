#include "DTimer.h"

DTimer::DTimer() :
	elapsedTicks(0), FrameCount(0), framesPerSecond(0), framesThisSecond(0),
	isFixedTimeStep(false), leftOverTicks(false), qpcSecondCounter(0),
	targetElapsedTicks(TicksPerSecond / 60), totalTicks(0)
{
	QueryPerformanceFrequency(&qpcFrequency);
	QueryPerformanceCounter(&qpcLastTime);

	qpcMaxDelta = qpcFrequency.QuadPart / 10;
	// �������������Ϊ0.1s
}

DTimer::~DTimer()
{
}

const UINT64 DTimer::GetElapsedTicks()
{
	return elapsedTicks;
}

const double DTimer::GetElapsedSeconds()
{
	return TicksToSeconds(elapsedTicks);
}

const UINT64 DTimer::GetTotalTicks()
{
	return totalTicks;
}

const double DTimer::GetTotalSeconds()
{
	return TicksToSeconds(totalTicks);
}

UINT32 DTimer::GetFrameCount()
{
	return FrameCount;
}

UINT32 DTimer::GetFramePerSecond()
{
	return framesPerSecond;
}

void DTimer::SetFixedFramerate(bool _isFixedTimeStep)
{
	isFixedTimeStep = _isFixedTimeStep;
}

void DTimer::SetTargetElapsedTicks(UINT64 _targetElapsed)
{
	targetElapsedTicks = _targetElapsed;
}

void DTimer::SetTargetElapsedSeconds(double _targetElapsed)
{
	targetElapsedTicks = SecondsToTicks(_targetElapsed);
}

double DTimer::TicksToSeconds(UINT64 _ticks)
{
	return static_cast<double>(_ticks) / TicksPerSecond;
}

UINT64 DTimer::SecondsToTicks(double _seconds)
{
	return static_cast<UINT64>(_seconds * TicksPerSecond);
}

void DTimer::ResetElapsedTime()
{
	QueryPerformanceCounter(&qpcLastTime);

	leftOverTicks = 0;
	framesPerSecond = 0;
	framesThisSecond = 0;
	qpcSecondCounter = 0;
}

void DTimer::Tick(LpUpdateFunc _update)
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	UINT64 delta = currentTime.QuadPart - qpcLastTime.QuadPart;
	qpcLastTime = currentTime;
	qpcSecondCounter += delta;

	if (delta > qpcMaxDelta)
	{
		delta = qpcMaxDelta;
	}	// ���ƹ����delta��������debug���ж�

	delta *= TicksPerSecond;
	delta /= qpcFrequency.QuadPart;
	// QPC��λת���ʵ�λ

	UINT32 lastFrameCount = FrameCount;

	if (isFixedTimeStep)
	{
		if (abs(static_cast<int>(delta - targetElapsedTicks)) < TicksPerSecond / 4000)	// ??
		{
			delta = targetElapsedTicks;
		}
		leftOverTicks += delta;
		// ��ֹʧ֮��������ǧ��̶�֡��ʱ�����˴�ֱͬ���ĳ����������ʾ���������ۻ���΢С���

		while (leftOverTicks >= targetElapsedTicks)
		{
			elapsedTicks = targetElapsedTicks;
			totalTicks += targetElapsedTicks;
			leftOverTicks -= targetElapsedTicks;
			FrameCount++;

			if (_update)
			{
				_update();
			}
		}
	}
	else
	{
		elapsedTicks = delta;
		totalTicks += delta;
		leftOverTicks = 0;
		FrameCount++;
		if (_update)
		{
			_update();
		}
	}

	if (FrameCount != lastFrameCount)
	{
		framesThisSecond++;
	}
	if (qpcSecondCounter >= static_cast<UINT64>(qpcFrequency.QuadPart))
	{
		framesPerSecond = framesThisSecond;
		framesThisSecond = 0;
		qpcSecondCounter %= qpcFrequency.QuadPart;
	}
}
