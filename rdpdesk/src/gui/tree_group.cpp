///////////////////////////////////////////////////////////////////////////////
// File name:   tree_group.cpp
// Version:     0.0
// Purpose:
// Time-stamp:  "2010-03-21 21:56:06"
// E-mail:      rdpdesk@rdpdesk.com
// $Id$
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#include "tree_group.hpp"

#include "fastconn_dialog.hpp"
#include "rdp_dialogs.hpp"

//#include "RDPOptionsDialog.hpp"
//#include "main_window.hpp"

//WX_DEFINE_OBJARRAY(GeneralOptionsArray);

RDPTree::RDPTree(Main_Frame * main, wxWindow *parent, const wxWindowID id,
               const wxPoint& pos, const wxSize& size,
			   long style):
wxTreeCtrl(parent, id, pos, size, style)
{
	main_frame = main;

	wxImageList * imglist = new wxImageList(16, 16, true, 2);
	imglist->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, wxSize(16,16)));
    imglist->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16)));

	imglist->Add(wxArtProvider::GetBitmap(wxART_ERROR, wxART_OTHER, wxSize(16,16)));

	imglist->Add(wxArtProvider::GetBitmap(wxART_INFORMATION, wxART_OTHER, wxSize(16,16)));
    this->AssignImageList(imglist);

    programsettings ps = load_main_settings();
	fcount = ps.favorites_count;
	fmax = ps.favorites_max;

	IsFavorites = FALSE;
	root = this->AddRoot(wxT("Settings"), 0);


}

