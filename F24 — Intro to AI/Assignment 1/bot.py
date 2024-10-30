import subprocess
from random import randint
from aiogram import Bot, types #сам бот
from aiogram.dispatcher import Dispatcher #для создания бота
from aiogram.utils import executor #для запуска бота
import asyncio
import os
import psutil

bot = Bot(token='')
loop = asyncio.get_event_loop()
dp = Dispatcher(bot, loop=loop)

class TeleportError(Exception):
    pass

user_sessions = {}
queue = []
class UserSession:
    def __init__(self, user_id):
        self.user_id = user_id
        self.info = 0
        self.keymaker_x = 0
        self.keymaker_y = 0
        self.key_x = 0
        self.key_y = 0
        self.process1 = 0
        self.process2 = 0
        self.mode = 0
        self.working = False
        self.last_proc = 0

async def check(x, y, user_id):
    a = 0
    res = []
    if user_sessions[user_id].info[x][y] != "":
        a += 1
        res.append(user_sessions[user_id].info[x][y])
    for dx in range(-1, 2):
        for dy in range(-1, 2):
            if (0 <= x+dx <= 8 and 0 <= y+dy <= 8 and (dx != 0 or dy != 0) and user_sessions[user_id].info[x+dx][y+dy] == 'A'):
                a += 1
                res.append("P")
                return [a, res]
    if (0 <= x-1 <= 8 and user_sessions[user_id].info[x-1][y] == 'S' or 0 <= x+1 <= 8 and user_sessions[user_id].info[x+1][y] == 'S' or 0 <= y-1 <= 8 and user_sessions[user_id].info[x][y-1] == 'S' or 0 <= y+1 <= 8 and user_sessions[user_id].info[x][y+1] == 'S'):
        a += 1
        res.append("P")
        return [a, res]

    return [a, res]

async def mapgen(user_id):
    user_sessions[user_id].mode = randint(0, 1)

    user_sessions[user_id].info = []
    for i in range(9):
        user_sessions[user_id].info.append([""] * 9)

    user_sessions[user_id].info[0][0] = 'N'
    user_sessions[user_id].keymaker_x = randint(0, 8)
    user_sessions[user_id].keymaker_y = randint(0, 8)
    while (user_sessions[user_id].info[user_sessions[user_id].keymaker_x][user_sessions[user_id].keymaker_y] != ''):
        user_sessions[user_id].keymaker_x = randint(0, 8)
        user_sessions[user_id].keymaker_y = randint(0, 8)
    user_sessions[user_id].info[user_sessions[user_id].keymaker_x][user_sessions[user_id].keymaker_y] = 'K'

    user_sessions[user_id].key_x = randint(0, 8)
    user_sessions[user_id].key_y = randint(0, 8)
    while (user_sessions[user_id].info[user_sessions[user_id].key_x][user_sessions[user_id].key_y] != ""):
        user_sessions[user_id].key_x = randint(0, 8)
        user_sessions[user_id].key_y = randint(0, 8)
    user_sessions[user_id].info[user_sessions[user_id].key_x][user_sessions[user_id].key_y] = 'B'

    for smith in range(randint(0, 3)):
        x = randint(0, 8)
        y = randint(0, 8)
        while (x == 0 and y == 0 or x == 0 and y == 1 or x == 1 and y == 0 or x == 1 and y == 1 or user_sessions[user_id].info[x][y] != '' or (x-1 >= 0 and y-1 >=0 and user_sessions[user_id].info[x-1][y-1] != '') or (x-1 >= 0 and user_sessions[user_id].info[x-1][y] != '') or (x-1 >= 0 and y+1 <= 8 and user_sessions[user_id].info[x-1][y+1] != '') or (y-1 >= 0 and user_sessions[user_id].info[x][y-1] != '') or (y+1 <= 8 and user_sessions[user_id].info[x][y+1] != '') or (x+1 <= 8 and y-1 >= 0 and user_sessions[user_id].info[x+1][y-1] != '') or (x+1 <= 8 and user_sessions[user_id].info[x+1][y] != '') or (x+1 <= 8 and y+1 <= 8 and user_sessions[user_id].info[x+1][y+1] != '')):
            x = randint(0, 8)
            y = randint(0, 8)
        user_sessions[user_id].info[x][y] = 'A'

    for sentiel in range(randint(0, 1)):
        x = randint(0, 8)
        y = randint(0, 8)
        while (x == 0 and y == 0 or x == 0 and y == 1 or x == 1 and y == 0 or user_sessions[user_id].info[x][y] != '' or (x-1 >= 0 and user_sessions[user_id].info[x-1][y] != '') or (x+1 <= 8 and user_sessions[user_id].info[x+1][y] != '') or (y-1 >= 0 and user_sessions[user_id].info[x][y-1] != '') or (y+1 <= 8 and user_sessions[user_id].info[x][y+1] != '')):
            x = randint(0, 8)
            y = randint(0, 8)
        user_sessions[user_id].info[x][y] = 'S'
    user_sessions[user_id].info[0][0] = ''

