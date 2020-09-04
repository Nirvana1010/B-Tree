static int f = 0;

template<class T>
class CBTree
{
private:
	double M;                  //B������С����
	int KEY_MAX;        //�ڵ�����ؼ��ֵ�������
	int KEY_MIN;          //�Ǹ��ڵ�����ؼ��ֵ���С����
	int CHILD_MAX;  //���ӽڵ��������
	int CHILD_MIN;  //���ӽڵ����С����
public:
	struct Node
	{
		bool isLeaf;             //�Ƿ���Ҷ�ӽڵ�
		int keyNum;              //�ڵ�����Ĺؼ�������
		T keyValue[100];     //�ؼ��ֵ�ֵ����
		Node *pChild[100]; //����ָ������
		Node *pParent;      //���ڵ�
  
		Node(bool b = true, int n = 0)
			:isLeaf(b), keyNum(n), pParent(NULL){}
	};
private:
	Node * m_pRoot;  //B���ĸ��ڵ�
public:
	CBTree(double n)
	{
		M = n+0.5;                  //B������С����
		KEY_MAX = 2*M-2;        //�ڵ�����ؼ��ֵ�������
		KEY_MIN = M-1;          //�Ǹ��ڵ�����ؼ��ֵ���С����
		CHILD_MAX = KEY_MAX+1;  //���ӽڵ��������
		CHILD_MIN = KEY_MIN+1;  //���ӽڵ����С����
		m_pRoot = NULL;  //����һ�ÿյ�B��
		cout << "M:" << M << endl;
		cout << "max:" << KEY_MAX << endl;
		cout << "min:" << KEY_MIN << endl;
	}
 
	~CBTree()
	{
		clear();
	}
 
	bool insert(const T &key)    //��B���в����½��key
	{
		if (contain(key))  //���ùؼ����Ƿ��Ѿ�����
		{
			cout << "���ظ��Ĺؼ���" << endl;
			return false;
		}
		else
		{
			if (m_pRoot==NULL)//����Ƿ�Ϊ����
			{
				m_pRoot = new Node();
			}
			if((KEY_MAX % 2) != 0)//���������ؼ���
			{
				if (m_pRoot->keyNum == KEY_MAX) //�����ڵ��Ƿ�����
				{
					Node *pNode = new Node();  //�����µĸ��ڵ�
					pNode->isLeaf = false;
					pNode->pChild[0] = m_pRoot;
					splitChild(pNode, 0, m_pRoot);   //���ѽڵ�
					m_pRoot = pNode;  //���¸��ڵ�ָ��
				}
				insert_evenTree(m_pRoot, key);
			}
			else if((KEY_MAX % 2) == 0)//��ż�����ؼ���
			{
				insert_oddTree(m_pRoot, key);
			}
			return true;
		}
	}
 
	bool remove(const T &key)    //��B��ɾ�����key
	{
		if (!search(m_pRoot, key))  //key������
		{
			cout << "δ�ҵ���Ӧ�Ĺؼ���" << endl;
			return false;
		}
		if (m_pRoot->keyNum==1)//������������ڵ���ֻ��һ���ؼ���
		{
			if (m_pRoot->isLeaf)//��Ҷ�ڵ�
			{
				clear();
				return true;
			}
			else//������Ҷ�ڵ�ʱ�������ӽڵ�
			{
				Node *pChild1 = m_pRoot->pChild[0];
				Node *pChild2 = m_pRoot->pChild[1];
				if (pChild1->keyNum == KEY_MIN && pChild2->keyNum == KEY_MIN)//���ڵ���Ҫ����������
				{
					mergeChild(m_pRoot, 0);
					deleteNode(m_pRoot);
					m_pRoot = pChild1;
				}
			}
		}
		recursive_remove(m_pRoot, key);
		return true;
	}

	void display()const //��ӡ���Ĺؼ���
	{
		displayInConcavo(m_pRoot,KEY_MAX*15);
	}

	bool contain(const T &key)const   //����key�Ƿ������B����
	{
		return search(m_pRoot, key);
	}

	void clear()                      //���B��
	{
		recursive_clear(m_pRoot);
		m_pRoot = NULL;
	}

private:
	void recursive_clear(Node *pNode)//ɾ����
	{
		if (pNode != NULL)
		{
			if (!pNode->isLeaf)
			{
				for(int i = 0; i <= pNode->keyNum; i++)
					recursive_clear(pNode->pChild[i]);
			}
			deleteNode(pNode);
		}
	}
 
