id=`pidof quant3_2`
if test -n $id
then
kill -9 $id
echo "kill quant3_2"
fi
echo "start"
nohup ./quant3_2 &
