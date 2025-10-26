""" ----------------------------------------------------------------------------
******** Search Code for
******** front & queue
******** 
********
******** Κώδικας για α DFS, BFS , BestFS
******** (επέκταση μετώπου μόνο)
******** Συγγραφέας: Κωνσταντίνος Καλογήρου-Πετράκης & Αντρέας Κουμαράκου
"""

import copy
import sys 

    
sys.setrecursionlimit(10**6)

# ******** Operators
# ******** Τελεστές

def go_to_floor1(state): #κώδικας που ορίζει πως θα κινηθει το ασανσέρ στον 1ο Όροφο
    if state[5] < 8 and state[1] > 0:   #Έλεγχος αν χωράνε 8 άτομα στο ανσανσέρ
        if state[1] > 8 - state[5]:     #Αν δεν χωράνε 8 άτομα στο ανσανσέρ
            new_state = [1] + [state[1] + state[5] - 8] + [state[2]] + [state[3]] + [state[4]] + [8]
        else:
            new_state = [1] + [0] + [state[2]] + [state[3]] + [state[4]] + [state[1] + state[5]]
        return new_state
    else:
        return None



def go_to_floor2(state): #Το ίδιο για τον 2ο
    if state[5] < 8 and state[2] > 0:
        if state[2] > 8 - state[5]:
            new_state = [2] + [state[1]] + [state[2] + state[5] - 8] + [state[3]] + [state[4]] + [8]
        else:
            new_state = [2] + [state[1]] + [0] + [state[3]] + [state[4]] + [state[2] + state[5]]
        return new_state
    else:
        return None


def go_to_floor3(state): #Το ίδιο για τον 3ο
    if state[5] < 8 and state[3] > 0:
        if state[3] > 8 - state[5]:
            new_state = [3] + [state[1]] + [state[2]] + [state[3] + state[5] - 8] + [state[4]] + [8]
        else:
            new_state = [3] + [state[1]] + [state[2]] + [0] + [state[4]] + [state[3] + state[5]]
        return new_state
    else:
        return None


def go_to_floor4(state): #Το ίδιο για τον 4ο
    if state[5] < 8 and state[4] > 0:
        if state[4] > 8 - state[5]:
            new_state = [4] + [state[1]] + [state[2]] + [state[3]] + [state[4] + state[5] - 8] + [8]
        else:
            new_state = [4] + [state[1]] + [state[2]] + [state[3]] + [0] + [state[4] + state[5]]
        return new_state
    else:
        return None


def go_to_floor5(state): #Αλλαγή κώδιακα, στην ταράτσα "αδειάζει" τους ενοίκους όταν είναι γεμάτο το ανσανσέρ ή όταν τελειώσει
    if state[-1] == 8 or state == [state[0]] + [0] + [0] + [0] + [0] +[state[-1]]: #Πρέπει το ανσανσέρ είτε να έχει 8 άτομα μέσα του είτε να είναι ένα βήμα πριν την τελική κατάσταση
        new_state = [5] + [state[1]] + [state[2]] + [state[3]] + [state[4]] + [0]
        return new_state
    else:
        return None

'''
Συνάρτηση εύρεσης απογόνων της τρέχουσας κατάστασης
'''
 
def find_children(state):
    #Δημιουργεία κενής λίστας
    children=[]
    
    # Δημιουργείται ένα βαθύ αντίγραφο του αντικειμένου state και το αποθηκεύει στη μεταβλητή floor_state. Tο ίδιο γίνεται για όλα τα floors
    floor1_state=copy.deepcopy(state)
    floor2_state=copy.deepcopy(state)
    floor3_state=copy.deepcopy(state)
    floor4_state=copy.deepcopy(state)
    floor5_state=copy.deepcopy(state)
      
    # Καλείται η συναρτηση go_to_floor και παιρνά την μεταβλητή floor_state σαν παράμετρο
    floor1_child=go_to_floor1(floor1_state)
    floor2_child=go_to_floor2(floor2_state)
    floor3_child=go_to_floor3(floor3_state)
    floor4_child=go_to_floor4(floor4_state)
    floor5_child=go_to_floor5(floor5_state)
    
    
    #Ελέγχεται αν η μεταβλητή floor_child δεν είναι κενή. Αν η μεταβλητή δεν είναι κενή τότε εκτελείται ο κώδικας μέσα στην συνθήκη όπου προστίθεται η τιμή της floor_child στην λίστα 
    if floor5_child!=None: 
        children.append(floor5_child)
        
    if floor4_child!=None: 
        children.append(floor4_child)
        
    if floor3_child!=None: 
        children.append(floor3_child)
            
    if floor2_child!=None: 
        children.append(floor2_child)
    
    if floor1_child!=None: 
        children.append(floor1_child)
       
        
    return children


