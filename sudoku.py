from collections import defaultdict
import itertools

# sample = [
#     [5, 3, 0, 0, 7, 0, 0, 0, 0],
#     [6, 0, 0, 1, 9, 5, 0, 0, 0],
#     [0, 9, 8, 0, 0, 0, 0, 6, 0],
#     [8, 0, 0, 0, 6, 0, 0, 0, 3],
#     [4, 0, 0, 8, 0, 3, 0, 0, 1],
#     [7, 0, 0, 0, 2, 0, 0, 0, 6],
#     [0, 6, 0, 0, 0, 0, 2, 8, 0],
#     [0, 0, 0, 4, 1, 9, 0, 0, 5],
#     [0, 0, 0, 0, 8, 0, 0, 7, 9],
# ]

sample = [
    [4, 0, 0, 0, 0, 0, 8, 0, 5],
    [0, 3, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 7, 0, 0, 0, 0, 0],
    [0, 2, 0, 0, 0, 0, 0, 6, 0],
    [0, 0, 0, 0, 8, 0, 4, 0, 0],
    [0, 0, 0, 0, 1, 0, 0, 0, 0],
    [0, 0, 0, 6, 0, 3, 0, 7, 0],
    [5, 0, 0, 2, 0, 0, 0, 0, 0],
    [1, 0, 4, 0, 0, 0, 0, 0, 0],
]

BOX_HEIGHT = 3
BOX_WIDTH = 3
BOARD_SIZE = (BOX_HEIGHT * BOX_WIDTH)


UNASSIGNED = 0
VAR_RANGE = tuple(range(1, BOARD_SIZE + 1))


houses = [
    [(r, c) for c in range(BOARD_SIZE)]
    for r in range(BOARD_SIZE)
] + [
    [(r, c) for r in range(BOARD_SIZE)]
    for c in range(BOARD_SIZE)
] + [
    [
        (r, c)
        for r in range(br, br + BOX_HEIGHT)
        for c in range(bc, bc + BOX_WIDTH)
    ]
    for br in range(0, BOARD_SIZE, BOX_HEIGHT)
    for bc in range(0, BOARD_SIZE, BOX_WIDTH)
]

house_mapping = defaultdict(list)

for house in houses:
    for p in house:
        house_mapping[p].append(house)


call_stack = []
history = []
elimination_stack = []

candidates = {
    (r, c): set(VAR_RANGE)
    for r in range(BOARD_SIZE)
    for c in range(BOARD_SIZE)
}


def solve():
    call_stack = []
    history = []
    elimination_stack = []

    candidates = {
        (r, c): set(VAR_RANGE)
        for r in range(BOARD_SIZE)
        for c in range(BOARD_SIZE)
    }

    def assign(p, v):
        for v_el in VAR_RANGE:
            if v_el != v:
                if not eliminate(p, v_el):
                    return False
        while elimination_stack:
            p, v = elimination_stack.pop()
            for house in house_mapping[p]:
                for p_other in house:
                    if p_other != p:
                        if not eliminate(p_other, v):
                            elimination_stack[:] = ()
                            return False
        return True

    def eliminate(p, v):
        if v in candidates[p]:
            candidates[p].remove(v)
            history.append((p, v))
            if len(candidates[p]) == 1:
                [v_p] = candidates[p]
                elimination_stack.append((p, v_p))
            elif len(candidates[p]) == 0:
                return False
        return True

    def backtrack():
        if not call_stack:
            return None
        p_res, i, history_len = call_stack.pop()
        while len(history) > history_len:
            p, v = history.pop()
            candidates[p].add(v)
        return p_res, i

    for r, c in itertools.product(range(BOARD_SIZE), repeat=2):
        if sample[r][c] != 0:
            if not assign((r, c), sample[r][c]):
                return

    should_backtrack = False
    while True:
        if should_backtrack:
            backtrack_res = backtrack()
            if backtrack_res is None:
                return
            p, i = backtrack_res
            i += 1
            if len(candidates[p]) <= i:
                # should_backtrack = True
                continue
        else:
            i = 0
            min_size = BOARD_SIZE + 1
            found_target = False
            for p in itertools.product(range(BOARD_SIZE), repeat=2):
                if 1 < len(candidates[p]) < min_size:
                    found_target = True
                    p_target = p
            if not found_target:
                yield [
                    [
                        list(candidates[(r, c)])[0]
                        for c in range(BOARD_SIZE)
                    ]
                    for r in range(BOARD_SIZE)
                ]
                should_backtrack = True
                continue
            p = p_target

        v = sorted(candidates[p])[i]
        call_stack.append((p, i, len(history)))
        should_backtrack = False
        if not assign(p, v):
            should_backtrack = True


import pprint; pprint.pprint(list(solve()))
