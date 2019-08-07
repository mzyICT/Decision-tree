#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>

#include "Tree.h"
using namespace std;

class DecisionTree {
private:

	struct attrItem
	{
		std::vector<int>  itemNum;  //itemNum[0] = itemLine.size()
		//itemNum[1] = decision num
		set<int>		  itemLine;
	};

	struct attributes
	{
		string attriName;
		vector<double> statResult;
		map<string, attrItem*> attriItem;
	};

	vector<attributes*> statTree;
	int attriNum;
	vector<vector<string>> infos;
	map<string, int> attr_clum;

public:
	DecisionTree(){
		attriNum = 0;
	}
	vector<vector<string>>& getInfos()
	{
		return infos;
	}
	vector<attributes*>& getStatTree()
	{
		return statTree;
	}
	int pretreatment(string filename, set<int>& readLineNum, vector<int>& readClumNum);
	int statister(vector<vector<string>>& infos, vector<attributes*>& statTree,
		set<int>& readLine, vector<int>& readClumNum);
	int compuDecisiNote(vector<attributes*>& statTree, int deciNum, int lineNum, vector<int>& readClumNum);
	double info_D(int deciNum, int sum);
	void resetStatTree(vector<attributes*>& statTree, vector<int>& readClumNum);
	double Info_attr(map<string, attrItem*>& attriItem, double& splitInfo, int lineNum);
	void CreatTree(TreeNode* treeHead, vector<attributes*>& statTree, vector<vector<string>>& infos,
		set<int>& readLine, vector<int>& readClumNum, int deep);
};
