#!/usr/bin/env ruby

def main
  walls = ARGV.map(&:to_i)

  peak = walls.max

  water = 0

  peak.times do |current_height|
    current_level = walls.map { |wall| wall > current_height ? 1 : 0 }

    left_boundary = current_level.index(1)
    right_boundary = current_level.rindex(1)

    water += current_level[left_boundary..right_boundary].count(0)
  end

  puts water
end

main
