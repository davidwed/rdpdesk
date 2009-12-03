#include "tree_group.hpp"

#include "main_window.hpp"

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


void RDPTree::ReloadSettings()
{
	if (IsFavorites) return;
	Benc bc;
	int lc = bc.Count(&main_frame->base);

	if (lc > 50)
	{
		wxBusyInfo wait(wxT("Please wait"));
	}

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
	
	RDPTree * favorites_tree = main_frame->m_panel_tree->favorites;
	favorites_tree->DeleteAllItems();
	favorites_tree->root = favorites_tree->AddRoot(wxT("Settings"), 0);
	favorites_tree->base = favorites_tree->AppendItem(favorites_tree->root,wxT("Favorites"),0);
	
	DeleteAllItems();
	root = this->AddRoot(wxT("Settings"), 0);

	base = this->AppendItem(root,wxT("Main"),0); 

	RDPConn lrdpconn;
	wxString item_text;

	for (int i = 0; i < lc; i++)
	{
		lrdpconn = bc.Get(&main_frame->base,i);
		wxTreeItemId id;

		item_text.Clear(); 
		if (lrdpconn.connection_name.Length() > 0)
		{
			item_text.assign(lrdpconn.connection_name);  
		}
		else
		{
			item_text.assign(lrdpconn.hostname);  
		}
	
		wxTreeItemId group_name;
		if (lrdpconn.group_name.Length() == 0)
		{
			group_name = base;
		}
		else
		{
			if (group_already_exists(lrdpconn.group_name))
			{
				group_name = find_this_group(lrdpconn.group_name);
			}
			else
			{
				group_name = AppendItem(root,lrdpconn.group_name,0);
			}
		}
		id = this->AppendItem(group_name,item_text,1);

		BOOL bDataExists = FALSE;
		for (int i = 0; i < count; i++)
		{
			if (saved_data[i].uniq_name == lrdpconn.uniq_name)
			{
				this->SetItemData(id,new RDPTreeData(saved_data[i].dwObjCount ,saved_data[i].dwConnCount,lrdpconn.uniq_name));
				bDataExists = TRUE;
				break;
			}
		}
		if (!bDataExists)
		{
			this->SetItemData(id,new RDPTreeData(0,0,lrdpconn.uniq_name));
		}

		if (lrdpconn.dwConnectionCount >= (DWORD)this->fcount) 
		{
			this->add_to_favorites(id,lrdpconn.dwConnectionCount);
		}
		this->redraw(id);

	}

	ExpandAll();
	delete [] saved_data;
	this->Refresh();
}

void RDPTree::LoadSettings() 
{
	if (IsFavorites) return;

	Benc bc;
	int lc = bc.Count(&main_frame->base);

	if (lc > 50)
	{
		wxBusyInfo wait(wxT("Please wait"));
	}
	
	RDPTree * favorites_tree = main_frame->m_panel_tree->favorites;
	favorites_tree->DeleteAllItems();
	favorites_tree->root = favorites_tree->AddRoot(wxT("Settings"), 0);
	favorites_tree->base = favorites_tree->AppendItem(favorites_tree->root,wxT("Favorites"),0);

	DeleteAllItems();
	root = this->AddRoot(wxT("Settings"), 0);

	base = this->AppendItem(root,wxT("Main"),0); 

	RDPConn lrdpconn;
	wxString item_text;

	for (int i = 0; i < lc; i++)
	{
		lrdpconn = bc.Get(&main_frame->base,i);
		wxTreeItemId id;
		
		item_text.Clear(); 
		if (lrdpconn.connection_name.Length() > 0)
		{
			item_text.assign(lrdpconn.connection_name);  
		}
		else
		{
			item_text.assign(lrdpconn.hostname);  
		}
	
		wxTreeItemId group_name;
		if (lrdpconn.group_name.Length() == 0)
		{
			group_name = base;
		}
		else
		{
			if (group_already_exists(lrdpconn.group_name))
			{
				group_name = find_this_group(lrdpconn.group_name);
			}
			else
			{
				group_name = AppendItem(root,lrdpconn.group_name,0);
			}
		}
		id = this->AppendItem(group_name,item_text,1);
		this->SetItemData(id,new RDPTreeData(0,0,lrdpconn.uniq_name));
		if (lrdpconn.dwConnectionCount >= (DWORD)this->fcount) 
		{
			this->add_to_favorites(id,lrdpconn.dwConnectionCount);
		}
		this->redraw(id);

	}

	ExpandAll();
	this->Refresh();
}

