# -*- coding: utf-8 -*-

import datetime
import string
import re
import getopt
import sys


#import argparse

#parser = argparse.ArgumentParser(description='Create empty class templates')
#parser.add_argument('--author', '-a', metavar='STRING', required=True,
                    #help='File author')

#args = parser.parse_args()

def main():
    author = ''
    className = ''
    package = ''

    try:
        opts, args = getopt.getopt(sys.argv[1:], "h:oacp", ["help", "operator", "author=", "package=", "class="])
    except getopt.GetoptError, err:
        # print help information and exit:
        print str(err) # will print something like "option -a not recognized"
        usage()
        sys.exit(2)
    output = None
    verbose = False
    operator = False;
    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            sys.exit()
        elif o in ("-o", "--operator"):
            operator = True
        elif o in ("-a", "--author"):
            author = a
        elif o in ("-p", "--package"):
            package = a
        elif o in ("-class", "--class"):
            className = a
        else:
            assert False, "unhandled option"
            usage()

    if operator:
        process('Operator.h.in', className + '.h', author, className, package)
        process('Operator.cpp.in', className + '.cpp', author, className, package)
        process('Operator.h.in', className + 'Test' + '.h', author, className, package)
        process('Operator.cpp.in', className + 'Test' + '.cpp', author, className, package)
    else:
        process('Class.h.in', className + '.h', author, className, package)
        process('Class.cpp.in', className + '.cpp', author, className, package)
        process('ClassTest.h.in', className + 'Test' + '.h', author, className, package)
        process('ClassTest.cpp.in', className + 'Test' + '.cpp', author, className, package)

def usage():
    print 'USAGE: python makeclass.py [--operator] --author="Sean Connery" --package="stream" --class="Operator"'

def replace(text, author, className, package):
    year = str(datetime.date.today().year)
    preprocessorMacro = package.upper() + '_' + className.upper() + '_H'
    preprocessorMacroTest = package.upper() + '_' + className.upper() + 'TEST_H'
    
    text = re.sub(r'@PREPROCESSOR_MACRO@', preprocessorMacro, text)
    text = re.sub(r'@PREPROCESSOR_MACRO_TEST@', preprocessorMacroTest, text)
    text = re.sub(r'@CLASS@', className, text)
    text = re.sub(r'@PACKAGE@', package, text)
    text = re.sub(r'@AUTHOR@', author, text)
    text = re.sub(r'@YEAR@', year, text)
    return text

def process(inFilePath, outFilePath, author, className, package):
    inFile = open(inFilePath, 'r')
    text = inFile.read()
    inFile.close()
    
    outFile = open(outFilePath, 'w')
    outFile.write(replace(text, author, className, package))
    outFile.close()

if __name__ == "__main__":
    main()
