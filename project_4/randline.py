#!/usr /bin/python                                                      #Tells the shell which interpreter to use

import random, sys                                                      #Import statements, similar to include statements
from optparse import OptionParser                                       #Import OptionParser class from optparse module

class randline:                                                         #The beginning of the class definition; randline
    def __init__(self, filename):                                       #The constructor
        f = open(filename, 'r')                                         #Creates a file handle
        self.lines = f.readlines()                                      #Reads the file into a list of strings called lines
        f.close()                                                       #Close the file

    def chooseLine(self):                                               #The beginning of a function belonging to randline
        return random.choice(self.lines)                                #Randomly select a number between 0 and
                                                                        #the size of the lines. Return the line corresponding
                                                                        #to the randomly selected number

def main():                                                             #The beginning of the main function
    version_msg = "%prog 2.0"                                           #Version message and Usage message
    usage_msg = "%prog [OPTION] FILE Output randomly selected lines from FILE."

parser = OptionParser (version_msg, usage= usage_msg)                   #Create OptionParser Instance

parser.add_option("-n", "--numlines", action="store", dest="numlines",  #Start defining options. Action "store" tells optparse
    default=1, help="output NUMLINES lines (default 1)"                 #to take next argument and store to the right destination
                                                                        #which is "numlines". Set the default value of "numlines"
                                                                        #to 1 and help message

options, args = parser.parse_args(sys.argv[1:])                         #Options: an object containing all options arg
                                                                        #Args: List of positional args leftover after parsing

try:                                                                    #Try Block
    numlines = int(options.numlines)                                    #Get numline from options and convert to int
except:                                                                 #Exception Handling
    parser.error("invalid NUMLINES: {0}".format( options.numlines ))    #Error message if not int type, replace {0} with input
                

if numlines < 0:                                                        #If numlines is negative, error messsage
   parser.error("negative count: {0}".format( numlines ))

if len(args) != 1:                                                      #If length of args is not 1, then error message
    parser.error("wrong number of operands")

input_file = args [0]                                                   #Assign first and only arg to variable input_file

try:                                                                    #Try Block
    generator = randline(input_file)                                    #Instantiate randline object with input_file
    for index in range(numlines):                                       #For loop from 0 to numlines - 1
        sys.stdout.write(generator.chooseline())                        #Print randomly chosen line
except IOError as (errno, strerror):                                    #Exception handling
    parser.error("I/O error({0}): {1}".                                 #Error message in the format "I/O error (errno):strerror"
        format( errno , strerror ))

if __name__ == "__main__":                                              #Make the Python file a standalone program
    main()