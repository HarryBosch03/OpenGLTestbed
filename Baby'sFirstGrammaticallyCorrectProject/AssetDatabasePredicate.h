#pragma once

#include "AssetDatabase.h"

namespace AssetDatabasePredicates
{
	typedef bool(*Predicate)(const AssetEntry& entry);

	Predicate MatchType(const AssetType& type);
	bool All(const AssetEntry&);
}