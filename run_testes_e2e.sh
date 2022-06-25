SERVER=./server
EXEC=./client
TMP_OUT_C=test-e2e-c.out
TMP_OUT_S=test-e2e-s.out

echo "Parte 1: Testes no protocolo IPv4"
for i in 0 1 2 3 4; do
  testname=$(printf "%02d" $i)
  $SERVER v4 51511 > $TMP_OUT_S & $EXEC 127.0.0.1 51511 < testes-e2e/$testname.in > $TMP_OUT_C
  if ! diff -qwB testes-e2e/$testname.out $TMP_OUT_C &>/dev/null; then
    echo "   Test $testname failed"
  else
    echo "   Test $testname passed"
  fi
done

echo "Parte 2: Testes no protocolo IPv6"
for i in 0 1 2 3 4; do
  testname=$(printf "%02d" $i)
  $SERVER v6 51511 > $TMP_OUT_S & $EXEC ::1 51511 < testes-e2e/$testname.in > $TMP_OUT_C
  if ! diff -qwB testes-e2e/$testname.out $TMP_OUT_C &>/dev/null; then
    echo "   Test $testname failed"
  else
    echo "   Test $testname passed"
  fi
done

rm $TMP_OUT_C
rm $TMP_OUT_S