<?php 
	include("convertnumber.php");
?>
<?php
$ip = $_POST['ip_out'];

//2020.04.01 Rechel:查看ip the "hostname" is a valid command in both windows and linux
$exec = exec("hostname -I"); 
$hostname = trim($exec); //remove any spaces before and after
$ip = gethostbyname($hostname); //resolves the hostname using local hosts resolver or DNS
?>
<?php
	$db23 = new PDO('sqlite:/var/www/html/database/tcs.db');
	$db23->exec('set names utf-8'); 
	// 2019.10.18 Eric：設定文字UTF-8格式
	//$sql22955 = "SELECT * FROM jobsequence INNER JOIN barcode ON jobsequence.job_id = barcode.selected_job where job_id = '$job_id_startget00'";
	$sql22955 = "SELECT * FROM barcode INNER JOIN jobsequence ON barcode.selected_job = jobsequence.job_id where barcode = '$job_id_startget00' limit 1";
	$stmt955 = $db23->query($sql22955); 
	// 2019.10.18 Eric：獲取承上$sql22955資料庫語法，$stmt955為一個變數
	while($row955 = $stmt955->fetch()) 
		{
			$barcode_job_name = $row955[0];
			$barcode_job_id = $row955[1];
        }
?>
<?php
	//$db2 = new PDO('sqlite:/var/www/database/tcs'); // 2019.10.18 Eric：連結到資料庫路徑
	$db99 = new PDO('sqlite:/var/www/html/database/tcs.db');
	$db99->exec('set names utf-8'); // 2019.10.18 Eric：設定文字UTF-8格式
	$str3 = "select * from device limit 1";
	$rows58 = $db99->query($str3); // 2019.10.18 Eric：獲取承上$sql資料庫語法，$rows為一個變數存取
	$row58 = $rows58->fetch(); // 2019.10.18 Eric：變數$row擷取$rows的$sql資料庫語法之所有欄位資料
	$unit = $row58[8];
	if($unit == "0"){
		$unit = "kgf.m";
	}
	if($unit == "1"){
		$unit = "N.m";
	}
	if($unit == "2"){
		$unit = "kgf.cm";
	}
	if($unit == "3"){
		$unit = "lbf.in";
	}
	$db99 = null;
?>
<!DOCTYPE html>
<html>
<style>
    body {
        overflow: hidden;
    }
	select > option{
        font-size: 40px;
	}
	#sidebar { 
-webkit-appearance: none;
appearance: none;
color:blue; 
font-weight:bold;
text-decoration:none; 
} 
.active {
  background-color: red;  
}

.inactive {
  background-color: #FFFFFF;
}

.torque{
        font-size: 40px;
}
.size{
    width:100px;
    height: 100px;
    background: #8BC34A;
	border - radius:100px;
}

