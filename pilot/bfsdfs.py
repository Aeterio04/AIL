
class node(object):

    def __init__(self, data):
        self.city = data
        self.edges = []
        self.visited=0
    def show(self):
        print(self.city)
    


def edge(source,destination):
    source.edges.append(destination)
    destination.edges.append(source)


def dfs(l):
    dfsnode(l[0],l)
    if(len(l)!=0):
        dfs(l)
def dfsnode(start_node,l):
    if start_node==None:
        return
    else:
        print(start_node.city)
        l.remove(start_node)
        for i in start_node.edges:
            if (i.visited==0):
                i.visited=1
            else:
                continue
            i.edges.remove(start_node)
            dfsnode(i,l)

def bfs(l):
    root=l[0]
    q=[root]
    while (len(q)!=0):
        print(q[0].city)
        l.remove(q[0])
        for i in q[0].edges:
            if(i.visited==0):
                i.visited=1
            else:
                continue
            i.edges.remove(q[0])
            q.append(i)
        q.pop(0)
    if(len(l)!=0):
        bfs(l)
    

node1 = node("bombay")
node2 = node("delhi")
node3= node("kolkata")
node4 = node("chennai")
node5 = node("pune")
node6= node("bengaluru")
node7= node("ahmedabad")
edge(node1,node5)
edge(node2,node3)  
edge(node2,node4)
edge(node2,node6)
edge(node1,node7)

# for i in node2.edges:
#     print(i.city)

l=[node3,node2,node4,node1,node5,node6,node7]
dfs(l)
# b0fs(node2)