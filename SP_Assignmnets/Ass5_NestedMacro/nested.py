d_flag = 0
e_flag = 0
code = open("a.txt",'r')
out = open("out.txt",'w')
mnt = []
mbt = []
stack = []
key_words = ['load','store','add','sub','mult','div','END']

def remove_char(s , l):
    count = 0
    for i in l:
        if i == s:
            count = count + 1
    for i in range(count):
        l.remove(s)
    return l        

def remove_newline(l):
    length = len(l) - 1
    s = l[length] 
    l[length] = s[0:len(s) - 1]
    return l
    
def add_newline(l):
    length = len(l) - 1
    s = l[length] 
    l[length] = s + '\n'
    return l

def replace_char(param , l):
    keys = list(param.keys())
    for i in range(len(l)):
        if l[i] in keys:
            l[i] = param[l[i]]
    return l

def replace_char_2(l1 , l2):
    c= 0
    for i in range(len(l2)):
        if l2[i][0] == '#':
            c = l2[i][1:]
            c = int(c)
            l2[i] = l1[c]
    return l2

def add_output(l , outpt):
    l = add_newline(l)
    l = " ".join(l)
    outpt.writelines((l))
    return

def get_mbt_index(l):
    for i in range(len(mnt)):
        if(mnt[i][0] == l[0]) and (mnt[i][1] == len(l) -1):
            ind = mnt[i][2]
            return ind

def nested_expand(l , ef , df):
    l = remove_char(',',l)
    ind = get_mbt_index(l)
    while ef == 1 and df == 0:
        l1 = mbt[ind].copy()
        ind = ind + 1
        if l1[0] == 'MEND':
            ef = 0
            return
        if l1[0] not in key_words:
            l1 = replace_char_2(l , l1)
            nested_expand(l1 , ef , df)
        else:
            l1 = replace_char_2(l , l1)
            add_output(l1,out)
    return

while True:
    line = code.readline()
    if not line:
        break
    line = line.split(" ")
    line = remove_newline(line)
    line = remove_char("" , line)
    if line[0] == "MACRO":
        d_flag = 1
        line = remove_char("," , line)
        paramenter = {}
        mnt_entry = []
        mnt_entry.append(line[1])
        mnt_entry.append(len(line) - 2)
        mnt_entry.append(len(mbt))
        mnt.append(mnt_entry)
        count = 1
        for i in range(2,len(line)):
            paramenter[line[i]] = '#' + str(count)
            count = count + 1
    if (d_flag == 1 and e_flag == 0) and line[0] != 'MACRO':
        line = replace_char(paramenter , line)
        mbt.append(line)
    if (d_flag == 1 and e_flag == 0) and line[0] == 'MEND':
        d_flag = 0
        del paramenter
        continue
    if d_flag == 0 and e_flag == 0:
        if line[0] not in key_words:
            e_flag = 1
            line = remove_char("," , line)
            index = get_mbt_index(line)
            while e_flag == 1 and d_flag == 0:
                line1 = mbt[index].copy()
                index = index + 1
                if line1[0] == 'MEND':
                    e_flag = 0
                    break
                if line1[0] not in key_words:
                    line1 = replace_char_2(line , line1)
                    nested_expand(line1 , e_flag , d_flag)
                else:
                    line1 = replace_char_2(line , line1)
                    add_output(line1,out)
            continue
        add_output(line , out)