#pragma once
#include <tuple>

class PModifier
{
	public:
		virtual ~PModifier();
		virtual float getNextValue() = 0;
		void setCurrentValue(float currentValue);
		float getCurrentValue() const;

		virtual void setDuration(int seconds);
		float getDuration() const;

		virtual void setRange(float startValue, float endValue);
		virtual void setStartValue(float startValue);
		virtual void setEndValue(float endValue);
		float getStartValue() const;
		float getEndValue() const;

		bool isFinished() const;
		void invert();

	protected:
		PModifier(int durationInSeconds, float startValue, float endValue);
		int _durationInSeconds;
		float _startValue;
		float _endValue;
		float _currentValue;
};

class LinearModifier final : public PModifier
{
	public:
		LinearModifier(int durationInSeconds, float startValue, float endValue);
		float getNextValue() override;
		void setDuration(int seconds) override;
		void setRange(float startValue, float endValue) override;
		void setStartValue(float startValue) override;
		void setEndValue(float endValue) override;

	private:
		float _delta;
		void setDelta();
};

class SmoothModifier final : public PModifier
{
	public:
		SmoothModifier(int durationInSeconds, float startValue, float endValue);
		float getNextValue() override;
		void setDuration(int seconds) override;

	private:
		float _domain;
		float _currentPoint;
		void setDomain();
};