static int f = 0;

template<class T>
class CBTree
{
private:
	double M;                  //B树的最小度数
	int KEY_MAX;        //节点包含关键字的最大个数
	int KEY_MIN;          //非根节点包含关键字的最小个数
	int CHILD_MAX;  //孩子节点的最大个数
	int CHILD_MIN;  //孩子节点的最小个数
public:
	struct Node
	{
		bool isLeaf;             //是否是叶子节点
		int keyNum;              //节点包含的关键字数量
		T keyValue[100];     //关键字的值数组
		Node *pChild[100]; //子树指针数组
		Node *pParent;      //父节点
  
		Node(bool b = true, int n = 0)
			:isLeaf(b), keyNum(n), pParent(NULL){}
	};
private:
	Node * m_pRoot;  //B树的根节点
public:
	CBTree(double n)
	{
		M = n+0.5;                  //B树的最小度数
		KEY_MAX = 2*M-2;        //节点包含关键字的最大个数
		KEY_MIN = M-1;          //非根节点包含关键字的最小个数
		CHILD_MAX = KEY_MAX+1;  //孩子节点的最大个数
		CHILD_MIN = KEY_MIN+1;  //孩子节点的最小个数
		m_pRoot = NULL;  //创建一棵空的B树
		cout << "M:" << M << endl;
		cout << "max:" << KEY_MAX << endl;
		cout << "min:" << KEY_MIN << endl;
	}
 
	~CBTree()
	{
		clear();
	}
 
	bool insert(const T &key)    //向B数中插入新结点key
	{
		if (contain(key))  //检查该关键字是否已经存在
		{
			cout << "有重复的关键字" << endl;
			return false;
		}
		else
		{
			if (m_pRoot==NULL)//检查是否为空树
			{
				m_pRoot = new Node();
			}
			if((KEY_MAX % 2) != 0)//有奇数个关键字
			{
				if (m_pRoot->keyNum == KEY_MAX) //检查根节点是否已满
				{
					Node *pNode = new Node();  //创建新的根节点
					pNode->isLeaf = false;
					pNode->pChild[0] = m_pRoot;
					splitChild(pNode, 0, m_pRoot);   //分裂节点
					m_pRoot = pNode;  //更新根节点指针
				}
				insert_evenTree(m_pRoot, key);
			}
			else if((KEY_MAX % 2) == 0)//有偶数个关键字
			{
				insert_oddTree(m_pRoot, key);
			}
			return true;
		}
	}
 
