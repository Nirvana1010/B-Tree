#include <iostream>
#include "BTree.h"
#pragma once
using namespace std;

int main()
{
	int i;
	int n;//输入关键字个数
	double order;//要建立的B树的阶数
	char ope;//进行插入或删除操作
	double degree;
	/*cout << "输入关键字个数：";
	cin >> n;
	cout << "输入你要建立的B树阶数：";
	cin >> order;*/
	order = 3;
	degree = order/2;
	CBTree<int> tree(degree);
	/*int* keyw;
	keyw = new int[n];
	for(i = 0; i < n; i++)
	{
		cin >> keyw[i];
		tree.insert(keyw[i]);		
	}*/
	int keyw[10] = {2, 8, 5, 4, 6, 3, 7, 9, 10, 1};
	for(i = 0; i < 10; i++)
	{
		tree.insert(keyw[i]);		
	}
	tree.display();
	while(1)
	{
		cout << "输入你接下来要进行的操作（I/D/E）";//I:插入 D:删除 E;退出
		cin >> ope;
		if(ope == 'I' || ope == 'i')//插入
		{
			while(1)
			{
				int temp;
				if(cin.get() == '#')
				{
					tree.display();
					break;
				}
				cin >> temp;
				tree.insert(temp);
			}
		}
		else if(ope == 'D' || ope == 'd')//删除
		{
			while(1)
			{
				if(cin.get() == '#')
				{
					tree.display();
					break;
				}
				int temp;
				cin >> temp;
				tree.remove(temp);
			}
		}
		else if(ope == 'E' || ope == 'e')//退出
			break;
		else
		{
			cout << "没有对应操作" << endl;
		}
	}
	return 0;
}