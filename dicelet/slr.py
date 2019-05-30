import re


def my_hash(_o):
    val = 0
    for c in bytes(_o, encoding="ascii"):
        val *= 7
        val += int(c)
    return val


def str_list_hash(o):
    var = 0
    for _ in o:
        var *= 7
        var += my_hash(_)
    return var


def unordered_list_hash(o):
    var = 0
    for _ in o:
        var += hash(_)
    return hash(var)


def beatiful_print(s):
    o = dict_item_source[s]
    var = o[0]
    should_len = 12 - len(var)
    var += ' ' * should_len
    var += "  -> "
    for _ in o[1:]:
        var += " "
        var += _
    print(var)


def closure(_list):
    for _ in dict_item_has_joined:
        dict_item_has_joined[_] = False
    for _ in _list:
        dict_item_has_joined[_] = True

    while True:
        _about_to_join = []
        for _ in _list:
            _source = dict_item_source[_]
            __target = ''
            for _c in range(0, len(_source) - 1):
                if _source[_c] == '*':
                    __target = _source[_c + 1]
                    break

            if not __target:
                continue

            if __target[0] == '"':
                continue

            for _a in dict_item_source.items():
                if dict_item_has_joined[_a[0]]:
                    continue
                if _a[1][0] == __target and _a[1][1] == '*':
                    _about_to_join.append(_a[0])
                    dict_item_has_joined[_a[0]] = True
        if _about_to_join:
            for _to_join in _about_to_join:
                _list.append(_to_join)
        else:
            break

    ret = []
    for _ in dict_item_has_joined.items():
        if _[1]:
            ret.append(_[0])
    return ret


def goto(_list, _item):
    __target = []
    for _ in _list:
        source = dict_item_source[_]
        for _pos in range(1, len(source) - 1):
            if source[_pos] == '*':
                if source[_pos + 1] == _item:
                    _cp = list(source)
                    _cp[_pos] = _item
                    _cp[_pos + 1] = '*'
                    __target.append(str_list_hash(_cp))
                break
    if __target:
        return closure(__target)
    else:
        return None


production_list = []
terminals_set = []
raw_production_list = []
with open("slr.txt", "r") as file:
    b = file.readlines()
    for p in b:
        if p[0:1] == '//':
            continue
        if p[0] == '#':
            break
        if p[-1] == '\n':
            p = p[0:-1]
        if p:
            raw_production_list.append(p)

regex_params = re.compile("^(\\S+) +->(?: +(\\S+))(?: +(\\S+))?(?: +(\\S+))?(?: +(\\S+))?(?: +(\\S+))?$")

for prod in raw_production_list:
    match = regex_params.findall(prod)[0]
    p = []
    for i in match[0:]:
        if i:
            if i not in terminals_set:
                terminals_set.append(i)
            p.append(i)
    production_list.append(p)

dict_prodution_index = {}
for counter in range(0, len(production_list)):
    key = str_list_hash(production_list[counter])
    dict_prodution_index[key] = counter

terminals_set.append('"$"')

dict_terminals_set_index = {}

for counter in range(0, len(terminals_set)):
    dict_terminals_set_index[terminals_set[0]] = counter

dict_FIRST = {}
dict_FOLLOW = {}

while True:
    item_added_in_loop = False
    for production in production_list:
        key = production[0]
        if key not in dict_FIRST:
            dict_FIRST.setdefault(key, [])
        _target = dict_FIRST.get(key)

        _what = production[1]
        if _what[0] == '"' and _what not in _target:
            _target.append(_what)
            item_added_in_loop = True
        else:
            _target1 = dict_FIRST.get(_what)
            if _target1:
                for _t1 in _target1:
                    if _t1 not in _target:
                        _target.append(_t1)
                        item_added_in_loop = True
    if not item_added_in_loop:
        break

dict_FOLLOW['S'] = ['"$"']

while True:
    item_added_in_loop = False
    for production in production_list:
        for _c in range(1, len(production)):
            if _c < len(production)-1:
                key = production[_c]
                if key[0] == '"':
                    continue
                if key not in dict_FOLLOW:
                    dict_FOLLOW.setdefault(key, [])
                _target = dict_FOLLOW.get(key)
                _next = production[_c+1]
                if _next[0] == '"':
                    if _next not in _target:
                        _target.append(_next)
                        item_added_in_loop = True
                else:
                    _next_first = dict_FIRST[_next]
                    for _t in _next_first:
                        if _t not in _target:
                            _target.append(_t)
                            item_added_in_loop = True
            else:
                key = production[_c]
                if key[0] == '"':
                    continue
                if key not in dict_FOLLOW:
                    dict_FOLLOW.setdefault(key, [])
                _target = dict_FOLLOW.get(key)
                _other = dict_FOLLOW.get(production[0])
                if _other:
                    for _t in _other:
                        if _t not in _target:
                            _target.append(_t)
                            item_added_in_loop = True
    if not item_added_in_loop:
        break

