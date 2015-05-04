package codegen.prof;

import java.io.IOException;
import java.util.HashMap;

import javax.management.RuntimeErrorException;

import codegen.prof.BasicBlock.bbType;

public class Profiler {

	
	static HashMap<String,String> libFuncs = new HashMap<>();
	
	static {
		libFuncs.put("__floatsidf","floatsidf.o");
		libFuncs.put("__unorddf2","unorddf2.o");
		libFuncs.put("__subdf3","subdf3.o");
		libFuncs.put("__subvsi3","_subvsi3.o");
		libFuncs.put("__adddf3","adddf3.o");
		libFuncs.put("__fixsfsi","fixsfsi.o");
		libFuncs.put("__bswapdi2","_bswapdi2.o");
		libFuncs.put("__clzsi2","_clzsi2.o");
		libFuncs.put("__clrsbdi2","_clrsbdi2.o");
		libFuncs.put("__mulsc3","_mulsc3.o");
		libFuncs.put("__paritydi2","_paritydi2.o");
		libFuncs.put("__powidf2","_powidf2.o");
		libFuncs.put("__negdf2","negdf2.o");
		libFuncs.put("__trampoline_setup","tramp.o");
		libFuncs.put("__mulvsi3","_mulvsi3.o");
		libFuncs.put("__paritysi2","_paritysi2.o");
		libFuncs.put("__muldc3","_muldc3.o");
		libFuncs.put("__enable_execute_stack","enable-execute-stack.o");
		libFuncs.put("__cmpdi2","_cmpdi2.o");
		libFuncs.put("__floatundisf","_floatundisf.o");
		libFuncs.put("__mulvdi3","_mulvdi3.o");
		libFuncs.put("__fixdfdi","_fixdfdi.o");
		libFuncs.put("__divsc3","_divsc3.o");
		libFuncs.put("__powisf2","_powisf2.o");
		libFuncs.put("__mulsf3","mulsf3.o");
		libFuncs.put("__udiv_w_sdiv","_udiv_w_sdiv.o");
		libFuncs.put("__umoddi3","_umoddi3.o");
		libFuncs.put("__moddi3","_moddi3.o");
		libFuncs.put("__lesf2","lesf2.o");
		libFuncs.put("udivmodsi4","lib2-divmod.o");
		libFuncs.put("__divsi3","lib2-divmod.o");
		libFuncs.put("__modsi3","lib2-divmod.o");
		libFuncs.put("__udivsi3","lib2-divmod.o");
		libFuncs.put("__umodsi3","lib2-divmod.o");
		libFuncs.put("__bswapsi2","_bswapsi2.o");
		libFuncs.put("__absvsi2","_absvsi2.o");
		libFuncs.put("__udivmoddi4","_udivmoddi4.o");
		libFuncs.put("__floatunsisf","floatunsisf.o");
		libFuncs.put("__popcountsi2","_popcountsi2.o");
		libFuncs.put("__addsf3","addsf3.o");
		libFuncs.put("__divdf3","divdf3.o");
		libFuncs.put("__gedf2","gedf2.o");
		libFuncs.put("base_of_encoded_value","unwind-c.o");
		libFuncs.put("read_encoded_value_with_base","unwind-c.o");
		libFuncs.put("__gcc_personality_v0","unwind-c.o");
		libFuncs.put("__ashldi3","_ashldi3.o");
		libFuncs.put("__floatdisf","_floatdisf.o");
		libFuncs.put("__fixunssfdi","_fixunssfdi.o");
		libFuncs.put("__negvsi2","_negvsi2.o");
		libFuncs.put("udivmodhi4","lib2-divmod-hi.o");
		libFuncs.put("__divhi3","lib2-divmod-hi.o");
		libFuncs.put("__modhi3","lib2-divmod-hi.o");
		libFuncs.put("__udivhi3","lib2-divmod-hi.o");
		libFuncs.put("__umodhi3","lib2-divmod-hi.o");
		libFuncs.put("__ledf2","ledf2.o");
		libFuncs.put("__udivdi3","_udivdi3.o");
		libFuncs.put("__mulsi3","lib2-mul.o");
		libFuncs.put("__floatunsidf","floatunsidf.o");
		libFuncs.put("__extendsfdf2","extendsfdf2.o");
		libFuncs.put("__fixsfdi","_fixsfdi.o");
		libFuncs.put("__divsf3","divsf3.o");
		libFuncs.put("__fixunsdfdi","_fixunsdfdi.o");
		libFuncs.put("read_sleb128","unwind-dw2.o");
		libFuncs.put("execute_cfa_program","unwind-dw2.o");
		libFuncs.put("uw_frame_state_for","unwind-dw2.o");
		libFuncs.put("execute_stack_op","unwind-dw2.o");
		libFuncs.put("uw_update_context_1","unwind-dw2.o");
		libFuncs.put("uw_init_context_1","unwind-dw2.o");
		libFuncs.put("uw_update_context","unwind-dw2.o");
		libFuncs.put("_Unwind_RaiseException_Phase2","unwind-dw2.o");
		libFuncs.put("_Unwind_ForcedUnwind_Phase2","unwind-dw2.o");
		libFuncs.put("uw_install_context_1","unwind-dw2.o");
		libFuncs.put("_Unwind_GetGR","unwind-dw2.o");
		libFuncs.put("_Unwind_GetCFA","unwind-dw2.o");
		libFuncs.put("_Unwind_SetGR","unwind-dw2.o");
		libFuncs.put("_Unwind_GetIP","unwind-dw2.o");
		libFuncs.put("_Unwind_GetIPInfo","unwind-dw2.o");
		libFuncs.put("_Unwind_SetIP","unwind-dw2.o");
		libFuncs.put("_Unwind_GetLanguageSpecificData","unwind-dw2.o");
		libFuncs.put("_Unwind_GetRegionStart","unwind-dw2.o");
		libFuncs.put("_Unwind_FindEnclosingFunction","unwind-dw2.o");
		libFuncs.put("_Unwind_GetDataRelBase","unwind-dw2.o");
		libFuncs.put("_Unwind_GetTextRelBase","unwind-dw2.o");
		libFuncs.put("_Unwind_GetEhEncoding","unwind-dw2.o");
		libFuncs.put("__frame_state_for","unwind-dw2.o");
		libFuncs.put("_Unwind_DebugHook","unwind-dw2.o");
		libFuncs.put("_Unwind_RaiseException","unwind-dw2.o");
		libFuncs.put("_Unwind_ForcedUnwind","unwind-dw2.o");
		libFuncs.put("_Unwind_Resume","unwind-dw2.o");
		libFuncs.put("_Unwind_Resume_or_Rethrow","unwind-dw2.o");
		libFuncs.put("_Unwind_DeleteException","unwind-dw2.o");
		libFuncs.put("_Unwind_Backtrace","unwind-dw2.o");
		libFuncs.put("__absvdi2","_absvdi2.o");
		libFuncs.put("__emutls_get_address","emutls.o");
		libFuncs.put("__emutls_register_common","emutls.o");
		libFuncs.put("__eqdf2","eqdf2.o");
		libFuncs.put("__truncdfsf2","truncdfsf2.o");
		libFuncs.put("__negvdi2","_negvdi2.o");
		libFuncs.put("__ucmpdi2","_ucmpdi2.o");
		libFuncs.put("__fixunssfsi","_fixunssfsi.o");
		libFuncs.put("__clrsbsi2","_clrsbsi2.o");
		libFuncs.put("__addvdi3","_addvdi3.o");
		libFuncs.put("__muldi3","_muldi3.o");
		libFuncs.put("__subvdi3","_subvdi3.o");
		libFuncs.put("read_sleb128","unwind-dw2-fde.o");
		libFuncs.put("frame_downheap","unwind-dw2-fde.o");
		libFuncs.put("frame_heapsort","unwind-dw2-fde.o");
		libFuncs.put("size_of_encoded_value","unwind-dw2-fde.o");
		libFuncs.put("read_encoded_value_with_base","unwind-dw2-fde.o");
		libFuncs.put("get_cie_encoding","unwind-dw2-fde.o");
		libFuncs.put("fde_unencoded_compare","unwind-dw2-fde.o");
		libFuncs.put("base_from_object.isra.4","unwind-dw2-fde.o");
		libFuncs.put("fde_single_encoding_compare","unwind-dw2-fde.o");
		libFuncs.put("classify_object_over_fdes","unwind-dw2-fde.o");
		libFuncs.put("linear_search_fdes","unwind-dw2-fde.o");
		libFuncs.put("fde_mixed_encoding_compare","unwind-dw2-fde.o");
		libFuncs.put("add_fdes","unwind-dw2-fde.o");
		libFuncs.put("search_object.isra.6","unwind-dw2-fde.o");
		libFuncs.put("__register_frame_info_header_bases","unwind-dw2-fde.o");
		libFuncs.put("__register_frame_info_bases","unwind-dw2-fde.o");
		libFuncs.put("__register_frame_info","unwind-dw2-fde.o");
		libFuncs.put("__register_frame","unwind-dw2-fde.o");
		libFuncs.put("__register_frame_info_table_bases","unwind-dw2-fde.o");
		libFuncs.put("__register_frame_info_table","unwind-dw2-fde.o");
		libFuncs.put("__register_frame_table","unwind-dw2-fde.o");
		libFuncs.put("__deregister_frame_info_bases","unwind-dw2-fde.o");
		libFuncs.put("__deregister_frame_info","unwind-dw2-fde.o");
		libFuncs.put("__deregister_frame","unwind-dw2-fde.o");
		libFuncs.put("_Unwind_Find_Index","unwind-dw2-fde.o");
		libFuncs.put("_Unwind_Find_FDE","unwind-dw2-fde.o");
		libFuncs.put("__negdi2","_negdi2.o");
		libFuncs.put("__ashrdi3","_ashrdi3.o");
		libFuncs.put("__popcountdi2","_popcountdi2.o");
		libFuncs.put("__clzdi2","_clzdi2.o");
		libFuncs.put("__ffsdi2","_ffsdi2.o");
		libFuncs.put("__ffssi2","_ffssi2.o");
		libFuncs.put("__lshrdi3","_lshrdi3.o");
		libFuncs.put("__ctzsi2","_ctzsi2.o");
		libFuncs.put("__eprintf","_eprintf.o");
		libFuncs.put("__divdi3","_divdi3.o");
		libFuncs.put("__fixunsdfsi","_fixunsdfsi.o");
		libFuncs.put("__floatsisf","floatsisf.o");
		libFuncs.put("__ctzdi2","_ctzdi2.o");
		libFuncs.put("__floatundidf","_floatundidf.o");
		libFuncs.put("__clear_cache","_clear_cache.o");
		libFuncs.put("__negsf2","negsf2.o");
		libFuncs.put("__muldf3","muldf3.o");
		libFuncs.put("__fixdfsi","fixdfsi.o");
		libFuncs.put("__addvsi3","_addvsi3.o");
		libFuncs.put("__unordsf2","unordsf2.o");
		libFuncs.put("__divdc3","_divdc3.o");
		libFuncs.put("__eqsf2","eqsf2.o");
		libFuncs.put("__floatdidf","_floatdidf.o");
		libFuncs.put("__gcc_bcmp","__gcc_bcmp.o");
		libFuncs.put("__subsf3","subsf3.o");
		libFuncs.put("__gesf2","gesf2.o");

	}
	CFG cfg;
	Parser parser;
	public Profiler(String filename){
		parser = new Parser(filename);
	}
	
