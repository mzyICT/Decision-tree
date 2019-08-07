/*
思想：
每次从数据集中根据最大信息增益选取一个最好特征，将数据进行划分，每次划分都会消耗一个特征，
使得特征越来越少，当所有数据集都是同一类，或者消耗完所有特征时，划分结束。

信息熵：  Entropy(D) = -sum( p_i * log_2(p_i) )
D为样本集合
1<= i <= 样本D中所包含的类别数
p_i 为第i个类别在D中所占的比例
信息增益：Gain(D_all) = Entropy(D_all) - sum( D_i / D_all * Entorpy(D_i) )
D_all 为总数据集
D_i 为第i个子数据集
1<= i <= 子数据集数
*/
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <math.h>
using namespace std;

/*set的特性是，所有元素都会根据元素的键值自动排序，set的元素不像map那样可以同时拥有实值(value)和键值(key),set元素的键值就是实值，实值就是键值。set不允许两个元素有相同的键值*/
/*算法（Algorithm）为一个计算的具体步骤，常用于计算、数据处理和自动推理。C++ 算法库（Algorithms library）为 C++ 程序提供了大量可以用来对容器及其它序列进行算法操作的函数。这些组件可以为函数或函数模板，大部份由头文件 <algorithm> 提供，一小部份位于 <numeric>、<cstdlib> 中。*/
#define N 14
#define feature 4

vector< vector<string> > X;
string x[N][feature + 1] =
{
	{ "Sunny", "Hot", "High", "Weak", "no" },
	{ "Sunny", "Hot", "High", "Strong", "no" },
	{ "Overcast", "Hot", "High", "Weak", "yes" },
	{ "Rain", "Mild", "High", "Weak", "yes" },
	{ "Rain", "Cool", "Normal", "Weak", "yes" },
	{ "Rain", "Cool", "Normal", "Strong", "no" },
	{ "Overcast", "Cool", "Normal", "Strong", "yes" },
	{ "Sunny", "Mild", "High", "Weak", "no" },
	{ "Sunny", "Cool", "Normal", "Weak", "yes" },
	{ "Rain", "Mild", "Normal", "Weak", "yes" },
	{ "Sunny", "Mild", "Normal", "Strong", "yes" },
	{ "Overcast", "Mild", "High", "Strong", "yes" },
	{ "Overcast", "Hot", "Normal", "Weak", "yes" },
	{ "Rain", "Mild", "High", "Strong", "no" },
};

//属性的名称，比如 Outlook取值：Sunny, Overcast, Rain
string attribute[] =   {"Outlook","Temperature","Humidity","Wind"};

vector<string> attributes;

/*
*****************************************************************
函数名称：createDataset
功能：创建数据集
参数：无
返回值：无（操作对象为全局变量）
*****************************************************************
*/

void createDataset()//创建数据集
{
	//创建数据集//N*(feature + 1)的二维vector
	X = vector< vector<string> >(N, vector<string>(feature + 1));
	//将x的值放入X中
	int i, j;
	for (i = 0; i<N; i++)
	{
		for (int j = 0; j<feature + 1; j++)
		{
			X[i][j] = x[i][j];
		}
	}
	
	//创建属性
	for (i = 0; i<feature; i++)//这里用vector的push_back函数,从尾部插入数字
		attributes.push_back(attribute[i]);
}

/*
*****************************************************************
函数名称：calcShanno
功能：计算给定数据集的香农熵
参数：string二维容器（引用）
返回值：给定数据集的香农熵（double）
*****************************************************************
*/

double calcShanno(vector< vector<string> > &data)//计算给定数据集的香农熵
{
	int n = data.size(); //二维容器一共多少行
	map<string, int> classCounts; //构架键实对
	int i;
	int label = data[0].size() - 1; //标签的数量为每行数量－1（方便classCounts下标对应最后的分类，其实就是让下标对应yes no）
	for (i = 0; i<n; i++)  //初始为0//每一行的yes no对应的实值都初始化为0
	{
		classCounts[data[i][label]] = 0; //关键字都是“yes”“no”，只不过对应的实值都是0
	}
	for (i = 0; i<data.size(); i++)  //每当出现一次，+1
	{
		classCounts[data[i][label]] += 1; //这里累计yes,no的次数//这里就体现了map键对应的值可以被修改的属性
	}

	//计算香农熵
	double shanno = 0;
	map<string, int>::iterator it; //迭代器是指针的泛指
	for (it = classCounts.begin(); it != classCounts.end(); it++)//这里一共就两个键值对yes-9,no-5(当时这是第一次的时候)
	{
		double prob = (double)(it->second) / (double)n;//最后类别所占比例//这个对的second就是yes或者no的数量
		shanno -= prob * (log(prob) / log(2));
	}
	return shanno;
}

/*
*****************************************************************
函数名称：splitDataSet
功能：按照给定特征划分数据集，划分后的数据集中不包含给定特征，即新的数据集的维度少了一个
参数：string二维容器,  axis ：特征下标(0,1,2,3),  value：特征值(string:每种特征的具体种类)
返回值：给定数据集的香农熵（double）
*****************************************************************
*/

