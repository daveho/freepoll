#! /usr/bin/env ruby

# Fix automatic header dependencies output by g++ -M.
# Specifically, change object file targets so that they are
# in the "build" directory.

STDIN.each_line do |line|
  if /^\S/.match(line)
    line = "build/#{line}"
  end
  print line
end
