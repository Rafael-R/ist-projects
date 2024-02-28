for i in a b c d e test
do 
    ./src/client/client /tmp/$i.req /tmp/$i.resp /tmp/server tests/base/$i.jobs &
done

for i in 1 2 3 4 5 6 7 16 21 23 25
do 
    ./src/client/client /tmp/$i.req /tmp/$i.resp /tmp/server tests/public/$i.jobs &
done