id=`pidof quant3`
if test -n $id
then
kill -9 $id
echo "kill quant3"
fi
echo "start"
nohup ./quant3 -k 192.168.1.74:9092 -t east_wealth_test &
