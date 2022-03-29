#!/bin/bash

rm -f /tmp/length.dat
rm -f /tmp/max.dat
rm -f /tmp/hist.dat #attended Eugene's section; previously had > /tmp/...
make clean && make collatz
for ((i=2; i <10001; i++));
do
        echo -n $i " " >> /tmp/length.dat #found that " " after echo added a space
        ./collatz -n $i | wc -l >> /tmp/length.dat #used manpages for wc line number for wc
	echo -n $i " " >> /tmp/max.dat
        ./collatz -n $i | sort -n | tail -n 1 >> /tmp/max.dat #attended Eugene's section where we discussed tail and head method
	./collatz -n $i | wc -l >> /tmp/hist.dat
done
sort -n /tmp/hist.dat | uniq -c | tee /tmp/hist.dat #used lecture 2 to learn abotu tee command and also used manpages
gnuplot <<END
        set terminal pdf
        set output "collatz.pdf"
        set title "Collatz Sequence Lengths"
        set xlabel "n"
        set ylabel "length"
        set zeroaxis
        plot "/tmp/length.dat" with dots title ""
END
#attended Eugene's section where he went over how to set range
gnuplot <<END
        set terminal pdf
        set output "max_sequence.pdf"
        set title "Maximum Collatz Sequence Value"
        set xlabel "n"
        set ylabel "value"
        set zeroaxis
	set yrange[0:100000]
        plot "/tmp/max.dat" with dots title ""
END
#Eugene explained that using 2:1 switched coords, but didn;t work with hisrograms so used boxes from pg.64 of gnu documentation
gnuplot <<END
        set terminal pdf
        set output "hist_lengths.pdf"
        set title "Maximum Collatz Length Histogram"
        set xlabel "n"
        set ylabel "frequency"
        set zeroaxis
        set yrange[0:200]
	set xrange[0:225]
	set boxwidth .1
	plot "/tmp/hist.dat" using 2:1  with boxes title ""
END

