#include "OHAF_DataHandler.h"

namespace MaxsuOnHitAnimFW
{
	static const std::uint32_t max_graphnums = 255;

	static const std::uint32_t max_strlen = 255;

	static const std::uint32_t option_nums = 3;

	static const std::string files_path = "Data\\SKSE\\Plugins\\MaxsuOnHitAnimFiles\\Maxsu_OHAF_Config.ini";

	inline static const std::string GetStringValueFromINI(std::string key_name, const std::string section_name)
	{
		char return_string[max_strlen];

		const std::int32_t len = GetPrivateProfileStringA(section_name.c_str(), key_name.c_str(), "", return_string, sizeof(return_string), files_path.c_str());

		const std::string result = return_string;

		if (len > 0 && !result.empty()) {
			//logger::debug(FMT_STRING("Find String from INI File : \"{}\""), return_string);
			return return_string;
		}

		return "";
	}

	std::shared_ptr<OHAF_AnimGraphObj> DataHandler::LookUpAGObjByName(const string graph_name)
	{
		auto it = AG_Map.find(graph_name);

		if (it != AG_Map.end()) {
			logger::debug(FMT_STRING("Find a animation graph name \"{}\""), graph_name);
			return (*it).second;
		}

		return nullptr;
	}

	void DataHandler::AddAnimGraphObj(string GraphName, const string EventName, const string VarFloatName)
	{
		std::transform(GraphName.begin(), GraphName.end(), GraphName.begin(), [](unsigned char c) { return std::tolower(c); });

		auto it = AG_Map.find(GraphName);

		if (it != AG_Map.end()) {
			logger::error(FMT_STRING("animation graph \"{}\" already added! Skip Obj Constuct!"), GraphName);
			return;
		}

		auto thisobj = std::make_shared<OHAF_AnimGraphObj>(GraphName, EventName, VarFloatName);

		AG_Map.emplace(GraphName, thisobj);

		logger::info("Add a Animation Graph Obj!");
	}

	static bool GetAnimGraphObjValue(std::uint32_t graph_index, std::vector<string>& AGobj_arr)
	{
		graph_index++;

		if (graph_index <= 0) {
			logger::debug("Wrong Graph Index Passed!");
		}

		std::string SectionName = "AnimationGraph";
		SectionName += std::to_string(graph_index);

		logger::debug(FMT_STRING("Target Section Name is \"{}\""), SectionName);

		static const string key_names[option_nums] = {
			"GraphName",
			"AnimationEventName",
			"GraphVariableFloatName"
		};

		for (std::uint32_t i = 0; i < option_nums; i++) {
			AGobj_arr.insert(AGobj_arr.begin() + i, GetStringValueFromINI(key_names[i].c_str(), SectionName.c_str()));

			if (AGobj_arr[i].empty()) {
				logger::error(FMT_STRING("Can't find correct \"{}\" value in Section \"{}\", Skip Obj Constuct!"), key_names[i], SectionName);
				return false;
			}

			logger::info(FMT_STRING("Find \"{}\" {} as in Section \"{}\""), AGobj_arr[i], key_names[i], SectionName);
		}

		return true;
	}

	void DataHandler::GetAnimGraphsFromINIFiles()
	{
		std::int32_t graph_nums = GetPrivateProfileIntA("Main", "AnimationGraphNum", 0, files_path.c_str());

		logger::info("Find Graph Number is \"{}\"", graph_nums);

		if (graph_nums > max_graphnums) {
			logger::error("Graph number is beyond the maximum limitation! Stop Graphs Getting!");
			return;
		}

		for (std::int32_t i = 0; i < graph_nums; i++) {
			std::vector<string> AGobj_arr;

			if (GetAnimGraphObjValue(i, AGobj_arr)) {
				AddAnimGraphObj(AGobj_arr[0], AGobj_arr[1], AGobj_arr[2]);
			}
		}
	}

	DataHandler::DataHandler()
	{
		logger::info("OHAF_DataHandler Construct!");

		//------------------------------------Enable Mod Functions--------------------------------------------

		std::int32_t b_enablemod = GetPrivateProfileIntA("Main", "EnableModFunctions", 1, files_path.c_str());

		if (b_enablemod == 0) {
			EnableModFunctions = false;
			logger::info("Mod Functions Disable!");
			return;
		}
		//-------------------------------------------------------------------------------------------------------

		//-----------------------------------------Get Anim Graphs----------------------------------------------------

		GetAnimGraphsFromINIFiles();

		for (auto AG_pair : AG_Map) {
			auto thisAGObj = AG_pair.second;

			logger::info(FMT_STRING("Find a Animation Graph Object, GraphName : \"{}\", EventName : \"{}\", VariableFloat Name : \"{}\"."), thisAGObj->GraphName, thisAGObj->EventName, thisAGObj->VarFloatName);
		}
		//-------------------------------------------------------------------------------------------------------

		//------------------------------------------------Enable Debug Messgae-----------------------------------

		std::int32_t b_debugmes = GetPrivateProfileIntA("Main", "EnableDebugMessage", 0, files_path.c_str());

		if (b_debugmes == 1) {
			EnableDebugLog = true;
			logger::info("Enable Debug Message!");
		}
		//--------------------------------------------------------------------------------------------------------

		//------------------------------------------------Enable Spell-------------------------------------------

		std::int32_t b_enableSpell = GetPrivateProfileIntA("Main", "EnableSpellEffect", 1, files_path.c_str());

		if (b_enableSpell == 0) {
			EnableSpell = false;
			logger::info("Spell Effect Disable!");
		}
		//--------------------------------------------------------------------------------------------------------

		//----------------------------------------------Enable Player Character-----------------------------------

		std::int32_t b_enablePC = GetPrivateProfileIntA("Main", "EnablePlayerCharacterEffect", 1, files_path.c_str());

		if (b_enablePC == 0) {
			EnablePC = false;
			logger::info("Player Character Effect Disable!");
		}
		//--------------------------------------------------------------------------------------------------------

		//------------------------------------------------Enable NPC-----------------------------------------------

		std::int32_t b_enableNPC = GetPrivateProfileIntA("Main", "EnableNPCEffect", 1, files_path.c_str());

		if (b_enableNPC == 0) {
			EnableNPC = false;
			logger::info("NPC Effect Disable!");
		}
		//--------------------------------------------------------------------------------------------------------
	}
}
