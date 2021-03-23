<?php 
	//$exec = exec("systemctl restart apache2.service"); //"apache2" restart linux command
	include("initial_sql.php");
?>
<?php
//@$job_id_startget = $_GET["job_id"]; 
// 2019.10.18 Eric：GET表單中的'step_id'資料
	if ($job_id_startget == FALSE) 
		// 2019.10.18 Eric：$step_id_start擷取資料後發現沒有直接擷取step_id='1'資訊，反之則擷取該變數資訊
		$job_id_startget00 = "1";
	else
		@$job_id_startget00 = $_GET["barcode"];
?>
<?php
//$db = new PDO('sqlite:/mnt/ramdisk/tcs_memory.db');
//$db->exec('set names utf-8');
//$str="update iom set status = '0' where rowid = '20004'"; 
//IOM Enable Tool Status update
//$stmt=$db->query($str);
$db02 = new PDO('sqlite:/mnt/ramdisk/tcs_memory.db');
$db02->exec('set names utf-8');
$str02="update iom set type='2', value = '0' where rowid = '100'"; 
//IOM Enable Tool Status update
$stmt02=$db02->query($str02);
$db02 = null;
?>
<?php
	$db23 = new PDO('sqlite:/var/www/html/database/tcs');
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
	$db23 = null;
