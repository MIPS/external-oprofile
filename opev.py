#! /usr/bin/env python
"""
Read oprofile events file, generate C data struct for Android opcontrol.

Android does not use script for opcontrol, they use a C binary, which
has embedded data structures with the event set that is supported. 
Initially that is just Arm V6 and V7.

This tool allows us to convert various MIPS cpu event files for
inclusion, and should work with other processor arch's as well.

Neither Arm or Mips uses unit_masks, so that file is ignored.

Event entries in file look like this:

    event:0x1 counters:0,1 um:zero minimum:500 name:INSTRUCTIONS : Instructions completed

Note that first 5 items are name:value, then 6th item is multiword description
all this junk made into a list of dictionaries.

"""

import os, sys


def parse_event(line,ovf):
    ''' return dictionary of items from one line of event file '''
    fields = line.split(None, 5)                            # first 5 items are regular
    # Check for file include
    first = fields[0].split(':', 1)
    if first[0] == 'include':
        ev(first[1] + "/events", ovf)
        return None
    fields[5] = 'description:' +  fields[5].lstrip(': ')    # fixup descr as 6th item
    flist = [field.split(':', 1) for field in fields]
    return dict(flist)

def parse_ctr(s):
    ''' convert comma separated list of integers x,y,... , to CTR(x) | CTR(y) | ... '''
    ctrs = s.split(',')
    c = ''
    for i in range(len(ctrs)-1):
        c += ("CTR(%s) | " % ctrs[i])
    c += ("CTR(%s)" % ctrs[-1])
    return c

def ev(fname,ovf):
    ''' read file, parse, generate C data struct to std out '''
    evf = open(fname, "r")
    all_lines = evf.readlines()
    lines = [s.strip() for s in all_lines if s.strip()]     # strip blanks
    lines = [s for s in lines if not s.startswith('#')]     # strip comments
    eventlist = [parse_event(line,ovf) for line in lines]

    ovf.write("// events from file %s\n" % fname)
    for d in eventlist:
        if d!=None:
            ovf.write('    {%s, %s, "%s",\n' % (d['event'], parse_ctr(d['counters']), d['name']))
            ovf.write('     "%s"},\n' % d['description'])


if __name__ == "__main__" :
    if len(sys.argv) != 2:
        fname = "events/mips/24K/events"    # convenient testing
    else:
        fname = sys.argv[1]
    ovf = open(fname + ".h", "w")
    ev(fname, ovf)
