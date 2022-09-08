#pragma once


#include	"GenericTreeItem.h"
#include	"ModelCheckDlg.h"

// CLeftPane form view

class CLeftPane : public CTreeView
{
//	DECLARE_DYNCREATE(CLeftPane)

protected:
	CLeftPane(CWnd* pParent = NULL);           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLeftPane);
	virtual ~CLeftPane();

public:
	enum { IDD = IDD_LEFTPANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	virtual void OnInitialUpdate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void BuildTreeItem(STRUCT_TREE_ITEM * selectableTreeitem);
	virtual void UpdateParent(STRUCT_TREE_ITEM_SELECTABLE * selectableParentTreeitem);
	virtual void UpdateChildren(STRUCT_TREE_ITEM * selectableTreeitem, ENUM_TREE_ITEM_SELECT_STATE selectState);
	virtual void UpdateTree(STRUCT_TREE_ITEM * treeItem, bool * pChecked, bool * pUnchecked);
	virtual void UpdateTree(STRUCT_TREE_ITEM_SELECTABLE	* treeItemSelectable);
	virtual void UpdateAllTrees(STRUCT_TREE_ITEM_SELECTABLE * treeItemSelectable, ENUM_TREE_ITEM_SELECT_STATE selectState);

	afx_msg void Expand(STRUCT_TREE_ITEM * treeItem);
	afx_msg bool SelectTreeItem(STRUCT__IFC__OBJECT * ifcObject, STRUCT_TREE_ITEM * treeItem);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeforeExpand(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

private:
	CWnd* GetRightPane();

private:
	CModelCheckDlg	m_wndModelChecker;
public:
	afx_msg void OnViewModelChecker();
	afx_msg void OnUpdateViewModelChecker(CCmdUI* pCmdUI);
};
