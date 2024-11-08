#include <iostream>
#include <algorithm>
#define MAX_SIZE 1000+1

using namespace std;

class Node {
public:
	int key;
	Node* left;
	Node* right;
	int height = 0;
	Node(int key = NULL, Node* left = nullptr, Node* right = nullptr) : key(key), left(left), right(right) {};
};
class Stack {
	int numOfEntires = 0;
	int top = -1;
	Node* a[MAX_SIZE] = {};
public:
	void push(Node* newNode) {
		++numOfEntires;
		a[++top] = newNode;
	}
	Node* peek() {
		return a[top];
	}
	Node* pop() {
		--numOfEntires;
		return a[top--];
	}
	bool empty() {
		if (numOfEntires == 0) return true;
		else return false;
	}
};

// 최대값 반환
int max(int x, int y) {
	return (x >= y) ? x : y;
}
// 해당 노드의 height 반환
int __height(Node* node) {
	return node->height;
}
// 자신을 루트로 하는 트리의 모든 노드의 개수 반환
int __size(Node* node) {
	int cnt = 0;
	if (node != nullptr)
		cnt = 1 + __size(node->left) + __size(node->right);
	return cnt;
}
// 최대 키 노드 반환
Node* maxNode(Node* node, Stack& stack) {
	while (node->right != nullptr) {
		stack.push(node);
		node = node->right;
	}
	return node;
}
// 최소 키 노드 반환
Node* minNode(Node* node, Stack& stack) {
	while (node->left != nullptr) {
		stack.push(node);
		node = node->left;
	}
	return node;
}

// BST에 새로운 노드 생성
Node* getNodeBST() {
	Node* newNode = new Node();
	return newNode;
}
// BST 삽입 메서드
bool __insertBST(Node*& T, int newKey) {
	Node* q = NULL; Node* p = T; // q는 p의 부모 노드

	Stack stack;

	// step1 : newKey의 삽입 위치 q를 찾음
	while (p != nullptr) {
		if (newKey == p->key) // 이미 존재하면 true 반환
			return true;
		q = p;
		stack.push(q);
		if (newKey < p->key) p = p->left;
		else p = p->right;
	}

	// step2 : newKey를 갖는 노드 생성
	Node* newNode = getNodeBST();
	newNode->key = newKey;

	// step3 : q의 자식으로 newKey 삽입
	if (T == nullptr) T = newNode;
	else if (newKey < q->key) q->left = newNode;
	else q->right = newNode;

	// step4 : height 업데이트
	while (!stack.empty()) {
		q = stack.pop();
		q->height = 1 + max(q->left ? q->left->height : 0, q->right ? q->right->height : 0);
	}

	return false;
}
// BST 삭제 메서드
bool __eraseBST(Node*& T, int deleteKey) {
	Node* p = T; Node* q = nullptr;

	Stack stack;

	while (p != nullptr && deleteKey != p->key) {
		q = p;
		stack.push(q);

		if (deleteKey < p->key) p = p->left;
		else p = p->right;
	}

	// T에 삭제할 키가 없는 경우
	if (p == nullptr)
		return true;

	// p의 차수가 2인 경우 --> 0 혹은 1로 줄이기
	if (p->left != nullptr && p->right != nullptr) {
		stack.push(p);
		Node* tempNode = p;
		if (__height(p->left) > __height(p->right)) {
			// 왼쪽 서브 트리에서 최대 키 노드
			p = maxNode(p->left, stack);
		}
		else if (__height(p->left) < __height(p->right)) {
			// 오른쪽 서브 트리에서 최소 키 노드
			p = minNode(p->right, stack);
		}
		else {
			if (__size(p->left) >= __size(p->right)) {
				// 왼쪽 서브 트리에서 최대 키 노드
				p = maxNode(p->left, stack);
			}
			else {
				// 오른쪽 서브 트리에서 최소 키 노드
				p = minNode(p->right, stack);
			}
		}
		tempNode->key = p->key;
		q = stack.peek();
	}

	// p의 차수가 0인 경우
	if (p->left == nullptr && p->right == nullptr) {
		if (q == nullptr) T = nullptr; // 루트 노드를 삭제하는 경우
		else if (q->left == p) q->left = nullptr;
		else q->right = nullptr;
	}
	// p의 차수가 1인 경우
	else {
		if (p->left != nullptr) {
			if (q == nullptr) T = T->left; // 루트 노드를 삭제하는 경우
			else if (q->left == p) q->left = p->left;
			else q->right = p->left;
		}
		else {
			if (q == nullptr) T = T->right; // 루트 노드를 삭제하는 경우
			else if (q->left == p) q->left = p->right;
			else q->right = p->right;
		}
	}

	delete p;

	// height 업데이트
	while (!stack.empty()) {
		q = stack.pop();
		q->height = 1 + max(q->left ? q->left->height : 0, q->right ? q->right->height : 0);
	}

	return false;
}
// BST 메모리 삭제 메서드
void __clearBST(Node* T) {
	if (T->left != nullptr) {
		__clearBST(T->left);
	}
	if (T->right != nullptr) {
		__clearBST(T->right);
	}
	free(T);
}
// 트리 순회 메서드
void __inOrder(Node* root) {
	if (root == NULL)
		return;
	else {
		cout << "<";
		__inOrder(root->left);
		cout << ' ' << root->key << ' ';
		__inOrder(root->right);
		cout << ">";
	}
}

class BST {
public:
	Node* root;
	BST() : root(nullptr) {};
public:
	int height() const {
		return __height(root);
	}
public:
	int size() const {
		return __size(root);
	}
public:
	void inOrder() const {
		return __inOrder(root);
	}
public:
	bool insert(const int key) {
		return __insertBST(root, key);
	}
	bool erase(const int deleteKey) {
		return __eraseBST(root, deleteKey);
	}
	void clear() {
		return __clearBST(root);
	}
};
int main() {

	BST T;

	while (true) {
		char command; int key;
		cin >> command >> key;

		if (cin.eof())
			break;

		if (command == 'i') {
			if (T.insert(key)) {
				cout << command << " " << key << ": The key already exists" << endl;
			}
			else {
				T.inOrder();
				cout << endl;
			}
		}
		if (command == 'd') {
			if (T.erase(key)) {
				cout << command << " " << key << ": The key does not exist" << endl;
			}
			else {
				T.inOrder();
				cout << endl;
			}
		}
	}
	T.clear();

	return 0;
}
