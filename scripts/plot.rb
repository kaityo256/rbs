puts "p f(x,0,0)        lc 7 t \"\"\\"
puts ", f(x,0,0)        lc 7 w l t \"n=0\"\\"

n = 4
(1..n).each do |i|
  k = 0
  while (2 * k + 1).to_f / 2 ** i <= 1.1
    puts ", f(x,#{2 * k + 1}.0/2**#{i},#{i}) lc #{i} t \"\" \\"
    if k == 0 && n - i < 5
      puts ", f(x,#{2 * k + 1}.0/2**#{i},#{i}) lc #{i} w l t \"n=#{i}\" \\"
    else
      puts ", f(x,#{2 * k + 1}.0/2**#{i},#{i}) lc #{i} w l t \"\" \\"
    end
    k = k + 1
  end
end
