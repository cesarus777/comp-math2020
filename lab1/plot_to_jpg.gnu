set terminal jpeg size xsize, ysize
set output ofilename.".jpg"
set xrange [1:xrange_lim]
set xlabel "n"
set ylabel "error"
plot for[col=2:6] filename using 1:col title columnheader(col) with lines

