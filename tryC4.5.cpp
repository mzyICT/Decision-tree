#include "stdafx.h"
#include "DecisionTree.h"

int main(int argc, char* argv[]){
	string filename = "source.txt";
	DecisionTree dt;
	int attr_node = 0;
	TreeNode* treeHead = nullptr;
	set<int> readLineNum;
	vector<int> readClumNum;
	int deep = 0;
	if (dt.pretreatment(filename, readLineNum, readClumNum) == 0)
	{
		dt.CreatTree(treeHead, dt.getStatTree(), dt.getInfos(), readLineNum, readClumNum, deep);
	}
	return 0;
}
/*
* @function CreatTree 预处理函数，负责读入数据，并生成信息矩阵和属性标记
* @param: filename 文件名
* @param: readLineNum 可使用行set
* @param: readClumNum 可用属性set
* @return int 返回函数执行状态
*/
int DecisionTree::pretreatment(string filename, set<int>& readLineNum, vector<int>& readClumNum)
{
	ifstream read(filename.c_str());
	string itemline = "";
	getline(read, itemline);
	istringstream iss(itemline);
	string attr = "";
	while (iss >> attr)
	{
		attributes* s_attr = new attributes();
		s_attr->attriName = attr;
		//初始化属性名
		statTree.push_back(s_attr);
		//初始化属性映射
		attr_clum[attr] = attriNum;
		attriNum++;
		//初始化可用属性列
		readClumNum.push_back(0);
		s_attr = nullptr;
	}

	int i = 0;
	//添加具体数据
	while (true)
	{
		getline(read, itemline);
		if (itemline == "" || itemline.length() <= 1)
		{
			break;
		}
		vector<string> infoline;
		istringstream stream(itemline);
		string item = "";
		while (stream >> item)
		{
			infoline.push_back(item);
		}

		infos.push_back(infoline);
		readLineNum.insert(i);
		i++;
	}
	read.close();
	return 0;
}

int DecisionTree::statister(vector<vector<string>>& infos, vector<attributes*>& statTree,
	set<int>& readLine, vector<int>& readClumNum)
{
	//yes的总行数
	int deciNum = 0;
	//统计每一行
	set<int>::iterator iter_end = readLine.end();
	for (set<int>::iterator line_iter = readLine.begin(); line_iter != iter_end; ++line_iter)
	{
		bool decisLine = false;
		if (infos[*line_iter][attriNum - 1] == "yes")
		{
			decisLine = true;
			deciNum++; //无用，因为子树的和就是他，在计算时自动就加完这个数了
		}
		//如果该列未被锁定并且为属性列，进行统计
		for (int i = 0; i < attriNum - 1; i++)
		{
			if (readClumNum[i] == 0)
			{
				std::string tempitem = infos[*line_iter][i];
				auto map_iter = statTree[i]->attriItem.find(tempitem);
				//没有找到
				if (map_iter == (statTree[i]->attriItem).end())
				{
					//新建
					attrItem* attritem = new attrItem();
					attritem->itemNum.push_back(1);
					decisLine ? attritem->itemNum.push_back(1) : attritem->itemNum.push_back(0);
					attritem->itemLine.insert(*line_iter);
					//建立属性名->item映射
					(statTree[i]->attriItem)[tempitem] = attritem;
					attritem = nullptr;
				}
				else
				{
					(map_iter->second)->itemNum[0]++;
					(map_iter->second)->itemLine.insert(*line_iter);
					if (decisLine)
					{
						(map_iter->second)->itemNum[1]++;
					}
				}
			}
		}
	}
	return deciNum;
}

