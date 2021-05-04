set datafile separator ','
set ydata time
set timefmt "%s" # specify our time string format
set format y "%f" # otherwise it will show only MM:SS


set ylabel "Tempo" # label for the Y axis
set xlabel 'Quantidade de casos' # label for the X axis


set terminal svg
set output 'semThread.svg'
plot './v1/versao1.2.csv' using 2:5 with lines lw 3 title "Naive",'./v3/versao3.2.csv' using 2:5 with lines lw 3 title "Memorização sem otimização",'./v4/versao4.2.csv' using 2:5 with lines lw 3 title "Memorização com otimização"