///////////////////////////////////////////////////////////////////////////////
//! \brief reload data in tree
///////////////////////////////////////////////////////////////////////////////
void RDPTree::ReloadSettings()
{
   if (IsFavorites) return;
   wxTreeItemId id;
   wxString item_text;

   //Benc bc;
   //int lc = bc.Count(&main_frame->base);
   if (main_frame->all_connection_records.Count() > 50)
   {
      wxBusyInfo wait(wxT("Please wait"));
   }
   //
   int max_data_count = this->GetCount();
   RDPTreeData * saved_data = new RDPTreeData[max_data_count];
   int count = 0;
   wxTreeItemIdValue cookie;
   wxTreeItemId curr = this->GetFirstChild(root,cookie);
   while (curr.IsOk())
   {
      wxTreeItemIdValue cookie_child;
      wxTreeItemId curr_child = GetFirstChild(curr,cookie_child);
      while (curr_child.IsOk())
      {
	 RDPTreeData * rdptreedata = (RDPTreeData *)GetItemData(curr_child);
	 if (!rdptreedata) continue;
	 saved_data[count].uniq_name = rdptreedata->uniq_name;
	 saved_data[count].dwConnCount = rdptreedata->dwConnCount;
	 saved_data[count].dwObjCount = rdptreedata->dwObjCount;
	 count ++;
	 curr_child = GetNextChild(curr,cookie_child);
      }
      curr = GetNextChild(root,cookie);
   }
   RDPTree * favorites_tree = TREEPANEL(main_frame->m_panel_tree)->favorites;
   favorites_tree->DeleteAllItems();
   favorites_tree->root = favorites_tree->AddRoot(wxT("Settings"), 0);
   favorites_tree->base = favorites_tree->AppendItem(favorites_tree->root,wxT("Favorites"),0);
   DeleteAllItems();
   root = this->AddRoot(wxT("Settings"), 0);

   base = this->AppendItem(root,wxT("Main"),0);
   //RDPConn lrdpconn;
   Options_HashMap local_options;

   for (size_t i = 0; i < main_frame->all_connection_records.Count(); i++)
   {
      local_options = main_frame->all_connection_records.Item(i);

      if (local_options[wxT("connection_name")].IsEmpty() == true)
      {
	 item_text = local_options[wxT("hostname")];
      }
      else
      {
	 item_text = local_options[wxT("connection_name")];
      }
      wxTreeItemId group_name;
      if (local_options[wxT("group_name")].Length() == 0)
      {
	 group_name = base;
      }
      else
      {
	 if (group_already_exists(local_options[wxT("group_name")]))
	 {
	    group_name = find_this_group(local_options[wxT("group_name")]);
	 }
	 else
	 {
	    group_name = AppendItem(root,local_options[wxT("group_name")],0);
	 }
      }
      id = this->AppendItem(group_name,item_text,1);

      BOOL bDataExists = FALSE;
      for (int i = 0; i < count; i++)
      {
	 if (saved_data[i].uniq_name == wxAtoi(local_options[wxT("uniq_name")]))
	 {
	    this->SetItemData(id,new RDPTreeData(saved_data[i].dwObjCount,
						 saved_data[i].dwConnCount,
						 wxAtoi(local_options[wxT("uniq_name")])));
	    bDataExists = TRUE;
	    break;
	 }
      }
      if (!bDataExists)
      {
	 this->SetItemData(id,new RDPTreeData(0,0,
					      wxAtoi(local_options[wxT("uniq_name")])));
      }
      // if (lrdpconn.dwConnectionCount >= (DWORD)this->fcount)
      // {
      //    this->add_to_favorites(id,lrdpconn.dwConnectionCount);
      // }
      if (wxAtoi(local_options[wxT("connection_count")]) >= (DWORD)this->fcount)
      {
	 add_to_favorites(id, local_options);
      }
      this->redraw(id);
   }
   ExpandAll();
   delete [] saved_data;
   this->Refresh();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Load settings to tree
///////////////////////////////////////////////////////////////////////////////
void RDPTree::LoadSettings()
{
   if (IsFavorites) return;
   wxTreeItemId id;
   Options_HashMap local_options;
   wxString item_text;
   RDPTree * favorites_tree = TREEPANEL(main_frame->m_panel_tree)->favorites;

   if (main_frame->all_connection_records.Count() > 50)
   {
      wxBusyInfo wait(wxT("Please wait"));
   }
   favorites_tree->DeleteAllItems();
   favorites_tree->root = favorites_tree->AddRoot(wxT("Settings"), 0);
   favorites_tree->base = favorites_tree->AppendItem(favorites_tree->root,wxT("Favorites"),0);

   DeleteAllItems();
   root = this->AddRoot(wxT("Settings"), 0);
   base = this->AppendItem(root,wxT("Main"),0);

   item_text.Clear();
   for (size_t i = 0; i < main_frame->all_connection_records.Count(); i++)
   {
      local_options = main_frame->all_connection_records.Item(i);
      if (local_options[wxT("connection_name")].Length() == 0)
      {
	 item_text = local_options[wxT("hostname")];
      }
      else
      {
	 item_text = local_options[wxT("connection_name")];
      }

      wxTreeItemId group_name;
      if (local_options[wxT("group_name")].Length() == 0)
      {
	 group_name = base;
      }
      else
      {
	 if (group_already_exists(local_options[wxT("group_name")]))
	 {
	    group_name = find_this_group(local_options[wxT("group_name")]);
	 }
	 else
	 {
	    group_name = AppendItem(root,local_options[wxT("group_name")],0);
	 }
      }
      id = this->AppendItem(group_name,item_text,1);
      //std::cout << wxAtoi(item_text_uniq_name) << std::endl;
      this->SetItemData(id,new RDPTreeData(0,0,wxAtoi(local_options[wxT("uniq_name")])));
      if (wxAtoi(local_options[wxT("connection_count")]) >= (DWORD)this->fcount)
      {
	 add_to_favorites(id, local_options);
      }

      //if (lrdpconn.dwConnectionCount >= (DWORD)this->fcount)
      //{
      // this->add_to_favorites(id,lrdpconn.dwConnectionCount);
      //}
      this->redraw(id);
      local_options.clear();
   }
   ExpandAll();
   this->Refresh();
}

void RDPTree::delete_item(wxTreeItemId id)
{
   if (!id.IsOk())
   {
      return;
   }
   if (id == root)
   {
      return;
   }
   if (ItemHasChildren(id))
   {
      if (IsFavorites) return;
      wxMessageDialog dialog(NULL, _T("Selected group is not empty\n Detete this ?"),
			     _T("Group deleting"), wxNO_DEFAULT|wxYES_NO|wxCANCEL|wxICON_INFORMATION);
      if (dialog.ShowModal() != wxID_YES) return;

      wxTreeItemIdValue cookie;
      wxTreeItemId curr = GetFirstChild(id,cookie);
      while (curr.IsOk())
      {
	 RDPTreeData * rdptreedata = (RDPTreeData *)GetItemData(curr);
	 if (!rdptreedata) continue;
	 Benc bc;
	 main_frame->Del_Connections_Record(rdptreedata->uniq_name);
	 curr = GetNextChild(id,cookie);
      }
      this->DeleteChildren(id);
      this->Delete(id);
   }
   else
   {
      RDPTreeData * rdptreedata = (RDPTreeData *)GetItemData(id);
      if (!rdptreedata)
      {
	 Delete(id);
	 return;
      }
      Benc bc;
      main_frame->Del_Connections_Record(rdptreedata->uniq_name);
      this->Delete(id);
   }
}

void RDPTree::on_activated(wxTreeEvent& event)
{
   RDPTree * tree = TREEPANEL(main_frame->m_panel_tree)->rdptree;
   wxKeyEvent key_evt = event.GetKeyEvent();

   wxTreeItemId item = event.GetItem();
   if (!IsRDPConn(item)) return;

   RDPTreeData * rdptreedata_id = (RDPTreeData *)GetItemData(item);
   if (!rdptreedata_id) return;

   tree->curr_uniq_name = rdptreedata_id->uniq_name;

   wxCommandEvent evt;
   evt.SetId(1);
   on_tree_connect(evt);

   event.Veto();
}

void RDPTree::on_key_down(wxTreeEvent& event)
{
#ifdef __WXMSW__
	event.Veto();
#endif
#ifdef __WXGTK__
	event.Skip();
#endif
}

void RDPTree::on_dleft_click(wxMouseEvent& event)
{
	event.Skip();
	return;
}

void RDPTree::sort_group(wxTreeItemId group)
{

	if (!IsGroup(group)) return;

	wxTreeItemIdValue cookie;
	wxTreeItemId curr = GetFirstChild(group,cookie);
	wxArrayString arr_str;
	if (!curr.IsOk()) return;
	while (curr.IsOk())
	{
		arr_str.Add(GetItemText(curr));
		curr = GetNextChild(group,cookie);
	}

	arr_str.Sort();

	for (int i = 0; i < (int)arr_str.Count() ; i++)
	{
		move_item(find_this_item(arr_str.Item(i)),group);
	}
}

BOOL RDPTree::item_already_exists(wxTreeItemId id)
{
   RDPTreeData * rdptreedata_id = (RDPTreeData *)GetItemData(id);
   if (!rdptreedata_id) return FALSE;
   int uniq_name = rdptreedata_id->uniq_name;
   wxTreeItemIdValue cookie;
   wxTreeItemId curr = GetFirstChild(base,cookie);
   while (curr.IsOk())
   {
      RDPTreeData * rdptreedata = (RDPTreeData *)GetItemData(curr);
      if (rdptreedata)
      {
	 if (rdptreedata->uniq_name == uniq_name)
	 {
	    return TRUE;
	    std::cout << rdptreedata->uniq_name << "==" << uniq_name << std::endl;
	 }
      }
      curr = GetNextChild(base,cookie);
   }
   return FALSE;
}

BOOL RDPTree::group_already_exists(wxString group_name)
{
	wxTreeItemIdValue cookie;
	wxTreeItemId curr = GetFirstChild(root,cookie);
	while (curr.IsOk())
	{
		if (GetItemText(curr) == group_name)
		{
			return TRUE;
		}
		curr = GetNextChild(root,cookie);
	}
	return FALSE;
}

BOOL RDPTree::IsGroup(wxTreeItemId item)
{
   if (!item.IsOk()) return FALSE;
   if (item == root)
   {
      return FALSE;
   }
   
   //if (item == favorites) return FALSE;
   if (GetItemParent(item) != root) return FALSE;
   return TRUE;
}

BOOL RDPTree::IsRDPConn(wxTreeItemId item)
{
   if (!item.IsOk()) return FALSE;
   if (item == root) return FALSE;
   // if (!IsGroup(item))
   // {
   //    std::cout << __LINE__ << " YES  root1 " << __func__ << std::endl;
   //    return FALSE;
   // }
   if (!IsGroup(GetItemParent(item))) return FALSE;
   return TRUE;
}

BOOL RDPTree::IsFavoritesFull()
{
	if (!IsFavorites) return FALSE;

	if (GetChildrenCount(base) >= (size_t)this->fmax) return TRUE;
	return FALSE;

}

int RDPTree::find_min_favorites(int * un)
{
   if (!IsFavorites) return -1;

   DWORD dwMin = 0xFFFF;

   wxTreeItemIdValue cookie;
   wxTreeItemId curr = GetFirstChild(base,cookie);
   while (curr.IsOk())
   {

      RDPTreeData * rdptreedata = (RDPTreeData *)GetItemData(curr);

      if (rdptreedata)
      {
	 for (int i = 0 ;  i < main_frame->all_connection_records.Count(); i++ )
	 {
	    if (wxAtoi((main_frame->all_connection_records.Item(i))[wxT("uniq_name")]) == rdptreedata->uniq_name)
	    {
	       dwMin = wxAtoi((main_frame->all_connection_records.Item(i))[wxT("connection_count")]);
	       *un = rdptreedata->uniq_name;
	    }
	 }
	 // RDPConn lrdpconn;
	 // Benc bc;
	 // int line_num;
	 // line_num = bc.Find(&main_frame->base,rdptreedata->uniq_name);

	 // if (line_num >= 0)
	 // {
	 //    lrdpconn = bc.Get(&main_frame->base,line_num);
	 //    if (lrdpconn.dwConnectionCount < dwMin)
	 //    {
	 //       dwMin = lrdpconn.dwConnectionCount;
	 //       *un = rdptreedata->uniq_name;
	 //    }
	 // }
	 //}

	 curr = GetNextChild(base,cookie);
      }
   }
   return dwMin;
}


void RDPTree::add_to_favorites(wxTreeItemId item, Options_HashMap local_options)
{
   wxTreeItemId id;
   
   if (this->IsFavorites) return;
   if (!IsRDPConn(item)) return;
   if (TREEPANEL(main_frame->m_panel_tree)->favorites->item_already_exists(item)) return;


   if (!TREEPANEL(main_frame->m_panel_tree)->favorites->IsFavoritesFull())
   {
      RDPTreeData * rdptreedata_id = (RDPTreeData *)GetItemData(item);
      if (!rdptreedata_id) return;
      RDPTreeData * f_rdptreedata_id = new RDPTreeData(rdptreedata_id->dwObjCount,rdptreedata_id->dwConnCount,rdptreedata_id->uniq_name);

      //wxTreeItemId id =
      //TREEPANEL(main_frame->m_panel_tree)->favorites->AppendItem(TREEPANEL(main_frame->m_panel_tree)->favorites->base,GetItemText(item),1);
      if (local_options[wxT("connection_name")] != wxT(""))
      {
	 id = TREEPANEL(main_frame->m_panel_tree)->favorites->AppendItem(TREEPANEL(main_frame->m_panel_tree)->favorites->base,
									 local_options[wxT("connection_name")],1);
      }
      else
      {
	 id = TREEPANEL(main_frame->m_panel_tree)->favorites->AppendItem(TREEPANEL(main_frame->m_panel_tree)->favorites->base,
									 local_options[wxT("hostname")],1);
      }
      TREEPANEL(main_frame->m_panel_tree)->favorites->SetItemData(id,f_rdptreedata_id);
      TREEPANEL(main_frame->m_panel_tree)->favorites->Expand(TREEPANEL(main_frame->m_panel_tree)->favorites->base);
   }
   else
   {
      int un = 0;
      int min_count = TREEPANEL(main_frame->m_panel_tree)->favorites->find_min_favorites(&un);

      if (wxAtoi(local_options[wxT("connection_count")]) < (DWORD)min_count)
      {
	 return;
      }
      else
      {
	 wxTreeItemId id_temp =  TREEPANEL(main_frame->m_panel_tree)->favorites->find_item_by_uniq_name(un);
	 TREEPANEL(main_frame->m_panel_tree)->favorites->Delete(id_temp);

	 RDPTreeData * rdptreedata_id = (RDPTreeData *)GetItemData(item);
	 if (!rdptreedata_id) return;
	 RDPTreeData * f_rdptreedata_id = new RDPTreeData(rdptreedata_id->dwObjCount,rdptreedata_id->dwConnCount,rdptreedata_id->uniq_name);

	 wxTreeItemId id = TREEPANEL(main_frame->m_panel_tree)->favorites->AppendItem(TREEPANEL(main_frame->m_panel_tree)->favorites->base,GetItemText(item),1);
	 TREEPANEL(main_frame->m_panel_tree)->favorites->SetItemData(id,f_rdptreedata_id);

	 TREEPANEL(main_frame->m_panel_tree)->favorites->Expand(TREEPANEL(main_frame->m_panel_tree)->favorites->base);
      }
   }
}

wxTreeItemId RDPTree::create_new_group()
{
	wxTextEntryDialog dialog(this,wxT("Name of new group"),wxT("Please enter a group name"),wxT(""),wxOK | wxCANCEL);
	int iRes = dialog.ShowModal();
	if (iRes != wxID_OK) return NULL;

	wxString newgroupname = dialog.GetValue();
	if (newgroupname.Length() == 0) return NULL;
	if (group_already_exists(newgroupname)) return NULL;

	wxTreeItemId newgroup = AppendItem(root,newgroupname,0);
	return newgroup;
}

void RDPTree::delete_this_item(wxString item_text)
{
	wxTreeItemIdValue cookie;
	wxTreeItemId curr = GetFirstChild(root,cookie);
	while (curr.IsOk())
	{
		wxTreeItemIdValue cookie_child;
		wxTreeItemId curr_child = GetFirstChild(curr,cookie_child);
		while (curr_child.IsOk())
		{
			if (GetItemText(curr_child) == item_text)
			{
				Delete(curr_child);
				return;
			}
			curr_child = GetNextChild(curr,cookie_child);
		}
		curr = GetNextChild(root,cookie);
	}
}

wxTreeItemId RDPTree::find_this_item(wxString item_text)
{
	wxTreeItemIdValue cookie;
	wxTreeItemId curr = this->GetFirstChild(root,cookie);
	while (curr.IsOk())
	{
		wxTreeItemIdValue cookie_child;
		wxTreeItemId curr_child = GetFirstChild(curr,cookie_child);
		while (curr_child.IsOk())
		{
			if (GetItemText(curr_child) == item_text)
			{
				return curr_child;
			}
			curr_child = GetNextChild(curr,cookie_child);
		}
		curr = GetNextChild(root,cookie);
	}
	return NULL;
}

wxTreeItemId RDPTree::find_item_by_uniq_name(int uniq_name)
{
	wxTreeItemIdValue cookie;
	wxTreeItemId curr = this->GetFirstChild(root,cookie);
	while (curr.IsOk())
	{
		wxTreeItemIdValue cookie_child;
		wxTreeItemId curr_child = GetFirstChild(curr,cookie_child);
		while (curr_child.IsOk())
		{
			RDPTreeData * rdptreedata = (RDPTreeData *)GetItemData(curr_child);
			if (!rdptreedata) continue;

			if (rdptreedata->uniq_name == uniq_name)
			{
				return curr_child;
			}
			curr_child = GetNextChild(curr,cookie_child);
		}
		curr = GetNextChild(root,cookie);
	}
	return NULL;
}

wxTreeItemId RDPTree::find_this_group(wxString group_name)
{
   wxTreeItemIdValue cookie;
   wxTreeItemId curr = this->GetFirstChild(root,cookie);
   while (curr.IsOk())
   {
      if (GetItemText(curr) == group_name)
      {
	 return curr;
      }
      curr = GetNextChild(root,cookie);
   }
   return NULL;
}

void RDPTree::redraw(wxTreeItemId item)
{
	RDPTreeData * rdptreedata = (RDPTreeData *)GetItemData(item);
	if (!rdptreedata) return;

	if (rdptreedata->dwObjCount > 0)
	{
		if (rdptreedata->dwConnCount > 0)
		{
			SetItemImage(item,3);
		}
		else
		{
			SetItemImage(item,2);
		}
	}
	else
	{
		SetItemImage(item,1);
	}

}

void RDPTree::move_item(wxTreeItemId item, wxTreeItemId group)
{
   Options_HashMap local_options;

	wxTreeItemId temp = AppendItem(group, GetItemText(item), 1);
	RDPTreeData * rdptreedata = (RDPTreeData *)GetItemData(item);
	if (!rdptreedata)
	{
		wxMessageBox(wxT("This note is invalid.\nDeleting note..."),wxT("Error"),wxICON_ERROR);
		this->Delete(item);
		return;
	}

	int uniq_name = rdptreedata->uniq_name;
	SetItemData(temp,new RDPTreeData(rdptreedata->dwObjCount, rdptreedata->dwConnCount,
					 rdptreedata->uniq_name));
	redraw(temp);
	this->Delete(item);

	Benc bc;
	//int ln = bc.Find(&main_frame->base,uniq_name);
	//if (ln >= 0)
	//{
	for (size_t i = 0; i < main_frame->all_connection_records.Count(); i++)
	{
	   local_options = main_frame->all_connection_records.Item(i);
	   if (wxAtoi(local_options[wxT("uniq_name")]) == uniq_name)
	   {

	      //RDPConn rdpc = bc.Get(&main_frame->base,ln);
	      local_options[wxT("group_name")] = GetItemText(group);
	      main_frame->Add_Connections_Record(&local_options);

	      //rdpc.group_name = GetItemText(group);
	      //bc.Add(&main_frame->base,rdpc);
	   }
	}
	ReloadSettings();

}

void RDPTree::on_deleting_item(wxTreeEvent& event)
{

}

void RDPTree::createmenu(wxTreeEvent& event)
{
	RDPTree * tree = TREEPANEL(main_frame->m_panel_tree)->rdptree;
	wxTreeItemId itemId = event.GetItem();

	if (IsRDPConn(itemId))
	{
		RDPTreeData * rdptreedata = (RDPTreeData *)GetItemData(itemId);
		if (!rdptreedata) return;
		tree->curr_uniq_name = rdptreedata->uniq_name;
	}

	wxPoint clientpt = event.GetPoint();
	wxMenu menu;
	wxMenu * groupmenu = new wxMenu();
	if (itemId.IsOk())
	{
		if (root == itemId) return;

		if (GetItemParent(itemId) == root)
		{
			if (IsFavorites) return;

			wxString title = this->GetItemText(itemId);
			menu.SetTitle(title);

			if (this->base != itemId)
			{
				menu.Append(ID_TREE_DELETE,wxT("&Delete"));
			}
			//menu.Append(ID_TREE_SETCREDS,wxT("&Set credentials"));

			currstring = this->GetItemText(itemId);
		}
		else
		{

			wxString title = this->GetItemText(itemId);
			menu.SetTitle(title);
			menu.Append(ID_TREE_CONNECT,wxT("&Connect"));
			menu.Append(ID_TREE_CHANGE,wxT("&Change"));
			menu.Append(ID_TREE_CLONE,wxT("&Copy"));
			menu.Append(ID_TREE_DELETE,wxT("&Delete"));

			if (IsFavorites)
			{
				menu.Append(ID_TREE_NULLCOUNT,wxT("&Delete from favorites"));
			}




			groupmenu->Append(ID_TREE_ADDTONEWGROUP,wxT("&New group"));
			groupmenu->AppendSeparator();

			wxTreeItemIdValue cookie;
			wxTreeItemId curr = tree->GetFirstChild(tree->root,cookie);
			int count = 0;
			while (curr.IsOk())
			{
				groupmenu->Append((ID_TREE_ADDTOEXISTSGROUP + count),GetItemText(curr));
				count ++;
				curr = tree->GetNextChild(tree->root,cookie);
			}

			menu.AppendSubMenu(groupmenu,wxT("&Move to group"));
			currstring = title;
  		}
	}
	else
	{
		return;
		if (IsFavorites) return;
	}
	PopupMenu(&menu, clientpt);
}

void RDPTree::on_tree_set_credentials(wxCommandEvent& event)
{
   wxString title = wxString::Format(wxT("Set credentials for group <%s>"),currstring.data());
   CredsDialog * cd = new CredsDialog(this,wxCAPTION,title);
   int iRes = cd->ShowModal();

   if (iRes)
   {
      wxTreeItemId group_id = find_this_group(currstring);
      if (!group_id.IsOk()) return;

      wxTreeItemIdValue cookie;
      wxTreeItemId curr = GetFirstChild(group_id,cookie);
      while (curr.IsOk())
      {
	 RDPTreeData * rdptreedata = (RDPTreeData *)GetItemData(curr);
	 for (int i = 0 ;  i < main_frame->all_connection_records.Count(); i++ )
	 {
	    if (wxAtoi((main_frame->all_connection_records.Item(i))[wxT("uniq_name")]) == rdptreedata->uniq_name)
	    {
	       (main_frame->all_connection_records.Item(i))[wxT("username")] = cd->group_username;
	       (main_frame->all_connection_records.Item(i))[wxT("password")] = cd->group_password;
	       (main_frame->all_connection_records.Item(i))[wxT("domain")] = cd->group_domain;
	       main_frame->Add_Connections_Record(&((main_frame->all_connection_records.Item(i))));
	    }
	 }
	 curr = GetNextChild(group_id,cookie);
      }

   }

   delete cd;
}

void RDPTree::get_rdpbase_item(wxTreeItemId item, RDPConn * rdpconn)
{
	RDPTreeData * rdptreedata = (RDPTreeData *)GetItemData(item);
	if (!rdptreedata)
	{
		rdpconn->uniq_name = -1;
		return ;
	}
	int uniq_name = rdptreedata->uniq_name;

	Benc bc;
	int lc = bc.Count(&main_frame->base);

	for (int i = 0; i < lc; i++)
	{
		RDPConn lrdpconn = bc.Get(&main_frame->base,i);
		rdpconn = &lrdpconn;
		if (rdpconn->uniq_name ==  uniq_name)
		{
			return;
		}
	}

	rdpconn->uniq_name = -1;
	return ;
}

void RDPTree::refresh_rdpbase_item(RDPConn * rdpconn)
{
   if (!rdpconn) return;
   Options_HashMap local_options;
   
//   Benc bc;
//   RDPConn lrdpconn;

   //int lc = bc.Count(&main_frame->base);

   // for (int i = 0; i < main_frame->all_connection_records.Count(); i++)
   // {
   //    local_options = main_frame->all_connection_records.Item(i);
   //    if (wxAtoi(local_options[wxT("uniq_name")]) ==  rdpconn->uniq_name)
   //    {
   // 	 //RDPDialog * rdpdialog = new RDPDialog(main_frame,wxCAPTION,&lrdpconn,NULL,&(main_frame->base),FALSE/**/);
   // 	 //rdpdialog->LoadRDPConn();
   // 	 RDPDialog * rdpdialog = new RDPDialog(main_frame);
   // 	 rdpdialog->Set_Options(&local_options);
   // 	 wxCommandEvent evt;
   // 	 evt.SetId(1);
   // 	 rdpdialog->button_save_func(evt);
   // 	 delete rdpdialog;
   // 	 return;
   //    }
   // }
}


void RDPTree::on_tree_change(wxCommandEvent& event)
{

   RDPTree * tree = TREEPANEL(main_frame->m_panel_tree)->rdptree;
   Benc bc;
//   RDPConn lrdpconn;
   OptionsArray options;
   Options_HashMap local_options;
   //GeneralOptionsArray temp_struct;
   //options_struct temp1_struct;
   //wxString item_text_uniq_name, item_text_connection_name;


   //int lc = bc.Count(&main_frame->base);

   for (size_t i = 0; i < main_frame->all_connection_records.Count(); i++)
   {
   //    item_text_uniq_name.Clear();
   //    item_text_connection_name.Clear();

   //    //lrdpconn = bc.Get(&main_frame->base,i);
      local_options = main_frame->all_connection_records.Item(i);
   //    //temp_struct = temp_options.Item(1);
   //    for (uint j = 0; j < local_options.Count(); j++)
   //    {
   // 	 if (local_options.Item(j).Item(0).name == wxT("connection_name"))
   // 	 {
   // 	    item_text_connection_name = local_options.Item(j).Item(0).value;
   // 	 }
   // 	 if (local_options.Item(j).Item(0).name.Cmp(wxT("uniq_name")) == 0)
   // 	 {
   // 	    item_text_uniq_name = local_options.Item(j).Item(0).value;
   // 	 }
   //    }
   //std::cout << temp_struct.Count() << std::endl;
   //temp1_struct = temp_struct.Item(0);
   //wxMessageBox (temp1_struct.value);
   //options = bc.Get_Options(&main_frame->all_options,i);

      if (wxAtoi(local_options[wxT("uniq_name")]) ==  tree->curr_uniq_name)
      {
      //wxString connection_name(lrdpconn.connection_name);
      //wxString hostname(lrdpconn.hostname);
      //wxMessageBox();
      //RDPDialog * rdpdialog = new
      //RDPDialog(main_frame,wxCAPTION,&lrdpconn,NULL,&(main_frame->base),FALSE/*,*/);
      RDPDialog * rdpdialog = new RDPDialog(main_frame);

      //RDPOptionsDialog *options_dialog = new RDPOptionsDialog (this);
      //options_dialog->create();
      //options_dialog->set_options(&main_frame->all_connection_records.Item(0));
      //rdpdialog->LoadRDPConn();
      rdpdialog->Set_Options(&local_options);
      int iRes = rdpdialog->ShowModal();
      //main_frame->Add_Connections_Record(&(rdpdialog->Get_Options()));
      if (iRes)
      {
	 tree->ReloadSettings();
      }
      //delete rdpdialog;
      return;
      }
   }

}


void RDPTree::on_tree_clone(wxCommandEvent& event)
{
   RDPTree * tree = TREEPANEL(main_frame->m_panel_tree)->rdptree;
   Benc bc;
   Options_HashMap local_options;
//   bool found_connection_count = false;
   //RDPConn lrdpconn;
   //int lc = bc.Count(&main_frame->base);

   for (size_t i = 0; i < main_frame->all_connection_records.Count(); i++)
   {
      local_options = main_frame->all_connection_records.Item(i);

      if (wxAtoi(local_options[wxT("uniq_name")]) ==  tree->curr_uniq_name)
      {
	 if (local_options[wxT("connection_name")].IsEmpty() == false)
	 {
	    local_options[wxT("connection_name")] += wxT(" (copy)");
	 }
	 else
	 {
	    local_options[wxT("connection_name")] = local_options[wxT("hostname")] + wxT(" (copy)");
	 }
	 //wxMessageBox(local_options[wxT("uniq_name")]);

	 local_options[wxT("uniq_name")] = wxString::Format(wxT("%d"),
							    bc.generate_uniq_name());
	 local_options[wxT("connection_count")] = wxString::Format (wxT("%i"), 0);

	 //std::cout << __LINE__ << std::endl;
	 //wxMessageBox(local_options[wxT("uniq_name")]);
	 main_frame->Add_Connections_Record(&local_options);
	 ReloadSettings();
      }
   }

//   return;
}

void RDPTree::on_tree_delete_from_favorites(wxCommandEvent& event)
{
   wxTreeItemId item = GetSelection();
   Options_HashMap local_options;
   
   //RDPConn lrdpconn;

   RDPTreeData * rdptreedata = (RDPTreeData *)GetItemData(item);
   if (!rdptreedata)
   {
      Delete(item);
      return;
   }

   for (int i = 0; i < main_frame->all_connection_records.Count(); i++)
   {
      local_options = main_frame->all_connection_records.Item(i);
      if (rdptreedata->uniq_name == wxAtoi(local_options[wxT("uniq_name")]))
      {
	 (main_frame->all_connection_records.Item(i))[wxT("connection_count")] = wxT("");
      }
      break;
   }
   //Benc bc;
   //int ln = bc.Find(&main_frame->base,rdptreedata->uniq_name);
   // if (ln >= 0)
   // {
   //    RDPConn rdpc = bc.Get(&main_frame->base,ln);
   //    rdpc.dwConnectionCount = 0;
   //    bc.Add(&main_frame->base,rdpc);
   // }

   Delete(item);
}


void RDPTree::on_add_group(wxCommandEvent& event)
{
	RDPTree * tree = TREEPANEL(main_frame->m_panel_tree)->rdptree;

	wxTreeItemId item = GetSelection();
	wxString item_text = GetItemText(item);

	RDPTreeData * rdptreedata = (RDPTreeData *)GetItemData(item);
	if (!rdptreedata) return;

	int l_uniq_name = rdptreedata->uniq_name;

	if (event.GetId() == ID_TREE_ADDTONEWGROUP)
	{
		wxTreeItemId newgroup = tree->create_new_group();
		if (!newgroup) return;

		tree->move_item(tree->find_item_by_uniq_name(l_uniq_name),newgroup);
		tree->Expand(newgroup);

	}
	if (event.GetId() >= 7000)
	{
		wxTreeItemIdValue cookie;
		wxTreeItemId curr = tree->GetFirstChild(tree->root,cookie);
		int count = 0;
		while (curr.IsOk())
		{
			if (event.GetId() == (ID_TREE_ADDTOEXISTSGROUP + count))
			{
				tree->move_item(tree->find_item_by_uniq_name(l_uniq_name) ,curr);
				tree->SortChildren(curr);
				break;
			}
			count ++;
			curr = tree->GetNextChild(tree->root,cookie);
		}
	}
}


void RDPTree::on_right_click(wxMouseEvent& event)
{

}


void RDPTree::on_tree_connect(wxCommandEvent& event)
{

   RDPTree * tree = TREEPANEL(main_frame->m_panel_tree)->rdptree;
   //Benc bc;
   //RDPConn lrdpconn;
   //int lc = bc.Count(&main_frame->base);
   Options_HashMap local_options;
   for( size_t i = 0; i < main_frame->all_connection_records.Count(); i++)
   {
      local_options = main_frame->all_connection_records.Item(i);

      if (wxAtoi(local_options[wxT("uniq_name")]) ==  tree->curr_uniq_name)
      {
	 //main_frame->AddRDP(local_options,tree->curr_uniq_name);
	 //main_frame->AddRDP(local_options,tree->curr_uniq_name);
	 wxTreeItemId temp = find_item_by_uniq_name(tree->curr_uniq_name);
	 local_options[wxT("connection_count")] = wxString::Format (wxT("%i"),
								    1 + (wxAtoi(local_options[wxT("connection_count")])));
	 //wxMessageBox(local_options[wxT("connection_count")]);

	 //for (uint j = 0 ; j < local_options.count(); j++)
	 //{
	 //    if (local_options.Item(j).Item(0).name == wxT("uniq_name"))
	 //    {
	 //       local_options.Item(j).Item(0).value.Printf(wxT("%i"), bc.generate_uniq_name());
	 //    }
	 //if (local_options.Item(j).Item(0).name == wxT("connection_count"))
	 //{
	 //local_options.Item(j).Item(0).value.Printf(wxT("%i"),
	 //					  ++(wxAtoi(local_options.Item(j).Item(0).value)));
	 //if (local_options.Item(j).Item(0).value >= (DWORD)this->fcount)
	 //{
	 //	  add_to_favorites(temp,rdpc.dwConnectionCount);
	 //}
	 //}
	 //    if (local_options.Item(j).Item(0).name == wxT("connection_name"))
	 //    {
	 //       local_options.Item(j).Item(0).value  += wxT(" (copy)");
	 //    }
	 //}
//	 break;

	 //
	 tree->ReloadSettings();
	 //wxMessageBox(local_options[wxT("connection_count")]);

	 // for (int i = 0; i < main_frame->all_connection_records.Count(); i++)
	 // {
	 //    lrdpconn = bc.Get(&main_frame->base,i);

	 //    if (lrdpconn.uniq_name == tree->curr_uniq_name)
	 //    {
	 main_frame->AddRDP(local_options,tree->curr_uniq_name);
	 //wxTreeItemId temp = find_item_by_uniq_name(tree->curr_uniq_name);
	 // Benc bc;
	 // int lc = bc.Count(&main_frame->base);
	 // for (int i = 0; i < lc; i ++)
	 // {
	 //    RDPConn rdpc = bc.Get(&main_frame->base,i);
	 //    if (rdpc.uniq_name == tree->curr_uniq_name)
	 //    {
	 //       rdpc.dwConnectionCount ++;
	 //       bc.Add(&main_frame->base,rdpc);
	 if (wxAtoi(local_options[wxT("connection_count")]) >= (DWORD)this->fcount)
	 {
	    //   add_to_favorites(temp,local_options);
	 }
	 //       break;
	 //    }
	 // }
	 main_frame->Add_Connections_Record(&local_options);
//	 return;
      }
   }

}



void RDPTree::on_event_from_wxrdp(wxCommandEvent& event)
{

}

void RDPTree::from_wxrdp(int info_uniq_name, int type)
{
	wxTreeItemId item = find_item_by_uniq_name(info_uniq_name);
	if (item)
	{
//		wxMessageBox(wxT("item found"));

		RDPTreeData * rdptreedata = (RDPTreeData *)GetItemData(item);
		if (!rdptreedata) return;
		if (type == TREEDATA_INC_CONNCOUNT)
		{
			rdptreedata->dwConnCount ++;
		}
		else if (type == TREEDATA_DEC_CONNCOUNT)
		{
			if (rdptreedata->dwConnCount > 0) rdptreedata->dwConnCount --;
			else rdptreedata->dwConnCount = 0;
		}
		else if (type == TREEDATA_DEC_OBJCOUNT)
		{
			if (rdptreedata->dwObjCount > 0) rdptreedata->dwObjCount --;
			else
			{
				rdptreedata->dwObjCount = 0;
				rdptreedata->dwConnCount = 0;
			}

			if (rdptreedata->dwObjCount == 0)
			{
				rdptreedata->dwConnCount = 0;
			}
		}
		else if (type == TREEDATA_INC_OBJCOUNT)
		{
			rdptreedata->dwObjCount ++ ;
		}

		SetItemData(item,rdptreedata);
		redraw(item);

	}

}

void RDPTree::from_wxrdp(wxString info, int type)
{
	wxTreeItemId item = find_this_item(info);
	if (item)
	{
		RDPTreeData * rdptreedata = (RDPTreeData *)GetItemData(item);
		if (!rdptreedata) return;
		if (type == TREEDATA_INC_CONNCOUNT)
		{
			rdptreedata->dwConnCount ++;
		}
		else if (type == TREEDATA_DEC_CONNCOUNT)
		{
			if (rdptreedata->dwConnCount > 0) rdptreedata->dwConnCount --;
		}
		else if (type == TREEDATA_DEC_OBJCOUNT)
		{
			if (rdptreedata->dwObjCount > 0) rdptreedata->dwObjCount --;
		}
		else if (type == TREEDATA_INC_OBJCOUNT)
		{
			rdptreedata->dwObjCount ++ ;
		}

		SetItemData(item,rdptreedata);
		redraw(item);
	}
}

void RDPTree::on_tree_delete(wxCommandEvent& event)
{

   RDPTree * tree = TREEPANEL(main_frame->m_panel_tree)->rdptree;
   Options_HashMap local_options;
   Benc bc;

//   wxString item_text_uniq_name;
//   wxString item_text_connection_count;
   //bool found_connection_count = false;

   wxTreeItemId id = GetSelection();
   //item_text_connection_count.Clear();
   //item_text_uniq_name.Clear();
   //std::cout << main_frame->all_connection_records.Count() << std::endl;
   for (size_t i = 0; i < main_frame->all_connection_records.Count(); i++)
   {
      local_options = main_frame->all_connection_records.Item(i);
   //    for ( uint f = 0 ; f < local_options.Count(); f++)
   //    {
   // 	 if (local_options.Item(f).Item(0).name == wxT("uniq_name"))
   // 	 {
   // 	    item_text_uniq_name = local_options.Item(f).Item(0).value;
   // 	 }
   //    }
      if (wxAtoi(local_options[wxT("uniq_name")]) ==  tree->curr_uniq_name)
      {
	 //wxMessageBox(local_options[wxT("uniq_name")]);

	 this->delete_item(id);
	 // 	 //tree->ReloadSettings();
	 //main_frame->all_connection_records.RemoveAt(i);
	 bc.Save(&main_frame->all_connection_records);
	 ReloadSettings();
	 local_options.clear();
	 break;
      }
   }
   return;
   if (!id.IsOk()) return;
   if (ItemHasChildren(id))
   {
      wxMessageDialog dialog(NULL, _T("Selected group is not empty\n Detete this ?"),
			     _T("Group deleting"),
			     wxNO_DEFAULT|wxYES_NO|wxCANCEL|wxICON_INFORMATION);
      if (dialog.ShowModal() != wxID_YES) return;
   }

   DeleteChildren(id);
   Delete(id);


}


void RDPTree::on_begin_drag(wxTreeEvent &event)
{
	if (IsFavorites) return;
	if (!IsRDPConn(event.GetItem())) return;
	curr_drag = event.GetItem();
	event.Allow();
}

void RDPTree::on_end_drag(wxTreeEvent &event)
{
	if (IsFavorites) return;
	wxTreeItemId dest = event.GetItem();

	if (!IsGroup(dest)) return;
	this->move_item(curr_drag,dest);
	this->SortChildren(dest);
	Expand(dest);
	return;
}


TreePanel::TreePanel(Main_Frame * main, wxSplitterWindow * parent, wxWindowID id, const wxPoint& pt, const wxSize& sz, long style):
wxPanel(parent,id,pt,sz, style)
{
	main_frame = main;

	tree_notebook = new wxNotebook(this,this->GetId(),wxPoint(0,5), wxSize(this->GetClientSize().x , this->GetClientSize().y - 5) , wxNB_TOP | wxSTATIC_BORDER);
	//wxMessageBox(this->GetId());
	rdptree = new RDPTree(main_frame,tree_notebook/*this*/, ID_RDPTREE,wxPoint(0,0), wxSize(this->GetClientSize().x, this->GetClientSize().y - 5)  ,wxTR_DEFAULT_STYLE | wxSUNKEN_BORDER | wxTR_HIDE_ROOT);

	rdptree->SetQuickBestSize(true);

	tree_notebook->AddPage(rdptree,wxT("General"),true);

	favorites = new RDPTree(main_frame,tree_notebook, ID_RDPTREE,wxPoint(0,0), wxSize(this->GetClientSize().x, this->GetClientSize().y - 5)  ,wxTR_DEFAULT_STYLE | wxSUNKEN_BORDER | wxTR_HIDE_ROOT);
	favorites->SetQuickBestSize(true);
	favorites->IsFavorites = TRUE;
	favorites->base = favorites->AppendItem(favorites->root,wxT("Favorites"),0);

	tree_notebook->AddPage(favorites,wxT("Favorites"),true);
	tree_notebook->SetSelection(0);

	this->Show(false);
}


void TreePanel::on_size_change(wxSizeEvent& event)
{
	tree_notebook->SetSize(wxSize(this->GetClientSize().x , this->GetClientSize().y - 5));

}

void RDPTree::on_kill_focus(wxFocusEvent& event)
{
#ifdef __WXMSW__
	this->main_frame->nb->SetFocus();

	if (this->main_frame->nb->GetPageCount() == 0)
	{
		event.Skip();
		return;
	}
	wxAuiNotebookEvent evt;
	evt.SetId(1);
	this->main_frame->notebook_change_func(evt);
#endif
	event.Skip();
}


CredsDialog::CredsDialog(wxWindow* parent, int dialogType,const wxString& groupname):
wxDialog(parent, wxID_ANY, groupname,
                 wxDefaultPosition, wxSize(320, 185),dialogType)
{
	m_static_username = NULL;
	m_text_username = NULL;

	m_static_password = NULL;
	m_text_password = NULL;

	m_static_domain = NULL;
	m_text_domain = NULL;

	m_button_ok = NULL;
	m_button_cancel = NULL;

	sizer_top = new wxBoxSizer(wxVERTICAL);
	sizer_btn = new wxBoxSizer(wxHORIZONTAL);
	sizer_creds = new wxFlexGridSizer(2,10,10);

	m_static_username = new wxStaticText(this, wxID_ANY, wxT("Username"),wxDefaultPosition, wxDefaultSize);
	m_text_username = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(190,-1), wxTE_PROCESS_ENTER);
	m_static_password = new wxStaticText(this, wxID_ANY, wxT("Password"),wxDefaultPosition, wxDefaultSize);
	m_text_password = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(190,-1), wxTE_PASSWORD);
	m_static_domain = new wxStaticText(this, wxID_ANY, wxT("Domain"),wxDefaultPosition, wxDefaultSize);
	m_text_domain = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(190,-1), wxTE_PROCESS_ENTER);

	sizer_creds->Add(m_static_username, 0, wxALL | wxALIGN_LEFT ,5);
	sizer_creds->Add(m_text_username, 0, wxALL | wxALIGN_LEFT ,2);
	sizer_creds->Add(m_static_password, 0, wxALL | wxALIGN_LEFT ,5);
	sizer_creds->Add(m_text_password, 0, wxALL | wxALIGN_LEFT ,2);
	sizer_creds->Add(m_static_domain, 0, wxALL | wxALIGN_LEFT ,5);
	sizer_creds->Add(m_text_domain, 0, wxALL | wxALIGN_LEFT ,2);

	m_button_ok = new wxButton(this,ID_CREDSDIALOG_OK,wxT("OK"),wxDefaultPosition,wxSize(100,-1));
	m_button_cancel = new wxButton(this,ID_CREDSDIALOG_CANCEL,wxT("Cancel"),wxDefaultPosition,wxSize(100,-1));

	sizer_btn->Add(m_button_ok,0,wxALL,5);
	sizer_btn->Add(m_button_cancel,0,wxALL,5);

	sizer_top->Add(sizer_creds,0,wxALL | wxALIGN_LEFT ,5);
	sizer_top->Add(sizer_btn,0,wxALL | wxALIGN_LEFT ,5);

	SetSizer(sizer_top);
	sizer_top->SetSizeHints(this);
	sizer_top->Fit(this);


}

