
<?
function returnIfExists($json, $setting) {
    if ($json == null) {
        return "";
    }
    if (array_key_exists($setting, $json)) {
        return $json[$setting];
    }
    return "";
}

function convertAndGetSettings() {
    global $settings;
        
    $cfgFile = $settings['configDirectory'] . "/plugin.notification.json";
    if (file_exists($cfgFile)) {
        $j = file_get_contents($cfgFile);
        $json = json_decode($j, true);
        return $json;
    }
    $j = "{}";
    return json_decode($j, true);
}

$pluginJson = convertAndGetSettings();
?>


<div id="global" class="settings">
<fieldset>
<legend>FPP Notification Config</legend>

<script>

var notificationConfig = <? echo json_encode($pluginJson, JSON_PRETTY_PRINT); ?>;


var uniqueId = 1;
var modelOptions = "";

function SaveNotification() {

//pushover
    
    var notConfig = {"pushover":{},"telegram":{}};
    notConfig["pushover"]["token"] = document.getElementById("pushovertoken").value;
    notConfig["pushover"]["user"] = document.getElementById("pushoveruser").value;
    notConfig["telegram"]["bottoken"] = document.getElementById("telegrambottoken").value;
    notConfig["telegram"]["chatid"] = document.getElementById("telegramchatid").value;
    
    var data = JSON.stringify(notConfig);
    $.ajax({
        type: "POST",
        url: 'api/configfile/plugin.notification.json',
        dataType: 'json',
        async: false,
        data: data,
        processData: false,
        contentType: 'application/json',
        success: function (data) {
           SetRestartFlag(2);
        }
    });
}


$(document).ready(function() {
                  

});

</script>
<div>
Pushover Token:<input type='text' id='pushovertoken' minlength='7' maxlength='120' size='50' class='pushovertoken' />
Pushover User:<input type='text' id='pushoveruser' minlength='7' maxlength='120' size='50' class='pushoveruser' />
<br>
Telegram Bot Token:<input type='text' id='telegrambottoken' minlength='7' maxlength='120' size='50' class='telegrambottoken' />
Telegram Chat ID:<input type='text' id='telegramchatid' minlength='7' maxlength='120' size='50' class='telegramchatid' />
<br>
<br>

<table border=0>
<tr><td colspan='2'>
        <input type="button" value="Save" class="buttons genericButton" onclick="SaveNotification();">
    </td>
</tr>
</table>



</div>

<script>

document.getElementById("pushovertoken").value = notificationConfig["pushover"]["token"];
document.getElementById("pushoveruser").value = notificationConfig["pushover"]["user"];
document.getElementById("telegrambottoken").value = notificationConfig["telegram"]["bottoken"];
document.getElementById("telegramchatid").value = notificationConfig["telegram"]["chatid"];

</script>

</fieldset>
</div>