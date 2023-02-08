#include "OHAF_LoadGameFunc.h"

void Load()
{
	auto g_message = SKSE::GetMessagingInterface();
	g_message->RegisterListener(MaxsuOnHitAnimFW::EventCallback);
}
