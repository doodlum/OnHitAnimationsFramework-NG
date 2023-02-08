#pragma once

namespace MaxsuOnHitAnimFW
{
	using string = std::string;

	struct OHAF_AnimGraphObj
	{
		const string GraphName;

		const string EventName;

		const string VarFloatName;

		OHAF_AnimGraphObj() = delete;

		OHAF_AnimGraphObj(const OHAF_AnimGraphObj&) = delete;

		OHAF_AnimGraphObj& operator=(const OHAF_AnimGraphObj&) = delete;

		OHAF_AnimGraphObj(const string thisGraphName, const string thisEventName, const string thisVarFloatName) :
			GraphName(thisGraphName), EventName(thisEventName), VarFloatName(thisVarFloatName)
		{
			logger::info(FMT_STRING("Construct a Animation Graph Object, Graph Name {}, Event Name : {}, VariableFloat Name : {}."), GraphName, EventName, VarFloatName);
		}
	};

	class DataHandler
	{
	public:
		static DataHandler* GetSingleton()
		{
			static DataHandler singleton;

			return &singleton;
		}

		inline bool IsDebugLogEnable() const
		{
			return EnableDebugLog;
		}

		inline bool IsModFunctionsEnable() const
		{
			return EnableModFunctions;
		}

		inline bool IsSpellEnable() const
		{
			return EnableSpell;
		}

		inline bool IsEnablePC() const
		{
			return EnablePC;
		}

		inline bool IsEnableNPC() const
		{
			return EnableNPC;
		}

	private:
		DataHandler();

		~DataHandler() = default;

		DataHandler(const DataHandler&) = delete;

		DataHandler(DataHandler&&) = delete;

		DataHandler& operator=(const DataHandler&) = delete;

		DataHandler& operator=(DataHandler&&) = delete;

		bool EnableDebugLog = false;

		bool EnableModFunctions = true;

		bool EnableSpell = true;

		bool EnablePC = true;

		bool EnableNPC = true;

		//--------------------------------AnimGraph Handler---------------------------------------------------------

	public:
		std::shared_ptr<OHAF_AnimGraphObj> LookUpAGObjByName(const string graph_name);

	private:
		void AddAnimGraphObj(string GraphName, const string EventName, const string VarFloatName);

		void GetAnimGraphsFromINIFiles();

		std::unordered_map<string, std::shared_ptr<OHAF_AnimGraphObj>> AG_Map;

		//-------------------------------------------------------------------------------------------------------------
	};
}
