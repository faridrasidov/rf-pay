from src.defchiki import *
from flask import Flask, request
import logging



app = Flask(__name__)
app.config['PROPAGATE_EXCEPTIONS'] = False
app.config['DEBUG'] = False
log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)
devlist = get_devList()


@app.route('/chat', methods=['GET'])
def chat():
    if 'message' in request.args:
        message = request.args.get('message', '')
        response = check_andPay(message)
        if response:
            return response
        else:
            return " "
    else:
        pass


def check_andPay(datas):
    try:
        data = datas.replace('\r', "")
        if data:
            devName, cardId = data.split(":")
            Tim3, Dat3 = get_time()
            if devName in devlist:
                value = str(devlist[devName])
                PST, Response = pay_withPost(cardId, devlist[devName])
                log_write(devName, cardId, value, Tim3, Dat3, PST)
                show_output(devName, cardId, value, Tim3, Dat3, PST)
                return Response

            elif devName == "FIX":
                outCode = devName
                devName, cardId = cardId.split("/")
                value = str(devlist[devName])
                if baza_getExitCode(cardId) == SUCCESS and get_diff(cardId):
                    last_value = baza_getBalance(cardId)
                    last_value += devlist[devName]
                    baza_updateValue(last_value, cardId, FIX)
                    log_write(devName, cardId, value, Tim3, Dat3, outCode)
                    show_output(devName, cardId, value, Tim3, Dat3, outCode)
                else:
                    pass

    except Exception as E:
        print(f"{E}")


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=51511, debug=False)