void RDPTree::delete_item(wxTreeItemId id)
{
	if (!id.IsOk()) return;
	if (id == root) return;
	if (ItemHasChildren(id))
	{
		if (IsFavorites) return;
		wxMessageDialog dialog(NULL, _T("Selected group is not empty\n Detete this ?"),_T("Group deleting"), wxNO_DEFAULT|wxYES_NO|wxCANCEL|wxICON_INFORMATION);
		if (dialog.ShowModal() != wxID_YES) return; 

		wxTreeItemIdValue cookie;
		wxTreeItemId curr = GetFirstChild(id,cookie);
		
		while (curr.IsOk())
		{
		
			RDPTreeData * rdptreedata = (RDPTreeData *)GetItemData(curr);
			if (!rdptreedata) continue;
			
			Benc bc;
			int line_num;
		
			line_num = bc.Find(&main_frame->base,rdptreedata->uniq_name);
			bc.Delete(&main_frame->base,line_num);
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
		int line_num;
		line_num = bc.Find(&main_frame->base,rdptreedata->uniq_name);
		bc.Delete(&main_frame->base,line_num);
		this->Delete(id);
	}
}

void RDPTree::on_activated(wxTreeEvent& event)
{
	RDPTree * tree = main_frame->m_panel_tree->rdptree;
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
	if (item == root) return FALSE;
	if (GetItemParent(item) != root) return FALSE;
	return TRUE;
}

BOOL RDPTree::IsRDPConn(wxTreeItemId item)
{
	if (!item.IsOk()) return FALSE;
	if (item == root) return FALSE;
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
			RDPConn lrdpconn;
			Benc bc;
			int line_num;
			line_num = bc.Find(&main_frame->base,rdptreedata->uniq_name);
			
			if (line_num >= 0)
			{
				lrdpconn = bc.Get(&main_frame->base,line_num);
				if (lrdpconn.dwConnectionCount < dwMin)
				{
					dwMin = lrdpconn.dwConnectionCount;
					*un = rdptreedata->uniq_name;
				}
			}
		}

		curr = GetNextChild(base,cookie);
	}

	return dwMin;
}


