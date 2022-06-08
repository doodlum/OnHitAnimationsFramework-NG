#include "OHAF_Events.h"
#include "OHAF_Functions.h"
#include "OHAF_DataHandler.h"

namespace MaxsuOnHitAnimFW
{
	using EventResult = RE::BSEventNotifyControl;


//-------------------------------------OnHit Event-----------------------------------------------------------------------------------------------------

	bool OnHitEventHandler::RegisterOnHitEvent()
	{
		static OnHitEventHandler g_hiteventhandler;

		auto ScriptEventSource = RE::ScriptEventSourceHolder::GetSingleton();

		if (!ScriptEventSource)
		{
			logger::error("ScriptEventSource not found!");
			return false;
		}

		ScriptEventSource->AddEventSink(&g_hiteventhandler);

		logger::info("Register OnHit Event Handler!");

		return true;
	}


	EventResult OnHitEventHandler::ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource)
	{

		if (!a_event || !a_eventSource)
		{
			logger::error("Event Source Not Found!");
			return EventResult::kContinue;
		}

		logger::debug("OnHit Event Trigger!");

		auto OHAF_datahandler = DataHandler::GetSingleton();

		if (!OHAF_datahandler)
		{
			logger::error("OHAF DatahHandler Not Found!");
			return EventResult::kContinue;

		}
	
	//----------------------Check Hit Target-----------------------------------------------
	
		auto hit_target = a_event->target.get();

		if (!hit_target)
		{
			logger::error("Hit Target Not Found!");
			return EventResult::kContinue;
		}

		if (!hit_target->Is3DLoaded())
		{
			logger::debug("Hit Target Not 3D Loaded!");
			return EventResult::kContinue;
		}
		
		logger::debug(FMT_STRING("Hit Target Name is \"{}\", ID is \"{:x}\""), hit_target->GetName(), hit_target->GetFormID());
		
		if (hit_target->formType != RE::FormType::ActorCharacter)
		{
			logger::debug("Hit Target Not Actor!");
			return EventResult::kContinue;
		}

		if (hit_target->IsDead())
		{
			logger::debug("Hit Target is Dead!");
			return EventResult::kContinue;
		}


		if (hit_target->IsPlayerRef() && !OHAF_datahandler->IsEnablePC())
		{
			logger::debug("Disable Player Character!");
			return EventResult::kContinue;
		}
		else if (!hit_target->IsPlayerRef() && !OHAF_datahandler->IsEnableNPC())
		{
			logger::debug("Disable NPC!");
			return EventResult::kContinue;
		}

	//---------------------------------------------------------------------------------------



	
	//----------------------Check Target AnimGraph-----------------------------------------
	
		auto thisgraph = GetAnimGraph(hit_target->As<RE::Actor>());
	
		if (!thisgraph)
		{
			logger::error("Not Animation Graph Found in the actor!");
			return EventResult::kContinue;
		}
	
		std::string graph_name = thisgraph->projectName.data();

		std::transform(graph_name.begin(), graph_name.end(), graph_name.begin(), [](unsigned char c) { return std::tolower(c); });

		logger::debug(FMT_STRING("Target Actor Graph Name is \"{}\"."), graph_name);

		auto this_AGObj = OHAF_datahandler->LookUpAGObjByName(graph_name);

		if (!this_AGObj)
		{
			logger::debug("Not Matching AnimGraph Type Found For Hit Target!");
			return EventResult::kContinue;
		}
	
	//-------------------------------------------------------------------------------
	

	
	//----------------------Check Hit Source---------------------------------------------

		auto hitsource = RE::TESForm::LookupByID<RE::TESObjectWEAP>(a_event->source);

		if (!hitsource)
		{
			logger::debug("Weapon Hit Source Not Found!");
			return EventResult::kContinue;

		}
		
		if (hitsource->formType != RE::FormType::Weapon)
		{
			logger::debug("Hit Source Is Not Weapon!");
			return EventResult::kContinue;
		}
		else
			logger::debug(FMT_STRING("Weapon Name is \"{}\", ID is \"{:x}\""), hitsource->GetName(), hitsource->GetFormID());

	//-------------------------------------------------------------------------------------



	//----------------------Check Hit Flag--------------------------------------------------

		using HitFlag = RE::TESHitEvent::Flag;

		if (a_event->flags & HitFlag::kHitBlocked) 
		{
			logger::debug("Hit Is Blocked!");
			return EventResult::kContinue;
		}
	//--------------------------------------------------------------------------------------------


		float AnimVarFloat = 0.f;

		hit_target->GetGraphVariableFloat(this_AGObj->VarFloatName, AnimVarFloat);

		logger::debug(FMT_STRING("Current Variable Float {} is {}"), this_AGObj->VarFloatName.c_str(), AnimVarFloat);

		if (abs(AnimVarFloat) <= 1e-6)		//Check if the Graph Variable Float value equal to zero.
		{
			hit_target->NotifyAnimationGraph(this_AGObj->EventName.c_str());
			logger::debug("Trigger an OnHit Event Successfully!");
		}
	
	
		return EventResult::kContinue;
	
	}
//---------------------------------------------------------------------------------------------------------------------------------------------------------