/*
* @function CreatTree 递归DFS创建并输出决策树
* @param: treeHead 为生成的决定树
* @param: statTree 为状态树，此树动态更新，但是由于是DFS对数据更新，所以不必每次新建状态树
* @param: infos 数据信息
* @param: readLine 当前在infos中所要进行统计的行数，由函数外给出
* @param: deep 决定树的深度，用于打印
* @return void
*/
void DecisionTree::CreatTree(TreeNode* treeHead, vector<attributes*>& statTree, vector<vector<string>>& infos,
	set<int>& readLine, vector<int>& readClumNum, int deep)
{
	//有可统计的行
	if (readLine.size() != 0)
	{
		string treeLine = "";
		for (int i = 0; i < deep; i++)
		{
			treeLine += "--";
		}
		//清空其他属性子树，进行递归
		resetStatTree(statTree, readClumNum);
		//统计当前readLine中的数据：包括统计哪几个属性、哪些行，
		//并生成statTree（由于公用一个statTree，所有用引用代替），并返回目的信息数
		int deciNum = statister(getInfos(), statTree, readLine, readClumNum);
		int lineNum = readLine.size();
		int attr_node = compuDecisiNote(statTree, deciNum, lineNum, readClumNum);//本条复制为局部变量
		//该列被锁定
		readClumNum[attr_node] = 1;
		//建立树根
		TreeNode* treeNote = new TreeNode();
		treeNote->m_sAttribute = statTree[attr_node]->attriName;
		treeNote->m_iDeciNum = deciNum;
		treeNote->m_iUnDecinum = lineNum - deciNum;
		if (treeHead == nullptr)
		{
			treeHead = treeNote; //树根
		}
		else
		{
			treeHead->m_vChildren.push_back(treeNote); //子节点
		}
		cout << "节点-" << treeLine << ">" << statTree[attr_node]->attriName << " " << deciNum << " " << lineNum - deciNum << endl;
		//从孩子分支进行递归
		for (map<string, attrItem*>::iterator map_iterator = statTree[attr_node]->attriItem.begin();
			map_iterator != statTree[attr_node]->attriItem.end(); ++map_iterator)
		{
			//打印分支
			int sum = map_iterator->second->itemNum[0];
			int deci_Num = map_iterator->second->itemNum[1];
			cout << "分支--" << treeLine << ">" << map_iterator->first << endl;
			//递归计算、创建
			if (deci_Num != 0 && sum != deci_Num)
			{
				//计算有效行数
				set<int> newReadLineNum = map_iterator->second->itemLine;
				//DFS
				CreatTree(treeNote, statTree, infos, newReadLineNum, readClumNum, deep + 1);
			}
			else
			{
				//建立叶子节点
				TreeNode* treeEnd = new TreeNode();
				treeEnd->m_sAttribute = statTree[attr_node]->attriName;
				treeEnd->m_iDeciNum = deci_Num;
				treeEnd->m_iUnDecinum = sum - deci_Num;
				treeNote->m_vChildren.push_back(treeEnd);
				//打印叶子
				if (deci_Num == 0)
				{
					cout << "叶子---" << treeLine << ">no" << " " << sum << endl;
				}
				else
				{
					cout << "叶子---" << treeLine << ">yes" << " " << deci_Num << endl;
				}
			}
		}
		//还原属性列可用性
		readClumNum[attr_node] = 0;
	}
	
}
/*
* @function compuDecisiNote 计算C4.5
* @param: statTree 为状态树，此树动态更新，但是由于是DFS对数据更新，所以不必每次新建状态树
* @param: deciNum Yes的数据量
* @param: lineNum 计算set的行数
* @param: readClumNum 用于计算的set
* @return int 信息量最大的属性号
*/
int DecisionTree::compuDecisiNote(vector<attributes*>& statTree, int deciNum, int lineNum, vector<int>& readClumNum)
{
	double max_temp = 0;
	int max_attribute = 0;
	//总的yes行的信息量
	double infoD = info_D(deciNum, lineNum);
	for (int i = 0; i < attriNum - 1; i++)
	{
		if (readClumNum[i] == 0)
		{
			double splitInfo = 0.0;
			//info
			double info_temp = Info_attr(statTree[i]->attriItem, splitInfo, lineNum);
			statTree[i]->statResult.push_back(info_temp);
			//gain
			double gain_temp = infoD - info_temp;
			statTree[i]->statResult.push_back(gain_temp);
			//split_info
			statTree[i]->statResult.push_back(splitInfo);
			//gain_info
			double temp = gain_temp / splitInfo;
			statTree[i]->statResult.push_back(temp);
			//得到最大值*/
			if (temp > max_temp)
			{
				max_temp = temp;
				max_attribute = i;
			}
		}
	}
	return max_attribute;
}
/*
* @function Info_attr info_D 总信息量
* @param: deciNum 有效信息数
* @param: sum 总信息量
* @return double 总信息量比例
*/
double DecisionTree::info_D(int deciNum, int sum)
{
	double pi = (double)deciNum / (double)sum;
	double result = 0.0;
	if (pi == 1.0 || pi == 0.0)
	{
		return result;
	}
	result = pi * (log(pi) / log((double)2)) + (1 - pi)*(log(1 - pi) / log((double)2));
	return -result;
}
/*
* @function Info_attr 总信息量
* @param: deciNum 有效信息数
* @param: sum 总信息量
* @return double
*/
double DecisionTree::Info_attr(map<string, attrItem*>& attriItem, double& splitInfo, int lineNum)
{
	double result = 0.0;
	for (map<string, attrItem*>::iterator item = attriItem.begin();
		item != attriItem.end();
		++item
		)
	{
		double pi = (double)(item->second->itemNum[0]) / (double)lineNum;
		splitInfo += pi * (log(pi) / log((double)2));
		double sub_attr = info_D(item->second->itemNum[1], item->second->itemNum[0]);
		result += pi * sub_attr;
	}
	splitInfo = -splitInfo;
	return result;
}
/*
* @function resetStatTree 清理状态树
* @param: statTree 状态树
* @param: readClumNum 需要清理的属性set
* @return void
*/
void DecisionTree::resetStatTree(vector<attributes*>& statTree, vector<int>& readClumNum)
{
	for (int i = 0; i < readClumNum.size() - 1; i++)
	{
		if (readClumNum[i] == 0)
		{
			map<string, attrItem*>::iterator it_end = statTree[i]->attriItem.end();
			for (map<string, attrItem*>::iterator it = statTree[i]->attriItem.begin();
				it != it_end; it++)
			{
				delete it->second;
			}
			statTree[i]->attriItem.clear();
			statTree[i]->statResult.clear();
		}
	}
}


