set datafile separator ','
set ydata time
set timefmt "%s" # specify our time string format
set format y "%f" # otherwise it will show only MM:SS


set ylabel "Tempo" # label for the Y axis
set xlabel 'Quantidade de casos' # label for the X axis

set title "Versão 1"
set terminal svg
set output 'v1.svg'
plot 'versao1.2.csv' using 2:5 with lines lw 3 title 'Saida padrão' ,'versao1_Com_2.csv' using 2:5 with lines lw 3 title "Saida comentada",'versao1_Null_2.csv' using 2:5 with lines lw 3 title "Saida /dev/null"