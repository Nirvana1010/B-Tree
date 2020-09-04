#include <iostream>
#include "BTree.h"
#pragma once
using namespace std;

int main()
{
	int i;
	int n;//����ؼ��ָ���
	double order;//Ҫ������B���Ľ���
	char ope;//���в����ɾ������
	double degree;
	/*cout << "����ؼ��ָ�����";
	cin >> n;
	cout << "������Ҫ������B��������";
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
		cout << "�����������Ҫ���еĲ�����I/D/E��";//I:���� D:ɾ�� E;�˳�
		cin >> ope;
		if(ope == 'I' || ope == 'i')//����
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
		else if(ope == 'D' || ope == 'd')//ɾ��
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
		else if(ope == 'E' || ope == 'e')//�˳�
			break;
		else
		{
			cout << "û�ж�Ӧ����" << endl;
		}
	}
	return 0;
}