from src.enviki import *
from datetime import datetime
import sqlite3


def baza_connect() -> tuple:
    if not os.path.exists(dbPath):
        os.makedirs(dbPath)
    conn = sqlite3.connect(dbPath + f'{dbName}.db')
    cur = conn.cursor()
    return conn, cur


def baza_commit(conn, cur) -> None:
    conn.commit()
    cur.close()
    conn.close()


def baza_create() -> None:
    conn, cur = baza_connect()
    cur.execute(f'''CREATE TABLE IF NOT EXISTS {clientsData} (
                id INTEGER PRIMARY KEY AUTOINCREMENT, 
                uuid TEXT, 
                balance INTEGER,
                last_exit_code INTEGER,
                last_update TIMESTAMP
                )''')
    baza_commit(conn, cur)


def baza_getIds() -> list:
    conn, cur = baza_connect()
    cur.execute(f'SELECT uuid FROM {clientsData}')
    uuids = cur.fetchall()
    allIds = []
    for item in uuids:
        allIds.append(item[0])
    baza_commit(conn, cur)
    return allIds


def baza_get(uuid:str, getValue:str) -> any:
    conn, cur = baza_connect()
    cur.execute(f'SELECT {getValue} FROM {clientsData} WHERE uuid=?', (uuid,))
    last_update = cur.fetchall()[0][0]
    baza_commit(conn, cur)
    return last_update


def baza_getLastUpdate(uuid:str) -> str:
    return baza_get(uuid=uuid, getValue='last_update')


def baza_getExitCode(uuid:str) -> int:
    return baza_get(uuid=uuid, getValue='last_exit_code')


def baza_getBalance(uuid: str) -> int:
    if baza_checkId(uuid):
        return baza_get(uuid=uuid, getValue='balance')
    else:
        return -1


def baza_updateBalance(balance: int, uuid: str, exitCode: int) -> None:
    currentTime = datetime.now()
    currentTime = currentTime.strftime('%Y-%m-%d %H:%M:%S')
    conn, cur = baza_connect()
    if not exitCode:
        cur.execute(f'UPDATE {clientsData} SET balance = ? WHERE uuid = ?', (balance, uuid))
    else:
        cur.execute(f'''UPDATE {clientsData} 
                        SET balance = ?, 
                        last_exit_code = ?, 
                        last_update = ? 
                        WHERE uuid = ?''', (balance, exitCode, currentTime, uuid))
    baza_commit(conn, cur)


def baza_checkId(uuid: str) -> bool:
    baza_create()
    Ids = baza_getIds()
    if uuid in Ids:
        return True
    else:
        return False


def baza_register(uuid: str) -> str:
    if baza_checkId(uuid):
        return "Already Registered"
    else:
        conn, cur = baza_connect()
        cur.execute(f'INSERT INTO {clientsData} (uuid, balance) VALUES (\'%s\',\'%s\')' % (uuid, 0))
        baza_commit(conn, cur)
        if baza_checkId(uuid):
            return "Registered Successfully"
        else:
            return "Try Again"

if not os.path.exists(dbPath + f'{dbName}.db'):
    baza_create()
