# ordered_list
This is an implementation of an ordered singly linked list with node-level locks to
ensure mutual exclusion. Additional implementations of quick pop and quick push are included.
Quick pop and quick push inherit from the base class orderedlist and provide an override for 
pop and push operations with O(1) time complexity, respectively.
