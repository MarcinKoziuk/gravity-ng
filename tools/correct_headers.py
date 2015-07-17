#!/usr/bin/env python

import os

def yn_prompt(msg, path):
    if not yn_prompt.yes:
        if not path in yn_prompt.yesfiles:
           answ = raw_input("{} (y)es, (n)o, (f)ile, (a)ll".format(msg)).lower()
           if answ == 'y':
               return True
           if answ == 'f':
               yn_prompt.yesfiles.append(path)
               return True
           if answ == 'a':
               yn_prompt.yes = True
               return True
           return False
    return True

yn_prompt.yesfiles = []
yn_prompt.yes = False    

def guard_pattern(relfile):
    relfile = relfile.upper().replace('.', '_').replace('/', '_')
    return relfile

def correct_header_guard_line(path, relfile, lines, index, desired_guard):
    if lines[index] != desired_guard:
        msg = 'In: {}\n Should I fix this header guard: {}?\n With: {}' \
            .format(path, lines[index], desired_guard)
        do_correct = yn_prompt(msg, path) 
        if do_correct:
            lines[index] = desired_guard
            return False
    return True

def correct_header_guard(path, relfile):
    header_guard = guard_pattern(relfile)
    lines = []
    ifndef_index = None
    define_index = None
    endif_index = None

    with open(path, 'r') as headerfile:
        lines = headerfile.read().splitlines()

    for i, line in enumerate(lines):
        if ifndef_index == None:        
            if line.startswith('#ifndef') and line.find('_HPP'):
                ifndef_index = i
        if define_index == None:
            if line.startswith('#define') and line.find('_HPP'):
                define_index = i
        if line.startswith('#endif') and line.find('/*'):
            endif_index = i
    
    if ifndef_index == None and define_index == None and endif_index == None:
        return
    if not all([ifndef_index, define_index, endif_index]):
        print "Incomplete guards in {}?".format(relfile)
        return

    write = False

    header_guard_ifndef = '#ifndef {}'.format(header_guard)
    if not correct_header_guard_line(path, relfile, lines, ifndef_index, header_guard_ifndef):
        write = True

    header_guard_define = '#define {}'.format(header_guard)
    if not correct_header_guard_line(path, relfile, lines, define_index, header_guard_define):
        write = True
                    
    header_guard_endif = '#endif /* {} */'.format(header_guard)
    if not correct_header_guard_line(path, relfile, lines, endif_index, header_guard_endif):
        write = True

    if write:
        print "Correcting {}!".format(relfile);
        with open(path, 'w') as headerfile:
            for item in lines:
                headerfile.write("{}\n".format(item))
    else:
        #print "{} looks ok! :)".format(relfile);
        pass

def correct_filename_comment(path, relfile):
    lines = []

    with open(path, 'r') as sourcefile:
        lines = sourcefile.read().splitlines()

    if len(lines) <= 1:
        return

    filename_index = None
    for i, line in enumerate(lines):
        if line.startswith(' * ') and i == 1:
            filename_index = i
            break

    if filename_index == None:
        print "No filename in {}?".format(relfile)        
        return

    write = False
    desired_line = ' * {}'.format(relfile)
    if lines[filename_index] != desired_line:
        msg = 'In: {}\n Should I fix this filename comment: {}?\n With: {}' \
            .format(path, lines[filename_index], desired_line)
        do_correct = yn_prompt(msg, path) 
        if do_correct:
            lines[filename_index] = desired_line
            write = True

    if write:
        print "Correcting {}!".format(relfile);
        with open(path, 'w') as sourcefile:
            for item in lines:
                sourcefile.write("{}\n".format(item))
    else:
        #print "{} looks ok! :)".format(relfile);
        pass

def correct_header_guards_in_directory(dir):
    for subdir, dirs, files in os.walk(dir):
        for file in files:
            ext = os.path.splitext(file)
            path = os.path.join(subdir, file)
            
            if ext[1] == '.hpp' or ext[1] == '.inl':
                relfile = os.path.relpath(path, dir).replace('\\', '/')
                correct_header_guard(path, relfile)

def correct_filename_comments_in_directory(dir):
    for subdir, dirs, files in os.walk(dir):
        for file in files:
            ext = os.path.splitext(file)
            path = os.path.join(subdir, file)

            if ext[1] == '.hpp' or ext[1] == '.cpp' or ext[1] == '.inl':
                relfile = os.path.relpath(path, dir).replace('\\', '/')
                correct_filename_comment(path, relfile)

correct_header_guards_in_directory('../include')
correct_filename_comments_in_directory('../include/gravity')
correct_filename_comments_in_directory('../src')

#for subdir, dirs, files in os.walk(rootdir):
#    for file in files:
#        ext = os.path.splitext(file)
#        
#
#        print subdir+'/'+file
