import dis
import add  # your python file

# Disassemble specific functions
print()
print('>>> Python bytecode for the add() function')
print(dis.dis(add.add_numbers))
