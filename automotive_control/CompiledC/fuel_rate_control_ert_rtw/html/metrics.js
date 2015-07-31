function CodeMetrics() {
	 this.metricsArray = {};
	 this.metricsArray.var = new Array();
	 this.metricsArray.fcn = new Array();
	 this.metricsArray.fcn["fuel_rate_control.c:fuel_rate_control_Fail"] = {file: "/home/jonah/fingerprinting/automotive_control/gen/fuel_rate_control_ert_rtw/fuel_rate_control.c",
	stack: 4,
	stackTotal: 4};
	 this.metricsArray.fcn["fuel_rate_control.c:fuel_rate_control_Fueling_Mode"] = {file: "/home/jonah/fingerprinting/automotive_control/gen/fuel_rate_control_ert_rtw/fuel_rate_control.c",
	stack: 0,
	stackTotal: 0};
	 this.metricsArray.fcn["fuel_rate_control_initialize"] = {file: "/home/jonah/fingerprinting/automotive_control/gen/fuel_rate_control_ert_rtw/fuel_rate_control.c",
	stack: 8,
	stackTotal: 8};
	 this.metricsArray.fcn["fuel_rate_control_step"] = {file: "/home/jonah/fingerprinting/automotive_control/gen/fuel_rate_control_ert_rtw/fuel_rate_control.c",
	stack: 28,
	stackTotal: 64};
	 this.metricsArray.fcn["look2_iflf_linlca"] = {file: "/home/jonah/fingerprinting/automotive_control/gen/fuel_rate_control_ert_rtw/fuel_rate_control.c",
	stack: 36,
	stackTotal: 36};
	 this.getMetrics = function(token) { 
		 var data;
		 data = this.metricsArray.var[token];
		 if (!data) {
			 data = this.metricsArray.fcn[token];
			 if (data) data.type = "fcn";
		 } else { 
			 data.type = "var";
		 }
	 return data;}
}
	 CodeMetrics.instance = new CodeMetrics();