def heuristic(state, goal):#Η ευριστική συνάρτηση που χρησιμοποιείται στην μέθοδο bestFS υλοποιείται με την απόσταση Manhattan
    distance = sum(abs(s - g) for s, g in zip(state, goal)) # Υπολογίζει το άθροισμα των απολύτων διαφορών ανά ζεύγος στοιχείων από τις δύο λίστες γνωστό και ως απόσταση Manhatan
    return distance

""" ----------------------------------------------------------------------------
**** FRONT
**** Διαχείριση Μετώπου
"""

""" ----------------------------------------------------------------------------
** initialization of front
** Αρχικοποίηση Μετώπου
"""

def make_front(state):
    return [state]

""" ----------------------------------------------------------------------------
**** expanding front
**** επέκταση μετώπου    
"""

def expand_front(front, method , goal):   
    if method == 'DFS':        #πρώτα σε βάθος αναζήτηση στο μέτωπο
        if front:
            print("Front:")
            print(front)
            node = front.pop(0) #Αφαιρείται το πρώτο στοιχείο της λίστας
            for child in find_children(node):   
                front.insert(0, child)# εκτελεί μια επανάληψη για κάθε παιδί που βρέθηκε από τη συνάρτηση find_children() για τον τρέχοντα κόμβο node. Κάθε παιδί προστίθεται στην αρχή της λίστας front χρησιμοποιώντας τη μέθοδο insert(0, child)
        return front
    
    elif method == 'BFS':   #πρώτα σε πλάτος αναζήτηση στο μέτωπο
        if front:#Ίδιος κώδικας με το DFS. Με αντικατάσταση του insert με το append
            print("Front:")
            print(front)
            node = front.pop(0)#Αφαιρείται το πρώτο στοιχείο της λίστας
            for child in find_children(node):    
                front.append(child) #Εκτελείται μια επανάληψη  η οποία προσθέτει κάθε παιδί που επιστρέφει η συνάρτηση find_children() για τον τρέχοντα κόμβο node στο τέλος της λίστας front, χρησιμοποιώντας τη μέθοδο append().
        return front
    
    elif method == 'BestFS':
        if front:              #Αλγόριθμος πρώτης καλύτερης αναζήτησης στο μέτωπο
            print("Front:")
            print(front)
            node = front.pop(0)#Αφαιρείται το πρώτο στοιχείο της λίστας
            heuristic_values = [heuristic(child, goal) for child in find_children(node)]#Υπολογίζει τις ευρετικές τιμές για όλα τα παιδιά του κόμβου χρησιμοποιώντας την ευριστική συνάρτηση 
            sorted_children = [x for _, x in sorted(zip(heuristic_values, find_children(node)))]#Τα παιδιά του κόμβου node ταξινομούνται με βάση τις ευρετικές τιμές τους. Αυτό γίνεται δημιουργώντας ένα ζευγάρι τιμών με τη μέθοδο zip και ταξινομώντας τα σε αύξουσα σειρά ως προς την ευρετική τιμή.
            for child in sorted_children:#Ταξινομημένα σύμφωνα με την ευρετική τιμή, τα παιδιά προστίθενται στο τέλος της λίστας 
                front.append(child)
        return front
       
""" ----------------------------------------------------------------------------
**** QUEUE
**** Διαχείριση ουράς
"""

""" ----------------------------------------------------------------------------
** initialization of queue
** Αρχικοποίηση ουράς
"""       


def make_queue(state):
   return [[state]]

""" ----------------------------------------------------------------------------
**** expanding queue
**** επέκταση ουράς
"""


