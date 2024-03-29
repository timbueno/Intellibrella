import re

def readFileData(file):
    f = open(file, 'r')
    userList = f.readlines()
    f.close()
    userDict = {}
    for line in userList:
        if line[0] != '#':
            x = re.search(r'([\w\@\.]+)\s*:\s*([\w\@\.\/]+)', line)
            if x != None:
                userDict[x.group(1)] = x.group(2)
    return userDict