	void deleteNode(Node *&pNode)//ɾ���ڵ�
	{
		if (pNode != NULL)
		{
			delete pNode;
			pNode = NULL;
		}
	}
 
	bool search(Node *pNode, const T &key)const //���ҹؼ���
	{
		if (pNode==NULL)  //���ڵ�ָ���Ƿ�Ϊ�գ���ýڵ��Ƿ�ΪҶ�ӽڵ�
		{
			return false;
		}
		else
		{
			int i;
			for (i = 0; i < pNode->keyNum && key > pNode->keyValue[i]; i++)
			{//�ҵ�ʹkey <= pNode->keyValue[i]��������С�±�i
			}
			if (i < pNode->keyNum && key == pNode->keyValue[i])//�ڵ�ǰ�ڵ����ҵ��ؼ���
			{
				return true;
			}
			else
			{
				if (pNode->isLeaf)   //���ýڵ��Ƿ�ΪҶ�ڵ㣬��Ҷ�ڵ�˵���ؼ��ֲ�������
				{
					return false;
				}
				else
				{
					return search(pNode->pChild[i], key);//���ӽڵ��м�������
				}
			}
		}
	}
 	
	void splitChild(Node *Parent, int nChildIndex, Node *pChild)  //�����ӽڵ�
	{
		//��pChild���ѳ�pLeftNode��pChild�����ڵ�
		Node *pRightNode = new Node();//���Ѻ���ҽڵ�
		pRightNode->isLeaf = pChild->isLeaf;//�Ƿ�ΪҶ�ڵ�
		pRightNode->keyNum = KEY_MIN;
		int i;
		for (i = 0; i < KEY_MIN; i++)//�����ؼ��ֵ�ֵ
		{
			pRightNode->keyValue[i] = pChild->keyValue[i+CHILD_MIN];
		}
		if (!pChild->isLeaf)  //�������Ҷ�ӽڵ㣬�������ӽڵ�ָ��
		{
			for (i = 0; i < CHILD_MIN; i++)
			{
				pRightNode->pChild[i] = pChild->pChild[i+CHILD_MIN];
			}
		}
		
		pChild->keyNum = KEY_MIN;  //�����������Ĺؼ��ָ���
 
		for (i = Parent->keyNum; i > nChildIndex; i--)//�����ڵ��е�nChildIndex������йؼ��ֵ�ֵ������ָ�������һλ
		{
			Parent->pChild[i+1] = Parent->pChild[i];
			Parent->keyValue[i] = Parent->keyValue[i-1];
		}
		Parent->keyNum++;  //���¸��ڵ�Ĺؼ��ָ���
		Parent->pChild[nChildIndex+1] = pRightNode;  //�洢������ָ��
		pRightNode->pParent = Parent;
		pChild->pParent = Parent;//�洢���ӽڵ�ĸ��ڵ�
		Parent->keyValue[nChildIndex] = pChild->keyValue[KEY_MIN];//�ѽڵ���м�ֵ�ᵽ���ڵ�

		if(!pRightNode->isLeaf)//�����ҽڵ���ӽڵ�ĸ�ָ��
		{
			for (i = 0; i < CHILD_MIN; i++)
			{
				pRightNode->pChild[i]->pParent = pRightNode;
			}
		}
	}
 
	void insert_evenTree(Node *pNode, const T &key)//�ڷ����ڵ��в���ؼ���
	{
		int i = pNode->keyNum;  //��ȡ�ڵ��ڹؼ��ָ���
		if (pNode->isLeaf)      //pNode��Ҷ�ӽڵ�
		{
			while (i > 0 && key < pNode->keyValue[i-1])   //�Ӻ���ǰ�����ҹؼ��ֵĲ���λ��
			{
				pNode->keyValue[i] = pNode->keyValue[i-1];  //�����λ
				i--;
			}
			pNode->keyValue[i] = key;  //����ؼ��ֵ�ֵ
			pNode->keyNum++; //���½ڵ�ؼ��ֵĸ���
		}
		else//pNode���ڲ��ڵ�
		{
			while(i > 0 && key < pNode->keyValue[i-1])   //�Ӻ���ǰ�����ҹؼ���Ҫ���������
				i--;
			Node *pChild = pNode->pChild[i];  //Ŀ���������ָ��
			if (pChild->keyNum == KEY_MAX)  //�����ڵ�����
			{
				splitChild(pNode, i, pChild);//���������ڵ�
				if(key > pNode->keyValue[i])   //Ѱ��Ҫ�������������ǰ��������һ������
					pChild = pNode->pChild[i+1];
			}
			insert_evenTree(pChild, key);  //����ؼ��ֵ�Ŀ�������ڵ�
		}
	}
 
