#include <Environment/RepairPhase.h>

FString RepairPhaseToString(RepairPhase phase) {
	FString str = FString();
	switch (phase) {
	case UNREPAIRED:
		break;
	case REPAIRING:
		break;
	case REPAIR_COMPLETED:
		break;
	case REPAIRED:
		break;
	default:
		break;
	}
	return str;
}