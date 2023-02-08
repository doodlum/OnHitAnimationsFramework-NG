#pragma once

namespace MaxsuOnHitAnimFW
{
	class OnHitEventHandler : public RE::BSTEventSink<RE::TESHitEvent>
	{
	public:
		virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource);

		static bool RegisterOnHitEvent();

	private:
		OnHitEventHandler() = default;

		~OnHitEventHandler() = default;

		OnHitEventHandler(const OnHitEventHandler&) = delete;

		OnHitEventHandler(OnHitEventHandler&&) = delete;

		OnHitEventHandler& operator=(const OnHitEventHandler&) = delete;

		OnHitEventHandler& operator=(OnHitEventHandler&&) = delete;
	};

	class OnEffectStartHandler : public RE::BSTEventSink<RE::TESMagicEffectApplyEvent>
	{
	public:
		virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource);

		static bool RegisterEffectStartEvent();

	private:
		OnEffectStartHandler() = default;

		~OnEffectStartHandler() = default;

		OnEffectStartHandler(const OnEffectStartHandler&) = delete;

		OnEffectStartHandler(OnEffectStartHandler&&) = delete;

		OnEffectStartHandler& operator=(const OnEffectStartHandler&) = delete;

		OnEffectStartHandler& operator=(OnEffectStartHandler&&) = delete;
	};
}
