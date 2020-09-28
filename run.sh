gcc "$1" -o ./out/"$1".out

params=""
for N in "$@"
do
  if [  "$N" != "$1" ]; then
    params="${params} $N"
  fi
done

./out/$1.out  ${params}