set term pdfcairo font "Helvetica,22"
set out "performance.pdf"

f(x,p0,n) = x > p0? -32*log((1-x)/(1-p0))+n+1 : -32*log(x/p0)+n+1

set style fill transparent solid 0.25  border
set style function filledcurves y1=10
set samples 500
set xra [0.0:0.5]
set yra [0:10]

set xla "p"
set yla "N_r"
set xtics (0,1,1.0/2,1.0/4,3.0/4,1.0/8,3.0/8,5.0/8,7.0/8)
set ytics 1
set key above maxrows 2
set grid

set y2label "Elapsed Time [msec]" offset -2,0
set y2tics 200
set y2range [0:1000]

set style fill transparent solid 0.25  border
set style function filledcurves y1=10
p f(x,0,0)        lc 7 t ""\
, f(x,0,0)        lc 7 w l t "n=0"\
, f(x,1,0)        lc 7 t ""\
, f(x,1.0/2**1,1) lc 1 t "" \
, f(x,1.0/2**1,1) lc 1 w l t "n=1" \
, f(x,1.0/2**2,2) lc 2 t "" \
, f(x,1.0/2**2,2) lc 2 w l t "n=2" \
, f(x,3.0/2**2,2) lc 2 t "" \
, f(x,3.0/2**2,2) lc 2 w l t "" \
, f(x,1.0/2**3,3) lc 3 t "" \
, f(x,1.0/2**3,3) lc 3 w l t "n=3" \
, f(x,3.0/2**3,3) lc 3 t "" \
, f(x,3.0/2**3,3) lc 3 w l t "" \
, f(x,5.0/2**3,3) lc 3 t "" \
, f(x,5.0/2**3,3) lc 3 w l t "" \
, f(x,7.0/2**3,3) lc 3 t "" \
, f(x,7.0/2**3,3) lc 3 w l t "" \
, f(x,1.0/2**4,4) lc 4 t "" \
, f(x,1.0/2**4,4) lc 4 w l t "n=4" \
, f(x,3.0/2**4,4) lc 4 t "" \
, f(x,3.0/2**4,4) lc 4 w l t "" \
, f(x,5.0/2**4,4) lc 4 t "" \
, f(x,5.0/2**4,4) lc 4 w l t "" \
, f(x,7.0/2**4,4) lc 4 t "" \
, f(x,7.0/2**4,4) lc 4 w l t "" \
, f(x,9.0/2**4,4) lc 4 t "" \
, f(x,9.0/2**4,4) lc 4 w l t "" \
, f(x,11.0/2**4,4) lc 4 t "" \
, f(x,11.0/2**4,4) lc 4 w l t "" \
, f(x,13.0/2**4,4) lc 4 t "" \
, f(x,13.0/2**4,4) lc 4 w l t "" \
, f(x,15.0/2**4,4) lc 4 t "" \
, f(x,15.0/2**4,4) lc 4 w l t "" \
, "rbsgen0_0p.dat" t "" w l dt (5,5) lc 7 lw 3 axes x1y2\
, "rbsgen3_1n.dat" t "" w l dt (5,5) lc 3 lw 3 axes x1y2\
, "rbsgen3_1p.dat" t "" w l dt (5,5) lc 3 lw 3 axes x1y2\
, "rbsgen4_3n.dat" t "" w l dt (5,5) lc 4 lw 3 axes x1y2\
, "rbsgen4_3p.dat" t "" w l dt (5,5) lc 4 lw 3 axes x1y2\
, "rbsgen2_1n.dat" t "" w l dt (5,5) lc 2 lw 3 axes x1y2\
, "rbsgen2_1p.dat" t "" w l dt (5,5) lc 2 lw 3 axes x1y2\
, "rbsgen4_5n.dat" t "" w l dt (5,5) lc 4 lw 3 axes x1y2\
, "rbsgen4_5p.dat" t "" w l dt (5,5) lc 4 lw 3 axes x1y2\
, "rbsgen3_3n.dat" t "" w l dt (5,5) lc 3 lw 3 axes x1y2\
, "rbsgen3_3p.dat" t "" w l dt (5,5) lc 3 lw 3 axes x1y2\
, "rbsgen4_7n.dat" t "" w l dt (5,5) lc 4 lw 3 axes x1y2\
, "rbsgen4_7p.dat" t "" w l dt (5,5) lc 4 lw 3 axes x1y2\
, "rbsgen1_1n.dat" t "" w l dt (5,5) lc 1 lw 3 axes x1y2\