	bool remove(const T &key)    //从B中删除结点key
	{
		if (!search(m_pRoot, key))  //key不存在
		{
			cout << "未找到相应的关键字" << endl;
			return false;
		}
		if (m_pRoot->keyNum==1)//特殊情况：根节点中只有一个关键字
		{
			if (m_pRoot->isLeaf)//是叶节点
			{
				clear();
				return true;
			}
			else//根不是叶节点时有两个子节点
			{
				Node *pChild1 = m_pRoot->pChild[0];
				Node *pChild2 = m_pRoot->pChild[1];
				if (pChild1->keyNum == KEY_MIN && pChild2->keyNum == KEY_MIN)//根节点需要“缩回来”
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

	void display()const //打印树的关键字
	{
		displayInConcavo(m_pRoot,KEY_MAX*15);
	}

	bool contain(const T &key)const   //检查该key是否存在于B树中
	{
		return search(m_pRoot, key);
	}

	void clear()                      //清空B树
	{
		recursive_clear(m_pRoot);
		m_pRoot = NULL;
	}

private:
	void recursive_clear(Node *pNode)//删除树
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
 
	void deleteNode(Node *&pNode)//删除节点
	{
		if (pNode != NULL)
		{
			delete pNode;
			pNode = NULL;
		}
	}
 
	bool search(Node *pNode, const T &key)const //查找关键字
	{
		if (pNode==NULL)  //检测节点指针是否为空，或该节点是否为叶子节点
		{
			return false;
		}
		else
		{
			int i;
			for (i = 0; i < pNode->keyNum && key > pNode->keyValue[i]; i++)
			{//找到使key <= pNode->keyValue[i]成立的最小下标i
			}
			if (i < pNode->keyNum && key == pNode->keyValue[i])//在当前节点中找到关键字
			{
				return true;
			}
			else
			{
				if (pNode->isLeaf)   //检查该节点是否为叶节点，是叶节点说明关键字不在树中
				{
					return false;
				}
				else
				{
					return search(pNode->pChild[i], key);//在子节点中继续查找
				}
			}
		}
	}
 	
	void splitChild(Node *Parent, int nChildIndex, Node *pChild)  //分裂子节点
	{
		//将pChild分裂成pLeftNode和pChild两个节点
		Node *pRightNode = new Node();//分裂后的右节点
		pRightNode->isLeaf = pChild->isLeaf;//是否为叶节点
		pRightNode->keyNum = KEY_MIN;
		int i;
		for (i = 0; i < KEY_MIN; i++)//拷贝关键字的值
		{
			pRightNode->keyValue[i] = pChild->keyValue[i+CHILD_MIN];
		}
		if (!pChild->isLeaf)  //如果不是叶子节点，拷贝孩子节点指针
		{
			for (i = 0; i < CHILD_MIN; i++)
			{
				pRightNode->pChild[i] = pChild->pChild[i+CHILD_MIN];
			}
		}
		
		pChild->keyNum = KEY_MIN;  //更新左子树的关键字个数
 
		for (i = Parent->keyNum; i > nChildIndex; i--)//将父节点中的nChildIndex后的所有关键字的值和子树指针向后移一位
		{
			Parent->pChild[i+1] = Parent->pChild[i];
			Parent->keyValue[i] = Parent->keyValue[i-1];
		}
		Parent->keyNum++;  //更新父节点的关键字个数
		Parent->pChild[nChildIndex+1] = pRightNode;  //存储右子树指针
		pRightNode->pParent = Parent;
		pChild->pParent = Parent;//存储孩子节点的父节点
		Parent->keyValue[nChildIndex] = pChild->keyValue[KEY_MIN];//把节点的中间值提到父节点

		if(!pRightNode->isLeaf)//更新右节点的子节点的父指针
		{
			for (i = 0; i < CHILD_MIN; i++)
			{
				pRightNode->pChild[i]->pParent = pRightNode;
			}
		}
	}
 
	void insert_evenTree(Node *pNode, const T &key)//在非满节点中插入关键字
	{
		int i = pNode->keyNum;  //获取节点内关键字个数
		if (pNode->isLeaf)      //pNode是叶子节点
		{
			while (i > 0 && key < pNode->keyValue[i-1])   //从后往前，查找关键字的插入位置
			{
				pNode->keyValue[i] = pNode->keyValue[i-1];  //向后移位
				i--;
			}
			pNode->keyValue[i] = key;  //插入关键字的值
			pNode->keyNum++; //更新节点关键字的个数
		}
		else//pNode是内部节点
		{
			while(i > 0 && key < pNode->keyValue[i-1])   //从后往前，查找关键字要插入的子树
				i--;
			Node *pChild = pNode->pChild[i];  //目标子树结点指针
			if (pChild->keyNum == KEY_MAX)  //子树节点已满
			{
				splitChild(pNode, i, pChild);//分裂子树节点
				if(key > pNode->keyValue[i])   //寻找要插入的子树，当前子树或下一个子树
					pChild = pNode->pChild[i+1];
			}
			insert_evenTree(pChild, key);  //插入关键字到目标子树节点
		}
	}
 
	void displayInConcavo(Node *pNode, int count)const//打印树
	{
		if (pNode != NULL)
		{
			int i, j;
			for (i = 0; i < pNode->keyNum; i++)//逐个打印节点关键字
			{
				if (!pNode->isLeaf)//不是叶节点时，递归进入子节点
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
	
	void mergeChild(Node *pParent, int index)//合并两个子节点
	{
		Node *pChild1 = pParent->pChild[index];
		Node *pChild2 = pParent->pChild[index+1];
		//将pChild2数据合并到pChild1
		pChild1->keyNum = KEY_MAX;
		pChild1->keyValue[KEY_MIN] = pParent->keyValue[index];//将父节点index的值下移
		int i;
		for (i = 0; i < KEY_MIN; i++)//将右兄弟节点的值合并到左子节点中
		{
			pChild1->keyValue[i+KEY_MIN+1] = pChild2->keyValue[i];
		}
		if (!pChild1->isLeaf)//若合并的节点是内部节点
		{
			for (i = 0; i < CHILD_MIN; i++)
			{
				pChild1->pChild[i+CHILD_MIN] = pChild2->pChild[i];//将右兄弟节点的指针合并到左子节点中
			}
		}
 
		//父节点删除index的key，index后的往前移一位
		pParent->keyNum--;
		for(i = index; i < pParent->keyNum; i++)
		{
			pParent->keyValue[i] = pParent->keyValue[i+1];
			pParent->pChild[i+1] = pParent->pChild[i+2];
		}
		deleteNode(pChild2);  //删除pChild2
	}
 
	//递归的删除关键字
    void recursive_remove(Node *pNode, const T &key)
	{
		int i = 0;
		while(i < pNode->keyNum && key > pNode->keyValue[i])//寻找要删除的key所在的目标节点
			i++;
		if (i < pNode->keyNum && key == pNode->keyValue[i])//关键字key在节点pNode中
		{
			if (pNode->isLeaf)//pNode是个叶节点
			{
				//从pNode中删除k
				pNode->keyNum--;
				for (; i < pNode->keyNum; i++)//将i后面的关键字前移一位
				{
					pNode->keyValue[i] = pNode->keyValue[i+1];
				}
				return ;
			}
			else//pNode是个内部节点
			{
				Node *pChildPrev = pNode->pChild[i];//节点pNode中key前面的子节点
				Node *pChildNext = pNode->pChild[i+1];//节点pNode中key后面的子节点
				if (pChildPrev->keyNum >= CHILD_MIN)//节点pChildPrev中至少包含CHILD_MIN个关键字
				{
					T prevKey = getPredecessor(pChildPrev); //获取key的前驱关键字
				    recursive_remove(pChildPrev, prevKey);//在节点中删去key的前驱
					pNode->keyValue[i] = prevKey;     //将key替换成key的前驱关键字
					return ;
				}
				else if (pChildNext->keyNum >= CHILD_MIN)//节点pChildNext中至少包含CHILD_MIN个关键字
				{
					T nextKey = getSuccessor(pChildNext); //获取key的后继关键字
					recursive_remove(pChildNext, nextKey);//在节点中删去key的后继
					pNode->keyValue[i] = nextKey;     //替换成key的后继关键字
					return ;
				}
				else//节点pChildPrev和pChildNext中都只包含CHILD_MIN-1个关键字
				{
					mergeChild(pNode, i);//合并节点，将父节点“缩回来”
					recursive_remove(pChildPrev, key);//在合并后的节点中删去key
				}
			}
		}
		else//关键字key不在节点pNode中
		{
			Node *pChildNode = pNode->pChild[i];//包含key的子树根节点
			if (pChildNode->keyNum == KEY_MIN)//只有t-1个关键字，需要向兄弟节点“借”一个关键字
			{
				Node *pLeft = i > 0 ? pNode->pChild[i-1] : NULL;  //左兄弟节点
				Node *pRight = i < pNode->keyNum ? pNode->pChild[i+1] : NULL;//右兄弟节点
				int j;
				if (pLeft && pLeft->keyNum >= CHILD_MIN)//左兄弟节点至少有CHILD_MIN个关键字
				{
					//父节点中i-1的关键字下移至pChildNode中
					for (j = pChildNode->keyNum; j > 0; j--)  
					{
						pChildNode->keyValue[j] = pChildNode->keyValue[j-1];
					}
					pChildNode->keyValue[0] = pNode->keyValue[i-1];
					
					if (!pLeft->isLeaf)  
					{
						for (j = pChildNode->keyNum+1; j > 0; j--) //pLeft节点中合适的子女指针移植到pChildNode中
						{
							pChildNode->pChild[j] = pChildNode->pChild[j-1];
						}
						pChildNode->pChild[0] = pLeft->pChild[pLeft->keyNum];
					}
					pChildNode->keyNum++;
					pNode->keyValue[i] = pLeft->keyValue[pLeft->keyNum-1];//pLeft节点中的最大关键字上升到pNode中
					pLeft->keyNum--;
				}
				else if (pRight && pRight->keyNum >= CHILD_MIN)//右兄弟节点至少有CHILD_MIN个关键字
				{
					//父节点中i的关键字下移至pChildNode中
					pChildNode->keyValue[pChildNode->keyNum] = pNode->keyValue[i];
					pChildNode->keyNum++;
					pNode->keyValue[i] = pRight->keyValue[0];//pRight节点中的最小关键字上升到pNode中
					pRight->keyNum--;
					for (j=0; j<pRight->keyNum; ++j)//将右兄弟节点中的关键字前移
					{
						pRight->keyValue[j] = pRight->keyValue[j+1];
					}
					if (!pRight->isLeaf)  
					{
						pChildNode->pChild[pChildNode->keyNum] = pRight->pChild[0];//pRight节点中合适的子女指针移植到pChildNode中
					    for (j=0; j<=pRight->keyNum; ++j)
					    {
							pRight->pChild[j] = pRight->pChild[j+1];
					    }
					}
				}
				//左右兄弟节点都只包含CHILD_MIN-1个节点
				else if (pLeft)//与左兄弟合并
				{
					mergeChild(pNode, i-1);
					pChildNode = pLeft;
				}
				else if (pRight)//与右兄弟合并
				{
					mergeChild(pNode, i);
				}
			}
			recursive_remove(pChildNode, key);
		}
	}
 
	T getPredecessor(Node *pNode)//找到前驱关键字
	{
		while (!pNode->isLeaf)
		{
			pNode = pNode->pChild[pNode->keyNum];
		}
		return pNode->keyValue[pNode->keyNum-1];
	}
 
	T getSuccessor(Node *pNode)//找到后继关键字
	{
		while (!pNode->isLeaf)
		{
			pNode = pNode->pChild[0];
		}
		return pNode->keyValue[0];
	}

	void insert_oddTree(Node *pNode, const T &key)//在奇数阶B树中插入关键字
	{
		int i = pNode->keyNum;
		int j = pNode->keyNum;
		int k = 0;
		int new_key;
		if(pNode->isLeaf)//要插入的节点是叶节点
		{
			if(pNode->keyNum == KEY_MAX && pNode->pParent != NULL)//这个叶节点已满且这个节点不是根节点
			{
				Node *temp = pNode;
				int count = 0;

				//将key插入叶节点
				while (j > 0 && key < pNode->keyValue[j-1])   //从后往前，查找关键字的插入位置
				{
					pNode->keyValue[j] = pNode->keyValue[j-1];  //向后移位
					j--;
				}
				pNode->keyValue[j] = key;  //插入关键字的值
				pNode->keyNum++; //更新节点关键字的个数
				count++;//分裂一次
				
				while(1)//记录父亲节点需要连续向上分裂的次数
				{
					pNode = pNode->pParent;
					if(pNode->keyNum == KEY_MAX)//节点已满，需要分裂
					{
						count++;
						if(pNode->pParent == NULL)//根节点已满
							break;
					}
					else//结束分裂
						break;
				}

				pNode = temp;
				for(k = 0; k < count; k++)//向上分裂count次
				{
					if(pNode->pParent == NULL)
					{
						Node *pRoot = new Node();  //创建新的根节点
						pRoot->isLeaf = false;
						pRoot->pChild[0] = pNode;
						splitChild(pRoot, 0, pNode);   //分裂节点
						m_pRoot = pRoot;  //更新根节点指针
					}
					else
					{
						i = pNode->pParent->keyNum;
						new_key = pNode->keyValue[pNode->keyNum/2];
						while(i > 0 && new_key < pNode->pParent->keyValue[i-1])//寻找提升后的关键字(new_key)插入的位置
							i--;
						splitChild(pNode->pParent, i, pNode);
						pNode = pNode->pParent;
					}
				}
			}
			else if(pNode->keyNum == KEY_MAX && pNode->pParent == NULL)//这个叶节点已满且这个节点是根节点
			{
				while (i > 0 && key < pNode->keyValue[i-1])   //从后往前，查找关键字的插入位置
				{
					pNode->keyValue[i] = pNode->keyValue[i-1];  //向后移位
					i--;
				}
				pNode->keyValue[i] = key;  //插入关键字的值
				pNode->keyNum++; //更新节点关键字的个数
				Node *pRoot = new Node();  //创建新的根节点
				pRoot->isLeaf = false;
				pRoot->pChild[0] = pNode;
				splitChild(pRoot, 0, pNode);   //分裂节点
				m_pRoot = pRoot;  //更新根节点指针
			}
			else//这个叶节点没有满
			{
				while (i > 0 && key < pNode->keyValue[i-1])   //从后往前，查找关键字的插入位置
				{
					pNode->keyValue[i] = pNode->keyValue[i-1];  //向后移位
					i--;
				}
				pNode->keyValue[i] = key;  //插入关键字的值
				pNode->keyNum++; //更新节点关键字的个数
			}
		}
		else//这个节点是内部节点
		{
			while(i > 0 && key < pNode->keyValue[i-1])   //从后往前，查找关键字要插入的子树
				i--;
			Node *pChild = pNode->pChild[i];  //目标子树结点指针
			insert_oddTree(pChild, key);//继续寻找要插入的叶节点
		}
	}
};