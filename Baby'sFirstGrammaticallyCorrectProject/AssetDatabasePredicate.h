#pragma once

#include "AssetDatabase.h"

namespace AssetDatabase
{
	typedef bool(*Predicate)(const AssetEntry& entry);

	namespace Predicates
	{
		Predicate MatchType(const AssetType& type);
		bool All(const AssetEntry&);
	}
}