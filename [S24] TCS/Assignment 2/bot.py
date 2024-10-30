import subprocess
from aiogram import Bot, types #сам бот
from aiogram.dispatcher import Dispatcher #для создания бота
from aiogram.utils import executor #для запуска бота
from asyncio import get_event_loop

bot = Bot(token='7010079615:AAFHMmdW-LUpw-rUJhfdRTkpJkztEqjLT3w')
loop = get_event_loop()
dp = Dispatcher(bot, loop=loop)

def run_cpp_program(input_data):
    # Запускаем программу на C++ и передаём в неё входные данные
    process = subprocess.Popen(['./Bot.exe'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    stdout, stderr = process.communicate(input_data)

    # Проверяем наличие ошибок при выполнении программы
    if process.returncode != 0:
        return f"Произошла ошибка: {stderr}"
    else:
        return stdout

@dp.message_handler()
async def start_command(message: types.Message):
    if (message.text == "/start"):
        await message.answer("Пришли мне любой input, я пришлю тебе ожидаемый output")
    else:
        output = run_cpp_program(message.text)
        if (len(output)) > 4096:
            with open('output.txt', 'w') as f:
                f.write(output)
            with open('output.txt', 'rb') as f:
                await message.reply_document(f, caption="Output слишком большой, я не могу поместить его в сообщение. Держи файл.")
        else:
            await message.reply(output)
    print(message.from_user.full_name + " — @" + message.from_user.username)

if __name__ == '__main__':
    executor.start_polling(dp)