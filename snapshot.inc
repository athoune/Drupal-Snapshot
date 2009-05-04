<?php
// $Id$

/**
 * The name of the current db
 */
function snapshot_default_db() {
  $url = (object)parse_url($GLOBALS['db_url']);
  return substr($url->path, 1);
}
