#include "AssetDatabasePredicate.h"

AssetType matchType;
bool MatchTypePredicate(const AssetEntry& entry)
{
	return entry.second->GetType() == matchType;
}

AssetDatabasePredicates::Predicate AssetDatabasePredicates::MatchType(const AssetType& type)
{
	matchType = type;
	return MatchTypePredicate;
}

bool AssetDatabasePredicates::All(const AssetEntry&) { return true; }