	void displayInConcavo(Node *pNode, int count)const//��ӡ��
	{
		if (pNode != NULL)
		{
			int i, j;
			for (i = 0; i < pNode->keyNum; i++)//�����ӡ�ڵ�ؼ���
			{
				if (!pNode->isLeaf)//����Ҷ�ڵ�ʱ���ݹ�����ӽڵ�
				{
					f++;
					displayInConcavo(pNode->pChild[i], count-4);				
				}
				for (j = count; j >= 0; j--)
				{
					cout<<"-";
				}
				//cout << pNode->keyValue[i] << endl;
				if(pNode->pParent != NULL)
					cout << pNode->keyValue[i] << "(" << pNode->pParent->keyValue[0] << ")" << endl;
				else 
					cout << pNode->keyValue[i] << "(NULL)" << endl;
			}
			if (!pNode->isLeaf)
			{
				displayInConcavo(pNode->pChild[i], count-4);
			}
		}
	}
	
	void mergeChild(Node *pParent, int index)//�ϲ������ӽڵ�
	{
		Node *pChild1 = pParent->pChild[index];
		Node *pChild2 = pParent->pChild[index+1];
		//��pChild2���ݺϲ���pChild1
		pChild1->keyNum = KEY_MAX;
		pChild1->keyValue[KEY_MIN] = pParent->keyValue[index];//�����ڵ�index��ֵ����
		int i;
		for (i = 0; i < KEY_MIN; i++)//�����ֵܽڵ��ֵ�ϲ������ӽڵ���
		{
			pChild1->keyValue[i+KEY_MIN+1] = pChild2->keyValue[i];
		}
		if (!pChild1->isLeaf)//���ϲ��Ľڵ����ڲ��ڵ�
		{
			for (i = 0; i < CHILD_MIN; i++)
			{
				pChild1->pChild[i+CHILD_MIN] = pChild2->pChild[i];//�����ֵܽڵ��ָ��ϲ������ӽڵ���
			}
		}
 
		//���ڵ�ɾ��index��key��index�����ǰ��һλ
		pParent->keyNum--;
		for(i = index; i < pParent->keyNum; i++)
		{
			pParent->keyValue[i] = pParent->keyValue[i+1];
			pParent->pChild[i+1] = pParent->pChild[i+2];
		}
		deleteNode(pChild2);  //ɾ��pChild2
	}
 
