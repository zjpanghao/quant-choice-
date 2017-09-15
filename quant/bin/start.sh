id=`pidof quant`
if test -n $id
then
kill -9 $id
echo "kill quant"
fi
echo "start"
nohup ./quant -k 192.168.1.74:9092 -t east_wealth &