CredsDialog::~CredsDialog()
{
	if(m_static_username) {delete m_static_username; m_static_username = NULL;}
	if(m_text_username) {delete m_text_username; m_text_username = NULL;}

	if(m_static_password) {delete m_static_password; m_static_password = NULL;}
	if(m_text_password) {delete m_text_password; m_text_password = NULL;}

	if(m_static_domain) {delete m_static_domain; m_static_domain = NULL;}
	if(m_text_domain) {delete m_text_domain; m_text_domain = NULL;}

	if(m_button_ok) {delete m_button_ok; m_button_ok = NULL;}
	if(m_button_cancel) {delete m_button_cancel; m_button_cancel = NULL;}

}

void CredsDialog::button_ok_func(wxCommandEvent &event)
{
	if (m_text_username) group_username = m_text_username->GetValue();
	if (m_text_password) group_password = m_text_password->GetValue();
	if (m_text_domain) group_domain = m_text_domain->GetValue();

	EndModal(1);
}

void CredsDialog::button_cancel_func(wxCommandEvent &event)
{
	EndModal(0);
}

void CredsDialog::dialog_hotkeys(wxKeyEvent &event)
{
	wxCommandEvent evt;
	evt.SetId(1);
	switch(event.GetKeyCode())
	{
	case WXK_RETURN:
		this->button_ok_func(evt);


		break;
	case WXK_ESCAPE:
		this->button_cancel_func(evt);
		break;
	default:
		break;

	}

	event.Skip();
}
