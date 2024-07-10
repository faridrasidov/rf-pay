from src.rdb_resource.rdb import *
import re
from datetime import timedelta


def get_diff(uuid: str) -> bool:
    current_date = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    current_date = datetime.strptime(current_date, "%Y-%m-%d %H:%M:%S")
    Tim3 = datetime.strptime(baza_getLastUpdate(uuid), "%Y-%m-%d %H:%M:%S")
    diff = current_date - Tim3
    return diff <= timedelta(seconds=10)


def get_devList() -> dict:
    confFilePath = confPath + 'Device.conf'
    if not os.path.exists(confPath):
        os.makedirs(confPath)
    if not os.path.exists(confFilePath):
        with open(confFilePath, 'w'):
            pass
    with open(confFilePath, 'r') as DevFile:
        content = DevFile.read()
    pattern = r"'([^']+)' *: *(\d+)"
    matches = re.findall(pattern, content)
    DevList = {key: int(value) for key, value in matches}
    return DevList


def get_time() -> [str, str]:
    now = datetime.now()
    current_date = now.strftime("%d/%m/%Y")
    current_time = now.strftime("%H:%M:%S")
    return current_time, current_date


def log_date() -> str:
    now = datetime.now()
    current_date = now.strftime("%d.%m.%Y")
    return current_date


def log_write(DevName: str, CardId: str, Count: str, T1me: str, D4te: str, Response: str) -> None:
    if not os.path.exists(logPath):
        os.makedirs(logPath)
    with open(logPath + log_date() + ".log", "a") as LogFile:
        toWrite = (f'{DevName}:'
                   f'{CardId}:'
                   f'{baza_getBalance(CardId)}:'
                   f'{Count}:'
                   f'{T1me}:'
                   f'{D4te}:'
                   f'{Response}')
        LogFile.write(toWrite + "\n")


def show_output(DevName: str, CardId: str, Count: str, T1me: str, D4te: str, Post: str) -> None:
    code = outPutColors[Post]
    print(f'\033[{code}m'
          f'{DevName} : '
          f'{CardId} : '
          f'{baza_getBalance(CardId)} : '
          f'{Count} : '
          f'{T1me} : '
          f'{D4te} : '
          f'{Post}\033[0m')


def pay_withPost(uid: str, value: int) -> [str, str]:
    if baza_checkId(uid):
        snapshot = baza_getBalance(uid)
        if snapshot >= value:
            snapshot -= value
            baza_updateValue(snapshot, uid, 0)
            if baza_getBalance(uid) == snapshot:
                Post_Data = f'{SUCCESS}:{snapshot}.'
                exitCode = SUCCESS
            else:
                Post_Data = f'{LOW_BALANCE}:{snapshot}.'
                exitCode = LOW_BALANCE
        else:
            Post_Data = f'{LOW_BALANCE}:{snapshot}.'
            exitCode = LOW_BALANCE
    else:
        Post_Data = f'{NOT_FOUND}:.'
        exitCode = NOT_FOUND

    snapshot = baza_getBalance(uid)
    baza_updateValue(snapshot, uid, exitCode)
    return f'{exitCode}', Post_Data
