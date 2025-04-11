class MFN_IDentityStruct : JsonApiStruct
{
	string head;
	string body;
	string persist;

	void MFN_IDentityStruct()
	{
		// these variables will be converted to JSON or filled from JSON
		RegV("head");
		RegV("body");
		RegV("persist");
	}
}

