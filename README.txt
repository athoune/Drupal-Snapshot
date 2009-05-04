-- SUMMARY --

Make a drupal snapshot with drush. Works only with Mysql and MyIsam.

Different methods can be used :
 * cp
 * rsync
 * rdiff-backup.

-- REQUIREMENTS --

Drush : http://drupal.org/project/drush

-- INSTALLATIONS --
Install php in command line version (php5-cli in debian like Linux)
Install and test drush
rsync is available in any classical Linux, but not rdiff-backup, look for your packet system to install it.

Yet, there is no release
$ cd sites/all/modules
$ git clone git://github.com/athoune/Drupal-Snapshot.git snapshot

-- CONFIGURATION --

Admin page is available at admin/settings/snapshot
Check that mysql's myisam files in the suggested path.
Check where backup is done.
Choose a backup method.

-- USAGE --

$ drush snapshot

The classical way is to add a simple script in /etc/cron.daily/mySnapshot

8<----------------
#!/bin/sh

cd /var/www/maDrupalSite && ./sites/all/modules/drush/drush.php snapshot
---------------->8

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
