#include "OHAF_DataHandler.h"
#include "OHAF_Events.h"

namespace MaxsuOnHitAnimFW
{
	void EventCallback(SKSE::MessagingInterface::Message* msg)
	{
		if (msg->type == SKSE::MessagingInterface::kDataLoaded) {
			logger::info("Data Load CallBack Trigger!");

			auto thisdatahandler = DataHandler::GetSingleton();

			if (!thisdatahandler) {
				logger::error("OnHitAnimFW Data Handler not found!");
				return;
			}

			if (!thisdatahandler->IsModFunctionsEnable()) {
				logger::info("Mod Function Disable!");
				return;
			}

			if (thisdatahandler->IsDebugLogEnable())
				logger::info("Debug Message Enable!");
			else
				spdlog::set_level(spdlog::level::info);

			if (OnHitEventHandler::RegisterOnHitEvent())
				logger::info("Register OnHit Eevent!");

			if (OnEffectStartHandler::RegisterEffectStartEvent())
				logger::info("Register OnEffectStart Eevent!");
		}
	}
}
