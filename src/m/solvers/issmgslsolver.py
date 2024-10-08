from pairoptions import pairoptions
from collections import OrderedDict


def issmgslsolver(*args):
    #ISSMSOLVE - return issm solver options
    #
    #   Usage:
    #      options = issmsolver

    #retrieve options provided in *args
    arguments = pairoptions(*args)

    options = OrderedDict()
    options['toolkit'] = 'issm'
    options['mat_type'] = 'dense'
    options['vec_type'] = 'seq'
    options['solver_type'] = 'gsl'

    #now, go through our arguments, and write over default options.
    for i in range(len(arguments.list)):
        arg1 = arguments.list[i][0]
        arg2 = arguments.list[i][1]
        found = 0
        for j in range(len(options)):
            joption = options[j][0]
            if joption == arg1:
                joption[1] = arg2
                options[j] = joption
                found = 1
                break
        if not found:
            #this option did not exist, add it:
            options.append([arg1, arg2])
    return options