void RDPTree::add_to_favorites(wxTreeItemId item, DWORD dwCurrentConnectionCount)
{
	if (this->IsFavorites) return; 
	if (!IsRDPConn(item)) return;
	if (main_frame->m_panel_tree->favorites->item_already_exists(item)) return;
	

	if (!main_frame->m_panel_tree->favorites->IsFavoritesFull())
	{
		RDPTreeData * rdptreedata_id = (RDPTreeData *)GetItemData(item);
		if (!rdptreedata_id) return;
		RDPTreeData * f_rdptreedata_id = new RDPTreeData(rdptreedata_id->dwObjCount,rdptreedata_id->dwConnCount,rdptreedata_id->uniq_name);
	
		wxTreeItemId id = main_frame->m_panel_tree->favorites->AppendItem(main_frame->m_panel_tree->favorites->base,GetItemText(item),1);
		main_frame->m_panel_tree->favorites->SetItemData(id,f_rdptreedata_id);
		main_frame->m_panel_tree->favorites->Expand(main_frame->m_panel_tree->favorites->base);
	}
	else
	{
		int un = 0;
		int min_count = main_frame->m_panel_tree->favorites->find_min_favorites(&un);
		
		if (dwCurrentConnectionCount < (DWORD)min_count) 
		{
			return;
		}
		else
		{
			wxTreeItemId id_temp = 	main_frame->m_panel_tree->favorites->find_item_by_uniq_name(un);
			main_frame->m_panel_tree->favorites->Delete(id_temp);

			RDPTreeData * rdptreedata_id = (RDPTreeData *)GetItemData(item);
			if (!rdptreedata_id) return;
			RDPTreeData * f_rdptreedata_id = new RDPTreeData(rdptreedata_id->dwObjCount,rdptreedata_id->dwConnCount,rdptreedata_id->uniq_name);
	
			wxTreeItemId id = main_frame->m_panel_tree->favorites->AppendItem(main_frame->m_panel_tree->favorites->base,GetItemText(item),1);
			main_frame->m_panel_tree->favorites->SetItemData(id,f_rdptreedata_id);
	
			main_frame->m_panel_tree->favorites->Expand(main_frame->m_panel_tree->favorites->base);
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
	wxTreeItemId temp = AppendItem(group, GetItemText(item), 1);
	
	RDPTreeData * rdptreedata = (RDPTreeData *)GetItemData(item);
	if (!rdptreedata)
	{
		wxMessageBox(wxT("This note is invalid.\nDeleting note..."),wxT("Error"),wxICON_ERROR);
		this->Delete(item);
		return;
	}

	int uniq_name = rdptreedata->uniq_name;
	SetItemData(temp,new RDPTreeData(rdptreedata->dwObjCount, rdptreedata->dwConnCount, rdptreedata->uniq_name));
	redraw(temp);
	this->Delete(item);

	Benc bc;
	int ln = bc.Find(&main_frame->base,uniq_name);
	if (ln >= 0)
	{
		RDPConn rdpc = bc.Get(&main_frame->base,ln);
		rdpc.group_name = GetItemText(group);
		bc.Add(&main_frame->base,rdpc);
	}
}

void RDPTree::on_deleting_item(wxTreeEvent& event)
{
	
}

void RDPTree::createmenu(wxTreeEvent& event)
{
	RDPTree * tree = main_frame->m_panel_tree->rdptree;
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
			menu.Append(ID_TREE_SETCREDS,wxT("&Set credentials"));

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
			Benc bc;
			int ln = bc.Find(&main_frame->base,rdptreedata->uniq_name);
			if (ln >= 0)
			{
				RDPConn rdpc = bc.Get(&main_frame->base,ln);
				rdpc.username = cd->group_username;
				rdpc.password = cd->group_password;
				rdpc.domain = cd->group_domain;
				bc.Add(&main_frame->base,rdpc);
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

	Benc bc;
	RDPConn lrdpconn;
	
	int lc = bc.Count(&main_frame->base);

	for (int i = 0; i < lc; i++)
	{
		lrdpconn = bc.Get(&main_frame->base,i);
		if (lrdpconn.uniq_name ==  rdpconn->uniq_name)
		{
			RDPDialog * rdpdialog = new RDPDialog(main_frame,wxCAPTION,&lrdpconn,NULL,&(main_frame->base),FALSE/**/);
			rdpdialog->LoadRDPConn();
			wxCommandEvent evt;
			evt.SetId(1);
			rdpdialog->button_save_func(evt);
			delete rdpdialog;
			return;
		}
	}
}


void RDPTree::on_tree_change(wxCommandEvent& event)
{
	RDPTree * tree = main_frame->m_panel_tree->rdptree;
	Benc bc;
	RDPConn lrdpconn;
	int lc = bc.Count(&main_frame->base);

	for (int i = 0; i < lc; i++)
	{
		lrdpconn = bc.Get(&main_frame->base,i);
		if (lrdpconn.uniq_name ==  tree->curr_uniq_name)
		{
			wxString connection_name(lrdpconn.connection_name);
			wxString hostname(lrdpconn.hostname); 

			RDPDialog * rdpdialog = new RDPDialog(main_frame,wxCAPTION,&lrdpconn,NULL,&(main_frame->base),FALSE/*,*/);
			rdpdialog->LoadRDPConn();
			int iRes = rdpdialog->ShowModal();
			
			if (iRes)
			{
				tree->ReloadSettings();
			}
			delete rdpdialog;
			return;
		}
	}
}

void RDPTree::on_tree_clone(wxCommandEvent& event)
{
	RDPTree * tree = main_frame->m_panel_tree->rdptree;
	Benc bc;
	RDPConn lrdpconn;
	int lc = bc.Count(&main_frame->base);

	for (int i = 0; i < lc; i++)
	{
		lrdpconn = bc.Get(&main_frame->base,i);
		if (lrdpconn.uniq_name ==  tree->curr_uniq_name/*rdptreedata->uniq_name*/)
		{
			lrdpconn.uniq_name = bc.generate_uniq_name(&main_frame->base);
			lrdpconn.dwConnectionCount = 0;
			bc.Add(&main_frame->base,lrdpconn);
			tree->ReloadSettings();
			return;
		}
	}
}

void RDPTree::on_tree_delete_from_favorites(wxCommandEvent& event)
{
	wxTreeItemId item = GetSelection();
	RDPConn lrdpconn;
	
	RDPTreeData * rdptreedata = (RDPTreeData *)GetItemData(item);
	if (!rdptreedata)
	{
		Delete(item);
		return;
	}

	Benc bc;
	int ln = bc.Find(&main_frame->base,rdptreedata->uniq_name);
	if (ln >= 0)
	{
		RDPConn rdpc = bc.Get(&main_frame->base,ln);
		rdpc.dwConnectionCount = 0;
		bc.Add(&main_frame->base,rdpc);
	}
	
	Delete(item);
}


void RDPTree::on_add_group(wxCommandEvent& event)
{
	RDPTree * tree = main_frame->m_panel_tree->rdptree;

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
	RDPTree * tree = main_frame->m_panel_tree->rdptree;
	Benc bc;
	RDPConn lrdpconn;
	int lc = bc.Count(&main_frame->base);

	for (int i = 0; i < lc; i++)
	{
		lrdpconn = bc.Get(&main_frame->base,i);

		if (lrdpconn.uniq_name == tree->curr_uniq_name)
		{
			
			main_frame->AddRDP(lrdpconn,tree->curr_uniq_name); 

			wxTreeItemId temp = find_item_by_uniq_name(tree->curr_uniq_name);	
			Benc bc;
			int lc = bc.Count(&main_frame->base); 
			for (int i = 0; i < lc; i ++)
			{
				RDPConn rdpc = bc.Get(&main_frame->base,i);
				if (rdpc.uniq_name == tree->curr_uniq_name)
				{
					rdpc.dwConnectionCount ++;
					bc.Add(&main_frame->base,rdpc);
					
					if (rdpc.dwConnectionCount >= (DWORD)this->fcount)
					{
						add_to_favorites(temp,rdpc.dwConnectionCount);
					}
					break;
				}
			}

			return;
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
	wxTreeItemId id = GetSelection();
	this->delete_item(id);
	return;
	if (!id.IsOk()) return;
	if (ItemHasChildren(id))
	{
		wxMessageDialog dialog(NULL, _T("Selected group is not empty\n Detete this ?"),_T("Group deleting"), wxNO_DEFAULT|wxYES_NO|wxCANCEL|wxICON_INFORMATION);
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
