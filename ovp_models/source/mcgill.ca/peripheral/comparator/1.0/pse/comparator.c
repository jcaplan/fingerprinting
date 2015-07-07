
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Mon Sep  8 09:35:25 2014
//
////////////////////////////////////////////////////////////////////////////////

#include "comparator.igen.h"
#include "peripheral/ppm.h"
#include "peripheral/bhm.h"
#include "comparator.h"
#include "comparator.igen.h"
//////////////////////////////// Callback stubs ////////////////////////////////

int start [2][16];
int end   [2][16];
int head  [2][16];
int tail  [2][16];
int fprint[2][512];
int assignment[2][16];
int checkin[2][16];
int checkout[2][16];
int pause[2][16];
int fprints_ready[2][16];
int task_count[16];
int successful_reg;
int failed_reg;
exception_reg exception;




void printTest(int reg[2][16]){
	int i,j;
	for(i = 0; i < 2; i++){
		for(j = 0; j < 16; j++){
			bhmPrintf("%x,",reg[i][j]);
		}
		bhmPrintf("\n");
	}
}

void collision_occurred(int task){
	//write the exception register
	//write the checkin and checkout
	//reset the head and tail

	if(diagnosticLevel == 3){
		bhmPrintf("collision!\n");
		bhmPrintf("task %d fprint %x and %x don't match\n",task,fprint[0][tail[0][task]],fprint[1][tail[0][task]]);
	}
	task_count[task]++;
	failed_reg |= (1 << task);
	fprints_ready[0][task] = 0;
	fprints_ready[1][task] = 0;
	checkin[0][task] = 0;
	checkout[0][task] = 0;
	checkin[1][task] = 0;
	checkout[1][task] = 0;
	head[0][task] = start[0][task];
	tail[0][task] = start[0][task];

	head[1][task] = start[1][task];
	tail[1][task] = start[1][task];
	ppmWriteNet(handles.COMP_IRQ, 1);


}
void task_succeeded(int task){
	//write the exception register
	//write the checkin and checkout

	//wait if something already happened
	int i;
	task_count[task]++;
	for(i = 0; i < 2; i++){
		checkin[i][task] = 0;
		checkout[i][task] = 0;
	}
	exception.bits.task_id = task;
	successful_reg |= (1 << task);
	exception.bits.irq = 1;

	bhmPrintf("success task %d!\n",task);
	ppmWriteNet(handles.COMP_IRQ, 1);
}

void increase_pointer(int array[2][16], int core, int task){
	if(array[core][task] == end[core][task])
		array[core][task] = start[core][task];
	else
		array[core][task]++;

	if(diagnosticLevel == 3){
		bhmPrintf("task %d core %d head pointer increased: %d\n",task,core,array[core][task]);
	}

}

int check_fprint(int task){
	int test = 1;
	int i;
	while((tail[0][task] != head[0][task]) && (tail[1][task] != head[1][task])){
		//if the comparisons don't match
		if(fprint[0][tail[0][task]] != fprint[1][tail[1][task]]){
			test = 0;
			break;
		}
		if(diagnosticLevel == 3){
			bhmPrintf("task %d fprint %x and %x match\n",task,fprint[0][tail[0][task]],fprint[1][tail[0][task]]);
		}
		//else{
		increase_pointer(tail,0,task);
		increase_pointer(tail,1,task);
		//INCREASE THE TAIL POINTER
		//}	
	}
	if(diagnosticLevel == 3){
		bhmPrintf("finished comparing fingerprints in buffer for task %d\n",task);
	}
	


	for(i = 0; i < 2; i++){
		fprints_ready[i][task] = 0;
	}
	return test;
}
void do_comparison(void){
	//This mimics the FSM from the comparator
	//First is the signal task-check_i... 
	//if fprints_ready or checkin
	int fp;
	int chin;
	int task;
	for(task = 0; task < 16; task++){
		//If a fingerprint is ready on both cores or the task is complete
		//on both cores
		fp = (fprints_ready[0][task] == 1 && fprints_ready[1][task] == 1);
		chin = (checkin[0][task] == 1 && checkin[1][task] == 1);
		if( fp == 1 || chin == 1)
			break;
	}

	//If the previous search gets a hit
	if(task != 16){
		//else the task are completes
		while(chin == 1){ 				
			//If different number of fingerprints then collision
			bhmPrintf("checkin\n");
			if(head[0][task] != head[1][task]){
				bhmPrintf("head pointers don't match!\n");
				collision_occurred(task);
			}else{

				//Otherwise go through the remaining fingerprints
				//confirm that they match
				int test = check_fprint(task);
				
				//check final condition of test
				if(test == 1)
					task_succeeded(task);
				else{
					collision_occurred(task);
					bhmPrintf("comparison failed!\n");
				}
				chin = 0;
				for(task = 0; task < 16; task++){
					//If a fingerprint is ready on both cores or the task is complete
					//on both cores
					chin = (checkin[0][task] == 1 && checkin[1][task] == 1);
					if(chin == 1)
						break;
				}
			}
		}
		int chout = (checkout[0][task] == 1 && checkout[1][task] == 1);
		if(chout == 1 && fp == 1){

			int test = check_fprint(task);

			if(test == 0)
				collision_occurred(task);
				
		} 
	}

}