def extend_queue(queue, method , goal):
    if method=='DFS':       #πρώτα σε βάθος αναζήτηση στην ουρά
        print("Queue:")
        print(queue)
        node=queue.pop(0)#Αφαιρεί και αποθηκεύει τον πρώτο κόμβο από την ουρά
        queue_copy=copy.deepcopy(queue)#Δημιουργεί μια βαθιά αντιγραφή της ουράς για τη διατήρηση της αρχικής κατάστασης της ουράς
        children=find_children(node[-1])#Εντοπίζει τα παιδιά του τελευταίου κόμβου 
        for child in children:# Μέσα στην επανάληψη δημιουργείται μια καινούρια αντιγραφή του node  και προσθέτει κάθε παιδί ως νέο κόμβο στο τέλος αυτού του μονοπατιού
            path=copy.deepcopy(node)
            path.append(child)
            queue_copy.insert(0,path)
            
        return queue_copy    
    
    
    elif method=='BFS':     #πρώτα σε πλάτος αναζήτηση στην ουρά
        print("Queue:")         #Ίδιος κώδικας με το DFS. Με αντικατάσταση του insert με το append
        print(queue)
        node=queue.pop(0)#Αφαιρεί και αποθηκεύει τον πρώτο κόμβο από την ουρά
        queue_copy=copy.deepcopy(queue)#Δημιουργεί μια βαθιά αντιγραφή της ουράς για τη διατήρηση της αρχικής κατάστασης της ουράς
        children=find_children(node[-1])#Εντοπίζει τα παιδιά του τελευταίου κόμβου 
        for child in children:# Μέσα στην επανάληψη δημιουργείται μια καινούρια αντιγραφή του node  και προσθέτει κάθε παιδί ως νέο κόμβο στο τέλος αυτού του μονοπατιού
            path=copy.deepcopy(node)
            path.append(child)
            queue_copy.append(path)
    
        return queue_copy 
   
    elif method == 'BestFS':    #Αλγόριθμος πρώτης καλύτερης αναζήτησης στην ουρά
        print("Queue:")
        print(queue)
        node = queue.pop(0)#Αφαιρεί και αποθηκεύει τον πρώτο κόμβο από την ουρά
        queue_copy = copy.deepcopy(queue)#Δημιουργεί μια βαθιά αντιγραφή της ουράς για τη διατήρηση της αρχικής κατάστασης της ουράς
        children = find_children(node[-1])#Εντοπίζει τα παιδιά του τελευταίου κόμβου 
        heuristic_values = [heuristic(child, goal) for child in children]#Υπολογίζει τις ευρετικές τιμές για όλα τα παιδιά του κόμβου χρησιμοποιώντας την ευριστική συνάρτηση
        sorted_children = [x for _, x in sorted(zip(heuristic_values, children))]#Τα παιδιά του κόμβου node ταξινομούνται με βάση τις ευρετικές τιμές τους. Αυτό γίνεται δημιουργώντας ένα ζευγάρι τιμών με τη μέθοδο zip και ταξινομώντας τα σε αύξουσα σειρά ως προς την ευρετική τιμή.
        for child in sorted_children:# Μέσα στην επανάληψη δημιουργείται μια νέα αντίγραφη του μονοπατιού και προσθέτει κάθε παιδί ως νέο κόμβο στο τέλος αυτού του μονοπατιού
            path = copy.deepcopy(node)
            path.append(child)
            queue_copy.append(path)
        return queue_copy# Επιστρέφει τη νέα αντιγραμμένη ουρά



def find_solution(front, queue, closed, goal, method):
    # def find_solution(front, closed, method):
    
    if not front:
        print('_NO_SOLUTION_FOUND_')
    elif front[0] in closed:
        new_front = copy.deepcopy(front)
        new_front.pop(0)
        new_queue = copy.deepcopy(queue)
        new_queue.pop(0)
        find_solution(new_front, new_queue, closed, goal, method)
        # find_solution(new_front, new_queue, closed, method)
    elif front[0] == goal:  # elif is_goal_state(front[0]):
        print('_GOAL_FOUND_')
        # print(front[0])
        print(queue[0])
    else:
        closed.append(front[0])
        front_copy = copy.deepcopy(front)
        front_children = expand_front(front_copy, method , goal)
        queue_copy = copy.deepcopy(queue)
        queue_children = extend_queue(queue_copy, method , goal )
        closed_copy = copy.deepcopy(closed)
        find_solution(front_children, queue_children, closed_copy, goal, method)
        # find_solution(front_children, queue_children, closed_copy, method)
        
        
def main():
    initial_state = [0, 9, 4, 12, 7, 0] #Αρχική κατάσταση όπως αναφέρεται στην εργασία
    """ ----------------------------------------------------------------------------
    **** [όροφος ασανσέρ, ένοικοι 1ου, ένοικοι 2ου, ένοικοι 3ου, ένοικοι 4ου, άτομα στο ασανσέρ]
    """
    goal = [5, 0, 0, 0, 0, 0]   #Τελική κατάσταση
    print("1. Επιλογή DFS")
    print("2. Επιλογή BFS")
    print("3. Επιλογή  BestFS")
    print("4. Exit")

    while True: #Το menu επιλογής
        method = input("Παρακαλώ εισάγετε την επιλογή σας: ")

        if method == 'DFS':
            print("Επιλέχθηκε η Επιλογή DFS")
            print('____BEGIN__SEARCHING____')
            find_solution(make_front(initial_state), make_queue(initial_state), [], goal, method)
        elif method == 'BFS':
            print("Επιλέχθηκε η Επιλογή BFS")
            print('____BEGIN__SEARCHING____')
            find_solution(make_front(initial_state), make_queue(initial_state), [], goal, method)
        elif method == 'BestFS':
            print("Επιλέχθηκε η Επιλογή BestFS")
            print('____BEGIN__SEARCHING____')
            find_solution(make_front(initial_state), make_queue(initial_state), [], goal, method)
        elif method == 'Exit':
            print("Επιλέξατε να βγείτε.")
            break
        else:
            print("Μη έγκυρη επιλογή. Παρακαλώ επιλέξτε ξανά.")

if __name__ == "__main__":
    main()