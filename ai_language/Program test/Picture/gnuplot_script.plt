set terminal pngcairo size 1000,600 enhanced font 'Verdana,10'
set output 'Program test/Picture/learning_curves.png'
set multiplot layout 1,2 title 'Learning Curves'
set grid
set title 'Loss vs Epochs'
set xlabel 'Epochs'
set ylabel 'Loss'
plot 'Program test/Picture/gnuplot_data.dat' using 1:2 with lines lw 2 lt rgb 'red' title 'Loss'
set title 'Accuracy vs Epochs'
set xlabel 'Epochs'
set ylabel 'Accuracy'
plot 'Program test/Picture/gnuplot_data.dat' using 1:3 with lines lw 2 lt rgb 'blue' title 'Accuracy'
unset multiplot
