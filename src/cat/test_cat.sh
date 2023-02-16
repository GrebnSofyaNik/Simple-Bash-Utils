#!/bin/bash

COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
TEST_FILE="bytes.txt"
TEST_FILE2="test.txt"

for var in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
do
TEST1="$var $TEST_FILE"
./s21_cat $TEST1 > s21_cat.txt
cat $TEST1 > cat.txt
DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
then
(( COUNTER_SUCCESS++ ))
else
echo "$TEST1 FAIL"
(( COUNTER_FAIL++ ))
fi
rm s21_cat.txt cat.txt
done

for var in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
do
for var2 in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
do
if [ $var != $var2 ]
then
TEST1="$var $var2 $TEST_FILE"
./s21_cat $TEST1 > s21_cat.txt
cat $TEST1 > cat.txt
DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
then
(( COUNTER_SUCCESS++ ))
else
echo "$TEST1 FAIL"
(( COUNTER_FAIL++ ))
fi
rm s21_cat.txt cat.txt
fi
done
done

for var in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
do
TEST1="$var $TEST_FILE2"
./s21_cat $TEST1 > s21_cat2.txt
cat $TEST1 > cat2.txt
DIFF_RES="$(diff -s s21_cat2.txt cat2.txt)"
if [ "$DIFF_RES" == "Files s21_cat2.txt and cat2.txt are identical" ]
then
(( COUNTER_SUCCESS++ ))
else
echo "$TEST1 FAIL"
(( COUNTER_FAIL++ ))
fi
rm s21_cat2.txt cat2.txt
done

for var in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
do
for var2 in -b -e -n -s -t -v 
do
if [ $var != $var2 ]
then
TEST1="$var $var2 $TEST_FILE2"
./s21_cat $TEST1 > s21_cat2.txt
cat $TEST1 > cat2.txt
DIFF_RES="$(diff -s s21_cat2.txt cat2.txt)"
if [ "$DIFF_RES" == "Files s21_cat2.txt and cat2.txt are identical" ]
then
(( COUNTER_SUCCESS++ ))
else
echo "$TEST1 FAIL"
(( COUNTER_FAIL++ ))
fi
rm s21_cat2.txt cat2.txt
fi
done
done
​
for var in -b -e -n -s -t -v
do
for var2 in -b -e -n -s -t -v
do
if [ $var != $var2 ]
then
TEST1="$var $var2 $TEST_FILE $TEST_FILE2"
./s21_cat $TEST1 > s21_cat.txt
cat $TEST1 > cat.txt
DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
then
(( COUNTER_SUCCESS++ ))
else
echo "$TEST1 FAIL"
(( COUNTER_FAIL++ ))
fi
rm s21_cat.txt cat.txt
fi
done
done
TEST1="$TEST_FILE2"
./s21_cat -b -e -n -s -t -v $TEST1 > s21_cat3.txt
cat -b -e -n -s -t -v $TEST1 > cat3.txt
DIFF_RES="$(diff -s s21_cat3.txt cat3.txt)"
if [ "$DIFF_RES" == "Files s21_cat3.txt and cat3.txt are identical" ]
then
(( COUNTER_SUCCESS++ ))
else
echo "$TEST1 FAIL"
(( COUNTER_FAIL++ ))
fi
rm s21_cat3.txt cat3.txt

TEST1="$TEST_FILE"
./s21_cat -b -e -n -s -t -v $TEST1 > s21_cat3.txt
cat -b -e -n -s -t -v $TEST1 > cat3.txt
DIFF_RES="$(diff -s s21_cat3.txt cat3.txt)"
if [ "$DIFF_RES" == "Files s21_cat3.txt and cat3.txt are identical" ]
then
(( COUNTER_SUCCESS++ ))
else
echo "$TEST1 FAIL"
(( COUNTER_FAIL++ ))
fi
rm s21_cat3.txt cat3.txt

echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"