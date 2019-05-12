#pragma once
#include <functional>

template <class T>
struct Node
{
	Node* l;
	Node* r;
	T info;
};

template <class T>
class Tree
{
private:
	Node<T>* root;//Корень
	std::function<bool(T, T)> func;//Полиморфная обёртка функции добавления/сортировки
	
	void srthelp(Tree<T>& tmpTree, Node<T>* ptr);//Рекурсивная сортировка
	template<class T1>
	bool srchhelp(Tree<T>& tmpTree, const std::function<bool(T1, T)>& func, const T1& key, Node<T>* ptr);//Рекурсивный поиск
	Node<T>* copyTree(Node<T>* ptr);//Рекурсивное копирование
	void print(Node<T>* p, int& index);//Рекурсивный вывод
	void size(int& sz, Node<T>* ptr);//Рекурсивное вычисление размера
	T getElement(int index, Node<T>* ptr, T& element, bool& flag = false, int& counter = 0);
	void clearTree(Node<T>* ptr);//Рекурсивная очистка дерева
public:
	Tree() :root(nullptr), func([](T a, T b) {return a < b; })
	{
#ifdef DEBUG
		std::cout << "Tree constructor" << std::endl;
#endif // DEBUG
	}
	Tree(const Tree<T>& other)
	{
		this->root = copyTree(other.root);
		this->func = other.func;
	}
	~Tree()
	{
		clearTree(root);
#ifdef DEBUG
		std::cout << "Tree destructor" << std::endl;
#endif // DEBUG
	}
	Tree& operator=(const Tree& obj);
	bool isEmpty();
	int size();
	void push(T element);
	T getElement(int index);
	void erase(int index);
	void print();
	void sort(std::function<bool(T, T)> func);
	template<class T1>
	Tree<T> search(T1 key, std::function<bool(T1, T)> func);
};

template<class T>
Tree<T>& Tree<T>::operator=(const Tree<T>& obj)
{
	this->clearTree(root);
	this->root = nullptr;
	this->root = const_cast<Tree&>(obj).copyTree(obj.root);
	this->func = const_cast<Tree&>(obj).func;
	return *this;
}

template <class T>
bool Tree<T>::isEmpty()
{
	return !root;
}

template <class T>
int Tree<T>::size()
{
	int sz = 0;
	size(sz, root);
	return sz;
}

template <class T>
void Tree<T>::size(int& sz, Node<T>* ptr)
{
	if (ptr)
	{
		size(sz, ptr->l);
		size(sz, ptr->r);
		sz++;
	}
}

template<class T>
void Tree<T>::push(T element)
{

	if (this->isEmpty())
	{
		root = new Node<T>;
		root->l = nullptr;
		root->r = nullptr;
		root->info = element;
	}
	else
	{
		Node<T> *t, *p;
		t = p = root;
		while (t != nullptr)
		{
			p = t;
			if (func(element, t->info))
				t = t->l;
			else
				t = t->r;
		}
		t = new Node<T>;
		t->l = nullptr;
		t->r = nullptr;
		t->info = element;
		if (func(element, p->info))
			p->l = t;
		else
			p->r = t;
	}
}

template<class T>
T Tree<T>::getElement(int index, Node<T>* ptr, T& element, bool& flag, int& counter)
{
	if (flag)
		return element;
	if (ptr)
	{
		getElement(index, ptr->l, element, flag, counter);
		if (flag)
			return element;
		++counter;
		if (counter == index)
		{
			element = ptr->info;
			flag = true;
		}
		getElement(index, ptr->r, element, flag, counter);
	}
	return element;
}

template<class T>
T Tree<T>::getElement(int index)
{
	T element = T();
	if (index >= size())
		return element;
	bool flag = false;
	int counter = 0;
	element = getElement(index, root, element, flag, counter);
	return element;
}

template<class T>
void Tree<T>::erase(int index)
{
	if (this->isEmpty())
	{
		std::cout << "Tree is empty!";
		return;
	}
	T key = getElement(index);
	Node<T> *t, *p, *s;
	if (root->info == key)
	{
		t = root->l;
		p = root->r;
		delete root;
		if (p != nullptr)
		{
			s = p;
			while (p->l != nullptr)
				p = p->l;
			p->l = t;
			root = s;
			return;
		}
		else
			root = t;
	}
	else
	{
		t = p = root;
		while (t != nullptr)
		{
			if (key == t->info)
			{

				if (t->r != nullptr)
				{
					s = t->r;
					while (s->l != nullptr)
						s = s->l;
					s->l = t->l;

				}
				else
					s = t->l;
				delete t;
				t = nullptr;
				if (func(key, p->info))
					p->l = s;
				else
					p->r = s;
			}
			else
			{
				p = t;
				if (func(key, t->info))
					t = t->l;
				else
					t = t->r;
			}
		}
	}
}

template<class T>
void Tree<T>::clearTree(Node<T>* ptr)
{
	if (ptr)
	{
		clearTree(ptr->l);
		clearTree(ptr->r);
		delete ptr;
	}
}

template<class T>
void Tree<T>::srthelp(Tree<T>& tmpTree, Node<T>* ptr)
{
	if (ptr)
	{
		srthelp(tmpTree, ptr->l);
		srthelp(tmpTree, ptr->r);
		tmpTree.push(ptr->info);
	}
}

template<class T>
void Tree<T>::sort(std::function<bool(T, T)> func)
{
	Tree<T> tmpTree;
	tmpTree.func = func;
	this->func = func;
	if (this->isEmpty())
	{
		std::cout << "Tree is Empty!" << std::endl;
		return;
	}
	srthelp(tmpTree, root);
	*this = tmpTree;
}

template<class T>
template<class T1>
bool Tree<T>::srchhelp(Tree<T>& tmpTree, const std::function<bool(T1, T)>& func, const T1& key, Node<T>* ptr)
{
	if (ptr)
	{
		srchhelp(tmpTree, func, key, ptr->l);
		srchhelp(tmpTree, func, key, ptr->r);
		if (func(key, ptr->info))
			tmpTree.push(ptr->info);
	}
	return true;
}

template<class T>
template<class T1>
Tree<T> Tree<T>::search(T1 key, std::function<bool(T1, T)> func)
{
	Tree<T> tmpTree;
	srchhelp(tmpTree, func, key, this->root);
	return tmpTree;
}


template<class T>
void Tree<T>::print(Node<T> * ptr, int& index)
{
	if (ptr) {
		print(ptr->l, index);
		index++;
		std::cout << "|";
		std::cout.width(2);
		std::cout << index << " " << ptr->info << std::endl;
		print(ptr->r, index);
	}
}


template<class T>
void Tree<T>::print()
{
	int arg = 0;
	if (this->isEmpty())
		std::cout << "Tree is Empty!" << std::endl;
	else
		this->print(root, arg);
}

template<class T>
Node<T>* Tree<T>::copyTree(Node<T>* ptr)
{
	if (ptr == nullptr)
		return nullptr;
	Node<T>* newNode = new Node<T>;
	newNode->info = ptr->info;
	newNode->l = copyTree(ptr->l);
	newNode->r = copyTree(ptr->r);
	return newNode;
}

