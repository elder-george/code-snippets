@(set "script=%TMP%\%RANDOM%.sh") 
@ busybox tail -n +3 %0 > %script% & busybox sh %script% %* & exit/b
#!sh
whoami
        
for i in $(seq 1 2 20)
do
   echo "Welcome $i times"
done