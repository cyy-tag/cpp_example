//define binary tree structure and traverse helpers:
struct Node {
  int value;
  Node* left;
  Node* right;
  Node(int i=0) : value(i), left(nullptr), right(nullptr) {
  }
};

//tracerse tree, using fold expression
template<typename T, typename... TP>
Node* traverse( T np, TP... paths) {
  return (np ->* ... ->* paths); //np ->*paths1 ->* paths2
}