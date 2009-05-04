<?php
// $Id$

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

/**
 * Clean trailing slash
 */
function snapshot_normalize($path) {
  if (substr($path, -1) == '/') {
    return substr($path, 0, -1);
  }
  return $path;
}

function snapshot_snapshot() {
  $chrono = time();
  $url = (object)parse_url($GLOBALS['db_url']);
  $dbname = substr($url->path, 1);
  $db = snapshot_normalize(variable_get('snapshot_mysql_var', '/var/lib/mysql/')) .'/'. $dbname;
  $snapshot = snapshot_normalize(variable_get('snapshot_folder', '/tmp/snapshot/')) .'/'. $dbname;
  $copyator = array(snapshot_Cp, snapshot_Rsync, snapshot_Rdiff);
  $snapshot_copy = variable_get('snapshot_copy', 1);
  $c = new $copyator[$snapshot_copy]($db, $snapshot);
  $c->pre();
  db_query('FLUSH TABLES WITH READ LOCK');
  $c->log('lock');
  $c->copy();
  db_query('UNLOCK TABLES');
  $c->log('unlock');
  $c->post();
  $c->log('Chrono: '. (time() - $chrono) .'s');
}

Class snapshot_Copy {
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

class snapshot_Cp extends snapshot_Copy {
  public function pre() {
    rmdir($this->to);
    mkdir($this->to, 0744, TRUE);
    $this->log("$this->to is ready");
  }
  public function copy() {
    echo "beuha\n";
    $handle = opendir($this->from);
    echo $this->from;
    while (false !== ($file = readdir($handle))) {
      if ($file != "." && $file != "..") {
        copy("$this->from/$file", "$this->to/db/$file");
        $this->log("backup: $file");
      }
    }
    closedir($handle);
  }
}

class snapshot_Rsync extends snapshot_Copy {
  public function pre() {
    system('mkdir -p '. escapeshellarg($this->to) .'/db');
    system('mkdir -p '. escapeshellarg($this->to) .'/www');
    $this->log("$this->to is ready");
  }
  protected function bkp($from, $to) {
    passthru('rsync -av --delete-after '. escapeshellarg($from) .' '. escapeshellarg($to));
  }
  public function copy() {
    $this->bkp($this->from, $this->to .'/db/');
    $this->log('db');
    $this->bkp(system('pwd'), $this->to .'/www/');
    $this->log('www');
  }
}

class snapshot_Rdiff extends snapshot_Rsync {
  protected function bkp($from, $to) {
    passthru('rdiff-backup -v5 '. escapeshellarg($from) .' '. escapeshellarg($to));
  }
  public function post() {
    passthru('rdiff-backup -v5 --remove-older-than 2W '. escapeshellarg($to));
  }
}
