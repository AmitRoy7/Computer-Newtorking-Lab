mail_client(){  
parameter1=$1;
username=""
hostname=""
portname=""
subject=$2
filename=$3
username=$(echo $parameter1| cut -d'@' -f1)
hostname=$(echo $parameter1| cut -d'@' -f2)
hostname=$(echo $hostname  | cut -d':' -f1)
portname=$(echo $parameter1| cut -d':' -f2)


subject=""
m=$#

if [ $m -lt 3 ]
then
	echo "Incorrect number of arguments"
	return;
fi

for ((i=2;i<m;i++)) ;
do	
	if [ $i -ne 2 ]; 
	then subject+=" "; 
	fi; 
	subject+=${!i} ; 
done;

filename=${@:${#@}};



if ! [ -f "$filename" ]
then
	echo "Incorrect file name"
	echo "$filename not found."
	return;
fi

#filename=$(readlink -f $filename)
#echo $filename;
data=$(cat $filename)

#echo $username
#echo $hostname
#echo $portname
#echo $subject
#echo $filename
g++ -o Client Client.cpp
./Client $hostname $portname $username $subject $filename

#netcat amit-roy 20001 < /home/amit-roy/sample.txt
#netcat $hostname $portname < $(readlink -f foo.txt)
#telnet $hostname $portname
#mail_client bob@wagner.cse.univdhaka.edu:2222 “This is a test” mail.txt
}

