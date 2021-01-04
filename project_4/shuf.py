#!/usr/bin/python

import random
import sys
import argparse

class randline:
    def __init__(self, filename, file_status):
        self.random = None

        if file_status == True:
            f = open(filename, 'r')
            self.lines = f.readlines()
            f.close()
        else:
            self.lines = sys.stdin.readlines()

    def writePermutation(self, head_count, repeat):
        head_count_present = True
        if head_count == None:
            head_count_present = False
            
        if not repeat:
            if head_count == None:
                head_count = len(self.lines)
            elif head_count > len(self.lines):
                head_count = len(self.lines)
                
            self.random = random.sample(self.lines, head_count)
            for i in range(head_count):
                print(self.random[i].rstrip())

            return
        else:  # if repeat == true

            if len(self.lines) == 0:
                print("no lines to repeat", file=sys.stderr)
                exit(1)
            
            if head_count_present:
                while head_count > 0:
                    print(random.choice(self.lines).rstrip())
                    head_count -= 1
            else:
                while True:
                    self.random = random.sample(self.lines, len(self.lines))
                    for i in range(len(self.lines)):
                        print(self.random[i].rstrip())
                

def is_int(input_string):
    try:
        int(input_string)
        return True
    except:
        return False


def valid_range(splitstring, dashcount):
    if dashcount != 1 or len(splitstring) != 2 or not is_int(splitstring[0]) \
       or not is_int(splitstring[1]) or int(splitstring[0]) < 0 \
       or int(splitstring[0]) > int(splitstring[1]):
        return False
    else:
        return True
    

def print_range(input_string, head_count, repeat):
    #count the number of dashes
    dash_count = 0
    for i in input_string:
        if i == "-":
            dash_count += 1
       
    splitstring = input_string.split("-")

    # generate a list from the range
    list_to_permute = []
    if valid_range(splitstring, dash_count):
        for i in range(int(splitstring[0]), int(splitstring[1])+1):
            list_to_permute.append(i)
    else:
        print("error: input range must be LO-HI such that 0<=LO<=HI", file=sys.stderr)
        exit(1)

    head_count_present = True
    if head_count == None:
        head_count_present = False

    if not repeat:
        permuted_list = random.sample(list_to_permute, len(list_to_permute))

        if head_count == None:
            head_count = len(list_to_permute)
        elif head_count > len(list_to_permute):
            head_count = len(list_to_permute)

        for i in range(head_count):
            print(permuted_list[i])

    else:
        if head_count_present:
            while head_count > 0:
                print(random.choice(list_to_permute))
                head_count -= 1
        else:
            while True:
                permuted_list = random.sample(list_to_permute, len(list_to_permute))
                for  i in range(len(permuted_list)):
                    print(permuted_list[i])


if __name__ == '__main__':

    usage_msg = """%(prog)s [OPTION]... [FILE]

Write a random permutation of the input lines to standard output.
With no FILE, or when FILE is -, read standard input."""

    
    parser = argparse.ArgumentParser(usage=usage_msg)

    group = parser.add_mutually_exclusive_group()
    
    group.add_argument("--input-range", "-i",
                       help="treat each number LO through HI as an input line",
                       dest="input_range")
    parser.add_argument("--head-count", "-n", help="output at most COUNT lines",
                        dest="head_count", type=int)
    parser.add_argument("--repeat", "-r", help="output lines can be repeated",
                        action="store_true")
    group.add_argument("file", nargs="?")

    args = parser.parse_args()

    if args.head_count != None and args.head_count == 0:
        exit(0)
    elif args.head_count != None and args.head_count < 0:
        print("invalid line count", file=sys.stderr)
        exit(1)
        
    if args.input_range != None:
        print_range(args.input_range, args.head_count, args.repeat)
    else:
        if args.file == None:
            args.file = "-"
            
        if args.file == "-":
            is_file = False
        else:
            is_file = True
            
        generator = randline(args.file, is_file)
        generator.writePermutation(args.head_count, args.repeat)