?>
<?php 
	$db3 = new PDO('sqlite:/mnt/ramdisk/tcs_exchange.db');
	$db3->exec('set names utf-8');
	$db3_forward = new PDO('sqlite:/mnt/ramdisk/tcs_exchange2.db');
	$db3_forward->exec('set names utf-8');
	$db3_repeat = new PDO('sqlite:/var/www/html/database/tcs');
	$db3_repeat->exec('set names utf-8');
	$db99 = new PDO('sqlite:/var/www/html/database/tcs');
	$db99->exec('set names utf-8');
	$str303="select * from device limit 1";
	//2020.03.09 擷取網路資料庫device資料
	$stmt303=$db99->query($str303);
	while ($row303 = $stmt303->fetch())
	{
		$batch_mode = $row303[4]; //網路資料庫DB Batch Mode(判斷正數或倒數) 0為倒數;1為正數
	}
	
	//倒數部分
	if($batch_mode == "0")
	{	
	$sql6="select * from storedata where id = '1'"; 
	//IOM Tightening Seqence for counting
	$stmt6 = $db3->query($sql6); 
	// 2019.10.18 Eric：獲取承上$sql3資料庫語法，$stmt3為一個變數存取
	while($row6 = $stmt6->fetch()) 
		{
			$job_id3 = $row6[1];
			$seq_id3 = $row6[2];
			$tightening_program3 = $row6[3];
			$tightening_repeat3 = $row6[4];			
        }
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////

	//正數部分
	if($batch_mode == "1")
	{
	$sql6_forward="select * from storedata where id = '1'"; 
	//IOM Tightening Seqence for counting
	$stmt6_forward = $db3_forward->query($sql6_forward); 
	// 2019.10.18 Eric：獲取承上$sql3資料庫語法，$stmt3為一個變數存取
	while($row6_forward = $stmt6_forward->fetch()) 
		{
			//$job_id3 = $row6_forward[1];
			$seq_id3 = $row6_forward[2];
			$tightening_program3 = $row6_forward[3];
			$tightening_repeat3 = $row6_forward[4];			
        }
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////

	//Repeat停止
	if($batch_mode == "2")
	{
	$sql6_repeat="select * from storedata where id = '1'"; 
	//IOM Tightening Seqence for counting
	$stmt6_repeat = $db3_repeat->query($sql6_repeat); 
	// 2019.10.18 Eric：獲取承上$sql3資料庫語法，$stmt3為一個變數存取
	while($row6_repeat = $stmt6_repeat->fetch()) 
		{
			//$job_id3 = $row6_repeat[1];
			$seq_id3 = $row6_repeat[2];
			$tightening_program3 = $row6_repeat[3];
			$tightening_repeat3 = $row6_repeat[4];			
        }
	}
	$db3 = null;
	$db3_forward = null;
	$db3_repeat = null;
	$db99 = null;
			    ?>
<?php
	$db24 = new PDO('sqlite:/var/www/html/database/tcs');
	$db24->exec('set names utf-8'); 
	// 2019.10.18 Eric：設定文字UTF-8格式
	$db_select = new PDO('sqlite:/mnt/ramdisk/tcs_exchange.db');
	$db_select->exec('set names utf-8');
	// 2019.10.18 Eric：設定文字UTF-8格式
	$sql_select = "select * from storedata where id = '1'"; 
	//IOM Tightening Seqence for counting
	$stmt_select = $db_select->query($sql_select); 
	// 2019.10.18 Eric：獲取承上$sql3資料庫語法，$stmt3為一個變數存取
	while($row_select = $stmt_select->fetch()) 
		{
			$job_select11 = $row_select[1];		
        }
	$job_id_get = $job_select11;
	//$sql24 = "SELECT * FROM jobsequence INNER JOIN inputstep ON jobsequence.job_id = inputstep.inputstep0 where jobsequence = '$job_id_get' limit 1";
	//if($job_id_get == FALSE)
	//$sql24 = "SELECT * FROM jobsequence INNER JOIN inputstep ON jobsequence.job_id = inputstep.inputstep0 where job_id = '1' limit 1";
	//else
	$sql24 = "SELECT * FROM jobsequence INNER JOIN inputstep ON jobsequence.job_id = inputstep.inputstep0 where job_id = '$job_id_get' limit 1";
	$stmt24 = $db24->query($sql24); 
	// 2019.10.18 Eric：獲取承上$sql22955資料庫語法，$stmt955為一個變數
	while($row24 = $stmt24->fetch()) 
		{
			$job_name01 = $row24[1];
			$job_id01 = $row24[0];
			$btnImg01 = $row24[11];
			$btnImg02 = $row24[12];
			$btnImg03 = $row24[13];
			$btnImg04 = $row24[14];
			$btnImg05 = $row24[15];
			$btnImg06 = $row24[16];
			$btnImg07 = $row24[17];
			$btnImg08 = $row24[18];
        }
		$db24 = null;
?>
<?php
	//$db2 = new PDO('sqlite:/var/www/database/tcs'); // 2019.10.18 Eric：連結到資料庫路徑
	$db2 = new PDO('sqlite:/var/www/html/database/tcs');
	$db2->exec('set names utf-8'); // 2019.10.18 Eric：設定文字UTF-8格式
	$sql = "SELECT * FROM step2 where step_id='499'";
	$rows = $db2->query($sql); // 2019.10.18 Eric：獲取承上$sql資料庫語法，$rows為一個變數存取
	$row = $rows->fetch(); // 2019.10.18 Eric：變數$row擷取$rows的$sql資料庫語法之所有欄位資料
	$joint_offset = $row[5];
	$db2 = null;
?>
<?php
	//$db2 = new PDO('sqlite:/var/www/database/tcs'); // 2019.10.18 Eric：連結到資料庫路徑
	$db99 = new PDO('sqlite:/var/www/html/database/tcs');
	$db99->exec('set names utf-8'); // 2019.10.18 Eric：設定文字UTF-8格式
	$str3 = "select * from device limit 1";
	$rows58 = $db99->query($str3); // 2019.10.18 Eric：獲取承上$sql資料庫語法，$rows為一個變數存取
	$row58 = $rows58->fetch(); // 2019.10.18 Eric：變數$row擷取$rows的$sql資料庫語法之所有欄位資料
	$unit = $row58[8];
	$db99 = null;
?>
<?php
	$db22 = new PDO('sqlite:/var/www/html/database/tcs');
	$db22->exec('set names utf-8'); // 2019.10.18 Eric：設定文字UTF-8格式
	$sql = "SELECT * FROM step2 where step_id='500' ";
	
	// 2019.10.18 Eric：獲取承上$sql2277資料庫語法，$rows77為一個變數存取
	$rows = $db22->query($sql);
	
	// 2019.10.18 Eric：變數$row77擷取$rows77的$sql2277資料庫語法之所有欄位資料
	$row99 = $rows->fetch(); 
	$force = $row99[17];
    $force = $force*100;//2020.05.14 Rechel:小數轉換成百分比
	$db22 = null;
?>

<?php
	$db50 = new PDO('sqlite:/var/www/html/database/tcs');
	$db50->exec('set names utf-8'); // 2019.10.18 Eric：設定文字UTF-8格式
	$sql50 = "select * from tool limit 1";
	
	// 2019.10.18 Eric：獲取承上$sql2277資料庫語法，$rows77為一個變數存取
	$rows50 = $db50->query($sql50);
	
	// 2019.10.18 Eric：變數$row77擷取$rows77的$sql2277資料庫語法之所有欄位資料
	$row50 = $rows50->fetch(); 
	$gear_ratio = $row50[4];
	$db50 = null;
?>
<?php
//讀取ini檔
function read_ini_file_other($MCBpath_other)
{
    $str_other = parse_ini_file($MCBpath_other, true, INI_SCANNER_RAW);
    return $str_other;
	
}

$MCBpath_other = 'swapmcbstate.ini';	//MCB的INI檔
$array2 = read_ini_file_other($MCBpath_other); 

$u8StepID01 = $array2['Step_F']['u8StepID'];
$u8StepName01 = $array2['Step_F']['u8StepName'];
$u16StepRpm01 = $array2['Step_F']['u16StepRpm'];
$u16StepSlope01 = $array2['Step_F']['u16StepSlope'];
$u16StepMaxCurrent01 = $array2['Step_F']['u16StepMaxCurrent'];
$u16StepMaxTorque01 = $array2['Step_F']['u16StepMaxTorque'];
$u16StepMaxRevol01 = $array2['Step_F']['u16StepMaxRevol'];
$u16StepTime01 = $array2['Step_F']['u16StepTime'];
$u16StepAngle01 = $array2['Step_F']['u16StepAngle'];
$u16StepAngleWindow01 = $array2['Step_F']['u16StepAngleWindow'];
$u16StepTorqueWindow01 = $array2['Step_F']['u16StepTorqueWindow'];
$u16MinDutyCycle01 = $array2['Step_F']['u16MinDutyCycle'];
$u16StepFlag01 = $array2['Step_F']['u16StepFlag'];
$u16StepAngleUpperLimit01 = $array2['Step_F']['u16StepAngleUpperLimit'];
$u16StepAngleLowerLimit01 = $array2['Step_F']['u16StepAngleLowerLimit'];
$u16StepTorqueUpperLimit01 = $array2['Step_F']['u16StepTorqueUpperLimit'];
$u16StepTorqueLowerLimit01 = $array2['Step_F']['u16StepTorqueLowerLimit'];
$u16StepMonitorType01 = $array2['Step_F']['u16StepMonitorType'];
$u16StepDelaytime01 = $array2['Step_F']['u16StepDelaytime'];
?>
<?php
//讀取ini檔
function read_ini_file($MCBpath2)
{
    $str = parse_ini_file($MCBpath2, true, INI_SCANNER_RAW);
    return $str;
	
}

$MCBpath2 = '/mnt/ramdisk/swapmcbstate.ini';	//MCB的INI檔
$array = read_ini_file($MCBpath2); 

$cmd01 = $array['Page']['PageCmd'];
?>
<?php
/*
#region Function block.
# Convert array to integate.
function convertArrayToInt($arr){
	$Int = 0;
	foreach($arr as $key=>$value){
		if ($value==1){
			$Int += 1;
		}
		else{
			$Int += 0;
		}
		$Int = $Int << 1;
		//echo "key=".$key." value=".$value." Int=".$Int."<br>";
	}
	return $Int;
}
#endregion
//寫入ini檔
function write_ini_file($assoc_arr, $UIpath){
  	$content = arr2ini($assoc_arr);

	if (!$handle = fopen($UIpath , 'w')) 
	{
  		return false;
  	}

  	$success = fwrite($handle, $content);
  	fclose($handle);

  	return $success;
}

//arr2ini
function arr2ini(array $a, array $parent = array()){
  	$out = '';
  	foreach ($a as $k => $v)
  	{
  		if (is_array($v))
  		{
  			$sec = array_merge((array) $parent, (array) $k);
  			$out .= '[' . join('.', $sec) . ']' . PHP_EOL;
  			$out .= arr2ini($v, $sec);
  		}
  		else
  		{
            $v = str_replace('"', '\"', $v);
  			$out .= "$k=\"$v\"" . PHP_EOL;
  		}
  	}
  	return $out;
}

//讀取ini檔
function read_ini_file($MCBpath2)
{
    $str = parse_ini_file($MCBpath2, true, INI_SCANNER_RAW);
    return $str;
	
}

$MCBpath2 = '/mnt/ramdisk/swapmcbstate.ini';	//MCB的INI檔
$array = read_ini_file($MCBpath2); 

$cmd01 = $array['Page']['PageCmd'];

$u16Statusflags01 = $array['Status_TLG']['u16Statusflags'];
$u16ActError01 = $array['Status_TLG']['u16ActError'];
$u16ActProcNr01 = $array['Status_TLG']['u16ActProcNr'];
$u16ActStepNr01 = $array['Status_TLG']['u16ActStepNr'];
$u16ActCurr01 = $array['Status_TLG']['u16ActCurr'];
$u16ActTorque01 = $array['Status_TLG']['u16ActTorque'];
$u16ActRPM01 = $array['Status_TLG']['u16ActRPM'];
$u16MaxCurrent01 = $array['Status_TLG']['u16MaxCurrent'];
$u16MaxTorque01 = $array['Status_TLG']['u16MaxTorque'];
$u32Angle01 = $array['Status_TLG']['u32Angle'];
$u32Revolutions01 = $array['Status_TLG']['u32Revolutions'];
$u16TMDFlags01 = $array['Status_TLG']['u16TMDFlags'];
$s16Debug01 = $array['Status_TLG']['s16Debug'];
$s32Debug01 = $array['Status_TLG']['s32Debug'];

$UIpath = '/mnt/ramdisk/swapuicmd.ini';//UI的INI檔

//$arr_16 = array(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);//step_flag陣列
//include '/var/www/html/tcs10/tcs_toolkit.php';

//echo print_r($arr_16)."<br>";
//$step_flag = convertArrayToInt($arr_16);
//echo "step_flag=".$step_flag."<br>"."<br>";
 
  $Page = array( "PageNo"=>"1",
                 "PageCmd"=>"2"			 
              );
						
  $Control_TLG = array( "u16Ctrlflags"=>"9216",
	             "u16ControlMode"=>"1",
	             "u16WorkProc"=>"249",
				 "u16CtrlProgram"=>"1",
	             "u16ManRpm"=>"0",
				 "u16ManSlope"=>"0",
				 "u16ManMaxCurrent"=>"0",
				 "u16ManMaxTorque"=>"0",
				 "u16ManRpmMode"=>"0",
				 "u8TMDControl"=>"15"
			 );
						
    $array_write = array("Page"=>$Page,
						 "Control_TLG"=>$Control_TLG
                        );	
						
	//echo "Write array to INI = ";
	//echo '<pre>',print_r($array_write),'</pre>';
     write_ini_file($array_write,$UIpath);
*/
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

.btn.active.focus,.btn.active:focus,.btn.focus,.btn:active.focus,.btn:active:focus,
.btn:focus{outline:thin dotted;outline:5px auto -webkit-focus-ring-color;outline-offset:-2px}
.btn.focus,.btn:focus,.btn:hover{color:#333;text-decoration:none}
.btn.active,.btn:active{background-image:none;outline:0;-webkit-box-shadow:inset 0 3px 5px rgba(0,0,0,.125);

/*.nocursor { cursor:none; }*/
</style>
<script src="jquery/jquery.min.js.js" type="text/javascript" charset="UTF-8"></script>
<link href="css/tcs.css" rel="stylesheet" type="text/css">
<body bgcolor="white" onload="init();" class="nocursor">
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
		   <input onkeypress="return disablekeys();" onclick="myFunction55()" type="button" class="btn" style="height:50px;width:130px;font-size:20px;" value="Barcode-Job"></p>
		   &nbsp;&nbsp;
      </td>
      <td width="300" align="right" style="padding-right:20px;"><p>
           <a href="tool_info.php">
		   <input type="button" class="btn" style="height:50px;width:135px;font-size:30px;" value="Tool Info."></p>
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
	  <th rowspan="3" width="22%"><p id="ToolFastenStatus" style="font-size:65px;" >OK</p></th>
      <th rowspan="2" align="left" width="44%"><p id="result_status" style="font-size:25px;word-break: break-all;" >NO LOCK</p>
      </th>
      <th align="left" width="0.1%"><p id="rowid" style="font-size:25px;" ></p>
      </th> 
	  <th align="left" width="20%"><p id="status_img01" style="font-size:25px;margin-bottom:30px;margin-top:30px;" ><img style="FLOAT: center;height:160px;width:160px;background-repeat: no-repeat;" src="/tcs10/images/00.png"></p>
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
	  <input type="button" class="btn" style="height:50px;width:130px;font-size:30px;" value="Setup"></p>
	  </a>
	  </th>
      <th><p align="center">
	  <a href="datalog.php">
	  <input type="button" class="btn" style="height:50px;width:130px;font-size:30px;" value="Data"></p>
	  </a>
	  </th>
      <th><p align="center">
	  <a href="graph.php">
	  <input type="button" class="btn" style="height:50px;width:130px;font-size:30px;" value="Graph"></p>
	  </a>
	  </th>
      <th><p align="center">
	  <a href="tcsmain.php">
	  <input type="button" class="btn" style="height:50px;width:130px;font-size:30px;" value="Hot Key"></p>
	  </a>
	  </th>
    </tr>
    <tr>
      <th><p align="center">
      <input id="curtainInput" type="button" class="btn" style="height:50px;width:130px;font-size:30px;color: black;" value="Enable"></p>
	  </th>
      <th><p align="center">
	  <a href="confirm.php">
	  <input type="button" class="btn" style="height:50px;width:130px;font-size:30px;" value="Confirm"></p>
      </a>
	  </th>
      <th><p align="center">
	   <?php
	  if($batch_mode == "2")
	  echo '<input id="curtainRepeat" type="button" class="btn" style="height:50px;width:130px;font-size:30px;" value="None"></p>';
	  else
	  echo '<input id="curtainRepeat" type="button" class="btn" style="height:50px;width:130px;font-size:30px;" value="Repeat"></p>';  
	  ?>
	  </th>
      <th><p align="center">
	  <a href="barcode.php">
	  <input type="button" class="btn" style="height:50px;width:130px;font-size:30px;" value="Barcode"></p>
      </a>
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
	  <a href="setup_input_step.php"><input type="button" class="btn" style="height:50px;width:130px;font-size:30px;" value="Input"></p></a>
      </th>
      <th ><p align="center">      
        <table onclick="Select_job_step_start()" width="100" style="border:0px solid;padding:5px;margin-left: 20px;" rules="all" cellpadding='5';>
          <tr>
		  <td id="td1" value="<?php echo $btnImg01 ?>">1</td>
          <td id="td2" value="<?php echo $btnImg02 ?>">2</td>
          <td id="td3" value="<?php echo $btnImg03 ?>">3</td>
          <td id="td4" value="<?php echo $btnImg04 ?>">4</td>
          <td id="td5" value="<?php echo $btnImg05 ?>">5</td>
          <td id="td6" value="<?php echo $btnImg06 ?>">6</td>
          <td id="td7" value="<?php echo $btnImg07 ?>">7</td>
          <td id="td8" value="<?php echo $btnImg08 ?>">8</td>
          </tr>
          </table></p>
      </th>
	  
	  <th><p style="margin-left: 17px;">
	  <a href="setup_output_step.php"><input type="button" class="btn" style="height:50px;width:130px;font-size:30px;" value="Output"></p></a>
      </th>
      <th><p align="center">
          <table width="100" style="border:0px solid;padding:5px;margin-left: 20px;" rules="all" cellpadding='5';>
          <tr>
          <td>1</td>
          <td>2</td>
          <td>3</td>
          <td>4</td>
          <td>5</td>
          <td>6</td>
          <td>7</td>
          <td>8</td>
          </tr>
          </table></p>
      </th>
    </tr>
  </table>
</fieldset>
</td>
</tr>
</table>
<th rowspan="3" width="23%"><p id="status_change" style="font-size:25px;" >50.0</p></th>
<br><br>
<input type="hidden" id="PageNo" value="1">
<br><br>
<input type="hidden" id="PageCmd" value="<?php echo $cmd01;?>">
<br><br>
<input type="hidden" id="clock" size="50" />
<br><br>
</div>
<h3 id="rowid2" style="font-size:28px;" ></h3>
</body>
</html>
<script type="text/javascript">
document.getElementById("curtainInput").addEventListener(
  "click",
  function(event) {
    if (event.target.value === "Enable") {
      event.target.value = "Disable";
	  event.target.style = "height:50px;width:130px;font-size:30px;color: red;";
	  //stopCount();
	  disable01();
    } else {
      event.target.value = "Enable";
	  event.target.style = "height:50px;width:130px;font-size:30px;color: black;";
	  //startCount();
	  enable01();
    }
  },
  false
);

document.getElementById("curtainRepeat").addEventListener(
  "click",
  function(event) {
    if (event.target.value === "Repeat") {
      event.target.value = "None";
	  event.target.style = "height:50px;width:130px;font-size:30px;color: black;opacity: 1;";
	  repeat_ok();
    } else {
      event.target.value = "Repeat";
	  event.target.style = "height:50px;width:130px;font-size:30px;color: black;opacity: 1;";
	  repeat_no();
    }
  },
  false
);

var socket;		
var connect_flg = 0;
var i = 0;		// scroll index of dialog log area 

function repeat_ok()
  {
      jQuery.ajax({
       type: "POST",
       url: "repeat_enable.php",
       data: {repeatenable:"2"},
       cache: false,
       success: function(response)
       {
         //alert("Record successfully updated to Enable");
       },
	   complete: function (XHR, TS) { XHR = null }
     });
 }
function repeat_no()
  {
      jQuery.ajax({
       type: "POST",
       url: "repeat_disable.php",
       data: {repeatdisable:"0"},
       cache: false,
       success: function(response)
       {
         //alert("Record successfully updated to Disable");
       },
	   complete: function (XHR, TS) { XHR = null }
     });
 }
 
function enable01()
  {
      jQuery.ajax({
       type: "POST",
       url: "update_enable.php",
       data: {enable:"8192"},
       cache: false,
       success: function(response)
       {
         //alert("Record successfully updated to Enable");
       },
	   complete: function (XHR, TS) { XHR = null }
     });
 }
function disable01()
  {
      jQuery.ajax({
       type: "POST",
       url: "graph_sum_data1521.php",
       data: {disable:"0"},
       cache: false,
       success: function(response)
       {
         //alert("Record successfully updated to Disable");
       },
	   complete: function (XHR, TS) { XHR = null }
     });
 }

function myFunction55() {
  var job_id_startget_enter = prompt("Please enter Barcode to change Job:", "<?php echo $barcode_job_name;?>");
  var result = confirm('Are you Sure?');
  if (job_id_startget_enter == null || job_id_startget_enter == "") {
	return;
  } 
  if(result === true){ 
    jQuery.ajax({
       type: "POST",
       url: "update_job_barcode.php",
       data: {
		   job_name_barcode : job_id_startget_enter
	   },
       cache: false,
       success: function(response)
       {
         //alert("Change the Job!!");
		 location.href = "tcsmain005.php";
       },
	   complete: function (XHR, TS) { XHR = null }
     });
  }
}

function Select_job()
{
	var kk = $("#Job_ID").val();		//讀取Job值
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
	jQuery.ajax({
       type: "POST",
       url: "update_job.php",
       data: {job_id : kk},
       cache: false,
       success: function(response)
       {
         alert("Change the Job!!");
		 //location.href = "tcsmain522.php";
       },
	   complete: function (XHR, TS) { XHR = null }
     }); 
}

function Select_job_step_start()
{
	//var job_id_step_start = "<?php echo $job_id01; ?>";		//讀取Job值
	var job_td1 = "<?php echo $btnImg01; ?>";		//讀取Input腳位1的值
	var job_td2 = "<?php echo $btnImg02; ?>";		//讀取Input腳位2的值
	var job_td3 = "<?php echo $btnImg03; ?>";		//讀取Input腳位3的值
	var job_td4 = "<?php echo $btnImg04; ?>";		//讀取Input腳位4的值
	var job_td5 = "<?php echo $btnImg05; ?>";		//讀取Input腳位5的值
	var job_td6 = "<?php echo $btnImg06; ?>";		//讀取Input腳位6的值
	var job_td7 = "<?php echo $btnImg07; ?>";		//讀取Input腳位7的值
	var job_td8 = "<?php echo $btnImg08; ?>";		//讀取Input腳位8的值
	var job_id_step_start = prompt("Please enter Input Job:", "<?php echo $job_id01;?>");  //讀取Job值
    var result22 = confirm('Are you Sure?');
  if (job_id_step_start == null || job_id_step_start == "") {
	return;
  } 
  if(result22 === true){
	jQuery.ajax({
       type: "POST",
       url: "update_job_stepstart.php",
       data: {job_id_step : job_id_step_start,
			  td1 : job_td1,
			  td2 : job_td2,
			  td3 : job_td3,
			  td4 : job_td4,
			  td5 : job_td5,
			  td6 : job_td6,
			  td7 : job_td7,
			  td8 : job_td8
				},
       cache: false,
       success: function(response)
       {
         //alert("Change the Job!!");
		 location.href = "tcsmain_stepstart.php";
       },
	   complete: function (XHR, TS) { XHR = null }
     }); 
   }
}


var int = null;
 var int=self.setInterval("clock()",100)
        function clock()
        {
		    var t=new Date()
			document.getElementById("clock").value=t
            var _PageNo = '';
            var _PageCmd = '';
			
			// 發送 Ajax 查詢請求並處理
            var request_2 = new XMLHttpRequest();
            request_2.open("POST", "graph_sum_data1521.php");

            // POST 請求必須設置表頭在 open() 下面，send() 上面
            request_2.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
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
            // POST 參數須使用 send() 發送		
		    var obj_2 = {    
                "PageNo":document.getElementById("PageNo").value,
			    "PageCmd":document.getElementById("PageCmd").value
			}
            //str = obj_1.replace(/\\/g,"");
			var myJSON_2 = JSON.stringify(obj_2);
            console.log(myJSON_2);
			
			 var obj_offset = {    
                "JointOffset":<?php echo $joint_offset; ?>,
				"Unit":<?php echo $unit; ?>,
				"Force":<?php echo $force; ?>,
				"GearRatio":<?php echo $gear_ratio ?>,
				"TargetTorque":<?php echo $u16StepMaxTorque01; ?>,
				"TorqueWindow":<?php echo $u16StepTorqueWindow01; ?>,
				"TorqueUpperLimit":<?php echo $u16StepTorqueUpperLimit01; ?>,
				"TorqueLowerLimit":<?php echo $u16StepTorqueLowerLimit01; ?>,
				"TargetAngle":<?php echo $u16StepAngle01; ?>,
				"AngleWindow":<?php echo $u16StepAngleWindow01; ?>,
				"AngleUpperLimit":<?php echo $u16StepAngleUpperLimit01; ?>,
				"AngleLowerLimit":<?php echo $u16StepAngleLowerLimit01; ?>
			}
            //str = obj_1.replace(/\\/g,"");
			var myJSON_offset = JSON.stringify(obj_offset);
            //console.log(myJSON_22);
			var data_DB = JSON.parse(myJSON_offset);
			console.log(data_DB);
			var data_offset01 = data_DB.JointOffset;
			var unit01 = data_DB.Unit;
			var force01 = data_DB.Force;
			var gearratio01 = data_DB.GearRatio;
			var targettorque01 = (data_DB.TargetTorque/1862) * 3 - data_offset01;
			var targettorque250 = (data_DB.TargetTorque/1862) * 3;
			var torquewindow01 = (data_DB.TorqueWindow/1862) * 3;
			var torqueupperlimit01 = (data_DB.TorqueUpperLimit/1862) * 3;
			var torquelowerlimit01 = (data_DB.TorqueLowerLimit/1862) * 3;
			var targetangle01 = data_DB.TargetAngle;
			var anglewindow01 = data_DB.AngleWindow;
			var angleupperlimit01 = data_DB.AngleUpperLimit;
			var anglelowerlimit01 = data_DB.AngleLowerLimit;
			
            //str = obj_1.replace(/\\/g,"");
            var obj_3 = {
                "CmdSide":"UI",
                "Packet" :myJSON_2			
            }

            var myJSON_3 = JSON.stringify(obj_3);
			// console.log(myJSON_3);
			request_2.send(myJSON_3);
			
            request_2.onreadystatechange = function() {
                // 伺服器請求完成
                if (request_2.readyState === 4) {
                    // 伺服器回應成功
                    if (request_2.status === 200) {
                        var type = request_2.getResponseHeader("Content-Type");   // 取得回應類型
                        //alert(type);
						console.log(type);
						
                        // 判斷回應類型，這裡使用 JSON
                        if (type.indexOf("application/json") === 0) {               
                            console.log(request_2.responseText)            
                            var data = JSON.parse(request_2.responseText);
							console.log(data);
							//alert(data);
                            //if (data.PageNum) {
                            //    document.getElementById("EchoPageCmd").innerHTML = '員工：' + data.name + '，儲存成功！';
                            //}
	
							//document.getElementById("u32Angle").innerHTML = data.u32Angle;
							//var word=75;
							var u16Statusflags01 = data.u16Statusflags;
							var u16Statusflags_turn=[];
							for(var x=15; x>=0; x--){
								u16Statusflags_turn.push((u16Statusflags01&Math.pow(2,x))>>x);
							}
							console.log(u16Statusflags_turn);
							//document.getElementById("u16Statusflags").innerHTML = u16Statusflags_turn[0];
							
							var u32Angle01 = data.u32Angle;
							var u16ActTorque_status = data.u16ActTorque/1862*7;
							var u16MaxTorque_status = data.u16MaxTorque/1862*7;
							
							document.getElementById("status_change").innerHTML = u16Statusflags_turn[2];
							var status_change01 = document.getElementById("status_change").innerHTML;
							document.getElementById("u16ActTorque").innerHTML = u16ActTorque_status.toFixed(3);
							document.getElementById("u16MaxTorque").innerHTML = u16MaxTorque_status.toFixed(3);
							
							if(u16Statusflags_turn[0] == "0" && u16Statusflags_turn[15] == "1"){ //起子未鎖附也未啟動
								document.getElementById("myDIV1").style.backgroundColor = "#d4d4d4";
								document.getElementById("myDIV2").style.backgroundColor = "#d4d4d4";
								document.getElementById("myDIV3").style.backgroundColor = "#d4d4d4";
								document.getElementById("ToolFastenStatus").innerHTML = "--";
								document.getElementById("result_status").innerHTML = "...";
								document.getElementById("u16ActTorque").innerHTML = data.u16ActTorque_status; //顯示IOM Actual Torque
								//document.getElementById("u32Angle").innerHTML = data.u32Angle; //顯示IOM Actual Angle
								if(data.u16ActProcNr == "249"){
								var u32Angle_forward = u32Angle01/10/200/gearratio01;
								console.log(parseFloat(u32Angle_forward).toFixed(1));
								document.getElementById("u32Angle").innerHTML = u32Angle_forward.toFixed(1);
								var u16StepAngle_forward = targetangle01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngle_forward).toFixed(1));
								document.getElementById("TargetAngle").innerHTML = u16StepAngle_forward.toFixed(1);
								var u16StepAngleWindow_forward = anglewindow01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleWindow_forward).toFixed(1));
								document.getElementById("AngleWindow").innerHTML = u16StepAngleWindow_forward.toFixed(1);
								var u16StepAngleUpperLimit_forward = angleupperlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleUpperLimit_forward).toFixed(1));
								document.getElementById("AngleUpperLimit").innerHTML = u16StepAngleUpperLimit_forward.toFixed(1);
								var u16StepAngleLowerLimit_forward = anglelowerlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleLowerLimit_forward).toFixed(1));
								document.getElementById("AngleLowerLimit").innerHTML = u16StepAngleLowerLimit_forward.toFixed(1);
								if(unit01 == "0"){ //N.m 轉 Kgf-m 
								document.getElementById("unit").innerHTML = "Kgf-m";
								targettorque01 = targettorque01 * 10.2;
								targettorque01 = targettorque01 * 0.01; 
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3);//顯示TargetTorque，小數點第三位
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.01; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.01; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.01; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16ActTorque_status = u16ActTorque_status * 10.2;
								u16ActTorque_status = u16ActTorque_status * 0.01; 
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								if(unit01 == "1"){ //N.m 轉 N.m
								document.getElementById("unit").innerHTML = "N.m";
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3);
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								if(unit01 == "2"){ //N.m 轉 Kgf-cm
								document.getElementById("unit").innerHTML = "Kgf-cm";
								targettorque01 = targettorque01 * 10.2;
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16ActTorque_status = u16ActTorque_status * 10.2; 
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								if(unit01 == "3"){ //Lbf-in 轉 N.m
								document.getElementById("unit").innerHTML = "Lbf-in";
								targettorque01 = targettorque01 * 10.2;
								targettorque01 = targettorque01 * 0.86805; 
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.86805; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.86805; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.86805; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16ActTorque_status = u16ActTorque_status * 10.2; 
								u16ActTorque_status = u16ActTorque_status * 0.86805; 
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								}
								if(data.u16ActProcNr == "250"){
								var u32Angle_reverse = u32Angle01/10/200/gearratio01;
								console.log(parseFloat(u32Angle_reverse).toFixed(1));
								document.getElementById("u32Angle").innerHTML = u32Angle_reverse.toFixed(1);
								var u16StepAngle_reverse = targetangle01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngle_reverse).toFixed(1));
								document.getElementById("TargetAngle").innerHTML = u16StepAngle_reverse.toFixed(1);
								var u16StepAngleWindow_reverse = anglewindow01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleWindow_reverse).toFixed(1));
								document.getElementById("AngleWindow").innerHTML = u16StepAngleWindow_reverse.toFixed(1);
								var u16StepAngleUpperLimit_reverse = angleupperlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleUpperLimit_reverse).toFixed(1));
								document.getElementById("AngleUpperLimit").innerHTML = u16StepAngleUpperLimit_reverse.toFixed(1);
								var u16StepAngleLowerLimit_reverse = anglelowerlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleLowerLimit_reverse).toFixed(1));
								document.getElementById("AngleLowerLimit").innerHTML = u16StepAngleLowerLimit_reverse.toFixed(1);
								if(unit01 == "0"){ //N.m 轉 Kgf-m 
								document.getElementById("unit").innerHTML = "Kgf-m";
								targettorque250 = targettorque250 * 10.2;
								targettorque250 = targettorque250 * 0.01; 
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.01; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.01; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.01; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16ActTorque_status = u16ActTorque_status * 10.2;
								u16ActTorque_status = u16ActTorque_status * 0.01; 
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								if(unit01 == "1"){ //N.m 轉 N.m
								document.getElementById("unit").innerHTML = "N.m";
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								if(unit01 == "2"){ //N.m 轉 Kgf-cm
								document.getElementById("unit").innerHTML = "Kgf-cm";
								targettorque250 = targettorque250 * 10.2;
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16ActTorque_status = u16ActTorque_status * 10.2; 
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								if(unit01 == "3"){ //Lbf-in 轉 N.m
								document.getElementById("unit").innerHTML = "Lbf-in";
								targettorque250 = targettorque250 * 10.2;
								targettorque250 = targettorque250 * 0.86805; 
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.86805; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.86805; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.86805; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16ActTorque_status = u16ActTorque_status * 10.2; 
								u16ActTorque_status = u16ActTorque_status * 0.86805; 
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
									}
								}
							if(u16Statusflags_turn[0] == "1"){ //起子未鎖附，開始啟動
								document.getElementById("myDIV1").style.backgroundColor = "#d4d4d4";
								document.getElementById("myDIV2").style.backgroundColor = "#d4d4d4";
								document.getElementById("myDIV3").style.backgroundColor = "#d4d4d4";
								document.getElementById("ToolFastenStatus").innerHTML = "--";
								document.getElementById("result_status").innerHTML = "Tightening...";
								document.getElementById("u16ActTorque").innerHTML = data.u16ActTorque_status; //顯示IOM Actual Torque
								//document.getElementById("u32Angle").innerHTML = data.u32Angle; //顯示IOM Actual Angle
								document.getElementById("status_img01").innerHTML = "<img style='FLOAT: center;height:160px;width:160px;background-repeat: no-repeat;' src='/tcs10/images/tightening.gif'>";
								//開始啟動狀態，顯示Tightening開始啟動圖形
								if(data.u16ActProcNr == "249"){
								var u32Angle_forward = u32Angle01/10/200/gearratio01;
								console.log(parseFloat(u32Angle_forward).toFixed(1));
								document.getElementById("u32Angle").innerHTML = u32Angle_forward.toFixed(1);
								var u16StepAngle_forward = targetangle01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngle_forward).toFixed(1));
								document.getElementById("TargetAngle").innerHTML = u16StepAngle_forward.toFixed(1);
								var u16StepAngleWindow_forward = anglewindow01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleWindow_forward).toFixed(1));
								document.getElementById("AngleWindow").innerHTML = u16StepAngleWindow_forward.toFixed(1);
								var u16StepAngleUpperLimit_forward = angleupperlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleUpperLimit_forward).toFixed(1));
								document.getElementById("AngleUpperLimit").innerHTML = u16StepAngleUpperLimit_forward.toFixed(1);
								var u16StepAngleLowerLimit_forward = anglelowerlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleLowerLimit_forward).toFixed(1));
								document.getElementById("AngleLowerLimit").innerHTML = u16StepAngleLowerLimit_forward.toFixed(1);
								if(unit01 == "0"){ //N.m 轉 Kgf-m 
								document.getElementById("unit").innerHTML = "Kgf-m";
								targettorque01 = targettorque01 * 10.2;
								targettorque01 = targettorque01 * 0.01; 
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3);//顯示TargetTorque，小數點第三位
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.01; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.01; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.01; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16ActTorque_status = u16ActTorque_status * 10.2;
								u16ActTorque_status = u16ActTorque_status * 0.01; 
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								if(unit01 == "1"){ //N.m 轉 N.m
								document.getElementById("unit").innerHTML = "N.m";
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3);
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								if(unit01 == "2"){ //N.m 轉 Kgf-cm
								document.getElementById("unit").innerHTML = "Kgf-cm";
								targettorque01 = targettorque01 * 10.2;
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16ActTorque_status = u16ActTorque_status * 10.2; 
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								if(unit01 == "3"){ //Lbf-in 轉 N.m
								document.getElementById("unit").innerHTML = "Lbf-in";
								targettorque01 = targettorque01 * 10.2;
								targettorque01 = targettorque01 * 0.86805; 
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.86805; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.86805; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.86805; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16ActTorque_status = u16ActTorque_status * 10.2; 
								u16ActTorque_status = u16ActTorque_status * 0.86805; 
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								}
								if(data.u16ActProcNr == "250"){
								var u32Angle_reverse = u32Angle01/10/200/gearratio01;
								console.log(parseFloat(u32Angle_reverse).toFixed(1));
								document.getElementById("u32Angle").innerHTML = u32Angle_reverse.toFixed(1);
								var u16StepAngle_reverse = targetangle01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngle_reverse).toFixed(1));
								document.getElementById("TargetAngle").innerHTML = u16StepAngle_reverse.toFixed(1);
								var u16StepAngleWindow_reverse = anglewindow01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleWindow_reverse).toFixed(1));
								document.getElementById("AngleWindow").innerHTML = u16StepAngleWindow_reverse.toFixed(1);
								var u16StepAngleUpperLimit_reverse = angleupperlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleUpperLimit_reverse).toFixed(1));
								document.getElementById("AngleUpperLimit").innerHTML = u16StepAngleUpperLimit_reverse.toFixed(1);
								var u16StepAngleLowerLimit_reverse = anglelowerlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleLowerLimit_reverse).toFixed(1));
								document.getElementById("AngleLowerLimit").innerHTML = u16StepAngleLowerLimit_reverse.toFixed(1);
								if(unit01 == "0"){ //N.m 轉 Kgf-m 
								document.getElementById("unit").innerHTML = "Kgf-m";
								targettorque250 = targettorque250 * 10.2;
								targettorque250 = targettorque250 * 0.01; 
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.01; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.01; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.01; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16ActTorque_status = u16ActTorque_status * 10.2;
								u16ActTorque_status = u16ActTorque_status * 0.01; 
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								if(unit01 == "1"){ //N.m 轉 N.m
								document.getElementById("unit").innerHTML = "N.m";
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								if(unit01 == "2"){ //N.m 轉 Kgf-cm
								document.getElementById("unit").innerHTML = "Kgf-cm";
								targettorque250 = targettorque250 * 10.2;
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16ActTorque_status = u16ActTorque_status * 10.2; 
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								if(unit01 == "3"){ //Lbf-in 轉 N.m
								document.getElementById("unit").innerHTML = "Lbf-in";
								targettorque250 = targettorque250 * 10.2;
								targettorque250 = targettorque250 * 0.86805; 
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.86805; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.86805; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.86805; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16ActTorque_status = u16ActTorque_status * 10.2; 
								u16ActTorque_status = u16ActTorque_status * 0.86805; 
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
									}
								}
							if(u16Statusflags_turn[0] == "0" && u16Statusflags_turn[4] == "1"){ //起子鎖附完，判斷為NG
								document.getElementById("myDIV1").style.backgroundColor = "red";
								document.getElementById("myDIV2").style.backgroundColor = "red";
								document.getElementById("myDIV3").style.backgroundColor = "red";
								document.getElementById("ToolFastenStatus").innerHTML = "NG";
								document.getElementById("result_status").innerHTML = "Status NG!";
								document.getElementById("u16ActTorque").innerHTML = data.u16MaxTorque_status; //鎖附NG，顯示IOM Max Torque
								//document.getElementById("u32Angle").innerHTML = data.u32Angle; //鎖附NG，顯示IOM MAX Angle
								document.getElementById("status_img01").innerHTML = "<img style='FLOAT: center;height:160px;width:160px; background-repeat: no-repeat;' src='/tcs10/images/00.png'>";
								//鎖附NG，顯示空白圖形
								if(data.u16ActProcNr == "249"){ //判斷249鎖螺絲，鎖附NG，顯示IOM MAX Angle
								var u32Angle_forward = u32Angle01/10/200/gearratio01;
								console.log(parseFloat(u32Angle_forward).toFixed(1));
								document.getElementById("u32Angle").innerHTML = u32Angle_forward.toFixed(1);
								var u16StepAngle_forward = targetangle01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngle_forward).toFixed(1));
								document.getElementById("TargetAngle").innerHTML = u16StepAngle_forward.toFixed(1);
								var u16StepAngleWindow_forward = anglewindow01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleWindow_forward).toFixed(1));
								document.getElementById("AngleWindow").innerHTML = u16StepAngleWindow_forward.toFixed(1);
								var u16StepAngleUpperLimit_forward = angleupperlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleUpperLimit_forward).toFixed(1));
								document.getElementById("AngleUpperLimit").innerHTML = u16StepAngleUpperLimit_forward.toFixed(1);
								var u16StepAngleLowerLimit_forward = anglelowerlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleLowerLimit_forward).toFixed(1));
								document.getElementById("AngleLowerLimit").innerHTML = u16StepAngleLowerLimit_forward.toFixed(1);
								var u16MaxTorque_status = (data.u16MaxTorque/1862)* 3 - data_offset01;
								if(unit01 == "0"){ //N.m 轉 Kgf-m 
								document.getElementById("unit").innerHTML = "Kgf-m";
								targettorque01 = targettorque01 * 10.2;
								targettorque01 = targettorque01 * 0.01; 
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3); //鎖附NG，TargetTorque，小數點第三位
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.01; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3); //鎖附NG，TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.01; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.01; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16MaxTorque_status = u16MaxTorque_status * 10.2;
								u16MaxTorque_status = u16MaxTorque_status * 0.01; 
								console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附NG，顯示IOM MAX Torque
										}
								if(unit01 == "1"){ //N.m 轉 N.m
								document.getElementById("unit").innerHTML = "N.m";
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3);
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3); //鎖附NG，TargetWindow，小數點第三位
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附NG，顯示IOM MAX Torque
										}
								if(unit01 == "2"){ //N.m 轉 Kgf-cm
								document.getElementById("unit").innerHTML = "Kgf-cm";
								targettorque01 = targettorque01 * 10.2;
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3); //鎖附NG，TargetTorque，小數點第三位
								torquewindow01 = torquewindow01 * 10.2;
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3); //鎖附NG，TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16MaxTorque_status = u16MaxTorque_status * 10.2; 
								console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附NG，顯示IOM MAX Torque
										}
								if(unit01 == "3"){ //Lbf-in 轉 N.m
								document.getElementById("unit").innerHTML = "Lbf-in";
								targettorque01 = targettorque01 * 10.2;
								targettorque01 = targettorque01 * 0.86805; 
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.86805; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3); //鎖附NG，TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.86805; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.86805; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16MaxTorque_status = u16MaxTorque_status * 10.2; 
								u16MaxTorque_status = u16MaxTorque_status * 0.86805; 
								console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附NG，顯示IOM MAX Torque
										}
									}
								if(data.u16ActProcNr == "250"){ //判斷250退螺絲，顯示IOM MAX Angle
								var u32Angle_reverse = u32Angle01/10/200/gearratio01;
								console.log(parseFloat(u32Angle_reverse).toFixed(1));
								document.getElementById("u32Angle").innerHTML = u32Angle_reverse.toFixed(1);
								var u16StepAngle_reverse = targetangle01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngle_reverse).toFixed(1));
								document.getElementById("TargetAngle").innerHTML = u16StepAngle_reverse.toFixed(1);
								var u16StepAngleWindow_reverse = anglewindow01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleWindow_reverse).toFixed(1));
								document.getElementById("AngleWindow").innerHTML = u16StepAngleWindow_reverse.toFixed(1);
								var u16StepAngleUpperLimit_reverse = angleupperlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleUpperLimit_reverse).toFixed(1));
								document.getElementById("AngleUpperLimit").innerHTML = u16StepAngleUpperLimit_reverse.toFixed(1);
								var u16StepAngleLowerLimit_reverse = anglelowerlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleLowerLimit_reverse).toFixed(1));
								document.getElementById("AngleLowerLimit").innerHTML = u16StepAngleLowerLimit_reverse.toFixed(1);
								var u16MaxTorque_status = (data.u16MaxTorque/1862)* 3 / (force01);
								//console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附NG，顯示IOM MAX Torque
								if(unit01 == "0"){ //N.m 轉 Kgf-m 
								document.getElementById("unit").innerHTML = "Kgf-m";
								targettorque250 = targettorque250 * 10.2;
								targettorque250 = targettorque250 * 0.01; 
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.01; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3); //顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.01; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.01; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16MaxTorque_status = u16MaxTorque_status * 10.2;
								u16MaxTorque_status = u16MaxTorque_status * 0.01; 
								console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附NG，顯示IOM MAX Torque
										}
								if(unit01 == "1"){ //N.m 轉 N.m
								document.getElementById("unit").innerHTML = "N.m"; 
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								console.log(parseFloat(targetwindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附NG，顯示IOM MAX Torque
										}
								if(unit01 == "2"){ //N.m 轉 Kgf-cm
								document.getElementById("unit").innerHTML = "Kgf-cm";
								targettorque250 = targettorque250 * 10.2;
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16MaxTorque_status = u16MaxTorque_status * 10.2; 
								console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附NG，顯示IOM MAX Torque
										}
								if(unit01 == "3"){ //Lbf-in 轉 N.m
								document.getElementById("unit").innerHTML = "Lbf-in";
								targettorque250 = targettorque250 * 10.2;
								targettorque250 = targettorque250 * 0.86805; 
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.86805; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.86805; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.86805; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16MaxTorque_status = u16MaxTorque_status * 10.2; 
								u16MaxTorque_status = u16MaxTorque_status * 0.86805; 
								console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附NG，顯示IOM MAX Torque
										}
									}
								}
							if(u16Statusflags_turn[0] == "0" && $("#status_change").change()){ //起子鎖附完，判斷為OK
								document.getElementById("myDIV1").style.backgroundColor = "#00CC33";
								document.getElementById("myDIV2").style.backgroundColor = "#00CC33";
								document.getElementById("myDIV3").style.backgroundColor = "#00CC33";
								document.getElementById("ToolFastenStatus").innerHTML = "OK";
								document.getElementById("result_status").innerHTML = "Status OK!";
								document.getElementById("u16ActTorque").innerHTML = data.u16MaxTorque_status; //鎖附OK，顯示IOM Max Torque
								//document.getElementById("u32Angle").innerHTML = data.u32Angle; //鎖附OK，顯示IOM MAX Angle
								document.getElementById("status_img01").innerHTML = "<img style='FLOAT: center;height:160px;width:160px;background-repeat: no-repeat;' src='/tcs10/images/00.png'>";
								//鎖附OK，顯示空白圖形
								if(data.u16ActProcNr == "249"){ //判斷249鎖螺絲，鎖附NG，顯示IOM MAX Angle
								var u32Angle_forward = u32Angle01/10/200/gearratio01;
								console.log(parseFloat(u32Angle_forward).toFixed(1));
								document.getElementById("u32Angle").innerHTML = u32Angle_forward.toFixed(1);
								var u16StepAngle_forward = targetangle01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngle_forward).toFixed(1));
								document.getElementById("TargetAngle").innerHTML = u16StepAngle_forward.toFixed(1);
								var u16StepAngleWindow_forward = anglewindow01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleWindow_forward).toFixed(1));
								document.getElementById("AngleWindow").innerHTML = u16StepAngleWindow_forward.toFixed(1);
								var u16StepAngleUpperLimit_forward = angleupperlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleUpperLimit_forward).toFixed(1));
								document.getElementById("AngleUpperLimit").innerHTML = u16StepAngleUpperLimit_forward.toFixed(1);
								var u16StepAngleLowerLimit_forward = anglelowerlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleLowerLimit_forward).toFixed(1));
								document.getElementById("AngleLowerLimit").innerHTML = u16StepAngleLowerLimit_forward.toFixed(1);
								var u16MaxTorque_status = (data.u16MaxTorque/1862)* 3 - data_offset01;
								//console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附OK，顯示IOM MAX Torque
								if(unit01 == "0"){ //N.m 轉 Kgf-m 
								document.getElementById("unit").innerHTML = "Kgf-m";
								targettorque01 = targettorque01 * 10.2;
								targettorque01 = targettorque01 * 0.01; 
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.01; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.01; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.01; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16MaxTorque_status = u16MaxTorque_status * 10.2;
								u16MaxTorque_status = u16MaxTorque_status * 0.01; 
								console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附NG，顯示IOM MAX Torque
										}
								if(unit01 == "1"){ //N.m 轉 N.m
								document.getElementById("unit").innerHTML = "N.m";
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3);
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附NG，顯示IOM MAX Torque
										}
								if(unit01 == "2"){ //N.m 轉 Kgf-cm
								document.getElementById("unit").innerHTML = "Kgf-cm";
								targettorque01 = targettorque01 * 10.2; 
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16MaxTorque_status = u16MaxTorque_status * 10.2; 
								console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附NG，顯示IOM MAX Torque
										}
								if(unit01 == "3"){ //Lbf-in 轉 N.m
								document.getElementById("unit").innerHTML = "Lbf-in";
								targettorque01 = targettorque01 * 10.2;
								targettorque01 = targettorque01 * 0.86805; 
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.86805; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.86805; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.86805; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16MaxTorque_status = u16MaxTorque_status * 10.2; 
								u16MaxTorque_status = u16MaxTorque_status * 0.86805; 
								console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附NG，顯示IOM MAX Torque
										}	
									}
								if(data.u16ActProcNr == "250"){ //判斷250退螺絲，顯示IOM MAX Angle
								var u32Angle_reverse = u32Angle01/10/200/gearratio01;
								console.log(parseFloat(u32Angle_reverse).toFixed(1));
								document.getElementById("u32Angle").innerHTML = u32Angle_reverse.toFixed(1);
								var u16StepAngle_reverse = targetangle01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngle_reverse).toFixed(1));
								document.getElementById("TorqueAngle").innerHTML = u16StepAngle_reverse.toFixed(1);
								var u16StepAngleWindow_reverse = anglewindow01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleWindow_reverse).toFixed(1));
								document.getElementById("AngleWindow").innerHTML = u16StepAngleWindow_reverse.toFixed(1);
								var u16StepAngleUpperLimit_reverse = angleupperlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleUpperLimit_reverse).toFixed(1));
								document.getElementById("AngleUpperLimit").innerHTML = u16StepAngleUpperLimit_reverse.toFixed(1);
								var u16StepAngleLowerLimit_reverse = anglelowerlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleLowerLimit_reverse).toFixed(1));
								document.getElementById("AngleLowerLimit").innerHTML = u16StepAngleLowerLimit_reverse.toFixed(1);
								var u16MaxTorque_status = (data.u16MaxTorque/1862)* 3 / (force01);
								//console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附OK，顯示IOM MAX Torque
								if(unit01 == "0"){ //N.m 轉 Kgf-m 
								document.getElementById("unit").innerHTML = "Kgf-m";
								targettorque250 = targettorque250 * 10.2;
								targettorque250 = targettorque250 * 0.01; 
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.01; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.01; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.01; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16MaxTorque_status = u16MaxTorque_status * 10.2;
								u16MaxTorque_status = u16MaxTorque_status * 0.01; 
								console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附NG，顯示IOM MAX Torque
										}
								if(unit01 == "1"){ //N.m 轉 N.m
								document.getElementById("unit").innerHTML = "N.m"; 
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附NG，顯示IOM MAX Torque
										}
								if(unit01 == "2"){ //N.m 轉 Kgf-cm
								document.getElementById("unit").innerHTML = "Kgf-cm";
								targettorque250 = targettorque250 * 10.2;
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16MaxTorque_status = u16MaxTorque_status * 10.2; 
								console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附NG，顯示IOM MAX Torque
										}
								if(unit01 == "3"){ //Lbf-in 轉 N.m
								document.getElementById("unit").innerHTML = "Lbf-in";
								targettorque250 = targettorque250 * 10.2;
								targettorque250 = targettorque250 * 0.86805; 
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.86805; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.86805; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.86805; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16MaxTorque_status = u16MaxTorque_status * 10.2; 
								u16MaxTorque_status = u16MaxTorque_status * 0.86805; 
								console.log(parseFloat(u16MaxTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16MaxTorque_status).toFixed(3); //鎖附NG，顯示IOM MAX Torque
										}
									}
								}
							if(u16Statusflags_turn[0] == "1" && u16Statusflags_turn[1] == "1"){ //起子鎖附完，開始反轉
								document.getElementById("myDIV1").style.backgroundColor = "#d4d4d4";
								document.getElementById("myDIV2").style.backgroundColor = "#d4d4d4";
								document.getElementById("myDIV3").style.backgroundColor = "#d4d4d4";
								document.getElementById("ToolFastenStatus").innerHTML = "--";
								document.getElementById("result_status").innerHTML = "Reversing...";
								document.getElementById("u16ActTorque").innerHTML = data.u16ActTorque_status; //顯示IOM Actual Torque
								//document.getElementById("u32Angle").innerHTML = data.u32Angle; //顯示IOM Actual Angle
								document.getElementById("status_img01").innerHTML = "<img style='FLOAT: center;height:160px;width:160px;background-repeat: no-repeat;' src='/tcs10/images/reversing.gif'>";
								//反轉狀態，顯示Reverse反轉圖形
								if(data.u16ActProcNr == "249"){
								var u32Angle_forward = u32Angle01/10/200/gearratio01;
								console.log(parseFloat(u32Angle_forward).toFixed(1));
								document.getElementById("u32Angle").innerHTML = u32Angle_forward.toFixed(1);
								var u16StepAngle_forward = targetangle01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngle_forward).toFixed(1));
								document.getElementById("TargetAngle").innerHTML = u16StepAngle_forward.toFixed(1);
								var u16StepAngleWindow_forward = anglewindow01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleWindow_forward).toFixed(1));
								document.getElementById("AngleWindow").innerHTML = u16StepAngleWindow_forward.toFixed(1);
								var u16StepAngleUpperLimit_forward = angleupperlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleUpperLimit_forward).toFixed(1));
								document.getElementById("AngleUpperLimit").innerHTML = u16StepAngleUpperLimit_forward.toFixed(1);
								var u16StepAngleLowerLimit_forward = anglelowerlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleLowerLimit_forward).toFixed(1));
								document.getElementById("AngleLowerLimit").innerHTML = u16StepAngleLowerLimit_forward.toFixed(1);
								if(unit01 == "0"){ //N.m 轉 Kgf-m 
								document.getElementById("unit").innerHTML = "Kgf-m";
								targettorque01 = targettorque01 * 10.2;
								targettorque01 = targettorque01 * 0.01; 
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.01; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.01; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.01; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16ActTorque_status = u16ActTorque_status * 10.2;
								u16ActTorque_status = u16ActTorque_status * 0.01; 
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								if(unit01 == "1"){ //N.m 轉 N.m
								document.getElementById("unit").innerHTML = "N.m";
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3);
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								if(unit01 == "2"){ //N.m 轉 Kgf-cm
								document.getElementById("unit").innerHTML = "Kgf-cm";
								targettorque01 = targettorque01 * 10.2; 
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16ActTorque_status = u16ActTorque_status * 10.2; 
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								if(unit01 == "3"){ //Lbf-in 轉 N.m
								document.getElementById("unit").innerHTML = "Lbf-in";
								targettorque01 = targettorque01 * 10.2;
								targettorque01 = targettorque01 * 0.86805; 
								console.log(parseFloat(targettorque01).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque01).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.86805; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.86805; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.86805; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16ActTorque_status = u16ActTorque_status * 10.2; 
								u16ActTorque_status = u16ActTorque_status * 0.86805; 
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
									}
								if(data.u16ActProcNr == "250"){
								var u32Angle_reverse = u32Angle01/10/200/gearratio01;
								console.log(parseFloat(u32Angle_reverse).toFixed(1));
								document.getElementById("u32Angle").innerHTML = u32Angle_reverse.toFixed(1);
								var u16StepAngle_reverse = targetangle01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngle_reverse).toFixed(1));
								document.getElementById("TorqueAngle").innerHTML = u16StepAngle_reverse.toFixed(1);
								var u16StepAngleWindow_reverse = anglewindow01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleWindow_reverse).toFixed(1));
								document.getElementById("AngleWindow").innerHTML = u16StepAngleWindow_reverse.toFixed(1);
								var u16StepAngleUpperLimit_reverse = angleupperlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleUpperLimit_reverse).toFixed(1));
								document.getElementById("AngleUpperLimit").innerHTML = u16StepAngleUpperLimit_reverse.toFixed(1);
								var u16StepAngleLowerLimit_reverse = anglelowerlimit01/10/200/gearratio01;
								console.log(parseFloat(u16StepAngleLowerLimit_reverse).toFixed(1));
								document.getElementById("AngleLowerLimit").innerHTML = u16StepAngleLowerLimit_reverse.toFixed(1);
								if(unit01 == "0"){ //N.m 轉 Kgf-m 
								document.getElementById("unit").innerHTML = "Kgf-m";
								targettorque250 = targettorque250 * 10.2;
								targettorque250 = targettorque250 * 0.01; 
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.01; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.01; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.01; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16ActTorque_status = u16ActTorque_status * 10.2;
								u16ActTorque_status = u16ActTorque_status * 0.01; 
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								if(unit01 == "1"){ //N.m 轉 N.m
								document.getElementById("unit").innerHTML = "N.m";
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								if(unit01 == "2"){ //N.m 轉 Kgf-cm
								document.getElementById("unit").innerHTML = "Kgf-cm";
								targettorque250 = targettorque250 * 10.2;
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16ActTorque_status = u16ActTorque_status * 10.2; 
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
								if(unit01 == "3"){ //Lbf-in 轉 N.m
								document.getElementById("unit").innerHTML = "Lbf-in";
								targettorque250 = targettorque250 * 10.2;
								targettorque250 = targettorque250 * 0.86805; 
								console.log(parseFloat(targettorque250).toFixed(3));
								document.getElementById("TargetTorque").innerHTML = parseFloat(targettorque250).toFixed(3);
								torquewindow01 = torquewindow01 * 10.2;
								torquewindow01 = torquewindow01 * 0.86805; 
								console.log(parseFloat(torquewindow01).toFixed(3));
								document.getElementById("TargetWindow").innerHTML = parseFloat(torquewindow01).toFixed(3);//顯示TargetWindow，小數點第三位
								torqueupperlimit01= torqueupperlimit01* 10.2;
								torqueupperlimit01= torqueupperlimit01* 0.86805; 
								console.log(parseFloat(torqueupperlimit01).toFixed(3));
								document.getElementById("TorqueUpperLimit").innerHTML = parseFloat(torqueupperlimit01).toFixed(3);//顯示TorqueUpperLimit，小數點第三位
								torquelowerlimit01= torquelowerlimit01* 10.2;
								torquelowerlimit01= torquelowerlimit01* 0.86805; 
								console.log(parseFloat(torquelowerlimit01).toFixed(3));
								document.getElementById("TorqueLowerLimit").innerHTML = parseFloat(torquelowerlimit01).toFixed(3);//顯示TorqueLowerLimit，小數點第三位
								u16ActTorque_status = u16ActTorque_status * 10.2; 
								u16ActTorque_status = u16ActTorque_status * 0.86805; 
								console.log(parseFloat(u16ActTorque_status).toFixed(3));
								document.getElementById("u16ActTorque").innerHTML = parseFloat(u16ActTorque_status).toFixed(3); //顯示IOM Act Torque
										}
									}
								}

                        }
						else{
						    //console.log(request.responseText);
						}
                    } 
				    else {
                        alert("Error code = " + request.status);
                    }
                }
            }
        }
		
