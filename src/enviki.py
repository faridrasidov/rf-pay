import os

# [outCodes]
FIX = -1
SUCCESS = 200
NOT_FOUND = 800
LOW_BALANCE = 400

# [outColors]
outPutColors = {'200':92, '400':93, '800':95, 'FIX':91}

# [dirs]
scriptDirectory = os.path.dirname(os.path.abspath(__file__))
rootDirectory = os.path.dirname(scriptDirectory)
logPath = rootDirectory + '/logs/'
dbPath = rootDirectory + '/database/'
confPath = rootDirectory + '/conf/'

# [rdb]
dbName = 'rf-Pay'
clientsData = 'CardInfo'
