function RTW_Sid2UrlHash() {
	this.urlHashMap = new Array();
	/* <Root>/g_socValue */
	this.urlHashMap["YoungMan_LT:765"] = "YoungMan_LT.c:559,598,640&YoungMan_LT.h:65";
	/* <Root>/Tavg */
	this.urlHashMap["YoungMan_LT:766"] = "YoungMan_LT.c:551,579,597,632&YoungMan_LT.h:68";
	/* <Root>/g_lowestTemperature */
	this.urlHashMap["YoungMan_LT:783"] = "YoungMan_LT.c:614&YoungMan_LT.h:71";
	/* <Root>/g_lowestCellVoltage */
	this.urlHashMap["YoungMan_LT:803"] = "YoungMan_LT.c:580&YoungMan_LT.h:74";
	/* <Root>/BigDischargePowerAdjust */
	this.urlHashMap["YoungMan_LT:706"] = "YoungMan_LT.c:540,658,661";
	/* <Root>/ChargeEndSOCAjust */
	this.urlHashMap["YoungMan_LT:784"] = "YoungMan_LT.c:574,673,676";
	/* <Root>/FastChrgPowerAjust */
	this.urlHashMap["YoungMan_LT:751"] = "YoungMan_LT.c:589,668,671";
	/* <Root>/PowerOnSOCAjust */
	this.urlHashMap["YoungMan_LT:792"] = "YoungMan_LT.c:609,678,681";
	/* <Root>/PulseRechargePowerAdjust */
	this.urlHashMap["YoungMan_LT:661"] = "YoungMan_LT.c:621,663,666";
	/* <S1>/Add */
	this.urlHashMap["YoungMan_LT:712"] = "YoungMan_LT.c:555";
	/* <S1>/Constant4 */
	this.urlHashMap["YoungMan_LT:716"] = "YoungMan_LT.c:546";
	/* <S1>/Data Type Conversion */
	this.urlHashMap["YoungMan_LT:770"] = "YoungMan_LT.c:550";
	/* <S1>/Gain */
	this.urlHashMap["YoungMan_LT:722"] = "YoungMan_LT.c:78,570&YoungMan_LT.h:101";
	/* <S2>/2-D Lookup
Table3 */
	this.urlHashMap["YoungMan_LT:786"] = "YoungMan_LT.c:75,179,577&YoungMan_LT.h:44,98";
	/* <S2>/Add */
	this.urlHashMap["YoungMan_LT:787"] = "YoungMan_LT.c:581";
	/* <S2>/Constant4 */
	this.urlHashMap["YoungMan_LT:788"] = "YoungMan_LT.c:578";
	/* <S3>/2-D Lookup
Table3 */
	this.urlHashMap["YoungMan_LT:753"] = "YoungMan_LT.c:184,594&YoungMan_LT.h:49";
	/* <S3>/Add */
	this.urlHashMap["YoungMan_LT:754"] = "YoungMan_LT.c:599";
	/* <S3>/Constant4 */
	this.urlHashMap["YoungMan_LT:755"] = "YoungMan_LT.c:595";
	/* <S3>/Data Type Conversion1 */
	this.urlHashMap["YoungMan_LT:771"] = "YoungMan_LT.c:596";
	/* <S3>/Gain */
	this.urlHashMap["YoungMan_LT:760"] = "YoungMan_LT.c:72,605&YoungMan_LT.h:95";
	/* <S4>/1-D Lookup
Table */
	this.urlHashMap["YoungMan_LT:793"] = "YoungMan_LT.c:69,612&YoungMan_LT.h:92";
	/* <S4>/Add */
	this.urlHashMap["YoungMan_LT:794"] = "YoungMan_LT.c:615";
	/* <S4>/Constant4 */
	this.urlHashMap["YoungMan_LT:795"] = "YoungMan_LT.c:613";
	/* <S4>/Data Type Conversion */
	this.urlHashMap["YoungMan_LT:797"] = "msg=rtwMsg_notTraceable&block=YoungMan_LT:797";
	/* <S5>/Add */
	this.urlHashMap["YoungMan_LT:667"] = "YoungMan_LT.c:636";
	/* <S5>/Constant4 */
	this.urlHashMap["YoungMan_LT:671"] = "YoungMan_LT.c:629";
	/* <S5>/Data Type Conversion */
	this.urlHashMap["YoungMan_LT:769"] = "YoungMan_LT.c:631";
	/* <S5>/Gain */
	this.urlHashMap["YoungMan_LT:677"] = "YoungMan_LT.c:66,651&YoungMan_LT.h:89";
	/* <S6>/Switch */
	this.urlHashMap["YoungMan_LT:743"] = "YoungMan_LT.c:545,568";
	/* <S6>/and */
	this.urlHashMap["YoungMan_LT:744"] = "YoungMan_LT.c:552";
	/* <S6>/linearizationSOF */
	this.urlHashMap["YoungMan_LT:747"] = "YoungMan_LT.c:174,558&YoungMan_LT.h:39";
	/* <S6>/zero */
	this.urlHashMap["YoungMan_LT:748"] = "YoungMan_LT.c:547";
	/* <S7>/Compare */
	this.urlHashMap["YoungMan_LT:745:2"] = "YoungMan_LT.c:553";
	/* <S7>/Constant */
	this.urlHashMap["YoungMan_LT:745:3"] = "YoungMan_LT.c:548";
	/* <S8>/Compare */
	this.urlHashMap["YoungMan_LT:746:2"] = "YoungMan_LT.c:554";
	/* <S8>/Constant */
	this.urlHashMap["YoungMan_LT:746:3"] = "YoungMan_LT.c:549";
	/* <S9>/Switch */
	this.urlHashMap["YoungMan_LT:698"] = "YoungMan_LT.c:626,649";
	/* <S9>/and */
	this.urlHashMap["YoungMan_LT:699"] = "YoungMan_LT.c:633";
	/* <S9>/linearizationSOF */
	this.urlHashMap["YoungMan_LT:702"] = "YoungMan_LT.c:189,639&YoungMan_LT.h:54";
	/* <S9>/zero */
	this.urlHashMap["YoungMan_LT:703"] = "YoungMan_LT.c:630";
	/* <S10>/Compare */
	this.urlHashMap["YoungMan_LT:700:2"] = "YoungMan_LT.c:634";
	/* <S10>/Constant */
	this.urlHashMap["YoungMan_LT:700:3"] = "YoungMan_LT.c:627";
	/* <S11>/Compare */
	this.urlHashMap["YoungMan_LT:701:2"] = "YoungMan_LT.c:635";
	/* <S11>/Constant */
	this.urlHashMap["YoungMan_LT:701:3"] = "YoungMan_LT.c:628";
	this.getUrlHash = function(sid) { return this.urlHashMap[sid];}
}
RTW_Sid2UrlHash.instance = new RTW_Sid2UrlHash();
function RTW_rtwnameSIDMap() {
	this.rtwnameHashMap = new Array();
	this.sidHashMap = new Array();
	this.rtwnameHashMap["<Root>"] = {sid: "YoungMan_LT"};
	this.sidHashMap["YoungMan_LT"] = {rtwname: "<Root>"};
	this.rtwnameHashMap["<S1>"] = {sid: "YoungMan_LT:706"};
	this.sidHashMap["YoungMan_LT:706"] = {rtwname: "<S1>"};
	this.rtwnameHashMap["<S2>"] = {sid: "YoungMan_LT:784"};
	this.sidHashMap["YoungMan_LT:784"] = {rtwname: "<S2>"};
	this.rtwnameHashMap["<S3>"] = {sid: "YoungMan_LT:751"};
	this.sidHashMap["YoungMan_LT:751"] = {rtwname: "<S3>"};
	this.rtwnameHashMap["<S4>"] = {sid: "YoungMan_LT:792"};
	this.sidHashMap["YoungMan_LT:792"] = {rtwname: "<S4>"};
	this.rtwnameHashMap["<S5>"] = {sid: "YoungMan_LT:661"};
	this.sidHashMap["YoungMan_LT:661"] = {rtwname: "<S5>"};
	this.rtwnameHashMap["<S6>"] = {sid: "YoungMan_LT:740"};
	this.sidHashMap["YoungMan_LT:740"] = {rtwname: "<S6>"};
	this.rtwnameHashMap["<S7>"] = {sid: "YoungMan_LT:745"};
	this.sidHashMap["YoungMan_LT:745"] = {rtwname: "<S7>"};
	this.rtwnameHashMap["<S8>"] = {sid: "YoungMan_LT:746"};
	this.sidHashMap["YoungMan_LT:746"] = {rtwname: "<S8>"};
	this.rtwnameHashMap["<S9>"] = {sid: "YoungMan_LT:695"};
	this.sidHashMap["YoungMan_LT:695"] = {rtwname: "<S9>"};
	this.rtwnameHashMap["<S10>"] = {sid: "YoungMan_LT:700"};
	this.sidHashMap["YoungMan_LT:700"] = {rtwname: "<S10>"};
	this.rtwnameHashMap["<S11>"] = {sid: "YoungMan_LT:701"};
	this.sidHashMap["YoungMan_LT:701"] = {rtwname: "<S11>"};
	this.rtwnameHashMap["<Root>/g_socValue"] = {sid: "YoungMan_LT:765"};
	this.sidHashMap["YoungMan_LT:765"] = {rtwname: "<Root>/g_socValue"};
	this.rtwnameHashMap["<Root>/Tavg"] = {sid: "YoungMan_LT:766"};
	this.sidHashMap["YoungMan_LT:766"] = {rtwname: "<Root>/Tavg"};
	this.rtwnameHashMap["<Root>/g_lowestTemperature"] = {sid: "YoungMan_LT:783"};
	this.sidHashMap["YoungMan_LT:783"] = {rtwname: "<Root>/g_lowestTemperature"};
	this.rtwnameHashMap["<Root>/g_lowestCellVoltage"] = {sid: "YoungMan_LT:803"};
	this.sidHashMap["YoungMan_LT:803"] = {rtwname: "<Root>/g_lowestCellVoltage"};
	this.rtwnameHashMap["<Root>/BigDischargePowerAdjust"] = {sid: "YoungMan_LT:706"};
	this.sidHashMap["YoungMan_LT:706"] = {rtwname: "<Root>/BigDischargePowerAdjust"};
	this.rtwnameHashMap["<Root>/ChargeEndSOCAjust"] = {sid: "YoungMan_LT:784"};
	this.sidHashMap["YoungMan_LT:784"] = {rtwname: "<Root>/ChargeEndSOCAjust"};
	this.rtwnameHashMap["<Root>/FastChrgPowerAjust"] = {sid: "YoungMan_LT:751"};
	this.sidHashMap["YoungMan_LT:751"] = {rtwname: "<Root>/FastChrgPowerAjust"};
	this.rtwnameHashMap["<Root>/PowerOnSOCAjust"] = {sid: "YoungMan_LT:792"};
	this.sidHashMap["YoungMan_LT:792"] = {rtwname: "<Root>/PowerOnSOCAjust"};
	this.rtwnameHashMap["<Root>/PulseRechargePowerAdjust"] = {sid: "YoungMan_LT:661"};
	this.sidHashMap["YoungMan_LT:661"] = {rtwname: "<Root>/PulseRechargePowerAdjust"};
	this.rtwnameHashMap["<Root>/BiggestDischargeCurt "] = {sid: "YoungMan_LT:761"};
	this.sidHashMap["YoungMan_LT:761"] = {rtwname: "<Root>/BiggestDischargeCurt "};
	this.rtwnameHashMap["<Root>/BiggestFeedbackCurt"] = {sid: "YoungMan_LT:762"};
	this.sidHashMap["YoungMan_LT:762"] = {rtwname: "<Root>/BiggestFeedbackCurt"};
	this.rtwnameHashMap["<Root>/m_askcurrent "] = {sid: "YoungMan_LT:768"};
	this.sidHashMap["YoungMan_LT:768"] = {rtwname: "<Root>/m_askcurrent "};
	this.rtwnameHashMap["<Root>/PowerOnSOCAjustFactor"] = {sid: "YoungMan_LT:799"};
	this.sidHashMap["YoungMan_LT:799"] = {rtwname: "<Root>/PowerOnSOCAjustFactor"};
	this.rtwnameHashMap["<Root>/ChargeEndSOCAjustFactor"] = {sid: "YoungMan_LT:800"};
	this.sidHashMap["YoungMan_LT:800"] = {rtwname: "<Root>/ChargeEndSOCAjustFactor"};
	this.rtwnameHashMap["<S1>/SOC"] = {sid: "YoungMan_LT:707"};
	this.sidHashMap["YoungMan_LT:707"] = {rtwname: "<S1>/SOC"};
	this.rtwnameHashMap["<S1>/T"] = {sid: "YoungMan_LT:708"};
	this.sidHashMap["YoungMan_LT:708"] = {rtwname: "<S1>/T"};
	this.rtwnameHashMap["<S1>/Add"] = {sid: "YoungMan_LT:712"};
	this.sidHashMap["YoungMan_LT:712"] = {rtwname: "<S1>/Add"};
	this.rtwnameHashMap["<S1>/Constant4"] = {sid: "YoungMan_LT:716"};
	this.sidHashMap["YoungMan_LT:716"] = {rtwname: "<S1>/Constant4"};
	this.rtwnameHashMap["<S1>/Data Type Conversion"] = {sid: "YoungMan_LT:770"};
	this.sidHashMap["YoungMan_LT:770"] = {rtwname: "<S1>/Data Type Conversion"};
	this.rtwnameHashMap["<S1>/Gain"] = {sid: "YoungMan_LT:722"};
	this.sidHashMap["YoungMan_LT:722"] = {rtwname: "<S1>/Gain"};
	this.rtwnameHashMap["<S1>/linearizationSOF"] = {sid: "YoungMan_LT:740"};
	this.sidHashMap["YoungMan_LT:740"] = {rtwname: "<S1>/linearizationSOF"};
	this.rtwnameHashMap["<S1>/biggestDischargeCurrent"] = {sid: "YoungMan_LT:750"};
	this.sidHashMap["YoungMan_LT:750"] = {rtwname: "<S1>/biggestDischargeCurrent"};
	this.rtwnameHashMap["<S2>/g_CellLowestVol"] = {sid: "YoungMan_LT:785"};
	this.sidHashMap["YoungMan_LT:785"] = {rtwname: "<S2>/g_CellLowestVol"};
	this.rtwnameHashMap["<S2>/T"] = {sid: "YoungMan_LT:802"};
	this.sidHashMap["YoungMan_LT:802"] = {rtwname: "<S2>/T"};
	this.rtwnameHashMap["<S2>/2-D Lookup Table3"] = {sid: "YoungMan_LT:786"};
	this.sidHashMap["YoungMan_LT:786"] = {rtwname: "<S2>/2-D Lookup Table3"};
	this.rtwnameHashMap["<S2>/Add"] = {sid: "YoungMan_LT:787"};
	this.sidHashMap["YoungMan_LT:787"] = {rtwname: "<S2>/Add"};
	this.rtwnameHashMap["<S2>/Constant4"] = {sid: "YoungMan_LT:788"};
	this.sidHashMap["YoungMan_LT:788"] = {rtwname: "<S2>/Constant4"};
	this.rtwnameHashMap["<S2>/ChargeEndSOCAjustFactor"] = {sid: "YoungMan_LT:791"};
	this.sidHashMap["YoungMan_LT:791"] = {rtwname: "<S2>/ChargeEndSOCAjustFactor"};
	this.rtwnameHashMap["<S3>/SOC"] = {sid: "YoungMan_LT:763"};
	this.sidHashMap["YoungMan_LT:763"] = {rtwname: "<S3>/SOC"};
	this.rtwnameHashMap["<S3>/T"] = {sid: "YoungMan_LT:764"};
	this.sidHashMap["YoungMan_LT:764"] = {rtwname: "<S3>/T"};
	this.rtwnameHashMap["<S3>/2-D Lookup Table3"] = {sid: "YoungMan_LT:753"};
	this.sidHashMap["YoungMan_LT:753"] = {rtwname: "<S3>/2-D Lookup Table3"};
	this.rtwnameHashMap["<S3>/Add"] = {sid: "YoungMan_LT:754"};
	this.sidHashMap["YoungMan_LT:754"] = {rtwname: "<S3>/Add"};
	this.rtwnameHashMap["<S3>/Constant4"] = {sid: "YoungMan_LT:755"};
	this.sidHashMap["YoungMan_LT:755"] = {rtwname: "<S3>/Constant4"};
	this.rtwnameHashMap["<S3>/Data Type Conversion1"] = {sid: "YoungMan_LT:771"};
	this.sidHashMap["YoungMan_LT:771"] = {rtwname: "<S3>/Data Type Conversion1"};
	this.rtwnameHashMap["<S3>/Gain"] = {sid: "YoungMan_LT:760"};
	this.sidHashMap["YoungMan_LT:760"] = {rtwname: "<S3>/Gain"};
	this.rtwnameHashMap["<S3>/AskChaCur"] = {sid: "YoungMan_LT:767"};
	this.sidHashMap["YoungMan_LT:767"] = {rtwname: "<S3>/AskChaCur"};
	this.rtwnameHashMap["<S4>/T"] = {sid: "YoungMan_LT:801"};
	this.sidHashMap["YoungMan_LT:801"] = {rtwname: "<S4>/T"};
	this.rtwnameHashMap["<S4>/1-D Lookup Table"] = {sid: "YoungMan_LT:793"};
	this.sidHashMap["YoungMan_LT:793"] = {rtwname: "<S4>/1-D Lookup Table"};
	this.rtwnameHashMap["<S4>/Add"] = {sid: "YoungMan_LT:794"};
	this.sidHashMap["YoungMan_LT:794"] = {rtwname: "<S4>/Add"};
	this.rtwnameHashMap["<S4>/Constant4"] = {sid: "YoungMan_LT:795"};
	this.sidHashMap["YoungMan_LT:795"] = {rtwname: "<S4>/Constant4"};
	this.rtwnameHashMap["<S4>/Data Type Conversion"] = {sid: "YoungMan_LT:797"};
	this.sidHashMap["YoungMan_LT:797"] = {rtwname: "<S4>/Data Type Conversion"};
	this.rtwnameHashMap["<S4>/PowerOnSOCAjustFactor"] = {sid: "YoungMan_LT:798"};
	this.sidHashMap["YoungMan_LT:798"] = {rtwname: "<S4>/PowerOnSOCAjustFactor"};
	this.rtwnameHashMap["<S5>/SOC"] = {sid: "YoungMan_LT:662"};
	this.sidHashMap["YoungMan_LT:662"] = {rtwname: "<S5>/SOC"};
	this.rtwnameHashMap["<S5>/T"] = {sid: "YoungMan_LT:663"};
	this.sidHashMap["YoungMan_LT:663"] = {rtwname: "<S5>/T"};
	this.rtwnameHashMap["<S5>/Add"] = {sid: "YoungMan_LT:667"};
	this.sidHashMap["YoungMan_LT:667"] = {rtwname: "<S5>/Add"};
	this.rtwnameHashMap["<S5>/Constant4"] = {sid: "YoungMan_LT:671"};
	this.sidHashMap["YoungMan_LT:671"] = {rtwname: "<S5>/Constant4"};
	this.rtwnameHashMap["<S5>/Data Type Conversion"] = {sid: "YoungMan_LT:769"};
	this.sidHashMap["YoungMan_LT:769"] = {rtwname: "<S5>/Data Type Conversion"};
	this.rtwnameHashMap["<S5>/Gain"] = {sid: "YoungMan_LT:677"};
	this.sidHashMap["YoungMan_LT:677"] = {rtwname: "<S5>/Gain"};
	this.rtwnameHashMap["<S5>/linearizationSOF"] = {sid: "YoungMan_LT:695"};
	this.sidHashMap["YoungMan_LT:695"] = {rtwname: "<S5>/linearizationSOF"};
	this.rtwnameHashMap["<S5>/biggestRechargeCurrent"] = {sid: "YoungMan_LT:705"};
	this.sidHashMap["YoungMan_LT:705"] = {rtwname: "<S5>/biggestRechargeCurrent"};
	this.rtwnameHashMap["<S6>/soc"] = {sid: "YoungMan_LT:741"};
	this.sidHashMap["YoungMan_LT:741"] = {rtwname: "<S6>/soc"};
	this.rtwnameHashMap["<S6>/tem"] = {sid: "YoungMan_LT:742"};
	this.sidHashMap["YoungMan_LT:742"] = {rtwname: "<S6>/tem"};
	this.rtwnameHashMap["<S6>/Switch"] = {sid: "YoungMan_LT:743"};
	this.sidHashMap["YoungMan_LT:743"] = {rtwname: "<S6>/Switch"};
	this.rtwnameHashMap["<S6>/and"] = {sid: "YoungMan_LT:744"};
	this.sidHashMap["YoungMan_LT:744"] = {rtwname: "<S6>/and"};
	this.rtwnameHashMap["<S6>/greaterEq"] = {sid: "YoungMan_LT:745"};
	this.sidHashMap["YoungMan_LT:745"] = {rtwname: "<S6>/greaterEq"};
	this.rtwnameHashMap["<S6>/lessEq"] = {sid: "YoungMan_LT:746"};
	this.sidHashMap["YoungMan_LT:746"] = {rtwname: "<S6>/lessEq"};
	this.rtwnameHashMap["<S6>/linearizationSOF"] = {sid: "YoungMan_LT:747"};
	this.sidHashMap["YoungMan_LT:747"] = {rtwname: "<S6>/linearizationSOF"};
	this.rtwnameHashMap["<S6>/zero"] = {sid: "YoungMan_LT:748"};
	this.sidHashMap["YoungMan_LT:748"] = {rtwname: "<S6>/zero"};
	this.rtwnameHashMap["<S6>/BigDisPower"] = {sid: "YoungMan_LT:749"};
	this.sidHashMap["YoungMan_LT:749"] = {rtwname: "<S6>/BigDisPower"};
	this.rtwnameHashMap["<S7>/u"] = {sid: "YoungMan_LT:745:1"};
	this.sidHashMap["YoungMan_LT:745:1"] = {rtwname: "<S7>/u"};
	this.rtwnameHashMap["<S7>/Compare"] = {sid: "YoungMan_LT:745:2"};
	this.sidHashMap["YoungMan_LT:745:2"] = {rtwname: "<S7>/Compare"};
	this.rtwnameHashMap["<S7>/Constant"] = {sid: "YoungMan_LT:745:3"};
	this.sidHashMap["YoungMan_LT:745:3"] = {rtwname: "<S7>/Constant"};
	this.rtwnameHashMap["<S7>/y"] = {sid: "YoungMan_LT:745:4"};
	this.sidHashMap["YoungMan_LT:745:4"] = {rtwname: "<S7>/y"};
	this.rtwnameHashMap["<S8>/u"] = {sid: "YoungMan_LT:746:1"};
	this.sidHashMap["YoungMan_LT:746:1"] = {rtwname: "<S8>/u"};
	this.rtwnameHashMap["<S8>/Compare"] = {sid: "YoungMan_LT:746:2"};
	this.sidHashMap["YoungMan_LT:746:2"] = {rtwname: "<S8>/Compare"};
	this.rtwnameHashMap["<S8>/Constant"] = {sid: "YoungMan_LT:746:3"};
	this.sidHashMap["YoungMan_LT:746:3"] = {rtwname: "<S8>/Constant"};
	this.rtwnameHashMap["<S8>/y"] = {sid: "YoungMan_LT:746:4"};
	this.sidHashMap["YoungMan_LT:746:4"] = {rtwname: "<S8>/y"};
	this.rtwnameHashMap["<S9>/soc"] = {sid: "YoungMan_LT:696"};
	this.sidHashMap["YoungMan_LT:696"] = {rtwname: "<S9>/soc"};
	this.rtwnameHashMap["<S9>/tem"] = {sid: "YoungMan_LT:697"};
	this.sidHashMap["YoungMan_LT:697"] = {rtwname: "<S9>/tem"};
	this.rtwnameHashMap["<S9>/Switch"] = {sid: "YoungMan_LT:698"};
	this.sidHashMap["YoungMan_LT:698"] = {rtwname: "<S9>/Switch"};
	this.rtwnameHashMap["<S9>/and"] = {sid: "YoungMan_LT:699"};
	this.sidHashMap["YoungMan_LT:699"] = {rtwname: "<S9>/and"};
	this.rtwnameHashMap["<S9>/greaterEq"] = {sid: "YoungMan_LT:700"};
	this.sidHashMap["YoungMan_LT:700"] = {rtwname: "<S9>/greaterEq"};
	this.rtwnameHashMap["<S9>/lessEq"] = {sid: "YoungMan_LT:701"};
	this.sidHashMap["YoungMan_LT:701"] = {rtwname: "<S9>/lessEq"};
	this.rtwnameHashMap["<S9>/linearizationSOF"] = {sid: "YoungMan_LT:702"};
	this.sidHashMap["YoungMan_LT:702"] = {rtwname: "<S9>/linearizationSOF"};
	this.rtwnameHashMap["<S9>/zero"] = {sid: "YoungMan_LT:703"};
	this.sidHashMap["YoungMan_LT:703"] = {rtwname: "<S9>/zero"};
	this.rtwnameHashMap["<S9>/BigDisPower"] = {sid: "YoungMan_LT:704"};
	this.sidHashMap["YoungMan_LT:704"] = {rtwname: "<S9>/BigDisPower"};
	this.rtwnameHashMap["<S10>/u"] = {sid: "YoungMan_LT:700:1"};
	this.sidHashMap["YoungMan_LT:700:1"] = {rtwname: "<S10>/u"};
	this.rtwnameHashMap["<S10>/Compare"] = {sid: "YoungMan_LT:700:2"};
	this.sidHashMap["YoungMan_LT:700:2"] = {rtwname: "<S10>/Compare"};
	this.rtwnameHashMap["<S10>/Constant"] = {sid: "YoungMan_LT:700:3"};
	this.sidHashMap["YoungMan_LT:700:3"] = {rtwname: "<S10>/Constant"};
	this.rtwnameHashMap["<S10>/y"] = {sid: "YoungMan_LT:700:4"};
	this.sidHashMap["YoungMan_LT:700:4"] = {rtwname: "<S10>/y"};
	this.rtwnameHashMap["<S11>/u"] = {sid: "YoungMan_LT:701:1"};
	this.sidHashMap["YoungMan_LT:701:1"] = {rtwname: "<S11>/u"};
	this.rtwnameHashMap["<S11>/Compare"] = {sid: "YoungMan_LT:701:2"};
	this.sidHashMap["YoungMan_LT:701:2"] = {rtwname: "<S11>/Compare"};
	this.rtwnameHashMap["<S11>/Constant"] = {sid: "YoungMan_LT:701:3"};
	this.sidHashMap["YoungMan_LT:701:3"] = {rtwname: "<S11>/Constant"};
	this.rtwnameHashMap["<S11>/y"] = {sid: "YoungMan_LT:701:4"};
	this.sidHashMap["YoungMan_LT:701:4"] = {rtwname: "<S11>/y"};
	this.getSID = function(rtwname) { return this.rtwnameHashMap[rtwname];}
	this.getRtwname = function(sid) { return this.sidHashMap[sid];}
}
RTW_rtwnameSIDMap.instance = new RTW_rtwnameSIDMap();
