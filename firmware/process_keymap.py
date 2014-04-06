import json

KM = json.load(open("keymap.json"))

def find_bank(leftmod, rightmod):
    for bank in KM["banks"]:
        if (leftmod == None and "leftmod" not in bank) or (leftmod == bank["leftmod"]):
            if (rightmod == None and "rightmod" not in bank) or (rightmod == bank["rightmod"]):
                return bank

MAPPINGS = set() # key is (leftmod, rightmod, row, col)

for bank in KM["banks"]:
    if "leftmod" in bank:
        leftmod = bank["leftmod"]
    if "rightmod" in bank:
        rightmod = bank["rightmod"]
    
    for mapping in bank["mappings"]:
        
def make_mapping(leftmod, rightmod, row, col):
    bank = find_bank(leftmod, rightmod)
    basekey