	//�ݹ��ɾ���ؼ���
    void recursive_remove(Node *pNode, const T &key)
	{
		int i = 0;
		while(i < pNode->keyNum && key > pNode->keyValue[i])//Ѱ��Ҫɾ����key���ڵ�Ŀ��ڵ�
			i++;
		if (i < pNode->keyNum && key == pNode->keyValue[i])//�ؼ���key�ڽڵ�pNode��
		{
			if (pNode->isLeaf)//pNode�Ǹ�Ҷ�ڵ�
			{
				//��pNode��ɾ��k
				pNode->keyNum--;
				for (; i < pNode->keyNum; i++)//��i����Ĺؼ���ǰ��һλ
				{
					pNode->keyValue[i] = pNode->keyValue[i+1];
				}
				return ;
			}
			else//pNode�Ǹ��ڲ��ڵ�
			{
				Node *pChildPrev = pNode->pChild[i];//�ڵ�pNode��keyǰ����ӽڵ�
				Node *pChildNext = pNode->pChild[i+1];//�ڵ�pNode��key������ӽڵ�
				if (pChildPrev->keyNum >= CHILD_MIN)//�ڵ�pChildPrev�����ٰ���CHILD_MIN���ؼ���
				{
					T prevKey = getPredecessor(pChildPrev); //��ȡkey��ǰ���ؼ���
				    recursive_remove(pChildPrev, prevKey);//�ڽڵ���ɾȥkey��ǰ��
					pNode->keyValue[i] = prevKey;     //��key�滻��key��ǰ���ؼ���
					return ;
				}
				else if (pChildNext->keyNum >= CHILD_MIN)//�ڵ�pChildNext�����ٰ���CHILD_MIN���ؼ���
				{
					T nextKey = getSuccessor(pChildNext); //��ȡkey�ĺ�̹ؼ���
					recursive_remove(pChildNext, nextKey);//�ڽڵ���ɾȥkey�ĺ��
					pNode->keyValue[i] = nextKey;     //�滻��key�ĺ�̹ؼ���
					return ;
				}
				else//�ڵ�pChildPrev��pChildNext�ж�ֻ����CHILD_MIN-1���ؼ���
				{
					mergeChild(pNode, i);//�ϲ��ڵ㣬�����ڵ㡰��������
					recursive_remove(pChildPrev, key);//�ںϲ���Ľڵ���ɾȥkey
				}
			}
		}
		else//�ؼ���key���ڽڵ�pNode��
		{
			Node *pChildNode = pNode->pChild[i];//����key���������ڵ�
			if (pChildNode->keyNum == KEY_MIN)//ֻ��t-1���ؼ��֣���Ҫ���ֵܽڵ㡰�衱һ���ؼ���
			{
				Node *pLeft = i > 0 ? pNode->pChild[i-1] : NULL;  //���ֵܽڵ�
				Node *pRight = i < pNode->keyNum ? pNode->pChild[i+1] : NULL;//���ֵܽڵ�
				int j;
				if (pLeft && pLeft->keyNum >= CHILD_MIN)//���ֵܽڵ�������CHILD_MIN���ؼ���
				{
					//���ڵ���i-1�Ĺؼ���������pChildNode��
					for (j = pChildNode->keyNum; j > 0; j--)  
					{
						pChildNode->keyValue[j] = pChildNode->keyValue[j-1];
					}
					pChildNode->keyValue[0] = pNode->keyValue[i-1];
					
					if (!pLeft->isLeaf)  
					{
						for (j = pChildNode->keyNum+1; j > 0; j--) //pLeft�ڵ��к��ʵ���Ůָ����ֲ��pChildNode��
						{
							pChildNode->pChild[j] = pChildNode->pChild[j-1];
						}
						pChildNode->pChild[0] = pLeft->pChild[pLeft->keyNum];
					}
					pChildNode->keyNum++;
					pNode->keyValue[i] = pLeft->keyValue[pLeft->keyNum-1];//pLeft�ڵ��е����ؼ���������pNode��
					pLeft->keyNum--;
				}
				else if (pRight && pRight->keyNum >= CHILD_MIN)//���ֵܽڵ�������CHILD_MIN���ؼ���
				{
					//���ڵ���i�Ĺؼ���������pChildNode��
					pChildNode->keyValue[pChildNode->keyNum] = pNode->keyValue[i];
					pChildNode->keyNum++;
					pNode->keyValue[i] = pRight->keyValue[0];//pRight�ڵ��е���С�ؼ���������pNode��
					pRight->keyNum--;
					for (j=0; j<pRight->keyNum; ++j)//�����ֵܽڵ��еĹؼ���ǰ��
					{
						pRight->keyValue[j] = pRight->keyValue[j+1];
					}
					if (!pRight->isLeaf)  
					{
						pChildNode->pChild[pChildNode->keyNum] = pRight->pChild[0];//pRight�ڵ��к��ʵ���Ůָ����ֲ��pChildNode��
					    for (j=0; j<=pRight->keyNum; ++j)
					    {
							pRight->pChild[j] = pRight->pChild[j+1];
					    }
					}
				}
				//�����ֵܽڵ㶼ֻ����CHILD_MIN-1���ڵ�
				else if (pLeft)//�����ֵܺϲ�
				{
					mergeChild(pNode, i-1);
					pChildNode = pLeft;
				}
				else if (pRight)//�����ֵܺϲ�
				{
					mergeChild(pNode, i);
				}
			}
			recursive_remove(pChildNode, key);
		}
	}
 
	T getPredecessor(Node *pNode)//�ҵ�ǰ���ؼ���
	{
		while (!pNode->isLeaf)
		{
			pNode = pNode->pChild[pNode->keyNum];
		}
		return pNode->keyValue[pNode->keyNum-1];
	}
 
