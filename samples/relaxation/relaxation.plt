set term post eps enhanced color 24
set out "relaxation.eps"

set xla "t"
set yla "{/Symbol r}" offset 2,0
set key on left bottom Left 
set log xy
set style data line
p 1.0*x**-0.159464 t "t^{-{/Symbol a}}" lw 3 lc 0 \
,"serial.dat" lw 2 t "Scalar"\
, "msc.dat"      lw 2 t "Multi-spin coding" \

!ps2pdf relaxation.eps
