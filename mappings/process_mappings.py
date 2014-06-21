def read_csv_lines(file):
	for line in file:
		line = line.strip()
		if line and not line.startswith("#"):
			yield line.split(",")

NUM_BUTTONS = 40
NUM_BANKS = 16

MAPPINGS = dict(read_csv_lines(open('mappings.csv')))
BUTTON_NAMES = dict()
for name,row,col in read_csv_lines(open('button_names.csv')):
	BUTTON_NAMES[(int(row),int(col))] = name
MNEMONICS = dict(read_csv_lines(open('mnemonics.csv')))

def get_key_sequence(key_sequence_or_mnemonic):
	return MNEMONICS.get(key_sequence_or_mnemonic, key_sequence_or_mnemonic)

def make_mapping_struct(mapping):
	if not mapping:
		return "{ }"
	key_sequence = get_key_sequence(mapping)
	tokenized = key_sequence.split("+")
	if len(tokenized) > 1:
		modifiers_string = ""
		for modifier in tokenized[:-1]:
			modifier_name = "KEY_" + modifier
			if modifiers_string:
				modifiers_string = modifiers_string + "|" + modifier_name
			else:
				modifiers_string = modifier_name
	else:
		modifiers_string = "0"
	key_name = "KEY_" + tokenized[-1]
	return "{{{key_name}, {modifiers_string}, 0, KEY_BACKSPACE, 0 }}".format(**locals())

def make_button_desc(bank, button):
	right_shift = bank / 4
	left_shift = bank % 4
	row = button / 10
	col = button % 10
	try:
		button_desc = BUTTON_NAMES[(row, col)]
		if right_shift != 0:
			button_desc = "R" + str(right_shift) + "+" + button_desc
		if left_shift != 0:
			button_desc = "L" + str(left_shift) + "+" + button_desc
		return button_desc
	except KeyError:
		return None

if __name__ == '__main__':
	with open("../firmware/mappings.inc","w") as outfile:
		outfile.write("const static Mapping keyMappings[16][40] PROGMEM = {\n");
		for bank in range(NUM_BANKS):
			outfile.write("\t{\n")
			for button in range(NUM_BUTTONS):
				button_desc = make_button_desc(bank, button)
				mapping = MAPPINGS.get(button_desc, None)
				outfile.write("\t\t" + make_mapping_struct(mapping) + ",\n")
			outfile.write("\t},\n")
		outfile.write("};\n")

