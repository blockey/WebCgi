#First make the cgi project
make

#Tar the cigs.
tar -zcvf cpe_cgi.tar.gz *.cgi

#Copy to the tftp directory
cp cpe_cgi.tar.gz /mnt/hgfs/tftp/


