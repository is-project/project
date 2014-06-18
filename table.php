<?php
    if(!file_exists('config.php')) {
        die('File "config.php" not found.');
    }

    require_once 'config.php';

    $db = mysql_connect(DB_HOST, DB_USER, DB_PASS) or die("Could not connect to mysql server.");
    mysql_select_db(DB_NAME, $db) or die('Could not select database.');
?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title></title>

    <style type="text/css" media="all">
        
        * {
            -webkit-print-color-adjust:exact;
        }

        table {
            background-color: #ffffff;
            border-spacing: 0px;
            border-collapse: collapse;
        }

        th {
            border: 1px solid #8cacbb;
            background-color: #dee7ec !important;
            padding: 3px;
        }

        td {
            border: 1px solid #8cacbb;
            padding: 3px;
        }

        a {
            text-decoration: none;
            color: black;
        }

    </style>
</head>
<body>

    <h1 id="toc-datenbank-is-project">Datenbank is-project</h1>

    <h2 id="toc-daten-fur-tabelle-projects">Daten für Tabelle projects</h2>

    <table>
        <thead>
            <tr>
                <th>project</th>
                <th>parent_project</th>
                <th>name</th>
                <th>description</th>
                <th>record_structure</th>
            </tr>
        </thead>

        <tbody>
            <?php
                $result = mysql_query("SELECT * FROM `projects`");
                while($row = mysql_fetch_object($result)) {
                    print "
                        <tr>
                            <td>{$row->project}</td>
                            <td>{$row->parent_project}</td>
                            <td>{$row->name}</td>
                            <td>{$row->description}</td>
                            <td>[...]</td>
                        </tr>
                    ";
                }
            ?>
        </tbody>
    </table>

    <h2 id="toc-daten-fur-tabelle-users">Daten für Tabelle users</h2>

    <table>
        <thead>
            <tr>
                <th>user</th>
                <th>name</th>
                <th>email</th>
                <th>password</th>
                <th>description</th>
                <th>valid_until</th>
            </tr>
        </thead>

        <tbody>
            <?php
                $result = mysql_query("SELECT * FROM `users`");
                while($row = mysql_fetch_object($result)) {
                    print "
                        <tr>
                            <td>{$row->user}</td>
                            <td>{$row->name}</td>
                            <td>{$row->email}</td>
                            <td>{$row->password}</td>
                            <td>{$row->description}</td>
                            <td>{$row->valid_until}</td>
                        </tr>
                    ";
                }
            ?>
        </tbody>
    </table>

    <h2 id="toc-daten-fur-tabelle-permissions">Daten für Tabelle permissions</h2>

    <table>
        <thead>
            <tr>
                <th>permission</th>
                <th>name</th>
                <th>description</th>
            </tr>
        </thead>

        <tbody>
            <?php
                $result = mysql_query("SELECT * FROM `permissions`");
                while($row = mysql_fetch_object($result)) {
                    print "
                        <tr>
                            <td>{$row->permission}</td>
                            <td>{$row->name}</td>
                            <td>{$row->description}</td>
                        </tr>
                    ";
                }
            ?>

            <tr>
                <td>&nbsp;</td>
                <td>&nbsp;</td>
                <td>&nbsp;</td>
            </tr>
            <tr>
                <td>&nbsp;</td>
                <td>&nbsp;</td>
                <td>&nbsp;</td>
            </tr>
            <tr>
                <td>&nbsp;</td>
                <td>&nbsp;</td>
                <td>&nbsp;</td>
            </tr>
        </tbody>
    </table>

    <h2 id="toc-daten-fur-tabelle-groups">Daten für Tabelle groups</h2>

    <table>
        <thead>
            <tr>
                <th>group</th>
                <th>project</th>
                <th>name</th>
                <th>system</th>
            </tr>
        </thead>

        <tbody>
            <?php
                $result = mysql_query("SELECT * FROM `groups`");
                while($row = mysql_fetch_object($result)) {
                    print "
                        <tr>
                            <td>{$row->group}</td>
                            <td>{$row->project}</td>
                            <td>{$row->name}</td>
                            <td>{$row->system}</td>
                        </tr>
                    ";
                }
            ?>
        </tbody>
    </table>

    <h2 id="toc-daten-fur-tabelle-link-groups-permissions">Daten für Tabelle
    link_groups_permissions</h2>

    <!-- <table>
        <tr>
            <th>group</th>
            <?php
                $result = mysql_query("SELECT * FROM `link_groups_permissions`");
                while($row = mysql_fetch_object($result)) {
                    print "
                        <td>{$row->group}</td>
                    ";
                }
            ?>
        </tr>
        <tr>
            <th>permission</th>
            <?php
                $result = mysql_query("SELECT * FROM `link_groups_permissions`");
                while($row = mysql_fetch_object($result)) {
                    print "
                        <td>{$row->permission}</td>
                    ";
                }
            ?>
        </tr>

        <tbody>
            
        </tbody>
    </table> -->

    <table>
        <thead>
            <tr>
                <th>group</th>
                <th>permission</th>
            </tr>
        </thead>

        <tbody>
            <?php
                $result = mysql_query("SELECT * FROM `link_groups_permissions`");
                while($row = mysql_fetch_object($result)) {
                    print "
                        <tr>
                            <td>{$row->group}</td>
                            <td>{$row->permission}</td>
                        </tr>
                    ";
                }
            ?>
        </tbody>
    </table>

    <h2 id="toc-daten-fur-tabelle-link-users-groups">Daten für Tabelle
    link_users_groups</h2>

    <table>
        <thead>
            <tr>
                <th>user</th>
                <th>group</th>
            </tr>
        </thead>

        <tbody>
            <?php
                $result = mysql_query("SELECT * FROM `link_users_groups`");
                while($row = mysql_fetch_object($result)) {
                    print "
                        <tr>
                            <td>{$row->user}</td>
                            <td>{$row->group}</td>
                        </tr>
                    ";
                }
            ?>
        </tbody>
    </table>

</body>
</html>