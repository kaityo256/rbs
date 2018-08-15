$EPS = 1e-14

def f(x, p0, n)
  x > p0 ? -32 * Math.log((1 - x) / (1 - p0)) + n + 1 : -32 * Math.log(x / p0) + n + 1
end

def search(s, e, p1, p2)
  while true
    x = (s + e) * 0.5
    diff = p1.call(x) - p2.call(x)
    if diff.abs < $EPS
      y = p1.call(x)
      return x, y
    end
    if diff > 0.0
      e = x
    else
      s = x
    end
  end
end

def show(x1, x2, n1, n2, a)
  a.push [x1, n1 + 1]
  #puts "#{x1} #{n1 + 1}"
  p1 = lambda { |x| f(x, x1, n1) }
  p2 = lambda { |x| f(x, x2, n2) }
  search(x1, x2, p1, p2, a)
end

def show2(s1, s2, a)
  d1, n1 = s1
  d2, n2 = s2
  x1 = n1.to_f / (1 << d1)
  x2 = n2.to_f / (1 << d2)
  p1 = lambda { |x| f(x, x1, d1) }
  p2 = lambda { |x| f(x, x2, d2) }
  x, y = search(x1, x2, p1, p2)
  if a == []
    puts "      if (p <  #{x}){"
    puts "        rbs = new rbsgen#{d1}_#{n1}p<uint32_t>(p);"
  else
    puts "      } else if (p < #{n1.to_f} / (1 << #{d1})) {"
    puts "        rbs = new rbsgen#{d1}_#{n1}n<uint32_t>(p);"
    puts "      } else if (p < #{x}) {"
    puts "        rbs = new rbsgen#{d1}_#{n1}p<uint32_t>(p);"
  end
  a.push [x1, d1 + 1]
  a.push [x, y]
end

b = [[0, 0], [3, 1], [4, 3], [2, 1], [4, 5], [3, 3], [4, 7], [1, 1], [4, 9], [3, 5], [4, 11], [2, 3], [4, 13], [3, 7], [0, 1]]

a = []

(b.size - 1).times do |i|
  show2(b[i], b[i + 1], a)
end
a.push [1.0, 1]

puts "      } else{"
puts "        rbs = new rbsgen0_1n<uint32_t>(p);"
puts "      }"

open("points.dat", "w") do |f|
  a.each do |x, y|
    f.puts "#{x} #{y}"
  end
end

t = []
(a.size - 1).times do |i|
  x1 = a[i][0]
  x2 = a[i + 1][0]
  t.push sprintf("%.4f", ((x1 + x2) * 0.5))
end
puts "const double test_points[] = {"
puts t.join(",") + "};"
