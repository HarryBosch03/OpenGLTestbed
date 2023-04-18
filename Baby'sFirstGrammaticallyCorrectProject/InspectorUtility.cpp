#include "InspectorUtility.h"

int workingID = -1;

int& Utility::Inspector::WorkingID() { return workingID; }

std::string buffer;

const char* Utility::Inspector::AppendID(const std::string& text)
{
	buffer = text + "##" + std::to_string(workingID);
	return buffer.c_str();
}
