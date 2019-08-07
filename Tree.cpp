#include "stdafx.h"
#include "Tree.h"

TreeNode* CreateTreeNode(string value)
{
	TreeNode* pNode = new TreeNode();
	pNode->m_sAttribute = value;
	return pNode;
}

bool FindNode(TreeNode* pRoot, std::string& item)
{

	if (pRoot->m_sAttribute == item)
		return true;

	bool found = false;

	vector<TreeNode*>::iterator i = pRoot->m_vChildren.begin();
	while (!found && i < pRoot->m_vChildren.end())
	{
		found = FindNode(*i, item);
		++i;
	}

	return found;
}

void ConnectTreeNodes(TreeNode* pParent, TreeNode* pChild)
{
	if (pParent != NULL)
	{
		pParent->m_vChildren.push_back(pChild);
	}
}

void PrintTreeNode(TreeNode* pNode)
{
	if (pNode != NULL)
	{
		printf("value of this node is: %d.\n", pNode->m_sAttribute);
		printf("its children is as the following:\n");
		std::vector<TreeNode*>::iterator i = pNode->m_vChildren.begin();
		while (i < pNode->m_vChildren.end())
		{
			if (*i != NULL)
				printf("%s\t", (*i)->m_sAttribute);
			++i;
		}
		printf("\n");
	}
	else
	{
		printf("this node is null.\n");
	}

	printf("\n");
}

void PrintTree(TreeNode* pRoot)
{
	PrintTreeNode(pRoot);

	if (pRoot != NULL)
	{
		std::vector<TreeNode*>::iterator i = pRoot->m_vChildren.begin();
		while (i < pRoot->m_vChildren.end())
		{
			PrintTree(*i);
			++i;
		}
	}
}

void DestroyTree(TreeNode* pRoot)
{
	if (pRoot != NULL)
	{
		std::vector<TreeNode*>::iterator i = pRoot->m_vChildren.begin();
		while (i < pRoot->m_vChildren.end())
		{
			DestroyTree(*i);
			++i;
		}
		delete pRoot;
	}
}