	T getSuccessor(Node *pNode)//�ҵ���̹ؼ���
	{
		while (!pNode->isLeaf)
		{
			pNode = pNode->pChild[0];
		}
		return pNode->keyValue[0];
	}

	void insert_oddTree(Node *pNode, const T &key)//��������B���в���ؼ���
	{
		int i = pNode->keyNum;
		int j = pNode->keyNum;
		int k = 0;
		int new_key;
		if(pNode->isLeaf)//Ҫ����Ľڵ���Ҷ�ڵ�
		{
			if(pNode->keyNum == KEY_MAX && pNode->pParent != NULL)//���Ҷ�ڵ�����������ڵ㲻�Ǹ��ڵ�
			{
				Node *temp = pNode;
				int count = 0;

				//��key����Ҷ�ڵ�
				while (j > 0 && key < pNode->keyValue[j-1])   //�Ӻ���ǰ�����ҹؼ��ֵĲ���λ��
				{
					pNode->keyValue[j] = pNode->keyValue[j-1];  //�����λ
					j--;
				}
				pNode->keyValue[j] = key;  //����ؼ��ֵ�ֵ
				pNode->keyNum++; //���½ڵ�ؼ��ֵĸ���
				count++;//����һ��
				
				while(1)//��¼���׽ڵ���Ҫ�������Ϸ��ѵĴ���
				{
					pNode = pNode->pParent;
					if(pNode->keyNum == KEY_MAX)//�ڵ���������Ҫ����
					{
						count++;
						if(pNode->pParent == NULL)//���ڵ�����
							break;
					}
					else//��������
						break;
				}

				pNode = temp;
				for(k = 0; k < count; k++)//���Ϸ���count��
				{
					if(pNode->pParent == NULL)
					{
						Node *pRoot = new Node();  //�����µĸ��ڵ�
						pRoot->isLeaf = false;
						pRoot->pChild[0] = pNode;
						splitChild(pRoot, 0, pNode);   //���ѽڵ�
						m_pRoot = pRoot;  //���¸��ڵ�ָ��
					}
					else
					{
						i = pNode->pParent->keyNum;
						new_key = pNode->keyValue[pNode->keyNum/2];
						while(i > 0 && new_key < pNode->pParent->keyValue[i-1])//Ѱ��������Ĺؼ���(new_key)�����λ��
							i--;
						splitChild(pNode->pParent, i, pNode);
						pNode = pNode->pParent;
					}
				}
			}
			else if(pNode->keyNum == KEY_MAX && pNode->pParent == NULL)//���Ҷ�ڵ�����������ڵ��Ǹ��ڵ�
			{
				while (i > 0 && key < pNode->keyValue[i-1])   //�Ӻ���ǰ�����ҹؼ��ֵĲ���λ��
				{
					pNode->keyValue[i] = pNode->keyValue[i-1];  //�����λ
					i--;
				}
				pNode->keyValue[i] = key;  //����ؼ��ֵ�ֵ
				pNode->keyNum++; //���½ڵ�ؼ��ֵĸ���
				Node *pRoot = new Node();  //�����µĸ��ڵ�
				pRoot->isLeaf = false;
				pRoot->pChild[0] = pNode;
				splitChild(pRoot, 0, pNode);   //���ѽڵ�
				m_pRoot = pRoot;  //���¸��ڵ�ָ��
			}
			else//���Ҷ�ڵ�û����
			{
				while (i > 0 && key < pNode->keyValue[i-1])   //�Ӻ���ǰ�����ҹؼ��ֵĲ���λ��
				{
					pNode->keyValue[i] = pNode->keyValue[i-1];  //�����λ
					i--;
				}
				pNode->keyValue[i] = key;  //����ؼ��ֵ�ֵ
				pNode->keyNum++; //���½ڵ�ؼ��ֵĸ���
			}
		}
		else//����ڵ����ڲ��ڵ�
		{
			while(i > 0 && key < pNode->keyValue[i-1])   //�Ӻ���ǰ�����ҹؼ���Ҫ���������
				i--;
			Node *pChild = pNode->pChild[i];  //Ŀ���������ָ��
			insert_oddTree(pChild, key);//����Ѱ��Ҫ�����Ҷ�ڵ�
		}
	}
};