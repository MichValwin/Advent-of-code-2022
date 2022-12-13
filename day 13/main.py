import os
from functools import cmp_to_key

INPUT_FILE = "input.txt"

def parseLine(line, indexArrStart):
    currentList = []

    if(line[indexArrStart[0]] == '['): indexArrStart[0] += 1

    append = True

    while indexArrStart[0] < len(line):
        char = line[indexArrStart[0]]

        if char == '[':
            currentList.append(parseLine(line, indexArrStart))
        elif char == ']':
            break
        elif char == ',':
            append = True
        else:
            if append:
                currentList.append(char)
                append = False
            else:
                currentList[-1] += char


        indexArrStart[0] += 1
    
    return currentList

def compare_num(num1, num2):
    if num1 < num2:
        return -1
    elif num1 > num2:
        return 1
    else:
        return 0

def comparePairs(list1, list2):
    print("Compare", list1, "vs", list2)

    for i in range(0, len(list1)):
        if(i >= len(list2)):
            print("Right side ran out of items, so inputs are not in the right order")
            return 1 # Not order

        if isinstance(list1[i], list) and isinstance(list2[i], list):
            comp = comparePairs(list1[i], list2[i])
            if comp != 0: return comp
        elif isinstance(list1[i], list) and isinstance(list2[i], str):
            list2[i] = [list2[i]]
            comp = comparePairs(list1[i], list2[i])
            if comp != 0: return comp
        elif isinstance(list1[i], str) and isinstance(list2[i], list):
            list1[i] = [list1[i]]
            comp = comparePairs(list1[i], list2[i])
            if comp != 0: return comp
        else:
            print("Compare", list1[i], "vs", list2[i])
            comp = compare_num(int(list1[i]), int(list2[i]))
            if comp != 0: 
                if(comp < 0):
                    print("Left side is smaller, so inputs are in the right order")
                    return -1
                elif comp > 0:
                    print("Right side is smaller, so inputs are not in the right order")
                    return 1
                

    if(len(list1) < len(list2)):
        print("Left side ran out of items, so inputs are in the right order")
        return -1    

    return 0


total_pairs = []
packets = []


with open(INPUT_FILE, 'r') as input_file:
    pairs = []

    for line in input_file:
        line = line.strip()

        if len(line) > 0:
            packet = parseLine(line, [0])
            pairs.append(packet)
            packets.append(packet)
       
        if(len(pairs) == 2):
            total_pairs.append(pairs)
            pairs = []

pairs_in_order = []

print("Total pairs: ", len(total_pairs))

# Silver
for i in range(0, len(total_pairs)):
    pair = total_pairs[i]
    print("== Pair", i+1, "==")
    
    order = comparePairs(pair[0], pair[1])
    pairs_in_order.append(order < 0)

correct_packets = []

silver = 0
for i, in_order in enumerate(pairs_in_order):
    if in_order: correct_packets.append(i)

# Gold
gold = 1
packet1Insert = [['2']]
packet2Insert = [['6']]
packets.append(packet1Insert)
packets.append(packet2Insert)

sorted_packets = sorted(packets, key=cmp_to_key(comparePairs))

for i in range(0, len(sorted_packets)):
    if sorted_packets[i] == packet1Insert:
        gold *= (i+1)
    if sorted_packets[i] == packet2Insert:
        gold *= (i+1)
    print(sorted_packets[i])


print("Silver: ", sum(correct_packets) + len(correct_packets))
print("Gold: ", gold)

print("Sum pairs: ", sum(pairs_in_order))