dict_item_has_joined = {}
dict_item_source = {}
for pr in production_list:
    for i in range(1, len(pr) + 1):
        prn = list(pr)
        prn.insert(i, '*')
        dict_item_has_joined[str_list_hash(prn)] = False
        dict_item_source[str_list_hash(prn)] = prn

I0_c = []
for i in production_list:
    if i[0] == 'S':
        i_c = list(i)
        i_c.insert(1, '*')
        I0_c.append(str_list_hash(i_c))

I0_c = closure(I0_c)

dict_item_set = {unordered_list_hash(I0_c): I0_c}
list_of_standard_LR = [unordered_list_hash(I0_c)]

dict_fast_goto = {}

while True:
    _next_join = []
    item_added_in_loop = False
    for _is in dict_item_set.items():
        _is_cp = list(_is[1])
        for terminal in terminals_set:
            I_temp = goto(_is_cp, terminal)
            if I_temp:
                key = unordered_list_hash(I_temp)
                fast_goto = dict_fast_goto.get(_is[0])
                if not fast_goto:
                    dict_fast_goto.setdefault(_is[0], {})
                _ex_key = dict_fast_goto[_is[0]].get(terminal)
                if not _ex_key:
                    dict_fast_goto[_is[0]][terminal] = key
                    _next_join.append(I_temp)

    if _next_join:
        for _ in _next_join:
            _ex = dict_item_set.get(unordered_list_hash(_))
            if not _ex:
                dict_item_set[unordered_list_hash(_)] = _
                list_of_standard_LR.append(unordered_list_hash(_))
    else:
        break

dict_standard_LR_list_index = {}

counter = 0
for _ in list_of_standard_LR:
    print("I" + str(counter))
    dict_standard_LR_list_index[_] = counter
    for _1 in dict_item_set[_]:
        beatiful_print(_1)
    print("")
    counter += 1

dict_ACTION = {}
for _i in list_of_standard_LR:
    _item_set_i = dict_item_set[_i]
    _target = ''
    results_i = dict_ACTION.get(_i)
    if not results_i:
        dict_ACTION.setdefault(_i, {})
    for _item in _item_set_i:
        item = dict_item_source[_item]
        for pos in range(1, len(item) - 1):
            if item[pos] == '*':
                _target = item[pos + 1]

        if not _target:
            if item[0] == 'S' and item[-1] == '*':
                dict_ACTION[_i]['"$"'] = "ac"

        elif _target[0] == '"':
            _j = dict_fast_goto[_i][_target]
            dict_ACTION[_i][_target] = "s" + str(dict_standard_LR_list_index[_j])

        if item[-1] == '*' and not item[0] == 'S':
            item_cp = list(item)[0:-1]
            _key = str_list_hash(item_cp)
            for _t in dict_FOLLOW[item[0]]:
                _ex = dict_ACTION[_i].get(_t)
                if _ex:
                    dict_ACTION[_i][_t] = _ex + "/r" + str(dict_prodution_index[_key])
                else:
                    dict_ACTION[_i][_t] = "r" + str(dict_prodution_index[_key])

print('', end="\t")
for t in terminals_set:
    if t[0] == '"':
        # out = t + (' ' * (8 - len(t)))
        print(t, end="\t")
print("")

for _i in list_of_standard_LR:
    out = str(dict_standard_LR_list_index[_i])
    # space = ' ' * (8 - len(out))
    print(dict_standard_LR_list_index[_i], end="\t")
    for _t in terminals_set:
        if _t[0] == '"':
            act = dict_ACTION[_i].get(_t)
            if act:
                # space = ' ' * (8 - len(oper))
                print(act, end="\t")
            else:
                print(' ', end="\t")
    print("")

print("")
for _t in terminals_set:
    if not _t[0] == '"':
        print(_t, end="\t")
print("")