.btn.active.focus,.btn.active:focus,.btn.focus,.btn:active.focus,.btn:active:focus,
.btn:focus{outline:thin dotted;outline:5px auto -webkit-focus-ring-color;outline-offset:-2px}
.btn.focus,.btn:focus,.btn:hover{color:#333;text-decoration:none}
.btn.active,.btn:active{background-image:none;outline:0;-webkit-box-shadow:inset 0 3px 5px rgba(0,0,0,.125);

/*.nocursor { cursor:none; }*/
</style>
<script src="jquery/jquery.min.js.js" type="text/javascript" charset="UTF-8"></script>
<link href="css/tcs.css" rel="stylesheet" type="text/css">
<!-- <body bgcolor="white" onload="init();" class="nocursor">-->
<body bgcolor="white" class="nocursor">
<div id="container" style="width:800px;height:1280px">
<div id="content1" style="background-color:#EEEEEE;height:5%;width:800px;float:left;">
  <table width="800" height="100%" border="0">
    <tr>
      <td width="520" style="padding-left:12px;padding-bottom:20px;font-size:30px;">&nbsp;&nbsp;Job ID : 
           <select onchange="javascript:Select_job(this)" name="Job_ID" id="Job_ID" style="font-size:30px;">
		   <?php
		if ($row955[1] == ''){   
			$select07 = "Normal";
		}
		if ($row955[1] == '1'){   
			$select07 = "1";
		}
		// 2019.10.18 Eric：判斷變數$direction為選擇的項目為0時$direction2="NO"，反之則$direction2="YES"
		if ($row955[1] == '2'){
			$select07 = "2";
		}
		if ($row955[1] == '3'){   
			$select07 = "3";
		}
		if ($row955[1] == '100'){
			$select07 = "Clear";
		}
		if ($row955[1] == '101'){   
			$select07 = "Disable";
		}
		if ($row955[1] == '102'){
			$select07 = "NS";
		}
		if ($row955[1] == '103'){   
			$select07 = "AS";
		}
		// 2019.10.18 Eric：判斷變數$direction為選擇的項目為0時$direction2="NO"，反之則$direction2="YES"
		echo '<option id="job_id_select01" value="'.$row955[1].'" selected="true" disabled="disabled">'.$select07.'</option>';
		   ?>
		   <?php	
		    $db22 = new PDO('sqlite:/var/www/html/database/tcs');
			$db22->exec('set names utf-8'); // 2019.10.18 Eric：設定文字UTF-8格式
			//$sql22520 = "select distinct job_id from jobsequence";
			$sql22520 = "select distinct select_name from jobselect INNER JOIN jobsequence ON jobselect.select_id = jobsequence.job_id";
			$rows520 = $db22->query($sql22520); // 2019.10.18 Eric：獲取承上$sql22520資料庫語法，$rows520為一個變數存取
				while($row520 = $rows520->fetch()) 
				{
					echo '<option id="job_id_select"  value="'.$row520[0].'" >'.$row520[0].'</option>';
				}
				echo '<option id="job_id_select"  value="" >Normal</option>';
		?>
           </select>
      </td>
	  <td width="300" align="center" style="padding-center:0px;"><p>
		   <input id="barcode_job" onkeypress="return disablekeys();" onclick="myFunction55()" type="button" class="btn" style="height:50px;width:130px;font-size:20px;" value="Barcode-Job"></p>
		   &nbsp;&nbsp;
      </td>
	   <?php
		   //echo '<td width="5" align="right" style="padding-center:0px;visibility:hidden">';
		   echo '<td width="5" align="right" style="padding-center:0px;">';
		   echo '<p id="status01" style="height:30px;width:20px;font-size:20px;"></p>';
		   echo '</td>';
		   ?>
      <td width="300" align="right" style="padding-right:20px;"><p>
           <!--<a href="tool_info.php"> -->
		   <a href="tcsmain_demo55.php">
		   <input type="button" id="tool_info" class="btn" style="height:50px;width:135px;font-size:30px;" value="Tool Info."></p>
           </a>
		   &nbsp;&nbsp;
      </td>
    </tr>
  </table>
	<input id="demo" type="hidden" />
</div>
<div id="content2" style="background-color:#EEEEEE;height:14%;width:800px;clear:left;">
<table width="760" height="100%" align="center" border="0">
<tr>
<td>
<fieldset><legend><b><font size="6">Sequence Status</font></b></legend>
<table width="700" height="45" align="center" border="0">
  <tr>
    <th>
	<a href="update_seq_plus.php">
	<img id="image00" src="images/icon_m00.png" style="width:24px;height:25px;"/>
	</a>
	</th>
	<th width="16%" style="font-size:30px;">Seq:</th><th width="16%" ><div id="seq_id01" size="10" style="width:50px;font-size:30px;"></div></th>
    <th width="17%" style="font-size:30px;">TP:</th><th width="16%" ><div id="tightening_program01" size="10" style="width:50px;font-size:30px;"></div></th>
    <th width="16%" style="font-size:30px;">TR:</th><th width="16%" ><div id="tightening_repeat01" size="1" style="width:50px;font-size:30px;"></div></th>
  </tr>
</table>
<table width="700" height="45" align="center" border="0">
  <tr>
    <td width="20%" align="center" >
	<img id="rowid33" style="FLOAT: center" src="/tcs10/images/step001.png">
	</td>
  </tr>  
</table>
</fieldset>
</td>
</tr>
</table>
</div>

<div id="content3" style="background-color:#EEEEEE;height:50%;width:800px;clear:left;">
<table width="760" height="100%" align="center" border="0">
<tr>
<td>
<fieldset><legend><b><font size="6">Fastening Result</font></b></legend>

<table width="100%" height="10%" align="center" border="0">
<tr>
<td align="center">
<div id="myDIV1" style="background-color:#d4d4d4;width:100%;height:100%;float:left;">
<table width="100%" height="80%" align="center" border="0" style="margin-top:-38px;margin-bottom:-38px;">
<tr>
  <th><table width="100%" height="80%" align="center" border="0">
    <tr></tr>
    <tr>
	  <th rowspan="3" width="23%"><p id="u16ActTorque" style="font-size:65px;" >50.0</p></th>
	  <input type="hidden" id="u16MaxTorque" style="font-size:22px;" readonly />
      
   
    </tr>
    <tr>
      <th align="left" width="15%"><p id="unit" style="font-size:30px;margin-bottom:-65px;margin-top:60px;" >kgf.cm</p>
      </th>
      <th align="left" width="30%"><p id="rowid" style="font-size:25px;margin-bottom:30px;margin-top:60px;" >Target Torque:</p>
      </th>
      <th align="left" width="20%"><p id="TargetTorque" style="font-size:25px;margin-bottom:30px;margin-top:60px;" ></p>
      </th>
	  <th align="left" width="25%"><p id="rowid" style="font-size:25px;margin-bottom:30px;margin-top:60px;" >Hi:</p>
      </th>
	  <th align="left" width="20%"><p id="TorqueUpperLimit" style="font-size:25px;margin-bottom:30px;margin-top:60px;" >1</p>
      </th>
  
    </tr>
    <tr>
      <th align="left" width="22%"><p id="rowid" style="font-size:18px;" ></p>
      </th>
      <th align="left" width="30%"><p id="rowid" style="font-size:25px;margin-bottom:70px;margin-top:30px;" >Window:</p>
      </th>
      <th align="left" width="20%"><p id="TargetWindow" style="font-size:25px;margin-bottom:70px;margin-top:30px;" ></p>
      </th>
	  <th align="left" width="25%"><p id="rowid" style="font-size:25px;margin-bottom:70px;margin-top:30px;" >Lo:</p>
      </th>
	  <th align="left" width="20%"><p id="TorqueLowerLimit" style="font-size:25px;margin-bottom:70px;margin-top:30px;" >0</p>
      </th>
    </tr>
    <input id="demo22" type="hidden" />
  </table></th>
</tr>
</table>
</div>
</td>
</tr>
</table>

<table width="100%" height="90%" align="center" border="0">
<tr>
<td align="center">
<div id="myDIV2" style="background-color:#d4d4d4;width:100%;height:100%;float:left;">
<table width="100%" height="80%" align="center" border="0"  style="margin-top:-38px;margin-bottom:-38px;">
<tr>
  <th><table width="100%" height="80%" align="center" border="0">
    <tr></tr>
    <tr>
	  <th rowspan="3" width="23%"><p id="u32Angle" style="font-size:65px;" >180</p></th>
	  <input type="hidden" id="iom_max_angle01" style="font-size:22px;" readonly />
	  
    </tr>
    <tr>
      <th align="left" width="10%"><p id="rowid" style="font-size:30px;margin-bottom:-40px;margin-top:80px;" >Degree</p>
      </th>
      <th align="left" width="30%"><p id="rowid" style="font-size:25px;margin-bottom:15px;margin-top:40px;" >Target Angle:</p>
      </th>
      <th align="left" width="20%"><p id="TargetAngle" style="font-size:25px;margin-bottom:15px;margin-top:40px;" >0</p>
      </th>
	  <th align="left" width="25%"><p id="rowid" style="font-size:25px;margin-bottom:15px;margin-top:40px;" >Hi:</p>
      </th>
      <th align="left" width="20%"><p id="AngleUpperLimit" style="font-size:25px;margin-bottom:15px;margin-top:40px;" >1</p>
      </th>
  
    </tr>
	
    <tr>
      <th align="left" width="22%"><p id="rowid" style="font-size:18px;" ></p>
      </th>
      <th align="left" width="30%"><p id="rowid" style="font-size:25px;margin-bottom:45px;margin-top:30px;" >Window:</p>
      </th>
      <th align="left" width="20%"><p id="AngleWindow" style="font-size:25px;margin-bottom:45px;margin-top:30px;" >0</p>
      </th>
	  <th align="left" width="25%"><p id="rowid" style="font-size:25px;margin-bottom:45px;margin-top:30px;" >Lo:</p>
      </th>
      <th align="left" width="20%"><p id="AngleLowerLimit" style="font-size:25px;margin-bottom:45px;margin-top:30px;" >0</p>
      </th>
    </tr>
    
  </table></th>
</tr>
</table>
</div>
</td>
</tr>
</table>

<table width="100%" height="130" align="center" border="0">
<tr>
<td align="center">
<div id="myDIV3" style="background-color:#d4d4d4;width:100%;height:100%;float:left;">
<table width="100%" height="110" align="center" border="0"  style="margin-top:-38px;margin-bottom:-38px;">
<tr>
  <th><table width="100%" height="110" align="center" border="0">
    <tr></tr>
    <tr>
	  <th rowspan="3" width="22%"><p id="ToolFastenStatus" style="font-size:65px;" >Idle</p></th>
      <th rowspan="2" align="left" width="44%"><p id="result_status" style="font-size:35px;word-break: break-all;" >NO LOCK</p>
      </th>
      <th align="left" width="0.1%"><p id="rowid" style="font-size:25px;" ></p>
      </th> 
	  <th align="left" width="20%">
	  <p id="status_img01" style="font-size:25px;margin-bottom:30px;margin-top:30px;" >
	  <img style="FLOAT: center;height:160px;width:160px;background-repeat: no-repeat;" src="/tcs10/images/00.png">
	  </p>
      </th>
    </tr>
    
  </table></th>
</tr>
</table>
</div>
</td>
</tr>
</table>
</fieldset>
</td>
</tr>
</table>
</div>

<div id="content6" style="background-color:#EEEEEE;height:17%;width:800px;clear:left;">
<table width="93.5%" height="110" align="center" border="0">
<tr>
<td>
<fieldset><legend><b><font size="5"></font></b></legend>
	<table width="100%" height="110" align="center" border="0">
    <tr>
      <th><p align="center">
	  <a href="inputpassword.php">
	  <input type="button" class="btn" id="inputpassword" style="height:50px;width:130px;font-size:30px;" value="Setup"></p>
	  </a>
	  </th>
      <th><p align="center">
	  <a href="datalog.php">
	  <input type="button" class="btn" id="datalog" style="height:50px;width:130px;font-size:30px;" value="Data"></p>
	  </a>
	  </th>
      <th><p align="center">
	  <a href="graph.php">
	  <input type="button" class="btn" id="graph" style="height:50px;width:130px;font-size:30px;" value="Graph"></p>
	  </a>
	  </th>
      <th><p align="center">
	  <a href="barcode.php">
	  <input type="button" class="btn" id="barcode" style="height:50px;width:130px;font-size:30px;" value="Barcode"></p>
      </a>
	  </th>
    </tr>
    <tr>
      <th><p align="center">
      <input onclick="click_enable()" id="curtainenable" type="button" class="btn" style="height:50px;width:130px;font-size:30px;" value="Enable"></p>
	  </th>
      <th><p align="center">
	  <input onclick="click_disable()" id="curtaindisable" type="button" class="btn" style="height:50px;width:130px;font-size:30px;" value="Disable"></p>
      </th>
      <th><p align="center">
	  <input onclick="click_confirm()" id="curtainConfirm" type="button" class="btn" style="height:50px;width:130px;font-size:30px;" value="Confirm"></p>
      </th>
      <th><p align="center">
	  <input onclick="click_repeat()" id="curtainRepeat" type="button" class="btn" style="height:50px;width:130px;font-size:30px;" value="Repeat"></p>
	  </th>
    </tr>		
  </table>
</fieldset>
</td>
</tr>
</table>
</div>

<div id="content7" style="background-color:#EEEEEE;height:14%;width:800px;clear:left;">
<table width="93.5%" height="50" align="center" border="0">
<tr>
<td>
<fieldset><legend><b><font size="7"></font></b></legend>
	<table width="100%" height="50" align="center" border="0">
<tr>
      <th><p style="margin-left: 21px;">
	  <a href="setup_input_step.php"><input onclick="tool_disabled()" id="setup_input_step" type="button" class="btn" style="height:50px;width:130px;font-size:30px;" value="Input"></p></a>
      </th>
      <th ><p align="center">      
        <table onclick="Select_job_step_start()" width="100" style="border:0px solid;padding:5px;margin-left: 20px;" rules="all" cellpadding='5';>
          <tr>
		  <input type="hidden" id="rowid222" value="1">
		  <td id="td1">1</td>
          <td id="td2">2</td>
          <td id="td3">3</td>
          <td id="td4">4</td>
          <td id="td5">5</td>
          <td id="td6">6</td>
          <td id="td7">7</td>
          <td id="td8">8</td>
          </tr>
          </table></p>
      </th>
	  
	  <th><p style="margin-left: 17px;">
	  <a href="setup_output_step.php"><input onclick="tool_disabled()" id="setup_output_step" type="button" class="btn" style="height:50px;width:130px;font-size:30px;" value="Output"></p></a>
      </th>
      <th><p align="center">
          <table width="100" style="border:0px solid;padding:5px;margin-left: 20px;" rules="all" cellpadding='5';>
		  <tr>
          <td id="td01">1</td>
          <td id="td02">2</td>
          <td id="td03">3</td>
          <td id="td04">4</td>
          <td id="td05">5</td>
          <td id="td06">6</td>
          <td id="td07">7</td>
          <td id="td08">8</td>
          </tr>
          </table></p>
      </th>
    </tr>
  </table>
</fieldset>
</td>
</tr>
</table>
<!--
<th rowspan="3" width="23%"><p id="status_change" style="font-size:25px;" >50.0</p></th>
<br><br> -->
<input type="hidden" id="PageNo" value="1">
<br><br>
<input type="hidden" id="PageCmd" value="<?php echo $PageCmd01;?>">
<br><br>	
<input type="hidden" id="Seqstart" value="0">
<br><br>  
<input type="hidden" id="Seqstart300" value="0">
<br><br>  
<input type="hidden" id="Seqstart301" value="0">
<br><br>  
<input type="hidden" id="clock" size="50" />
<br><br>
<input name="ip" id="ip" type="hidden" value="127.0.0.1">
<input name="ip_out" id="ip_out" type="hidden" value="<?php echo $ip; ?>">
<input name="port" id="port" type="hidden" value="9000">
<br><br>
</div>
<h3 id="rowid2" style="font-size:28px;" ></h3>
</body>
</html>
<script src="jquery/moment.min.js.js"></script>
<script src="jquery/moment-timezone-with-data-10-year-range.js.js"></script>
<script type="text/javascript">
function myFunction55() {
  var job_id_startget_enter = prompt("Please enter Barcode to change Job:", "<?php echo $barcode_job_name;?>");
  var result = confirm('Are you Sure?');
  if (job_id_startget_enter == null || job_id_startget_enter == "") {
	return;
  } 
    var Command_sn301 = parseInt(document.getElementById('Seqstart301').value, 10);
      Command_sn301 = isNaN(Command_sn301) ? 0 : Command_sn301;				     // 判斷是否為數字
      Command_sn301++;														 // 累加數字
      document.getElementById('Seqstart301').value = padLeft(Command_sn301,3); //=> '001'
	  var Command_sns301 = padLeft(Command_sn301,3);  // 數字以三位數顯示
	  
	  if(Command_sn301 > 255){
		var Command_sn103 = padLeft(0,3);
	    Command_sn103 = isNaN(Command_sn103) ? 0 : Command_sn103; // 判斷是否為數字
        Command_sn103++;										   // 累加數字
        document.getElementById('Seqstart301').value = padLeft(Command_sn103,3); //=> '001'
	    var Command_sns301 = padLeft(Command_sn103,3);             // 數字以三位數顯示
	  }
  
    var dateString = moment.tz("Asia/Taipei").format("YYYYMMDD HH:mm:ss") + ",";
	var barcode_data = "," + String(Command_sns301) + "," + String(job_id_startget_enter);  	
	var barcode01 = 0;
	  
	  for(var i = 0; i < barcode_data.length; i++)
	  {
		  barcode01 += barcode_data.substr(i).charCodeAt(0);
	  }
	  //console.log(barcode01);
	  //將字串轉成10進制相加
	var barcode01_hex = barcode01.toString(16); //讀取barcode01_hex將10進制轉成16進制顯示
	var barcode = padLeft(barcode01_hex.toUpperCase(),4);   //讀取barcode顯示四位數並呈現大寫英文字母

	var barcodejob = "{CMD301," + dateString + String(barcode) + barcode_data + "}";
	
	//var myJSON_01 = JSON.stringify(obj_01);
    console.log(barcodejob);
	
	var barcodejob2 = {
				   //status: "1",
                   name: "CMD301",
                   message: barcodejob
                  };
	
	$.post('barcodejob.php', 
	{
		click_barcodejob: barcodejob
	}
	,function(data){
      //alert("Data: " + statusnow);
    });
	/*
	try 
	{ 
        //socket.send(selectjob); 
		socket.send(JSON.stringify(barcodejob2)); 
	} 
	catch(ex) 
	{ 
		log(ex); 
	}
	*/
	
	//$("message").focus();
}

function Select_job()
{
	//var kk = $("#Job_ID").val();		/*讀取Job值*/
	var kk = document.getElementById("Job_ID").value; /*讀取Job值*/
	if(kk == 'Clear'){
		var kk = "100";
	}
	if(kk == 'Disable'){
		var kk = "101";
	}
	if(kk == 'NS'){
		var kk = "102";
	}
	if(kk == 'AS'){
		var kk = "103";
	}
	if(kk == 'Normal'){
		var kk = "0";
	}
	var kk01 = padLeft(kk,3);   //讀取eighth_step顯示三位數
	
	var Command_sn301 = parseInt(document.getElementById('Seqstart301').value, 10);
      Command_sn301 = isNaN(Command_sn301) ? 0 : Command_sn301;				     // 判斷是否為數字
      Command_sn301++;														 // 累加數字
      document.getElementById('Seqstart301').value = padLeft(Command_sn301,3); //=> '001'
	  var Command_sns301 = padLeft(Command_sn301,3);  // 數字以三位數顯示
	  
	  if(Command_sn301 > 255){
		var Command_sn103 = padLeft(0,3);
	    Command_sn103 = isNaN(Command_sn103) ? 0 : Command_sn103; // 判斷是否為數字
        Command_sn103++;										   // 累加數字
        document.getElementById('Seqstart301').value = padLeft(Command_sn103,3); //=> '001'
	    var Command_sns301 = padLeft(Command_sn103,3);             // 數字以三位數顯示
	  }
	
	var dateString = moment.tz("Asia/Taipei").format("YYYYMMDD HH:mm:ss") + ",";
	var job_data = "," + String(Command_sns301) + "," + String(kk01); 
	var job01 = job_data.substr(0).charCodeAt(0) + job_data.substr(1).charCodeAt(0) + job_data.substr(2).charCodeAt(0)
	+ job_data.substr(3).charCodeAt(0) + job_data.substr(4).charCodeAt(0) + job_data.substr(5).charCodeAt(0)
	+ job_data.substr(6).charCodeAt(0) + job_data.substr(7).charCodeAt(0); //將字串轉成10進制相加
	var job01_hex = job01.toString(16); //讀取job01_hex將10進制轉成16進制顯示
	var job = padLeft(job01_hex.toUpperCase(),4);   //讀取job顯示四位數並呈現大寫英文字母

	var selectjob = "{CMD301," + dateString + String(job) + job_data + "}";
	
    //str = obj_1.replace(/\\/g,"");
	//var myJSON_01 = JSON.stringify(obj_01);
    console.log(selectjob);
			
	var selectjob2 = {
				   //status: "1",
                   name: "CMD301",
                   message: selectjob
                  };
	
	$.post('selectjob.php', 
	{
		click_selectjob: selectjob
	}
	,function(data){
      //alert("Data: " + statusnow);
    });
	
	/*
	try 
	{ 
        //socket.send(selectjob); 
		socket.send(JSON.stringify(selectjob2)); 
	} 
	catch(ex) 
	{ 
		log(ex); 
	}
	*/
	
	//$("message").focus();
}

function click_enable()
  {
	var kk2 = document.getElementById("curtainenable").value;   //讀取起子啟動/關閉
	if(kk2 == 'Enable'){
				var kk2 = "0";
			}
	var Command_sn = parseInt(document.getElementById('Seqstart').value, 10);
      Command_sn = isNaN(Command_sn) ? 0 : Command_sn;				     // 判斷是否為數字
      Command_sn++;														 // 累加數字
      document.getElementById('Seqstart').value = padLeft(Command_sn,3); //=> '001'
	  var Command_sns = padLeft(Command_sn,3);  // 數字以三位數顯示
	  
	  if(Command_sn > 255){
		var Command_sn00 = padLeft(0,3);
	    Command_sn00 = isNaN(Command_sn00) ? 0 : Command_sn00; // 判斷是否為數字
        Command_sn00++;										   // 累加數字
        document.getElementById('Seqstart').value = padLeft(Command_sn00,3); //=> '001'
	    var Command_sns = padLeft(Command_sn00,3);             // 數字以三位數顯示
	  }
			
	var dateString = moment.tz("Asia/Taipei").format("YYYYMMDD HH:mm:ss") + ",";	 		
	var tool_status = "," + String(Command_sns) + "," + String(kk2);  	
	var status01 = tool_status.substr(0).charCodeAt(0) + tool_status.substr(1).charCodeAt(0) + tool_status.substr(2).charCodeAt(0)
	+ tool_status.substr(3).charCodeAt(0) + tool_status.substr(4).charCodeAt(0) + tool_status.substr(5).charCodeAt(0); //將字串轉成10進制相加
	var status01_hex = status01.toString(16); //讀取status_repeat01_hex將10進制轉成16進制顯示
	var status = padLeft(status01_hex.toUpperCase(),4); //讀取status_repeat顯示四位數並呈現大寫英文字母

	var statusenable = "{CMD302," + dateString + String(status) + tool_status + "}";
			
	//var myJSON_02 = JSON.stringify(obj_02);
    console.log(statusenable);
	
	var statusenable2 = {
				   //status: "1",
                   name: "CMD302",
                   message: statusenable
                  };
				  			  			  
	$.post('click.php', 
	{
		click_enable: statusenable
	}
	,function(data){
      //alert("Data: " + statusnow);
    });
	
	/*
	try 
	{ 
        //socket.send(statusnow); 
		socket.send(JSON.stringify(statusnow2)); 
	} 
	catch(ex2) 
	{ 
		log(ex2); 
	}
	*/
	
	//$("message").focus();
 }
 
function click_disable()
  {
	var kk2 = document.getElementById("curtaindisable").value;   //讀取起子啟動/關閉
	if(kk2 == 'Disable'){
				var kk2 = "1";
			}
	var Command_sn = parseInt(document.getElementById('Seqstart').value, 10);
      Command_sn = isNaN(Command_sn) ? 0 : Command_sn;				     // 判斷是否為數字
      Command_sn++;														 // 累加數字
      document.getElementById('Seqstart').value = padLeft(Command_sn,3); //=> '001'
	  var Command_sns = padLeft(Command_sn,3);  // 數字以三位數顯示
	  
	  if(Command_sn > 255){
		var Command_sn00 = padLeft(0,3);
	    Command_sn00 = isNaN(Command_sn00) ? 0 : Command_sn00; // 判斷是否為數字
        Command_sn00++;										   // 累加數字
        document.getElementById('Seqstart').value = padLeft(Command_sn00,3); //=> '001'
	    var Command_sns = padLeft(Command_sn00,3);             // 數字以三位數顯示
	  }
			
	var dateString = moment.tz("Asia/Taipei").format("YYYYMMDD HH:mm:ss") + ",";	 		
	var tool_status = "," + String(Command_sns) + "," + String(kk2);  	
	var status01 = tool_status.substr(0).charCodeAt(0) + tool_status.substr(1).charCodeAt(0) + tool_status.substr(2).charCodeAt(0)
	+ tool_status.substr(3).charCodeAt(0) + tool_status.substr(4).charCodeAt(0) + tool_status.substr(5).charCodeAt(0); //將字串轉成10進制相加
	var status01_hex = status01.toString(16); //讀取status_repeat01_hex將10進制轉成16進制顯示
	var status = padLeft(status01_hex.toUpperCase(),4); //讀取status_repeat顯示四位數並呈現大寫英文字母

	var statusdisable = "{CMD302," + dateString + String(status) + tool_status + "}";
			
	//var myJSON_02 = JSON.stringify(obj_02);
    console.log(statusdisable);
	
	var statusdisable2 = {
				   //status: "1",
                   name: "CMD302",
                   message: statusdisable
                  };
				  			  			  
	$.post('click.php', 
	{
		update_time: dateString,
		click_disable: statusdisable
	}
	,function(data){
      //alert("Data: " + statusnow);
    });
	
	/*
	try 
	{ 
        //socket.send(statusnow); 
		socket.send(JSON.stringify(statusnow2)); 
	} 
	catch(ex2) 
	{ 
		log(ex2); 
	}
	*/
	
	//$("message").focus();
 }
 
function click_repeat()
  {
      //var kk2 = $("#curtainInput").val();		//讀取起子啟動/關閉
	  var kk_repeat = document.getElementById("curtainRepeat").value;   //讀取起子啟動/關閉
	  if(kk_repeat == 'Repeat'){
				var kk_repeat = "3";
			}
	  var Command_sn = parseInt(document.getElementById('Seqstart').value, 10);
      Command_sn = isNaN(Command_sn) ? 0 : Command_sn;				     // 判斷是否為數字
      Command_sn++;														 // 累加數字
      document.getElementById('Seqstart').value = padLeft(Command_sn,3); //=> '001'
	  var Command_sns = padLeft(Command_sn,3);  // 數字以三位數顯示
	  
	  if(Command_sn > 255){
		var Command_sn00 = padLeft(0,3);
	    Command_sn00 = isNaN(Command_sn00) ? 0 : Command_sn00; // 判斷是否為數字
        Command_sn00++;										   // 累加數字
        document.getElementById('Seqstart').value = padLeft(Command_sn00,3); //=> '001'
	    var Command_sns = padLeft(Command_sn00,3);             // 數字以三位數顯示
	  }

	  var dateString = moment.tz("Asia/Taipei").format("YYYYMMDD HH:mm:ss") + ",";	  
	  var tool_repeat = "," + String(Command_sns) + "," + String(kk_repeat);    
	  var status_repeat01 = tool_repeat.substr(0).charCodeAt(0) + tool_repeat.substr(1).charCodeAt(0) + tool_repeat.substr(2).charCodeAt(0)
	  + tool_repeat.substr(3).charCodeAt(0) + tool_repeat.substr(4).charCodeAt(0) + tool_repeat.substr(5).charCodeAt(0); //將字串轉成10進制相加
	  var status_repeat01_hex = status_repeat01.toString(16); //讀取status_repeat01_hex將10進制轉成16進制顯示
	  var status_repeat = padLeft(status_repeat01_hex.toUpperCase(),4); //讀取status_repeat顯示四位數並呈現大寫英文字母

	  var statusnow_repeat = "{CMD302," + dateString + String(status_repeat) + tool_repeat + "}";
			
    //str = obj_1.replace(/\\/g,"");
	//var myJSON_repeat = JSON.stringify(obj_repeat);
    console.log(statusnow_repeat);	
	
	var statusnow_repeat2 = {
				   //status: "1",
                   name: "CMD302",
                   message: statusnow_repeat
                  };
				  
	$.post('click.php', 
	{
		click_repeat: statusnow_repeat
	}
	,function(data){
      //alert("Data: " + statusnow);
    });
	
	/*
	try 
	{ 
        //socket.send(statusnow_repeat); 
		socket.send(JSON.stringify(statusnow_repeat2)); 
	} 
	catch(ex_repeat) 
	{ 
		log(ex_repeat); 
	}
	*/
	
	//$("message").focus();
 }
 
function click_confirm()
  {
	//var kk2 = $("#curtainInput").val();		//讀取起子啟動/關閉
	var kk_confirm = document.getElementById("curtainConfirm").value;   //讀取起子啟動/關閉
			if(kk_confirm == 'Confirm'){
				var kk_confirm = "2";
			}
	var Command_sn = parseInt(document.getElementById('Seqstart').value, 10);
      Command_sn = isNaN(Command_sn) ? 0 : Command_sn;				     // 判斷是否為數字
      Command_sn++;														 // 累加數字
      document.getElementById('Seqstart').value = padLeft(Command_sn,3); //=> '001'
	  var Command_sns = padLeft(Command_sn,3);  // 數字以三位數顯示
	  
	  if(Command_sn > 255){
		var Command_sn00 = padLeft(0,3);
	    Command_sn00 = isNaN(Command_sn00) ? 0 : Command_sn00; // 判斷是否為數字
        Command_sn00++;										   // 累加數字
        document.getElementById('Seqstart').value = padLeft(Command_sn00,3); //=> '001'
	    var Command_sns = padLeft(Command_sn00,3);             // 數字以三位數顯示
	  }
	
	var dateString = moment.tz("Asia/Taipei").format("YYYYMMDD HH:mm:ss") + ",";		
	var tool_confirm = "," + String(Command_sns) + "," + String(kk_confirm);  
	var status_confirm01 = tool_confirm.substr(0).charCodeAt(0) + tool_confirm.substr(1).charCodeAt(0) + tool_confirm.substr(2).charCodeAt(0)
	+ tool_confirm.substr(3).charCodeAt(0) + tool_confirm.substr(4).charCodeAt(0) + tool_confirm.substr(5).charCodeAt(0); //將字串轉成10進制相加
	var status_confirm01_hex = status_confirm01.toString(16); //讀取status_confirm01_hex將10進制轉成16進制顯示
	var status_confirm = padLeft(status_confirm01_hex.toUpperCase(),4); //讀取status_confirm顯示四位數並呈現大寫英文字母

	var statusnow_confirm = "{CMD302," + dateString + String(status_confirm) + tool_confirm + "}";
    //str = obj_1.replace(/\\/g,"");
	//var myJSON_confirm = JSON.stringify(obj_confirm);
    console.log(statusnow_confirm);
	
	var statusnow_confirm2 = {
				   //status: "1",
                   name: "CMD302",
                   message: statusnow_confirm
                  };
				  
	$.post('click.php', 
	{
		click_confirm: statusnow_confirm
	}
	,function(data){
      //alert("Data: " + statusnow);
    });
	
	/*
	try 
	{ 
        //socket.send(statusnow_confirm); 
		socket.send(JSON.stringify(statusnow_confirm2)); 
	} 
	catch(ex_confirm) 
	{ 
		log(ex_confirm); 
	}
	*/
	
	//$("message").focus();
 }
 
function Update_plusseq()
{
	//var seq = document.getElementById("Seqstart").value; /*讀取Job值*/
	/*
	if( typeof this.counter == 'undefined' ) {
					        this.counter = 1;
				        }
				
                var seq = document.getElementById("Seqstart");
                seq.value = this.counter;
                this.counter++;
	*/
							
	var seq = parseInt(document.getElementById('Seqstart').value, 10);
    seq = isNaN(seq) ? 0 : seq;
    seq++;
    document.getElementById('Seqstart').value = seq;	
	
	var dateString = moment.tz("Asia/Taipei").format("YYYYMMDD HH:mm:ss") + ",";	
	var seq_plus_start = "," + String(seq);  
	var seq_plus = seq_plus_start.length;

	var seq_plus_result = "{cmd3," + dateString + String(seq_plus) + seq_plus_start + "}";
	
    //str = obj_1.replace(/\\/g,"");
	//var myJSON_seq = JSON.stringify(seq_01);
    console.log(seq_plus_result);
			
	try 
	{ 
        socket.send(seq_plus_result); 
	} 
	catch(exseq) 
	{ 
		log(exseq); 
	}	
}
 
// run change function every 3 seconds
//setInterval(start, 3000);

var lockReconnect = false;//避免重複連接
var tt;
var socket;		
var connect_flg = 0;
var i = 0;		// scroll index of dialog log area

function padLeft(nr, n, str){   //將數字呈現多位元的函數
    return Array(n-String(nr).length+1).join(str||'0')+nr;
}
//or as a Number prototype method:
Number.prototype.padLeft = function (n,str){    //將數字呈現多位元的函數
    return Array(n-String(this).length+1).join(str||'0')+this;
}
//examples

var int = null;
 var int=self.setInterval("clock()",100)
        function clock()
        {
		    var t=new Date()
			document.getElementById("clock").value=t
            var _PageNo = '';
            var _PageCmd = '';
			
			var datetime = moment.tz("Asia/Taipei").format("YYYYMMDD HH:mm:ss.SSS");
			console.log(datetime);
			
			var Command_sn = parseInt(document.getElementById('Seqstart').value, 10);
			  Command_sn = isNaN(Command_sn) ? 0 : Command_sn;				     // 判斷是否為數字
			  Command_sn++;														 // 累加數字
			  document.getElementById('Seqstart').value = padLeft(Command_sn,3); //=> '001'
			  var Command_sns = padLeft(Command_sn,3);  // 數字以三位數顯示
			  
			  if(Command_sn > 255){
				var Command_sn00 = padLeft(0,3);
				Command_sn00 = isNaN(Command_sn00) ? 0 : Command_sn00; // 判斷是否為數字
				Command_sn00++;										   // 累加數字
				document.getElementById('Seqstart').value = padLeft(Command_sn00,3); //=> '001'
				var Command_sns = padLeft(Command_sn00,3);             // 數字以三位數顯示
			  }
			  
			  var save_data = "," + String(Command_sns);
			  
			  //var chk01 = save_data.length;
			  //var chk = padLeft(chk01,4);   //讀取chk顯示四位數
			  var save_data01 = 0;
			  
			  for(var i = 0; i < save_data.length; i++)
			  {
				  save_data01 += save_data.substr(i).charCodeAt(0);
			  }
			  console.log(save_data01);
			  //將字串轉成10進制相加
				  
			  var save_data01_hex = save_data01.toString(16); //讀取save_data01_hex將10進制轉成16進制顯示 
			  var chk = padLeft(save_data01_hex.toUpperCase(),4); //讀取chk顯示四位數並呈現大寫英文字母
			
			// 發送 Ajax 查詢請求並處理
            var request_2 = new XMLHttpRequest();
            request_2.open("POST", "socketclient.php");

            // POST 請求必須設置表頭在 open() 下面，send() 上面
            request_2.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
			// 如果想要以 POST 方式傳送資料，則必須先將 MIME 型態改好
			
			//request_2.setRequestHeader("Content-Type", "text/html; charset=utf-8");
			/*
			request_2.onreadystatechange =  function()
			{
               if (request_2.readyState == 4)
               {
                       setTimeout(clock, 500);
               request_2 = null;
			delete request_2;};       
			}
			*/
			
			/*
		    var obj_2 = {    
                "REQ300": "REQ300",
			    "yyyyMMdd HH:mm:ss": datetime,
				"Checksum": chk,
				"Command_sn": Command_sns,
			}
            //str = obj_1.replace(/\\/g,"");
		
			var myJSON_2 = JSON.stringify(obj_2);
            console.log(myJSON_2);
	
            //str = obj_1.replace(/\\/g,"");
            var obj_3 = {
                "CmdSide":"UI",
                "Packet" :myJSON_2			
            }
			*/

            //var myJSON_2 = JSON.stringify(obj_2);
			// console.log(myJSON_3);
			//request_2.send(myJSON_2);
			
            // POST 參數須使用 send() 發送	
			var obj_2 = "{REQ300," + datetime + "," + String(chk) + "," + Command_sns + "}";	
			console.log(obj_2);
			
			request_2.send(obj_2);
			
            request_2.onreadystatechange = function() {
                // 伺服器請求完成
                if (request_2.readyState === 4) {
                    // 伺服器回應成功
                    if (request_2.status === 200) {
                        var type = request_2.getResponseHeader("Content-Type");   // 取得回應類型
                        //alert(type);
						//console.log(type);
						
                        // 判斷回應類型，這裡使用 TEXT
                        if (type.indexOf("text/plain") === 0) {               
                            //console.log(request_2.responseText);  
							console.log(request_2.responseText.split("POST")[1]); 
							var data = request_2.responseText.split("POST")[1];	
							var umsg_splits = data.split(",", 35);
							//var data = JSON.parse(data_split);	
							//console.log(data);							
                            //var data = request_2.responseText;
							//console.log(data);
							//alert(data);
                            //if (data.PageNum) {
                            //    document.getElementById("EchoPageCmd").innerHTML = '員工：' + data.name + '，儲存成功！';
                            //}
	
							//document.getElementById("u32Angle").innerHTML = data.u32Angle;
							//var word=75;
							document.getElementById("unit").innerHTML = "<?php echo $unit;?>";
							document.getElementById("job_id_select").innerHTML = umsg_splits[7];
							document.getElementById("seq_id01").innerHTML = umsg_splits[8];
							document.getElementById("tightening_program01").innerHTML = umsg_splits[9];
							document.getElementById("tightening_repeat01").innerHTML = umsg_splits[10];
							document.getElementById("u16ActTorque").innerHTML = umsg_splits[17];
							document.getElementById("u32Angle").innerHTML = umsg_splits[18];
							document.getElementById("ToolFastenStatus").innerHTML = umsg_splits[21];
							
							if(umsg_splits[21] == 'NG'){
										document.getElementById("myDIV1").style.backgroundColor = "red";
										document.getElementById("myDIV2").style.backgroundColor = "red";
										document.getElementById("myDIV3").style.backgroundColor = "red";	
										//document.getElementById("status_img01").innerHTML = "<img style='FLOAT: center;height:160px;width:160px; background-repeat: no-repeat;' src='/tcs10/images/00.png'>";
										//document.getElementById("job_id_select").innerHTML = umsg_splits[7];
										document.getElementById("seq_id01").innerHTML = umsg_splits[8];
										document.getElementById("tightening_program01").innerHTML = umsg_splits[9];
										document.getElementById("tightening_repeat01").innerHTML = umsg_splits[10];
										document.getElementById("u16ActTorque").innerHTML = umsg_splits[17];
										//document.getElementById("u16ActTorque").style.color = "red";
										document.getElementById("u32Angle").innerHTML = umsg_splits[18];
										//document.getElementById("u32Angle").style.color = "red";
										document.getElementById("ToolFastenStatus").innerHTML = umsg_splits[21];
										document.getElementById("result_status").innerHTML = umsg_splits[24];
										//document.getElementById("ToolFastenStatus").style.color = "red";
										//document.getElementById("image00").style.opacity = "1";
										//document.getElementById("image00").disabled = false;
										/*
										//document.getElementById("Job_ID").style.opacity = "1";
										document.getElementById("barcode_job").style.opacity = "1";
										document.getElementById("tool_info").style.opacity = "1";
										document.getElementById("inputpassword").style.opacity = "1";
										document.getElementById("datalog").style.opacity = "1";
										document.getElementById("graph").style.opacity = "1";
										document.getElementById("barcode").style.opacity = "1";
										document.getElementById("curtainenable").style.opacity = "1";
										document.getElementById("curtaindisable").style.opacity = "1";
										document.getElementById("curtainConfirm").style.opacity = "1";
										document.getElementById("curtainRepeat").style.opacity = "1";
										document.getElementById("setup_input_step").style.opacity = "1";
										document.getElementById("setup_output_step").style.opacity = "1";
										//document.getElementById("Job_ID").disabled = false;
										document.getElementById("barcode_job").disabled = false;
										document.getElementById("tool_info").disabled = false;
										document.getElementById("inputpassword").disabled = false;
										document.getElementById("datalog").disabled = false;
										document.getElementById("graph").disabled = false;
										document.getElementById("barcode").disabled = false;
										document.getElementById("curtainenable").disabled = false;
										document.getElementById("curtaindisable").disabled = false;
										document.getElementById("curtainConfirm").disabled = false;
										document.getElementById("curtainRepeat").disabled = false;
										document.getElementById("setup_input_step").disabled = false;
										document.getElementById("setup_output_step").disabled = false;
										*/
										}
										
										if(umsg_splits[21] == 'OK'){
										document.getElementById("myDIV1").style.backgroundColor = "#00CC33";
										document.getElementById("myDIV2").style.backgroundColor = "#00CC33";
										document.getElementById("myDIV3").style.backgroundColor = "#00CC33";
										//document.getElementById("status_img01").innerHTML = "<img style='FLOAT: center;height:160px;width:160px; background-repeat: no-repeat;' src='/tcs10/images/00.png'>";
										//document.getElementById("job_id_select").innerHTML = umsg_splits[7];
										document.getElementById("seq_id01").innerHTML = umsg_splits[8];
										document.getElementById("tightening_program01").innerHTML = umsg_splits[9];
										document.getElementById("tightening_repeat01").innerHTML = umsg_splits[10];
										document.getElementById("u16ActTorque").innerHTML = umsg_splits[17];
										//document.getElementById("u16ActTorque").style.color = "green";
										document.getElementById("u32Angle").innerHTML = umsg_splits[18];
										//document.getElementById("u32Angle").style.color = "green";
										document.getElementById("ToolFastenStatus").innerHTML = umsg_splits[21];
										document.getElementById("result_status").innerHTML = umsg_splits[24];
										//document.getElementById("image00").style.opacity = "1";
										//document.getElementById("image00").disabled = false;
										//document.getElementById("ToolFastenStatus").style.color = "green";
										/*
										//document.getElementById("Job_ID").style.opacity = "1";
										document.getElementById("barcode_job").style.opacity = "1";
										document.getElementById("tool_info").style.opacity = "1";
										document.getElementById("inputpassword").style.opacity = "1";
										document.getElementById("datalog").style.opacity = "1";
										document.getElementById("graph").style.opacity = "1";
										document.getElementById("barcode").style.opacity = "1";
										document.getElementById("curtainenable").style.opacity = "1";
										document.getElementById("curtaindisable").style.opacity = "1";
										document.getElementById("curtainConfirm").style.opacity = "1";
										document.getElementById("curtainRepeat").style.opacity = "1";
										document.getElementById("setup_input_step").style.opacity = "1";
										document.getElementById("setup_output_step").style.opacity = "1";
										//document.getElementById("Job_ID").disabled = false;
										document.getElementById("barcode_job").disabled = false;
										document.getElementById("tool_info").disabled = false;
										document.getElementById("inputpassword").disabled = false;
										document.getElementById("datalog").disabled = false;
										document.getElementById("graph").disabled = false;
										document.getElementById("barcode").disabled = false;
										document.getElementById("curtainenable").disabled = false;
										document.getElementById("curtaindisable").disabled = false;
										document.getElementById("curtainConfirm").disabled = false;
										document.getElementById("curtainRepeat").disabled = false;
										document.getElementById("setup_input_step").disabled = false;
										document.getElementById("setup_output_step").disabled = false;
										*/
										}
																				
										if(umsg_splits[21] == 'Running'){
										//document.getElementById("ToolFastenStatus").style.color = "#000000";
										//document.getElementById("u16ActTorque").style.color = "#000000";
										//document.getElementById("u32Angle").style.color = "#000000";
										document.getElementById("myDIV1").style.backgroundColor = "#d4d4d4";
										document.getElementById("myDIV2").style.backgroundColor = "#d4d4d4";
										document.getElementById("myDIV3").style.backgroundColor = "#d4d4d4";
										document.getElementById("status_img01").innerHTML = "";
										document.getElementById("ToolFastenStatus").innerHTML = umsg_splits[21];
										//document.getElementById("image00").style.opacity = "0.5";
										//document.getElementById("image00").disabled = true;
										//document.getElementById("status_img01").innerHTML = "<img style='FLOAT: center;height:160px;width:160px;background-repeat: no-repeat;' src='/tcs10/images/tightening.gif'>";
									    //開始啟動狀態，顯示Tightening開始啟動圖形
										//document.getElementById("Job_ID").disabled = true;
										/*
										document.getElementById("barcode_job").disabled = true;
										document.getElementById("tool_info").disabled = true;
										document.getElementById("inputpassword").disabled = true;
										document.getElementById("datalog").disabled = true;
										document.getElementById("graph").disabled = true;
										document.getElementById("barcode").disabled = true;
										document.getElementById("curtainenable").disabled = true;
										document.getElementById("curtaindisable").disabled = true;
										document.getElementById("curtainConfirm").disabled = true;
										document.getElementById("curtainRepeat").disabled = true;
										document.getElementById("setup_input_step").disabled = true;
										document.getElementById("setup_output_step").disabled = true;
										//document.getElementById("Job_ID").style.opacity = "0.5";
										document.getElementById("barcode_job").style.opacity = "0.5";
										document.getElementById("tool_info").style.opacity = "0.5";
										document.getElementById("inputpassword").style.opacity = "0.5";
										document.getElementById("datalog").style.opacity = "0.5";
										document.getElementById("graph").style.opacity = "0.5";
										document.getElementById("barcode").style.opacity = "0.5";
										document.getElementById("curtainenable").style.opacity = "0.5";
										document.getElementById("curtaindisable").style.opacity = "0.5";
										document.getElementById("curtainConfirm").style.opacity = "0.5";
										document.getElementById("curtainRepeat").style.opacity = "0.5";
										document.getElementById("setup_input_step").style.opacity = "0.5";
										document.getElementById("setup_output_step").style.opacity = "0.5";
										*/
										}
										
										if(umsg_splits[21] == 'Reverse'){
										//document.getElementById("ToolFastenStatus").style.color = "#000000";
										//document.getElementById("u16ActTorque").style.color = "#000000";
										//document.getElementById("u32Angle").style.color = "#000000";
										document.getElementById("myDIV1").style.backgroundColor = "#d4d4d4";
										document.getElementById("myDIV2").style.backgroundColor = "#d4d4d4";
										document.getElementById("myDIV3").style.backgroundColor = "#d4d4d4";
										document.getElementById("status_img01").innerHTML = "";
										document.getElementById("ToolFastenStatus").innerHTML = umsg_splits[21];
										//document.getElementById("image00").style.opacity = "0.5";
										//document.getElementById("image00").disabled = true;
										//document.getElementById("status_img01").innerHTML = "<img style='FLOAT: center;height:160px;width:160px;background-repeat: no-repeat;' src='/tcs10/images/tightening.gif'>";
									    //開始啟動狀態，顯示Tightening開始啟動圖形
										//document.getElementById("Job_ID").disabled = true;
										/*
										document.getElementById("barcode_job").disabled = true;
										document.getElementById("tool_info").disabled = true;
										document.getElementById("inputpassword").disabled = true;
										document.getElementById("datalog").disabled = true;
										document.getElementById("graph").disabled = true;
										document.getElementById("barcode").disabled = true;
										document.getElementById("curtainenable").disabled = true;
										document.getElementById("curtaindisable").disabled = true;
										document.getElementById("curtainConfirm").disabled = true;
										document.getElementById("curtainRepeat").disabled = true;
										document.getElementById("setup_input_step").disabled = true;
										document.getElementById("setup_output_step").disabled = true;
										//document.getElementById("Job_ID").style.opacity = "0.5";
										document.getElementById("barcode_job").style.opacity = "0.5";
										document.getElementById("tool_info").style.opacity = "0.5";
										document.getElementById("inputpassword").style.opacity = "0.5";
										document.getElementById("datalog").style.opacity = "0.5";
										document.getElementById("graph").style.opacity = "0.5";
										document.getElementById("barcode").style.opacity = "0.5";
										document.getElementById("curtainenable").style.opacity = "0.5";
										document.getElementById("curtaindisable").style.opacity = "0.5";
										document.getElementById("curtainConfirm").style.opacity = "0.5";
										document.getElementById("curtainRepeat").style.opacity = "0.5";
										document.getElementById("setup_input_step").style.opacity = "0.5";
										document.getElementById("setup_output_step").style.opacity = "0.5";
										*/
										}
                        }
						else{
						    //console.log(request.responseText);
						}
                    } 
				    else {
                        alert("Error code = " + request_2.status);
                    }
                }
            }
        }


//function stopCount() {
//  clearTimeout(pollingTm2);
//}

//function startCount() {
//  send_sum();
//}

var isNS = (navigator.appName == "Netscape") ? 1 : 0;
if(navigator.appName == "Netscape") document.captureEvents(Event.MOUSEDOWN||Event.MOUSEUP);
function mischandler(){
    alert('This is Function Disabled');
return false;
}
function mousehandler(e){
var myevent = (isNS) ? e : event;
var eventbutton = (isNS) ? myevent.which : myevent.button;
if((eventbutton==2)||(eventbutton==3)) return false;
}
document.oncontextmenu = mischandler;
document.onmousedown = mousehandler;
document.onmouseup = mousehandler;
//select content code disable  alok goyal
function killCopy(e){
return false
}
function reEnable(){
return true
}
document.onselectstart=new Function ("return false")
//if (window.sidebar){
//document.onmousedown=killCopy
//document.onclick=reEnable
//}
function disablekeys()
{
	if (event.keyCode == 13)
	{
     return false;
    }
}

$('#curtainenable').click(function() {
    var disable = $('#curtaindisable');
	document.getElementById("curtaindisable").style.opacity = "0.5";
	var confirm = $('#curtainConfirm');
	document.getElementById("curtainConfirm").style.opacity = "0.5";
	var repeat = $('#curtainRepeat');
	document.getElementById("curtainRepeat").style.opacity = "0.5";

    // Disable the button.
    disable.prop('disabled', true);
	confirm.prop('disabled', true);
	repeat.prop('disabled', true);

    // Cause the button to re-enable after 1 seconds.
    setTimeout(function() {
        disable.prop('disabled', false);
		document.getElementById("curtaindisable").style.opacity = "1";
		confirm.prop('disabled', false);
		document.getElementById("curtainConfirm").style.opacity = "1";
		repeat.prop('disabled', false);
		document.getElementById("curtainRepeat").style.opacity = "1";
    }, 1000);
});

$('#curtaindisable').click(function() {
    var enable = $('#curtainenable');
	document.getElementById("curtainenable").style.opacity = "0.5";
	var confirm = $('#curtainConfirm');
	document.getElementById("curtainConfirm").style.opacity = "0.5";
	var repeat = $('#curtainRepeat');
	document.getElementById("curtainRepeat").style.opacity = "0.5";

    // Disable the button.
    enable.prop('disabled', true);
	confirm.prop('disabled', true);
	repeat.prop('disabled', true);

    // Cause the button to re-enable after 1 seconds.
    setTimeout(function() {
        enable.prop('disabled', false);
		document.getElementById("curtainenable").style.opacity = "1";
		confirm.prop('disabled', false);
		document.getElementById("curtainConfirm").style.opacity = "1";
		repeat.prop('disabled', false);
		document.getElementById("curtainRepeat").style.opacity = "1";
    }, 1000);
});

$('#curtainConfirm').click(function() {
    var disable = $('#curtaindisable');
	document.getElementById("curtaindisable").style.opacity = "0.5";
	var enable = $('#curtainenable');
	document.getElementById("curtainenable").style.opacity = "0.5";
	var repeat = $('#curtainRepeat');
	document.getElementById("curtainRepeat").style.opacity = "0.5";

    // Disable the button.
    disable.prop('disabled', true);
	enable.prop('disabled', true);
	repeat.prop('disabled', true);

    // Cause the button to re-enable after 1 seconds.
    setTimeout(function() {
        disable.prop('disabled', false);
		document.getElementById("curtaindisable").style.opacity = "1";
		enable.prop('disabled', false);
		document.getElementById("curtainenable").style.opacity = "1";
		repeat.prop('disabled', false);
		document.getElementById("curtainRepeat").style.opacity = "1";
    }, 1000);
});

$('#curtainRepeat').click(function() {
    var disable = $('#curtaindisable');
	document.getElementById("curtaindisable").style.opacity = "0.5";
	var enable = $('#curtainenable');
	document.getElementById("curtainenable").style.opacity = "0.5";
	var confirm = $('#curtainConfirm');
	document.getElementById("curtainConfirm").style.opacity = "0.5";

    // Disable the button.
    disable.prop('disabled', true);
	enable.prop('disabled', true);
	confirm.prop('disabled', true);

    // Cause the button to re-enable after 1 seconds.
    setTimeout(function() {
        disable.prop('disabled', false);
		document.getElementById("curtaindisable").style.opacity = "1";
		enable.prop('disabled', false);
		document.getElementById("curtainenable").style.opacity = "1";
		confirm.prop('disabled', false);
		document.getElementById("curtainConfirm").style.opacity = "1";
    }, 1000);
});
</script>