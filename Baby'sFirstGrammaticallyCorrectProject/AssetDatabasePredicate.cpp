#include "AssetDatabasePredicate.h"

AssetType matchType;
bool MatchTypePredicate(const AssetEntry& entry)
{
	return entry.second->GetType() == matchType;
}

AssetDatabase::Predicate AssetDatabase::Predicates::MatchType(const AssetType& type)
{
	matchType = type;
	return MatchTypePredicate;
}

bool AssetDatabase::Predicates::All(const AssetEntry&) { return true; }