PPM_WRITE_CB(memWr) {
	 COMP_CSR_SLAVE_address address;
	 address.value = (Uns32)(addr- handles.COMP_CSR_SLAVE);

	if(diagnosticLevel == 3){
	 	bhmPrintf("comparator csr: %x,%x,%x,%x,%x\n",(int)(addr- handles.COMP_CSR_SLAVE),bytes,(int)user,data,artifactAccess);
	}//bhmPrintf("core id: %x\n",address.bits.core_id);
	 
	 switch(address.bits.message_type){
	 	//check data type and file it
	 	//First ignore the 6 LSB 
	 	case START_POINTER:
	 		start[address.bits.core_id][address.start.pointer] = data;
	 		head[address.bits.core_id][address.start.pointer] = data;
	 		tail[address.bits.core_id][address.start.pointer] = data; 		
	 		break;
	 	case END_POINTER:
	 		end[address.bits.core_id][address.end.pointer] = data;
	 		break;
	 	case OTHER:
	 		//In this case only ignore 4 LSB (5:4 = address.reg.address)
	 		switch(address.reg.address){
	 			COMP_FPRINT_SLAVE_data d;
	 			case CORE_ASSIGNMENT:
	 				d.value = data;
	 				assignment[address.bits.core_id][d.assignment.task_id] = d.assignment.core_no;
	 				break;
	 			case EXCEPTION: 
	 				exception.bits.irq = 0;
	 				successful_reg = 0;
	 				failed_reg = 0;
	 				ppmWriteNet(handles.COMP_IRQ, 0);
	 				break;
	 		}
	 		break;
	 	//case COMPARATOR_EXCEPTION:
	 	//	bhmPrintf("exception\n"); 		
	 }
}

PPM_READ_CB(memRd) {
	COMP_CSR_SLAVE_address address;
	address.value = (Uns32)(addr- handles.COMP_CSR_SLAVE);
	Uns32 result = 0;
	switch (address.bits.message_type) {
	case  OTHER:
		if(address.reg.address == EXCEPTION){
    		if(address.reg.offset == 0){
    			bhmPrintf("STATUS READ: %x\n",exception.value);
    			result = exception.value;
    		}
    		else if(address.reg.offset == 4){
    			bhmPrintf("successful_reg: %x\n",successful_reg);
    			result = successful_reg;
    		}
    		else if(address.reg.offset == 8){
    			bhmPrintf("failed_reg: %x\n",failed_reg);
    			result = failed_reg;	
    		}
    	}
    	break;
    case START_POINTER:
    	result = task_count[address.start.pointer];
    	bhmPrintf("COMPARATOR: task_count[%d]: %x\n",(int)address.start.pointer,result);
    	task_count[address.start.pointer] = 0;
    default:
    	break;
	} 
	return result;
}




