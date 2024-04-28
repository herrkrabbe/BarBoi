#include <Environment/RepairPhase.h>

FString RepairPhaseToString(RepairPhase phase) {
	FString str = FString();
	switch (phase) {
	case UNREPAIRED:
		str = TEXT("UNREPAIRED");
		break;
	case REPAIRING:
		str = TEXT("REPAIRING");
		break;
	case REPAIR_COMPLETED:
		str = TEXT("REPAIR_COMPLETED");
		break;
	case REPAIRED:
		str = TEXT("REPAIRED");
		break;
	default:
		str = TEXT("ERROR");
		break;
	}
	return str;
}