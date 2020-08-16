ftp -n <<EOF
open 192.168.178.32 1337
user user secret
cd ux0:
cd tai
put vitaUsbtty.skprx vitaUsbtty.skprx
close
quit
EOF
