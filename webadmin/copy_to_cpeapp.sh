echo "cp cgi to cpeapp!"
cp *.cgi /home/global/cpeapp/app/webapp/httpd/cgi-bin/webadmin/
echo "mkapp"
cd /home/global/cpeapp/
./mkapp.sh
cd /mnt/hgfs/pc_share/cpe_web/httpd/cgi-bin/webadmin/

