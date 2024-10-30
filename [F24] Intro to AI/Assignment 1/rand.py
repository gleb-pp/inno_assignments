import subprocess
from time import sleep
from random import randint
import pyperclip as pc

process = subprocess.Popen(['./code.exe'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

info = []
for i in range(9):
    info.append([""] * 9)

def check(x, y):
    a = 0
    res = []
    if info[x][y] != "":
        a += 1
        res.append(info[x][y])
    for dx in range(-1, 2):
        for dy in range(-1, 2):
            if (0 <= x+dx <= 8 and 0 <= y+dy <= 8 and info[x+dx][y+dy] == 'A'):
                a += 1
                res.append("P")
                return [a, res]
    if (keyactivated):
        for dx in range(-1, 2):
            for dy in range(-1, 2):
                if (0 <= x+dx <= 8 and 0 <= y+dy <= 8 and info[x+dx][y+dy] == 'S'):
                    a += 1
                    res.append("P")
                    return [a, res]
        i = 2
    else:
        i = 1
    if (0 <= x-i <= 8 and info[x-i][y] == 'S' or 0 <= x+i <= 8 and info[x+i][y] == 'S' or 0 <= y-i <= 8 and info[x][y-i] == 'S' or 0 <= y+i <= 8 and info[x][y+i] == 'S'):
        a += 1
        res.append("P")
        return [a, res]

    return [a, res]


def show(neo_x, neo_y):
    with open('close.txt', encoding='utf-8', mode='w') as f1:
        a = list(map)
        a[map.find('𖨆')] = ' '
        a[map.find(str(neo_x) + " |") + 4 + neo_y * 4] = '𖨆'
        f1.write(''.join(a))

while True:
    info[0][0] = 'N'
    keymaker_x = randint(0, 8)
    keymaker_y = randint(0, 8)
    while (info[keymaker_x][keymaker_y] != ''):
        keymaker_x = randint(0, 8)
        keymaker_y = randint(0, 8)
    info[keymaker_x][keymaker_y] = 'K'

    key_x = randint(0, 8)
    key_y = randint(0, 8)
    while (info[key_x][key_y] != ""):
        key_x = randint(0, 8)
        key_y = randint(0, 8)
    info[key_x][key_y] = 'B'

    for smith in range(randint(0, 4)):
        x = randint(0, 8)
        y = randint(0, 8)
        while (x == 0 and y == 0 or x == 0 and y == 1 or x == 1 and y == 0 or x == 1 and y == 1 or info[x][y] != '' or (x-1 >= 0 and y-1 >=0 and info[x-1][y-1] != '') or (x-1 >= 0 and info[x-1][y] != '') or (x-1 >= 0 and y+1 <= 8 and info[x-1][y+1] != '') or (y-1 >= 0 and info[x][y-1] != '') or (y+1 <= 8 and info[x][y+1] != '') or (x+1 <= 8 and y-1 >= 0 and info[x+1][y-1] != '') or (x+1 <= 8 and info[x+1][y] != '') or (x+1 <= 8 and y+1 <= 8 and info[x+1][y+1] != '')):
            x = randint(0, 8)
            y = randint(0, 8)
        info[x][y] = 'A'

    for sentiel in range(randint(0, 2)):
        x = randint(0, 8)
        y = randint(0, 8)
        while (x == 0 and y == 0 or x == 0 and y == 1 or x == 1 and y == 0 or info[x][y] != '' or (x-1 >= 0 and info[x-1][y] != '') or (x+1 <= 8 and info[x+1][y] != '') or (y-1 >= 0 and info[x][y-1] != '') or (y+1 <= 8 and info[x][y+1] != '')):
            x = randint(0, 8)
            y = randint(0, 8)
        info[x][y] = 'S'
    info[0][0] = ''

    map = '    0   1   2   3   4   5   6   7   8\n  +---+---+---+---+---+---+---+---+---+\n'
    for i in range(9):
        lst = [str(i)] + [" "] * 9
        for j in range(9):
            if info[i][j] == 'A':
                lst[j + 1] = '■'
            if info[i][j] == 'S':
                lst[j + 1] = '□'
            if info[i][j] == 'B':
                lst[j + 1] = '⚷'
            if info[i][j] == 'K':
                lst[j + 1] = 'K'
            if info[i][j] == 'N':
                lst[j + 1] = '𖨆'
        map += " | ".join(lst) + " |\n  +---+---+---+---+---+---+---+---+---+\n"
    show(0, 0)
    print('Введите режим (1 или 2):')
    mode = input()
    if mode == "":
        info = []
        for i in range(9):
            info.append([""] * 9)
        process = subprocess.Popen(['./code.exe'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        continue
    else:
        mode = int(mode)
    pc.copy(map)
    process.stdin.write(str(mode) + "\n" + str(keymaker_x) + " " + str(keymaker_y) + "\n")
    process.stdin.flush()
    now_x = 0
    now_y = 0
    keyactivated = False

    while True:
        move = process.stdout.readline().strip()
        if 'e' in move:
            print(move)
            break
        move = move.split()
        now_x = int(move[1])
        now_y = int(move[2])
        if (now_x == key_x and now_y == key_y):
            keyactivated = True
            map = map.replace("■", " ").replace("⚷", " ")
        show(now_x, now_y)
        sleep(0.5)

        res = ''
        num = 0

        if mode == 1:
            left = -1
            right = 2
        else:
            left = -2
            right = 3

        for dx in range(left, right):
            for dy in range(left, right): 
                new_x = now_x + dx
                new_y = now_y + dy
                if 0 <= new_x <= 8 and 0 <= new_y <= 8:
                    a = check(new_x, new_y)
                    num += a[0]
                    for elem in a[1]:
                        res += str(new_x) + " " + str(new_y) + " " + elem + "\n"

        process.stdin.write(str(num) + '\n' + res)
        process.stdin.flush()
    sleep(8)
    info = []
    for i in range(9):
        info.append([""] * 9)
    process = subprocess.Popen(['./code.exe'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)