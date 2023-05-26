import graphviz
import pandas as pd

C_unit_list = []
C_edge_list = []
C_rate_list = []
T_unit_list = []
T_edge_list = []
T_rate_list = []


df = pd.read_csv('D.txt', sep=' ', header=None)

df[1] = df[1] / (df[1] + df[2]) * 100
df[2] = df[3] / (df[3] + df[4]) * 100

df = df.drop([3, 4], axis=1).round(1)

# Open the Concentrate file and save them into list
with open('C.txt', 'r') as file:
    file = file.readlines()
    header = file[-1][0]
    f = file[:-3]
    for line in f:
        # Split the line into two nodes number
        pair = line.strip().split(' ')
        C_edge_list.append(pair[:2])
        # Save the unit ID into a list
        for index, element in enumerate(pair):
            if element not in C_unit_list and index < 2:
                C_unit_list.append(element)
            if index == 2:
                C_rate_list.append(element)

# Open the Tailing file and save them into list
with open('T.txt', 'r') as file:
    file = file.readlines()
    header = file[-1][0]
    f = file[:-3]
    for line in f:
        # Split the line into two nodes number
        pair = line.strip().split(' ')
        T_edge_list.append(pair[:2])
        # Save the unit ID into a list
        for index, element in enumerate(pair):
            if element not in T_unit_list and index < 2:
                T_unit_list.append(element)
            if index == 2:
                T_rate_list.append(element)
unit_list = list(set(C_unit_list + T_unit_list))
unit_list.sort(key=lambda x: int(x))
tails = list(map(int, unit_list[-2:]))
# Initialize a graphviz instance
digraph = graphviz.Digraph(comment='The Directed Graph', format='png')
digraph.attr(rankdir='LR')
mid_set = False
tail_set = False

# Setting header node
digraph.node('Feed', 'Feed', shape='box')
digraph.node(str(header), 'Unit '+str(header), shape='box')
digraph.edge('Feed', str(header), color='blue')
# unit_list.remove(header)

for i in range(len(unit_list)):
    if i == int(header):
        continue
    elif i in tails:
        if tail_set is False:
            tail = graphviz.Digraph()
            tail_set = True
        if i == tails[0]:
            tail.node(str(tails[0]), 'Concentrate', shape='box')
        elif i == tails[1]:
            tail.node(str(tails[1]), 'Tailings', shape='box')
    else:
        if mid_set is False:
            mid = graphviz.Digraph()
            mid_set = True
        mid.node(unit_list[i], 'Unit '+unit_list[i], shape='box')

digraph.attr('edge', color='red')
for i in range(len(C_edge_list)):
    digraph.edge(C_edge_list[i][0], C_edge_list[i][1], label=str(
        df.iloc[int(C_edge_list[i][0])][1])+'%', fontcolor='red')
    # else:
    #     digraph.edge(C_edge_list[i][0], C_edge_list[i][1],
    #                  tailport='n', headport='w', label=C_rate_list[i])
digraph.attr('edge', color='blue')
for i in range(len(T_edge_list)):
    # if len(T_rate_list) == 0:
    digraph.edge(T_edge_list[i][0], T_edge_list[i][1], label=str(df.iloc[int(
        C_edge_list[i][0])][2])+'%', fontcolor='blue')
    # else:
    #     digraph.edge(T_edge_list[i][0], T_edge_list[i][1],
    #                  tailport='s', headport='w', label=T_rate_list[i])

digraph.subgraph(mid)
digraph.subgraph(tail)

# digraph.node('para','parametersasdiafoihafiuh\nsdiuhfbdsugeruagiuergiueriudiufghui')

digraph.attr(label='<<font color="red">&rarr;</font><font color="black">'
             'Concentrating Flow</font><br/><font color="blue">&rarr;</font>'
             '<font color="black">Tailing Flow</font>>', fontcolor='blue')
digraph.render(directory='./', view=True)
