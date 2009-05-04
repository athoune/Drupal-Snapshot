-- SUMMARY --

Make a drupal snapshot with drush. Works only with Mysql and MyIsam.

Different methods can be used :
 * cp
 * rsync
 * rdiff-backup.

-- REQUIREMENTS --

Drush : http://drupal.org/project/drush

-- INSTALLATIONS --

$ cd sites/all/modules
$ git clone git://github.com/athoune/Drupal-Snapshot.git snapshot

-- CONFIGURATION --

-- USAGE --

$ drush snapshot

-- CONTACT --
Mathieu - http://drupal.org/user/378820

[TODO]
Don't backup temporary tables.
Don't backup temporary files.
Options for differents backup type.
Handling fs snapshot like XFS, lvm or ZFS.
Handling rsnapshot.
Handling distant backup.
Mysql Innodb and Postgres
