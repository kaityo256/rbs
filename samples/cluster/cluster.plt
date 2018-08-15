set term postscript eps enhanced color 20
set out "cluster.eps"

set xla "t"
set yla "n"
set ytics (1,10,50)
set yra [1:50]
set key on left Left
set log xy
set style data line
p x**0.313686 t "t^{/Symbol Q}" lw 3 lc 0 \
,"serial.dat" lw 2 t "Scalar"\
, "msc.dat"   lw 2 t "Multi-spin coding" \

!ps2pdf cluster.eps