PPM_WRITE_CB(fprintWr) {
	 COMP_FPRINT_SLAVE_address a;
 	 COMP_FPRINT_SLAVE_data d;
 	 a.value = (Uns32)(addr - handles.COMP_FPRINT_SLAVE);
 	 d.value = (Uns32)data;
 	 int task,core;
 	 int index = 16;
 	 switch(a.bits.message_type){

 	 	case COMPARATOR_CS:
 	 		switch(d.state.status){
 	 			case CH_IN:
 	 				//Check the core ID against the assignment table
 	 				//then check in/out the task for the matching core
 	 				if(a.bits.core_id == assignment[0][d.state.task_id]){
 	 					if(checkout[0][d.state.task_id])
 	 						checkin[0][d.state.task_id] = 1;
 	 				}
 	 				else{
 	 					if(checkout[1][d.state.task_id])
 	 						checkin[1][d.state.task_id] = 1;
 	 				}
 	 				do_comparison();
 	 				break;
 	 			case CH_OUT:
 	 				if(a.bits.core_id == assignment[0][d.state.task_id])
						checkout[0][d.state.task_id] = 1;
					else
						checkout[1][d.state.task_id] = 1;
					//Debugging print stuff
 	 				printTest(checkout);
					bhmPrintf("CHECKOUT!!\n");
					break;
			}
			break;
 	 	case COMPARATOR_CRC:
 //	 		 bhmPrintf("crc\n");
 	 		 index = 16;
 	 		 //Find the matching table entry
 	 		 for(core = 0; core < 2; core ++){
	 	 		for(task = 0; task < 16; task++){
	 	 		 	if((a.bits.core_id == assignment[core][task]) && (checkout[core][task] == 1)\
	 	 		 		&& (pause[core][task] == 0) && (checkin[core][task] == 0)){
	 	 		 		index = task;
	 	 		 		break;
	 	 		 	}
	 	 		}
	 	 		//If we got a hit then stop looking
	 	 	 	if(index != 16)
	 				break;
 	 		} 	 	

 	 		if(diagnosticLevel == 3){
				bhmPrintf("comparator receives core %d fingerprint %x\n",a.bits.core_id,data);
			}

 	 		 //use the head pointer to update the fingerprint buffer then increment it
 	 		 fprint[core][head[core][index]]=data;
// 			 bhmPrintf("virtual core = %d, actual core = %d, task = %d, crc = %x\n",core,assignment[core][task],task,fprint[core][head[core][index]]);
 	 		 
 	 		 //INCREASE THE HEAD POINTER
 	 		 increase_pointer(head,core,task);
// 	 		 bhmPrintf("virtual core = %d, actual core = %d, head = %d\n",core,assignment[core][task],head[core][task]);
 	 		 //signal that fingerprints are ready
 	 		 fprints_ready[core][index]=1;
 	 		 do_comparison();

 	 		break;
 	 	case COMPARATOR_PAUSE:
 	 	 	 		 index = 16;
 	 		 //Find the matching table entry
 	 		 for(core = 0; core < 2; core ++){
	 	 		for(task = 0; task < 16; task++){
	 	 		 	if((a.bits.core_id == assignment[core][task]) && (checkout[core][task] == 1)\
	 	 		 		&& (pause[core][task] == 0) && (checkin[core][task] == 0)){
	 	 		 		index = task;
	 	 		 		break;
	 	 		 	}
	 	 		}
	 	 		//If we got a hit then stop looking
	 	 	 	if(index != 16)
	 				break;
 	 		} 	
 	 	//pausing doesn't work because...
 	 	//because there is no pause reg at address core id bits
 	 		pause[core][index] = 1;
 	 		bhmPrintf("pause\n");
 	 		printTest(pause);
 	 		break;
 	 	case COMPARATOR_UNPAUSE:
 	 	 	 		 index = 16;
 	 		 //Find the matching table entry
 	 		 for(core = 0; core < 2; core ++){
	 	 		for(task = 0; task < 16; task++){
	 	 		 	if((a.bits.core_id == assignment[core][task]) && (checkout[core][task] == 1)\
	 	 		 		&& (pause[core][task] == 1) && (checkin[core][task] == 0)){
	 	 		 		index = task;
	 	 		 		break;
	 	 		 	}
	 	 		}
	 	 		//If we got a hit then stop looking
	 	 	 	if(index != 16)
	 				break;
 	 		} 	
 	 		bhmPrintf("unpause\n");
 	 		pause[core][index] = 0;
 	 		printTest(pause);    
 	 		break;      
 	 }
    // YOUR CODE HERE (fprintWr)
}

PPM_CONSTRUCTOR_CB(constructor) {
    // YOUR CODE HERE (pre constructor)
    periphConstructor();

    // YOUR CODE HERE (post constructor)
}

PPM_DESTRUCTOR_CB(destructor) {
    // YOUR CODE HERE (destructor)
}