async def solve(process, user_id):
    global emerg

    # Кодируем строки в байты перед записью в stdin
    process.stdin.write((str(user_sessions[user_id].mode) + "\n" +
                         str(user_sessions[user_id].keymaker_x) + " " +
                         str(user_sessions[user_id].keymaker_y) + "\n").encode())
    
    now_x = 0
    now_y = 0

    while True:
        move = await process.stdout.readline()
        
        move = move.decode('utf-8')
        if 'e' in move:
            return move
        
        move = move.split()
        temp_x = int(move[1])
        temp_y = int(move[2])
        if not(temp_x + 1 == now_x and temp_y == now_y or temp_x - 1 == now_x and temp_y == now_y or temp_x == now_x and temp_y + 1 == now_y or temp_x == now_x and temp_y - 1 == now_y or temp_x == now_x and temp_y == now_y):
            raise TeleportError(f'From ({now_x}, {now_y}) to ({temp_x}, {temp_y})')
        now_x = int(move[1])
        now_y = int(move[2])

        res = ''
        num = 0

        if user_sessions[user_id].mode == 1:
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
                    a = await check(new_x, new_y, user_id)
                    num += a[0]
                    for elem in a[1]:
                        res += str(new_x) + " " + str(new_y) + " " + elem + "\n"

        # Кодируем результат перед записью
        process.stdin.write((str(num) + '\n' + res).encode())
        await asyncio.sleep(0)
            

async def show_map(user_id):
    map = '    0   1   2   3   4   5   6   7   8\n  +---+---+---+---+---+---+---+---+---+\n'
    user_sessions[user_id].info[0][0] = 'N'
    for i in range(9):
        lst = [str(i)] + [" "] * 9
        for j in range(9):
            if user_sessions[user_id].info[i][j] == 'A':
                lst[j + 1] = '■'
            if user_sessions[user_id].info[i][j] == 'S':
                lst[j + 1] = '□'
            if user_sessions[user_id].info[i][j] == 'B':
                lst[j + 1] = '⚷'
            if user_sessions[user_id].info[i][j] == 'K':
                lst[j + 1] = 'K'
            if user_sessions[user_id].info[i][j] == 'N':
                lst[j + 1] = 'N'
        map += " | ".join(lst) + " |\n  +---+---+---+---+---+---+---+---+---+\n"
    map += '\nN — Neo\n■ — Agent Smith\n□ — Sentiel\n⚷ — key\nK — keymaker (win)'
    return map

async def test(file, user_id, lang):
    k = 1
    for i in range(1000):
        # рабочий код
        working_code = await asyncio.create_subprocess_exec(
            './code.exe',
            stdin=asyncio.subprocess.PIPE,
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.PIPE,
        )
        # присланный код
        if (lang == 'cpp'):
            code_to_check = await asyncio.create_subprocess_exec(
                './' + file,
                stdin=asyncio.subprocess.PIPE,
                stdout=asyncio.subprocess.PIPE,
                stderr=asyncio.subprocess.PIPE,
            )
        else:
            code_to_check = await asyncio.create_subprocess_exec(
                'python',
                file,
                stdin=asyncio.subprocess.PIPE,
                stdout=asyncio.subprocess.PIPE,
                stderr=asyncio.subprocess.PIPE,
            )
        user_sessions[user_id].last_proc = code_to_check
        await mapgen(user_id)
        try:
            correct = await asyncio.wait_for(solve(working_code, user_id), timeout=1)
            given = await asyncio.wait_for(solve(code_to_check, user_id), timeout=1)
            if (correct != given):
                return [correct.replace('\n', ''), given.replace('\n', '')]
            if (k % 100 == 0):
                print(k)
            k += 1
        except asyncio.TimeoutError:
            code_to_check.terminate()
            return 1
        except TeleportError as e:
            code_to_check.terminate()
            return f'{e}'
        except:
            code_to_check.terminate()
            return 10
    return 0

@dp.message_handler()
async def start_command(message: types.Message):
    if (message.from_user.id in queue):
        if (queue[0] == message.from_user.id):
            await message.reply("Ваш код выполняется.")
        else:
            await message.reply("Ваш код в очереди. Перед вами человек: " + str(queue.index(message.from_user.id)) + ".")
    else:
        await message.reply("Пришли мне файл .cpp или .py")