	public void parseFile(String topName){
		try {
			cfg = parser.parse(topName);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	


	private int getWCET(String string) {
		// TODO Auto-generated method stub
		// 1. Get the root block
		// 2. Traverse the successors, calculating the WCET of each block
		// 3. If the block is a call or jump and the callee is known
		//    calculate and add the WCET of the callee
		// 4. If the block is the head of a loop, then calculate the WCET of the loop
		//    The loop may have a specified max iterations later on, for now assume a number
		//    The loop WCET will be MAXITER*WCET. If there are multiple exit points, then the loop is
		//    WCET*(MAXITER - 1) + maxWCET(successors)
		
		//Costs: Branch -> 4 (Assume mispredicted)
		//       jmp,call,return -> 4
		//       load,store -> 1 + memory delay
		//       else -> 1
		
		
		return 0;
	}

	
	private int getMaxStackSize(String name) {
		Function f = cfg.getFunction(name);
		return getMaxStackSize(f);
	}

	private int getMaxStackSize(Function f) {
		int size = 0;
		int maxCalledSize = 0;
		for(BasicBlock bb : f.bbList){
			size += bb.getStackSize();
			if(bb.type == bbType.CALL || bb.type == bbType.JUMP){
				//Don't add up for all functions, only add up for the maximum function that was called
				//(Matters when lots of functions are called)
				if(bb.callee != null && !bb.callee.equals(f)){
					int callSize = getMaxStackSize(bb.callee);
					if(callSize > maxCalledSize)
						maxCalledSize = callSize;
				}
			}
		}
		size += maxCalledSize;
		return size;
	}
	
	
	


	private String getLibs() {
		String s = "";
		for(Function f : cfg.fList){
			if(libFuncs.containsKey(f.label)){
				s += libFuncs.get(f.label) + ",";
			}
		}
		return s;
	}

	private String getLibFunctions() {
		String s = "";
		for(Function f : cfg.fList){
			if(libFuncs.containsKey(f.label)){
				s += f.label + ",";
			}
		}
		return s;
	}


	public static void main(String[] args) throws IOException{
		if(args.length != 2){
			throw new RuntimeErrorException(new Error("Usage java -jar Codegen.jar file entry_function"));
			
		}
		
		String filename=args[0];
		String entryPoint = args[1];
		System.out.println("results for " + filename + " from function " + entryPoint);
		Profiler prof = new Profiler(filename);
		prof.parseFile(entryPoint);
		System.out.println("Max stack height of main: " + prof.getMaxStackSize(entryPoint) + " bytes");
		System.out.println("WCET of main: " + prof.getWCET(entryPoint) + " clock cycles");
		System.out.println("Library functions are: " + prof.getLibFunctions());
		System.out.println("Add the following lines to linker file: ");
		System.out.println(prof.getLibs());
		
		
	}	
	
}