vector< vector<string> > splitDataSet(vector< vector<string> > data, int axis, string value)//按照给定特征划分数据集，划分后的数据集中不包含给定特征，即新的数据集的维度少了一个
{
	vector< vector<string> > result;
	for (int i = 0; i<data.size(); i++)//data.size=14
	{
		if (data[i][axis] == value)//data[i].size=5
		{
			//将“当前特征”这个维度去掉
			vector<string> removed(data[i].begin(), data[i].begin() + axis); //初始化为两个迭代器指定范围中元素的拷贝
			removed.insert(removed.end(), data[i].begin() + axis + 1, data[i].end());  //v.insert(v.end(), a[1], a[3]);//在尾部插入a[1]个a[3]//当然这里是指两个指针的的数据
			result.push_back(removed);
			//其实这里不是去掉，而是重新定义了数据集result，然后把除去<axis,value>这个,剩下的复制进去而已
		}
	}
	return result;
}

/*
*****************************************************************
函数名称：createFeatureList
功能：创建特征列表
参数：string二维容器（引用）,  axis ：特征下标(0,1,2,3)
返回值：特征的所有取值（string）
*****************************************************************
*/

vector<string> createFeatureList(vector< vector<string> > &data, int axis)//创建特征列表
{
	int n = data.size();         //=14
	vector<string>featureList;   //某个属性的特征的所有取值（如outlook的Sunny，Rain，overcast）
	set<string> s;
	for (int j = 0; j<n; j++)    //寻找该特征的所有可能取值
		s.insert(data[j][axis]); //把该特征所有取值都插入s容器
	set<string>::iterator it;
	for (it = s.begin(); it != s.end(); it++)
	{
		featureList.push_back(*it);//再把s容器所有值都插入特征列表
	}
	return featureList;
}

/*
*****************************************************************
函数名称：chooseBestFeatureToSplit
功能：选择最好的数据集划分方式
参数：string二维容器（引用）
返回值：当前分类的最好属性
*****************************************************************
*/

int chooseBestFeatureToSplit(vector< vector<string> > &data)//选择最好的数据集划分方式
{
	int n = data[0].size() - 1;				//=4
	double bestEntropy = calcShanno(data);  //初始香农熵
	double bestInfoGain = 0;		        //最大的信息增益
	int bestFeature = 0;					//最好的属性
	for (int i = 0; i<n; i++)				//所有特征
	{
		double newEntropy = 0;				                      //初始化新的香农熵
		vector<string> featureList = createFeatureList(data, i);  //该特征的所有可能取值（从0、1、2、3，即所有特征对应的值）//其实这里相当于某个特征的那一列。
		for (int j = 0; j<featureList.size(); j++)				  //featureList.size()=14
		{
			vector< vector<string> > subData = splitDataSet(data, i, featureList[j]); //这里的subData是数据集除去featureList[j]之后的数据集
			double prob = (double)subData.size() / (double)data.size(); //除去当前特征之后，剩余数据集所占比例
			newEntropy += prob * calcShanno(subData); //一次累加得到除去该特征之后的香农熵
		}
		double infoGain = bestEntropy - newEntropy;  //信息增益，即熵的减少，或数据无序度的减少
		if (infoGain > bestInfoGain) //这里是选取当前情况下，哪种特征的信息增益最大
		{
			bestInfoGain = infoGain;
			bestFeature = i;
		}
	}
	return bestFeature; //这里返回的是标号0、1、2、3
}

/*
*****************************************************************
函数名称：majorityCnt
功能：返回出现次数最多的分类名称
//如果数据集已处理了所有属性，但类标签依然不是唯一的，采用多数表决的方法定义叶子节点的分类
参数：string容器（引用）
返回值：string
*****************************************************************
*/

string majorityCnt(vector<string> &classList)
{
	int n = classList.size();
	map<string, int> classCount;
	int i;
	for (i = 0; i<n; i++)
	{
		classCount[classList[i]] = 0;
	}
	for (i = 0; i<n; i++)
	{
		classCount[classList[i]] += 1;
	}

	int maxCnt = 0;
	map<string, int>::iterator it;
	string result = "";
	for (it = classCount.begin(); it != classCount.end(); it++)
	{
		if (it->second > maxCnt)
		{
			maxCnt = it->second;
			result = it->first;
		}
	}
	return result;
}

struct Node //子叶节点
{
	string attribute; //属性
	string val; //特征值(string:每种特征的具体种类)
	bool isLeaf; //是否有孩子结点
	vector<Node*> childs; //孩子节点指针

	Node() //构造函数
	{
		val = "";
		attribute = "";
		isLeaf = false;
	}
};

Node *root = NULL; //静态结点

/*
*****************************************************************
函数名称：createTree
功能：递归构建决策树
参数：Node*根节点,二维容器string（引用），string容器：属性
返回值：Note*结点地址
*****************************************************************
*/

