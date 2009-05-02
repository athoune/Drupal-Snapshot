<?php

/**
 * Implementation of hook_help().
 */
function snapshot_help($section) {
  switch ($section) {
    case 'drush:snapshot':
      return t("Usage: drush [options] snapshot \n\n Take a database snapshot");
  }
}

/**
 * Implementation of hook_drush_command().
 */
function snapshot_drush_command() {
  $items['snapshot'] = array(
    'callback' => 'snapshot_snapshot',
    'description' => 'Take a snapshot',
  );
  return $items;
}

function snapshot_snapshot() {
	$url = (object)parse_url($GLOBALS['db_url']);
	$dbname = substr($url->path, 1);
	$db = variable_get('snapshot_mysql_var', '/var/lib/mysql/') . '/' . $dbname;
	$snapshot = variable_get('snapshot_folder', '/tmp/snapshot/') . '/' . $dbname;
	rmdir($snapshot);
	mkdir($snapshot, 0744, true);
	db_query('FLUSH TABLES WITH READ LOCK');
	echo "[Snapshot] lock\n";
	$handle = opendir($db);
	while(false !== ($file = readdir($handle))) {
		if($file != "." && $file != ".."){
			echo "[Snapshot] copying $file\n";
			copy("$db/$file", "$snapshot/$file");
		}
	}
	closedir($handle);
	db_query('UNLOCK TABLES');
	echo "[Snapshot] unlock";
}