function runtlccmd
% RUNTLCCMD - run tlc command (regenerate C code from .rtw file) for model Sum
% This function will run the tlc command stored in the variable 
% "tlccmd" in tlccmd.mat, whose contents is as follows:
% 
% 	tlc
% 	-r
% 	/Users/anthonydelage/Documents/MATLAB/Sum_ert_rtw/Sum.rtw
% 	/Applications/MATLAB_R2014a.app/rtw/c/ert/ert.tlc
% 	-O/Users/anthonydelage/Documents/MATLAB/Sum_ert_rtw
% 	-I/Applications/MATLAB_R2014a.app/rtw/c/ert
% 	-I/Users/anthonydelage/Documents/MATLAB/Sum_ert_rtw/tlc
% 	-I/Applications/MATLAB_R2014a.app/rtw/c/tlc/mw
% 	-I/Applications/MATLAB_R2014a.app/rtw/c/tlc/lib
% 	-I/Applications/MATLAB_R2014a.app/rtw/c/tlc/blocks
% 	-I/Applications/MATLAB_R2014a.app/rtw/c/tlc/fixpt
% 	-I/Applications/MATLAB_R2014a.app/stateflow/c/tlc
% 	-aFoldNonRolledExpr=1
% 	-aInlineInvariantSignals=0
% 	-aInlineParameters=2
% 	-aLocalBlockOutputs=1
% 	-aRollThreshold=5
% 	-aZeroInternalMemoryAtStartup=1
% 	-aZeroExternalMemoryAtStartup=1
% 	-aInitFltsAndDblsToZero=0
% 	-aForceBlockIOInitOptimize=0
% 	-aGenerateReport=0
% 	-aGenCodeOnly=0
% 	-aRTWVerbose=1
% 	-aIncludeHyperlinkInReport=0
% 	-aLaunchReport=0
% 	-aGenerateTraceInfo=0
% 	-aPortableWordSizes=0
% 	-aGenerateErtSFunction=0
% 	-aForceParamTrailComments=0
% 	-aGenerateComments=1
% 	-aIgnoreCustomStorageClasses=0
% 	-aIncHierarchyInIds=0
% 	-aMaxRTWIdLen=31
% 	-aShowEliminatedStatements=0
% 	-aIncDataTypeInIds=0
% 	-aInsertBlockDesc=0
% 	-aIgnoreTestpoints=0
% 	-aSimulinkBlockComments=1
% 	-aInlinedPrmAccess="Literals"
% 	-aTargetFcnLib="ansi_tfl_table_tmw.mat"
% 	-aGenFloatMathFcnCalls="None"
% 	-aIncludeMdlTerminateFcn=1
% 	-aCombineOutputUpdateFcns=1
% 	-aSuppressErrorStatus=0
% 	-aERTCustomFileBanners=1
% 	-aLogVarNameModifier="rt_"
% 	-aGenerateFullHeader=1
% 	-aGenerateSampleERTMain=1
% 	-aMatFileLogging=0
% 	-aMultiInstanceERTCode=0
% 	-aPurelyIntegerCode=0
% 	-aGenerateASAP2=0
% 	-aExtMode=0
% 	-aExtModeStaticAlloc=0
% 	-aExtModeStaticAllocSize=1000000
% 	-aExtModeTransport=0
% 	-aExtModeTesting=0
% 	-aInlinedParameterPlacement="Hierarchical"
% 	-aTargetOS="BareBoardExample"
% 	-aMultiInstanceErrorCode="Error"
% 	-aRateGroupingCode=1
% 	-aRTWCAPISignals=0
% 	-aRTWCAPIParams=0
% 	-aRootIOStructures=0
% 	-aERTCustomFileTemplate="example_file_process.tlc"
% 	-aGenerateTraceInfo=0
% 	-aIgnoreTestpoints=0
% 	-aProtectedModelReferenceTarget=0
% 	-p10000

   disp('This function will be obsoleted in a future release.') 
   mdl = 'Sum';

   sysopen = ~isempty(strmatch(mdl, find_system('type', 'block_diagram'), 'exact'));

   if ~sysopen

      disp([mfilename ': Error: model ' mdl ' is not open. Please open model ' mdl ' and then run ' mfilename ' again.']);

   else

      rtwprivate('rtwattic', 'setBuildDir', '/Users/anthonydelage/Documents/MATLAB/Sum_ert_rtw');
      rtwprivate('ec_set_replacement_flag', 'Sum');
      load tlccmd.mat;
      savedpwd = pwd;
      cd ..;
      coder.internal.ModelCodegenMgr.setInstance(modelCodegenMgr);
      tlccmd{end+1} = '-aSLCGUseRTWContext=0';
      feval(tlccmd{:});
      coder.internal.ModelCodegenMgr.setInstance([]);
      rtwprivate rtwattic clean;
      cd(savedpwd);

   end
