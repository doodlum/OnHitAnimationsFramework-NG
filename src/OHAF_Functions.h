#pragma once

namespace MaxsuOnHitAnimFW
{
	inline RE::BShkbAnimationGraph* GetAnimGraph(RE::Actor* thisactor)
	{
		if (!thisactor) {
			logger::debug("This Actor Not Found!");
			return nullptr;
		}

		auto c_process = thisactor->GetActorRuntimeData().currentProcess;

		if (!c_process) {
			logger::error("Current Process Not Found!");
			return nullptr;
		}

		auto middlehighprocess = c_process->middleHigh;

		if (!middlehighprocess) {
			logger::error("middlehighprocess Not Found!");
			return nullptr;
		}

		auto thisAGmanager = middlehighprocess->animationGraphManager.get();

		if (!thisAGmanager) {
			logger::error("thisAGmanager Not Found!");
			return nullptr;
		}

		auto thisgraph = thisAGmanager->graphs.begin()->get();

		if (thisgraph)
			return thisgraph;

		return nullptr;
	}

	RE::EffectSetting* GetEffectSettingFromList(RE::BSSimpleList<RE::ActiveEffect*>* thisList, std::uint16_t thisID);

	bool HasEffectSettingInList(RE::BSSimpleList<RE::ActiveEffect*>* thisList, RE::EffectSetting* this_effect);
}
