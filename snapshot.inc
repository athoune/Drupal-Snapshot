<?php
// $Id: snapshot.inc,v 1.1 2009/05/07 10:07:10 athoune Exp $

/**
 * The name of the current db
 */
function snapshot_default_db() {
  $url = (object)parse_url($GLOBALS['db_url']);
  return substr($url->path, 1);
}