var pollingTm2 = null;		
function send_sum(){
     $.ajax({
        type: "get",
		url: "graph_sum_data1_new.php",
        success:function(data2)
        {
            //console.log the response
			console.log(data2);
		    var kkk11 = data2.split(",");
		    console.log(kkk11);
			$('#seq_id01').html(kkk11[9]);
			$('#tightening_program01').html(kkk11[10]);
			$('#tightening_repeat01').html(kkk11[11]);
			$('#job_id_select01').html(kkk11[12]);
			var iom_tightening_repeat = document.getElementById("tightening_repeat01").innerHTML;
			var iom_tightening_program = document.getElementById("tightening_program01").innerHTML;
			var iom_tightening_seqence = document.getElementById("seq_id01").innerHTML;
			var barcode_job_id_select = document.getElementById("job_id_select01").innerHTML;
			var status_img = document.getElementById("status_img01").innerHTML;
			if(barcode_job_id_select == "100"){ //選擇代碼100，select bar顯示"Clear"
			 document.getElementById("job_id_select01").innerHTML = "Clear";
		    }
			if(barcode_job_id_select == "101"){ //選擇代碼101，select bar顯示"Disable"
			 document.getElementById("job_id_select01").innerHTML = "Disable";
		    }
			if(barcode_job_id_select == "102"){ //選擇代碼102，select bar顯示"NS"
			 document.getElementById("job_id_select01").innerHTML = "NS";
		    }
			if(barcode_job_id_select == "103"){ //選擇代碼103，select bar顯示"AS"
			 document.getElementById("job_id_select01").innerHTML = "AS";
		    }
			if(barcode_job_id_select == ""){ //起子未鎖附，開始啟動
			 document.getElementById("job_id_select01").innerHTML = "Normal";
		    }
			if(kkk11[5] == "0"){ //起子未鎖附，開始啟動
			 pollingTm2 = setTimeout(function(){
                send_sum();
             }, 100);
		    }
			if(kkk11[5] == "1"){ //起子鎖附完，判斷為NG
			 pollingTm2 = setTimeout(function(){
                send_sum();
             }, 100);
			 //stopCount();
		    }
			if(kkk11[5] == "2"){ //起子鎖附完，判斷為OK
			 pollingTm2 = setTimeout(function(){
                send_sum();
             }, 100);
			 //stopCount();
		    }
			if(kkk11[5] == "3"){ //起子鎖附完，開始反轉
			 pollingTm2 = setTimeout(function(){
                send_sum();
            }, 100);
		    }
			if(kkk11[11] == "0"){ //Tightening Repeat鎖附完，顯示"0"條件判斷
			 document.getElementById("myDIV1").style.backgroundColor = "#00CC33";
	         document.getElementById("myDIV2").style.backgroundColor = "#00CC33";
	         document.getElementById("myDIV3").style.backgroundColor = "#00CC33";
			 document.getElementById("ToolFastenStatus").innerHTML = "--";
			 document.getElementById("result_status").innerHTML = "Sequence changing...";
			 //讓結果顯示為"Sequence changing..."
			 document.getElementById("tightening_repeat01").innerHTML = "1";
			 //讓Tightening_repeat顯示為"1"
		    }
			if(kkk11[10] == "0"){ ///Tightening Program鎖附完，顯示"0"條件判斷
			 document.getElementById("myDIV1").style.backgroundColor = "#d4d4d4";
	         document.getElementById("myDIV2").style.backgroundColor = "#d4d4d4";
	         document.getElementById("myDIV3").style.backgroundColor = "#d4d4d4";
			 document.getElementById("ToolFastenStatus").innerHTML = "--";
			 document.getElementById("result_status").innerHTML = "Sequence ending...";
			 //讓結果顯示為"Sequence ending..."
			 document.getElementById("tightening_repeat01").innerHTML = "0";
			 //讓Tightening_repeat顯示為"0"
		    }
			if(kkk11[13] == "1" || kkk11[13] == "2" || kkk11[13] == "3"){ 
			 document.getElementById("td1").style.backgroundColor = "#00FF00";
			}
			if(kkk11[13] == "0" || kkk11[13] == ""){ 
			 document.getElementById("td1").style.backgroundColor = "#FFFFFF";
			}
			if(kkk11[14] == "1" || kkk11[14] == "2" || kkk11[14] == "3"){ 
			 document.getElementById("td2").style.backgroundColor = "#00FF00";
			}
			if(kkk11[14] == "0" || kkk11[14] == ""){ 
			 document.getElementById("td2").style.backgroundColor = "#FFFFFF";
			}
			if(kkk11[15] == "1" || kkk11[15] == "2" || kkk11[15] == "3"){ 
			 document.getElementById("td3").style.backgroundColor = "#00FF00";
			}
			if(kkk11[15] == "0" || kkk11[15] == ""){ 
			 document.getElementById("td3").style.backgroundColor = "#FFFFFF";
			}
			if(kkk11[16] == "1" || kkk11[16] == "2" || kkk11[16] == "3"){ 
			 document.getElementById("td4").style.backgroundColor = "#00FF00";
			}
			if(kkk11[16] == "0" || kkk11[16] == ""){ 
			 document.getElementById("td4").style.backgroundColor = "#FFFFFF";
			}
			if(kkk11[17] == "1" || kkk11[17] == "2" || kkk11[17] == "3"){ 
			 document.getElementById("td5").style.backgroundColor = "#00FF00";
			}
			if(kkk11[17] == "0" || kkk11[17] == ""){ 
			 document.getElementById("td5").style.backgroundColor = "#FFFFFF";
			}
			if(kkk11[18] == "1" || kkk11[18] == "2" || kkk11[18] == "3"){ 
			 document.getElementById("td6").style.backgroundColor = "#00FF00";
			}
			if(kkk11[18] == "0" || kkk11[18] == ""){ 
			 document.getElementById("td6").style.backgroundColor = "#FFFFFF";
			}
			if(kkk11[19] == "1" || kkk11[19] == "2" || kkk11[19] == "3"){ 
			 document.getElementById("td7").style.backgroundColor = "#00FF00";
			}
			if(kkk11[19] == "0" || kkk11[19] == ""){ 
			 document.getElementById("td7").style.backgroundColor = "#FFFFFF";
			}
			if(kkk11[20] == "1" || kkk11[20] == "2" || kkk11[20] == "3"){ 
			 document.getElementById("td8").style.backgroundColor = "#00FF00";
			}
			if(kkk11[20] == "0" || kkk11[20] == ""){ 
			 document.getElementById("td8").style.backgroundColor = "#FFFFFF";
			}
			/*
			var statusok01 = kkk11[21];
				var statusok01_turn=[];
				for(var x=15; x>=0; x--){
					statusok01_turn.push((statusok01&Math.pow(2,x))>>x);
				}
				console.log(statusok01_turn[2]);
			$('#status_change').html(statusok01_turn[2]);
			*/
        },
	    complete: function (XHR, TS) { XHR = null }
    });
}
//Call our function
send_sum();

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

</script>