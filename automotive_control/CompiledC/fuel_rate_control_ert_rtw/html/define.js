function CodeDefine() { 
this.def = new Array();
this.def["fuel_rate_control_M_"] = {file: "ert_main_c.html",line:27,type:"var"};
this.def["fuel_rate_control_M"] = {file: "ert_main_c.html",line:28,type:"var"};
this.def["fuel_rate_control_DW"] = {file: "ert_main_c.html",line:30,type:"var"};
this.def["fuel_rate_control_U"] = {file: "ert_main_c.html",line:31,type:"var"};
this.def["fuel_rate_control_Y"] = {file: "ert_main_c.html",line:32,type:"var"};
this.def["rt_OneStep"] = {file: "ert_main_c.html",line:46,type:"fcn"};
this.def["main"] = {file: "ert_main_c.html",line:84,type:"fcn"};
this.def["look2_iflf_linlca"] = {file: "fuel_rate_control_c.html",line:63,type:"fcn"};
this.def["fuel_rate_control_Fueling_Mode"] = {file: "fuel_rate_control_c.html",line:165,type:"fcn"};
this.def["fuel_rate_control_Fail"] = {file: "fuel_rate_control_c.html",line:396,type:"fcn"};
this.def["fuel_rate_control_step"] = {file: "fuel_rate_control_c.html",line:505,type:"fcn"};
this.def["fuel_rate_control_initialize"] = {file: "fuel_rate_control_c.html",line:1035,type:"fcn"};
this.def["RT_MODEL_fuel_rate_control_T"] = {file: "fuel_rate_control_h.html",line:40,type:"type"};
this.def["sld_FuelModes"] = {file: "fuel_rate_control_h.html",line:49,type:"type"};
this.def["EngSensors"] = {file: "fuel_rate_control_h.html",line:61,type:"type"};
this.def["DW_fuel_rate_control_T"] = {file: "fuel_rate_control_h.html",line:99,type:"type"};
this.def["ConstP_fuel_rate_control_T"] = {file: "fuel_rate_control_h.html",line:182,type:"type"};
this.def["ExtU_fuel_rate_control_T"] = {file: "fuel_rate_control_h.html",line:187,type:"type"};
this.def["ExtY_fuel_rate_control_T"] = {file: "fuel_rate_control_h.html",line:192,type:"type"};
this.def["fuel_rate_control_ConstP"] = {file: "fuel_rate_control_data_c.html",line:25,type:"var"};
this.def["int8_T"] = {file: "rtwtypes_h.html",line:53,type:"type"};
this.def["uint8_T"] = {file: "rtwtypes_h.html",line:54,type:"type"};
this.def["int16_T"] = {file: "rtwtypes_h.html",line:55,type:"type"};
this.def["uint16_T"] = {file: "rtwtypes_h.html",line:56,type:"type"};
this.def["int32_T"] = {file: "rtwtypes_h.html",line:57,type:"type"};
this.def["uint32_T"] = {file: "rtwtypes_h.html",line:58,type:"type"};
this.def["int64_T"] = {file: "rtwtypes_h.html",line:59,type:"type"};
this.def["uint64_T"] = {file: "rtwtypes_h.html",line:60,type:"type"};
this.def["real32_T"] = {file: "rtwtypes_h.html",line:61,type:"type"};
this.def["real64_T"] = {file: "rtwtypes_h.html",line:62,type:"type"};
this.def["real_T"] = {file: "rtwtypes_h.html",line:68,type:"type"};
this.def["time_T"] = {file: "rtwtypes_h.html",line:69,type:"type"};
this.def["boolean_T"] = {file: "rtwtypes_h.html",line:70,type:"type"};
this.def["int_T"] = {file: "rtwtypes_h.html",line:71,type:"type"};
this.def["uint_T"] = {file: "rtwtypes_h.html",line:72,type:"type"};
this.def["ulong_T"] = {file: "rtwtypes_h.html",line:73,type:"type"};
this.def["ulonglong_T"] = {file: "rtwtypes_h.html",line:74,type:"type"};
this.def["char_T"] = {file: "rtwtypes_h.html",line:75,type:"type"};
this.def["uchar_T"] = {file: "rtwtypes_h.html",line:76,type:"type"};
this.def["byte_T"] = {file: "rtwtypes_h.html",line:77,type:"type"};
this.def["pointer_T"] = {file: "rtwtypes_h.html",line:98,type:"type"};
}
CodeDefine.instance = new CodeDefine();
var testHarnessInfo = {OwnerFileName: "", HarnessOwner: "", HarnessName: "", IsTestHarness: "0"};
function Html2SrcLink() {
	this.html2SrcPath = new Array;
	this.html2Root = new Array;
	this.html2SrcPath["ert_main_c.html"] = "../ert_main.c";
	this.html2Root["ert_main_c.html"] = "ert_main_c.html";
	this.html2SrcPath["fuel_rate_control_c.html"] = "../fuel_rate_control.c";
	this.html2Root["fuel_rate_control_c.html"] = "fuel_rate_control_c.html";
	this.html2SrcPath["fuel_rate_control_h.html"] = "../fuel_rate_control.h";
	this.html2Root["fuel_rate_control_h.html"] = "fuel_rate_control_h.html";
	this.html2SrcPath["fuel_rate_control_data_c.html"] = "../fuel_rate_control_data.c";
	this.html2Root["fuel_rate_control_data_c.html"] = "fuel_rate_control_data_c.html";
	this.html2SrcPath["rtwtypes_h.html"] = "../rtwtypes.h";
	this.html2Root["rtwtypes_h.html"] = "rtwtypes_h.html";
	this.getLink2Src = function (htmlFileName) {
		 if (this.html2SrcPath[htmlFileName])
			 return this.html2SrcPath[htmlFileName];
		 else
			 return null;
	}
	this.getLinkFromRoot = function (htmlFileName) {
		 if (this.html2Root[htmlFileName])
			 return this.html2Root[htmlFileName];
		 else
			 return null;
	}
}
Html2SrcLink.instance = new Html2SrcLink();
var fileList = [
"ert_main_c.html","fuel_rate_control_c.html","fuel_rate_control_h.html","fuel_rate_control_data_c.html","rtwtypes_h.html"];
