#! /bin/bash

# Current directory should be moped/new-server/misc.
# You need the python module "suds" somewhere; modify PYTHONPATH below
# accordingly.
# There should be a newly started server and car or simulator running.
# If there is no car or simulator, only the last call will fail.

# Modify the SQL definition below if needed.
# Modify USERDUMP so it refers to a mysql dump of wordpress's user table
#  wp_users, modified so it enters the database "fresta2".
# Set USERID to your id in the SQL user database.

SQL="mysql -uroot -proot"

USERDUMP=~/moped/wp_users.dump
VIN=20UYA31581L000002
USERID=1

if true; then
    $SQL <<EOF
    drop database fresta2;
EOF

    $SQL <<EOF
    create database fresta2 DEFAULT CHARACTER SET utf8 DEFAULT COLLATE utf8_general_ci;
    grant all privileges on fresta2 . * to 'fresta'@'%';
    flush privileges;
EOF

    $SQL < ../fresta2-schema

    (echo "use fresta2";cat $USERDUMP) | $SQL

fi

PYTHONPATH=~/moped python <<EOF
from suds.client import Client
url='http://localhost:9990/moped/pws?wsdl'
client = Client(url)
client.options.cache.clear()

s = client.service

from testmoped import *
import testmoped
import base64

x=base64.b64encode(readfile("../../simulator/configs/system1.xml"))
x = s.addVehicleConfig("MOPED", x)
print x

x = s.addVehicle("minMOPED","$VIN", "MOPED")
print x

x = s.addUserVehicleAssociation($USERID, "$VIN", True)
print x

#app = testmoped.uploadplus(s, "SemiAutomaticReverseParking", "1.0")
app = testmoped.uploadplus(s, "PluginCreationTest2", "1.10")
#app = testmoped.uploadplus(s, "PluginCreationTest3", "1.0")
print app

x = s.installApp("$VIN", app)
print x


EOF

for t in Vehicle VehiclePlugin VehicleConfig Ecu Port Link PluginPortConfig PluginLinkConfig AppConfig; do
    echo "*** $t *******"
    echo "use fresta2; select * from $t;" | $SQL
done