for _i in list_of_standard_LR:
    # print(dict_standard_LR_list_index[_i], end="\t")
    _target = dict_fast_goto.get(_i)
    if _target:
        for _t in terminals_set:
            if not _t[0] == '"':
                _ids = _target.get(_t)
                if _ids:
                    _id = dict_standard_LR_list_index.get(_ids)
                    print(_id, end="\t")
                else:
                    print(' ', end='\t')
    print('\n', end='')

print("ACTION = {")
_o_first = True
for _i in list_of_standard_LR:
    out = str(dict_standard_LR_list_index[_i])
    # space = ' ' * (8 - len(out))
    # print(dict_standard_LR_list_index[_i], end="")
    if _o_first:
        _o_first = False
    else:
        print(",")

    print("    {", end="")

    _first = True

    for _t in terminals_set:
        if _t[0] == '"':
            if _first:
                _first = False
            else:
                print(', ', end="")
            act = dict_ACTION[_i].get(_t)
            if act:
                space = ' ' * (4 - len(act))
                if act[0] == 's':
                    print('{s,' + space + act[1:] + '}', end="")
                elif act[0] == 'r':
                    print('{r,' + space + act[1:] + '}', end="")
                elif act == 'ac':
                    print('{a, 0}', end="")
            else:
                print('{e,  0}', end="")
    print("}", end="")
print("\n};")

print("GOTO = {")
_o_first = True
for _i in list_of_standard_LR:
    # print(dict_standard_LR_list_index[_i], end="")
    _target = dict_fast_goto.get(_i)
    if _o_first:
        _o_first = False
    else:
        print(",")
    print("    {", end="")
    if _target:
        _first = True
        for _t in terminals_set:
            if not _t[0] == '"':
                if _first:
                    _first = False
                else:
                    print(', ', end="")
                _ids = _target.get(_t)
                if _ids:
                    _id = dict_standard_LR_list_index.get(_ids)
                    space = ' ' * (4 - len(str(_id)))
                    print('{' + space + str(_id) + '}', end="")
                else:
                    print('{   0}', end='')
    else:
        _first = True
        for _t in terminals_set:
            if not _t[0] == '"':
                if _first:
                    _first = False
                else:
                    print(', ', end="")
                print('{   0}', end='')
    print("}", end="")
print("\n};")

print("\nenum class terminals : uint8_t{")
counter = 0
for _t in terminals_set:
    if _t[0] == '"':
        space = ' ' * (12 - len(_t))
        print('    ' + _t + space + "= " + str(counter), end=",\n")
        counter += 1
print("};\n")

print("\nenum class nterminals : uint8_t{")
counter = 0
for _t in terminals_set:
    if not _t[0] == '"':
        space = ' ' * (12 - len(_t))
        print('    ' + _t + space + "= " + str(counter), end=",\n")
        counter += 1
print("};\n")


def translate_terminal(o):
    if o == '"NUM"':
        return 'number'
    if o == '"+"':
        return 'plus'
    if o == '"-"':
        return 'minus'
    if o == '"*"/"/"':
        return 'muldvi'
    if o == '"("':
        return 'lbracket'
    if o == '")"':
        return 'rbracket'
    if o == '"{"':
        return 'lbrace'
    if o == '"}"':
        return 'rbrace'
    if o == '","':
        return 'comma'
    if o == '"#"':
        return 'sharp'
    if o == '"d"':
        return 'd'
    if o == '"k"':
        return 'k'
    if o == '"kl"':
        return 'kl'
    return o


production_type_list = []
for _p in production_list:
    s = "production_"
    is_first = True
    for _i in _p:
        if is_first:
            is_first = False
        else:
            s += '_'
        if _i[0] == '"':
            s += translate_terminal(_i)
        else:
            s += _i
    production_type_list.append(s)

print("\nproduction_type = {")
_first = True
for _i in range(0, len(production_list)):
    _p = production_list[_i]
    if _first:
        _first = False
    else:
        print(",")
    print(production_type_list[_i], end="")
print("\n};\n")

print("\nPRODUCTION = {")
_first = True
for _i in range(0, len(production_list)):
    _p = production_list[_i]
    if _first:
        _first = False
    else:
        print(",")
    print("{production_type::" + production_type_list[_i] + ", nterminals::nterminal_" + _p[0] + ", " + str(len(_p) -1) + "}", end="")
print("\n};\n")

#
# # for production in production_list:
# for i in production_list:
#     print(i)
#
#
#
# next = input("input some params?")
#
