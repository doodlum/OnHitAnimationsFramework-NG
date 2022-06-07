#include "OHAF_Functions.h"

namespace MaxsuOnHitAnimFW
{

	RE::EffectSetting* GetEffectSettingFromList(RE::BSSimpleList<RE::ActiveEffect*>* thisList, std::uint16_t thisID)
	{
		if (!thisList)
		{
			logger::error("Effect List Is Null!");
			return nullptr;
		}
		
		for (auto this_ActEffect : (*thisList))
		{
			if (this_ActEffect->usUniqueID == thisID)
			{
				return this_ActEffect->GetBaseObject();
			}
		}

		return nullptr;
	}


	bool HasEffectSettingInList(RE::BSSimpleList<RE::ActiveEffect*>* thisList, RE::EffectSetting* this_effect)
	{
		if (!thisList)
		{
			logger::error("Effect List Is Null!");
			return false;
		}

		for (auto this_ActEffect : (*thisList))
		{
			if (this_ActEffect->GetBaseObject() == this_effect)
			{
				return true;
			}
		}

		return false;
	}


}