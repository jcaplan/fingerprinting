void RadarTrackerUpdatePointers(INT32U baseAddress, RT_MODEL_RadarTracker_T *RadarTracker_M){
	RadarTracker_M->ModelData.dwork = (DW_RadarTracker_T *)(baseAddress + sizeof(RadarTrackerStruct));
}

void main(){
	RT_MODEL_RadarTracker_T *RadarTracker_M =
			&RadarTrackerPackageStruct.RadarTracker_STRUCT.RadarTracker_M;
	ExtU_RadarTracker_T *RadarTracker_U =
			&RadarTrackerPackageStruct.RadarTracker_STRUCT.RadarTracker_U;
	ExtY_RadarTracker_T *RadarTracker_Y =
			&RadarTrackerPackageStruct.RadarTracker_STRUCT.RadarTracker_Y;
	RadarTrackerUpdatePointers((INT32U)&RadarTrackerPackageStruct, RadarTracker_M);
	RadarTracker_initialize(RadarTracker_M, &RadarTracker_U, &RadarTracker_Y);
	RadarTrackerUpdatePointers((INT32U)&RadarTrackerPackageStruct & 0x3FFFFF, RadarTracker_M);
}