//-------------------------------------EffectStart Event-----------------------------------------------------------------------------------------------------

	bool OnEffectStartHandler::RegisterEffectStartEvent()
	{
		static OnEffectStartHandler g_effecteventhandler;

		auto ScriptEventSource = RE::ScriptEventSourceHolder::GetSingleton();

		if (!ScriptEventSource)
		{
			logger::error("ScriptEventSource not found!");
			return false;
		}

		ScriptEventSource->AddEventSink(&g_effecteventhandler);

		logger::info("Register OnEffectStart Event Handler!");

		return true;

	}



	EventResult OnEffectStartHandler::ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource)
	{
		if (!a_event || !a_eventSource)
		{
			logger::error("Event Source Not Found!");
			return EventResult::kContinue;
		}


		logger::debug("OnEffectStart Event Trigger!");

		auto OHAF_datahandler = DataHandler::GetSingleton();

		if (!OHAF_datahandler)
		{
			logger::error("OHAF DatahHandler Not Found!");
			return EventResult::kContinue;

		}
		

		//----------------------Check Effect Target-----------------------------------------------

		auto effect_target = a_event->target.get();

		if (!effect_target)
		{
			logger::error("Effect Target Not Found!");
			return EventResult::kContinue;
		}

		if (!effect_target->Is3DLoaded())
		{
			logger::debug("Effect Target Not 3DLoaded!");
			return EventResult::kContinue;
		}
		

		logger::debug(FMT_STRING("Effect Target Name is \"{}\", ID is \"{:x}\""), effect_target->GetName(), effect_target->GetFormID());

		if (effect_target->formType != RE::FormType::ActorCharacter)
		{
			logger::debug("Effect Target Not Actor!");
			return EventResult::kContinue;
		}

		if (effect_target->IsDead())
		{
			logger::debug("Effect Target is Dead!");
			return EventResult::kContinue;
		}


		if (effect_target->IsPlayerRef() && !OHAF_datahandler->IsEnablePC())
		{
			logger::debug("Disable Player Character!");
			return EventResult::kContinue;
		}
		else if (!effect_target->IsPlayerRef() && !OHAF_datahandler->IsEnableNPC())
		{
			logger::debug("Disable NPC!");
			return EventResult::kContinue;
		}

		//----------------------------------------------------------------------------------------




		//----------------------Check Target AnimGraph--------------------------------------------

		auto thisgraph = GetAnimGraph(effect_target->As<RE::Actor>());

		if (!thisgraph)
		{
			logger::error("Not Animation Graph Found in the actor!");
			return EventResult::kContinue;
		}

		std::string graph_name = thisgraph->projectName.data();

		std::transform(graph_name.begin(), graph_name.end(), graph_name.begin(), [](unsigned char c) { return std::tolower(c); });

		logger::debug(FMT_STRING("Target Actor Graph Name is {}."), graph_name);

		auto this_AGObj = OHAF_datahandler->LookUpAGObjByName(graph_name);

		if (!this_AGObj)
		{
			logger::debug("Not Matching AnimGraph Type Found For Effect Target!");
			return EventResult::kContinue;
		}

		//-----------------------------------------------------------------------------------------




		//----------------------Check Effect Type--------------------------------------------------
	
		using Flags = RE::EffectSetting::EffectSettingData::Flag;
		using CType = RE::MagicSystem::CastingType;
		using DType = RE::MagicSystem::Delivery;

		auto this_effect = RE::TESForm::LookupByID<RE::EffectSetting>(a_event->magicEffect);

		if (!this_effect)
		{
			logger::debug("Magic Effect Form Not Found!");
			return  EventResult::kContinue;
		}
		else
		{
			logger::debug(FMT_STRING("Current Magic Effect name is \"{}\", ID is \"{:x}\""), this_effect->GetName(), this_effect->GetFormID());
		}


		if (!(this_effect->data.flags & Flags::kHostile))
		{
			logger::debug("Magic Effect Is Not Hostile!");
			return  EventResult::kContinue;
		}

		if (this_effect->data.flags & Flags::kPainless)
		{
			logger::debug("Magic Effect Is Painless!");
			return  EventResult::kContinue;
		}


		if (this_effect->data.delivery == DType::kSelf)
		{
			logger::debug("Magic Effect Is Self Delivered");
			return  EventResult::kContinue;

		}


		if (this_effect->GetMagickSkill() == RE::ActorValue::kAlteration || this_effect->GetMagickSkill() == RE::ActorValue::kConjuration || this_effect->GetMagickSkill() == RE::ActorValue::kIllusion)
		{
			logger::debug("Magic Skill Not Matched!");
			return  EventResult::kContinue;
		}


		if (this_effect->data.castingType != CType::kFireAndForget && this_effect->data.castingType != CType::kScroll)
		{
			logger::debug("Magic Effect Is Not Fire And Forget!");
			return  EventResult::kContinue;
		}


		if (!this_effect->conditions.IsTrue(a_event->caster.get(), a_event->target.get()))
		{
			logger::debug("Magic Effect Conditions Not Matched!");
			return  EventResult::kContinue;
		}

		//----------------------------------------------------------------------------------------


		float AnimVarFloat = 0.f;

		effect_target->GetGraphVariableFloat(this_AGObj->VarFloatName, AnimVarFloat);

		logger::debug(FMT_STRING("Current Variable Float {} is {}"), this_AGObj->VarFloatName.c_str(), AnimVarFloat);

		if (abs(AnimVarFloat) <= 1e-6)		//Check if the Graph Variable Float value equal to zero.
		{
			effect_target->NotifyAnimationGraph(this_AGObj->EventName.c_str());
			logger::debug("Trigger an OnEffectStart Event Successfully!");
		}
	
		return EventResult::kContinue;

	}
//---------------------------------------------------------------------------------------------------------------------------------------------------------







}
