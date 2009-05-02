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

function snapshot_normalize($path) {
	if(substr($path,-1) == '/')
		return substr($path,0,-1);
	return $path;
}

function snapshot_snapshot() {
	$url = (object)parse_url($GLOBALS['db_url']);
	$dbname = substr($url->path, 1);
	$db = snapshot_normalize(variable_get('snapshot_mysql_var', '/var/lib/mysql/')) . '/' . $dbname;
	$snapshot = snapshot_normalize(variable_get('snapshot_folder', '/tmp/snapshot/')) . '/' . $dbname;
	$c = new Cp($db, $snapshot);
	$c->pre();
	db_query('FLUSH TABLES WITH READ LOCK');
	$c->log('lock');
	$c->copy();
	db_query('UNLOCK TABLES');
	$c->log('unlock');
	$c->post();
}

Class Copy {
	protected $from, $to;
	public function __construct($f, $t) {
		$this->from = $f;
		$this->to = $t;
	}
	public function pre() {}
	public function copy() {}
	public function post() {}
	public function log($msg) {
		echo "[Snapshot] $msg\n";
	}
}

class Cp extends Copy {
	public function pre() {
		rmdir($this->to);
		mkdir($this->to, 0744, true);
		$this->log("$this->to is ready");
	}
	public function copy() {
		echo "beuha\n";
		$handle = opendir($this->from);
		echo $this->from;
		while(false !== ($file = readdir($handle))) {
			if($file != "." && $file != ".."){
				copy("$this->from/$file", "$this->to/$file");
				$this->log("backup: $file");
			}
		}
		closedir($handle);
	}
}