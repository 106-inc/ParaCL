#!/usr/bin/env python3

import argparse
import os

##################################################################
code_ext = ['hpp', 'cpp', 'c', 'h', 'hh', 'cc', 'c++', 'cxx']


cmd = 'clang-format -i --style="{BasedOnStyle: Microsoft, IndentWidth: 2}" '
##################################################################


def run_format(files_string, is_print, is_int):
    flist = files_string.split()
    check_list_ftype(flist)

    for elem in flist:
        is_form = True
        if is_int:
            print(f'\033[33mFormat "{elem}"? (Y/N): ', end='')
            answer = input()
            is_form = answer[0].upper() in ['Y', 'Д']

        if is_form:
            os.system(cmd + elem)
            if is_print:
                print(f'\033[32mFormatted: {elem}')

        elif is_print:
            print(f'\033[31mInterrupted fromatting {elem}.')


def check_list_ftype(flist):
    if type(flist) != list:
        return False

    for name in flist:
        if not check_ftype(name):
            return False

    return True


def check_ftype(filename):
    #filename = filename.split('/')
    #filename = filename[len(filename) - 1]

    if '.' not in filename:
        return False

    splatted = filename.split('.')
    return splatted[len(splatted) - 1] in code_ext


def check_fdir(lst_dir, dname):
    flist = ''

    for elem in lst_dir:
        new_dname = dname + '/' + elem

        if os.path.isdir(new_dname):
            flist += check_fdir(os.listdir(new_dname), new_dname)
            continue
        if check_ftype(elem):
            flist += new_dname + ' '

    return flist


def main():
    parser = argparse.ArgumentParser(description='Clang-format code formatting script.\nBy derzhavin3016.')
    group = parser.add_mutually_exclusive_group()

    parser.add_argument('--loc', '-l',  metavar='PATH', type=str, default='.', help='Start location (file or dir)')
    group.add_argument('--quiet', '-q', action='store_false', help='Quiet mode (no print)')
    group.add_argument('--interactive', '-i', action='store_true', help='Interactive mode')

    args = parser.parse_args()

    if os.path.isfile(args.loc):
        run_format(args.loc, args.quiet, args.interactive)
        return

    elif os.path.isdir(args.loc):
        ldir = os.listdir(args.loc)
        run_format(check_fdir(ldir, args.loc), args.quiet, args.interactive)
        return

    print(f'Strange path: {args.loc}')


if __name__ == '__main__':
    main()
