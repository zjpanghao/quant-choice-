#! /bin/bash
echo "ok"
while (true)
do
hour=`date +%H`
min=`date +%M`

if [ $hour -eq 17 ] && [ $min -eq 0 ] 
then
id=`pidof quant`
echo $id
if test -z $id
then
 echo "start quant"
 ./start.sh 
fi
fi

if [ $hour -eq 16 ] && [ $min -eq 0 ] 
then
echo $id
id=`pidof quant`
echo $id
if test -n $id
then
 echo "close quant"
 kill -9 $id
fi
fi
sleep 20
done