Node* createTree(Node *root, vector< vector<string> > &data, vector<string> &attribute)
{
	if (root == NULL)
		root = new Node();
	vector<string> classList;
	set<string> classList1; //这里运用了set关键字不可重复的特点，来作为检查样本数据是否有yes或no
	int i, j;
	int label = data[0].size() - 1; //=4
	int n = data.size(); //=14
	for (i = 0; i<n; i++) //收集当前数据的所有的yes no到classList，所有yes no的种类的到classList1
	{
		classList.push_back(data[i][label]);
		classList1.insert(data[i][label]);
	}
	if (classList1.size() == 1)  //如果所有实例都属于同一类，停止划分（只有yes或者no）
	{
		if (classList[0] == "yes")
			root->attribute = "yes";
		else
			root->attribute = "no"; 
		root->isLeaf = true;
		return root;
	}
	if (data[0].size() == 1)  //遍历完所有特征，返回出现次数最多的类别
	{
		root->attribute = majorityCnt(classList);
		return root;
	}

	int bestFeatureIndex = chooseBestFeatureToSplit(data); //这里得到当前情况下以哪种属性分类
	vector<string> featureList = createFeatureList(data, bestFeatureIndex);  //从上面得到的当前分类的最佳属性分类，然后这里把属性的所有可能特征值都放到链表
	string bestFeature = attribute[bestFeatureIndex]; //这里就体现了bestFeatureIndex = chooseBestFeatureToSplit(data)是int型的

	root->attribute = bestFeature;   //记录要划分的特征，放入结点

	for (i = 0; i<featureList.size(); i++)  //对于当前属性的每个可能值，创建新的分支
	{
		vector<string> subAttribute;  //新的属性列表，不包含当前要划分的特征，即消耗了一个特征，特征的维度少了一个
		for (j = 0; j<attribute.size(); j++)
		{
			if (bestFeature != attribute[j])
				subAttribute.push_back(attribute[j]); //把，不包含当前要划分的属性，的所有特征值全部压入容器
		}
		Node *newNode = new Node();
		newNode->val = featureList[i];   //记录属性的取值（取哪个特征字符串）
		createTree(newNode, splitDataSet(data, bestFeatureIndex, featureList[i]), subAttribute);//此时用函数splitDataSet创建一个新的数据集，当然这里排除了bestFeatureIndex，同时，把剩余的属性放进去
		root->childs.push_back(newNode); //然后把结点压入孩子指针容器
	}
	return root;
}

/*
*****************************************************************
函数名称：print
功能：打印
参数：Node*根节点,树的深度
返回值：无
*****************************************************************
*/

void print(Node *root, int depth)
{
	int i;
	for (i = 0; i<depth; i++)
		cout << "\t";

	if (root->val != "")
	{
		cout << root->val << endl;
		for (i = 0; i<depth + 1; i++)
			cout << "\t";
	}
	cout << root->attribute << endl;
	vector<Node*>::iterator it;
	for (it = root->childs.begin(); it != root->childs.end(); it++)
	{
		print(*it, depth + 1);
	}
}

/*
*****************************************************************
函数名称：clsssify
功能：预测
参数：Node*根节点,属性列表（string容器），
返回值：无
*****************************************************************
*/

string classify(Node *root, vector<string> &attribute, string *test)
{
	string firstFeature = root->attribute;
	int firstFeatureIndex;
	int i;
	for (i = 0; i<feature; i++)  //找到根节点是第几个特征
	{
		if (firstFeature == attribute[i])
		{
			firstFeatureIndex = i;
			break;
		}
	}
	if (root->isLeaf)  //如果是叶子节点，直接输出结果
		return root->attribute;
	for (i = 0; i<root->childs.size(); i++)
	{
		if (test[firstFeatureIndex] == root->childs[i]->val)
		{
			return classify(root->childs[i], attribute, test);
		}
	}
}

//释放节点
void freeNode(Node *root)
{
	if (root == NULL)
		return;
	vector<Node*>::iterator it;
	for (it = root->childs.begin(); it != root->childs.end(); it++)
		freeNode(*it);
	delete root;
}

int main(void)
{
	createDataset();
	root = createTree(root, X, attributes);
	print(root, 0);

	string test[6][4] = 
	{{"Sunny","Hot","Normal","Weak"},
	{"Sunny","Hot","Normal","Strong"},
	{"Rain","Hot","Normal","Weak"},
	{"Sunny","Mild","Normal","Weak"},
	{"Sunny","Cool","High","Strong"},
	{"Sunny","Cool","High","Weak"}};
	int i,j;
	cout << endl << "属性：";
	for (i = 0; i<feature; i++)
		cout << attributes[i] << "\t";
	

	for (j = 0; j < 6; j++)
	{
		cout << endl << "例子：";
		for (i = 0; i < feature; i++)
		{
			cout << test[j][i] << "\t";
		}
		cout << endl << "预测：";
		cout << classify(root, attributes, test[j]) << endl;
	}
	
	freeNode(root);

	return 0;
}