@dp.message_handler(content_types=types.ContentType.DOCUMENT)
async def handle_document(message: types.Message):
    if message.document.mime_type == "text/x-c++src" or message.document.mime_type == "text/x-python" or message.document.file_name.endswith(('.cpp', '.py')):
        
        if message.from_user.id not in user_sessions:
            user_sessions[message.from_user.id] = UserSession(id)
        if (queue.count(message.from_user.id) > 5):
            await message.reply("Вы отправили больше 5 кодов подряд, подождите, пока хотя бы один из предыдущих закончит тестирование.")
            return
        queue.append(message.from_user.id)
        if (queue[0] != message.from_user.id):
            await message.reply("Принято. Ожидайте. Перед вами человек: " + str(queue.index(message.from_user.id)) + ".")
            while (queue[0] != message.from_user.id):
                await asyncio.sleep(5)
        while (user_sessions[message.from_user.id].working):
            await asyncio.sleep(5)

        user_sessions[message.from_user.id].working = True
        print('Начинаю выполнение для', message.from_user.username)

        if message.document.mime_type == "text/x-c++src" or message.document.file_name.endswith(('.cpp')):
            # Сохранение файла
            file_info = await bot.get_file(message.document.file_id)
            downloaded_file = await bot.download_file(file_info.file_path)
            file_name = str(message.from_user.id) + ".cpp"

            # Сохраняем файл в текущую директорию
            with open(file_name, 'wb') as f:
                f.write(downloaded_file.read())

            # Компиляция файла с помощью g++
            compiled_file = str(message.from_user.id) + ".exe"
            compile_process = subprocess.run(['g++', file_name, '-o', compiled_file], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

            if compile_process.returncode != 0:
                await message.reply("Ошибка компиляции")
            else:
                try:
                    await message.reply("Запускаю тесты")
                    a = await test(compiled_file, message.from_user.id, 'cpp')
                    if (a == 0):
                        await message.reply("1000 карт прошли успешно")
                    elif (a == 1):
                        await message.reply('```\n' + await show_map(message.from_user.id) + '\n```\nПревышено максимальное время работы', parse_mode="Markdown")
                    elif ('From' in a):
                        await message.reply('```\n' + await show_map(message.from_user.id) + '\n```\nНео телепортируется (разрешены ходы только в соседние ячейки)\n' + a, parse_mode="Markdown")
                    else:
                        await message.reply('```\n' + await show_map(message.from_user.id) + '\n```\nПравильный ответ — ' + a[0] + "\nВаш ответ — " + a[1], parse_mode="Markdown")
                except:
                    await message.reply("Ошибка выполнения программы.")
                try:
                    os.remove(compiled_file)
                except:
                    for proc in psutil.process_iter(['name']):
                        if proc.info['name'] == file_name:
                            proc.kill()
                    await asyncio.sleep(1)
                    os.remove(compiled_file)
            os.remove(file_name)
        else:
            # Сохранение файла
            file_info = await bot.get_file(message.document.file_id)
            downloaded_file = await bot.download_file(file_info.file_path)
            file_name = str(message.from_user.id) + ".py"

            # Сохраняем файл в текущую директорию
            with open(file_name, 'wb') as f:
                f.write(downloaded_file.read())
            try:
                await message.reply("Запускаю тесты")
                a = await test(file_name, message.from_user.id, 'py')
                if (a == 0):
                    await message.reply("1000 карт прошли успешно")
                elif (a == 1):
                    await message.reply('```\n' + await show_map(message.from_user.id) + '\n```\nПревышено максимальное время работы', parse_mode="Markdown")
                elif ('From' in a):
                    await message.reply('```\n' + await show_map(message.from_user.id) + '\n```\nНео телепортируется (разрешены ходы только в соседние ячейки)\n' + a, parse_mode="Markdown")
                else:
                    await message.reply('ОШИБКА```\n' + await show_map(message.from_user.id) + '\n```\nПравильный ответ — ' + a[0] + "\nВаш ответ — " + a[1], parse_mode="Markdown")
            except:
                await message.reply("Ошибка выполнения программы.")
            try:
                os.remove(file_name)
            except:
                for proc in psutil.process_iter(['name']):
                    if proc.info['name'] == file_name:
                        proc.kill()
                await asyncio.sleep(1)
                os.remove(file_name)
        queue.pop(0)
        user_sessions[message.from_user.id].working = False
        print('Выполнение для', message.from_user.username, 'завершено.')
    else:
        await message.reply("Пожалуйста, отправьте файл с расширением .cpp или .py")

if __name__ == '__main__':
    executor.start_polling(dp, skip_updates=True)