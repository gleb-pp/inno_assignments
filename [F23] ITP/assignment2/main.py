import sys
import os
import random

def generate():
    res = ""
    n = random.randint(0, 20)
    ids = []
    for _ in range(n):
        cmd = random.choices([1, 2, 3, 4, 5], [10, 10, 10, 10, 0.5])[0]
        if cmd == 1:
            res += "Add\n"

            if random.random() < 0.9 or len(ids) == 0:
                id = str(random.randint(1, 1_000_000))
                if id not in ids:
                    ids.append(id)
                res += id + '\n'
            else:
                res += ids[random.randint(0, len(ids)-1)] + '\n'
            
            name_len = random.randint(1, 17)
            for __ in range(name_len):
                res += random.choice('qwerbnmQWERTYUIOPASDFGHJKLZXCVBNMQWERTYUIOPASDFGHJKLZXCVBNM1234')
            res += '\n'

            res += random.choices(['Defender', 'Goalkeeper', 'Forward', 'Midfielder', 'nobody'],
                                  [10, 10, 10, 10, 3])[0]
            res += '\n'
            
            res += str(random.randint(16, 105))
            res += '\n'
            
            res += str(random.randint(-2, 1050))
            res += '\n'

        elif cmd == 2:
            res += "Update\n"

            if random.random() < 0.1 or len(ids) == 0:
                id = str(random.randint(0, 1_000_000))
                if id not in ids:
                    ids.append(id)
                res += id + '\n'
            else:
                res += ids[random.randint(0, len(ids)-1)] + '\n'
            
            name_len = random.randint(1, 17)
            for __ in range(name_len):
                res += random.choice('qwerbnmQWERTYUIOPASDFGHJKLZXCVBNMQWERTYUIOPASDFGHJKLZXCVBNM1234')
            res += '\n'

            res += random.choices(['Defender', 'Goalkeeper', 'Forward', 'Midfielder', 'nobody'],
                                  [10, 10, 10, 10, 3])[0]
            res += '\n'
            
            res += str(random.randint(16, 105))
            res += '\n'
            
            res += str(random.randint(-2, 1050))
            res += '\n'
        
        elif cmd == 3:
            res += 'Search\n'
            if random.random() < 0.5 and len(ids) > 0:
                res += random.choice(ids) + '\n'
            else:
                res += str(random.randint(0, 1_000_000)) + '\n'
        
        elif cmd == 4:
            res += 'Delete\n'
            if random.random() < 0.5 and len(ids) > 0:
                res += random.choice(ids) + '\n'
            else:
                res += str(random.randint(0, 1_000_000)) + '\n'
        
        else:
            res += 'AAAA\n'
    
    if random.random() < 2:
        res += "Display\n"
        
    return res
            

n, p1, p2 = sys.argv[1:4]

for i in range(int(n)):
    test = generate()
    
    f = open("input.txt", "w")
    f.write(test)
    f.close()

    os.system(p1)
    f = open("output.txt", "r")
    res1 = f.read()
    f.close()

    f = open("input.txt", "w")
    f.write(test)
    f.close()

    os.system(p2)
    f = open("output.txt", "r")
    res2 = f.read()
    f.close()

    if res1 != res2:
        print("Difference!")
        exit(0)

print("OK")