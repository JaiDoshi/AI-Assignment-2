#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>

using namespace std;

vector <pair <int, int>> sequence; 

struct cell
{
	int value = -1;
	
	vector <pair <int,int>> constraints;
	
	//only valid for cells which we have to find
	bool domain [10];
	
	int domain_size;
};

cell grid [10][10];

vector <pair<int, int>> get_constraints (int r, int c)
{
	vector <pair<int, int>> constraints;
	set <pair<int, int>> temp_constraints;
	
	for(int i=1; i<=9; ++i)
	{
		if(i!=r)
			temp_constraints.insert({i, c});
	}
	
	for(int j=1; j<=9; ++j)
	{
		if(j!=c)
			temp_constraints.insert({r, j});
	}
	
	int i = ((r-1)/3)*3 + 1, j = ((c-1)/3)*3 + 1;
	
	int temp_i = i, temp_j = j;
	
	for(; i<=temp_i+2; ++i)
	{
		for(j=temp_j; j<=temp_j+2; ++j)
		{
			if(i==r && j==c)
				continue;
			
			temp_constraints.insert({i, j});
		}
	}
	
	for(pair <int, int> each: temp_constraints)
	{
		constraints.push_back(each);
	}
	
	return constraints;
}

void pruneDomain()
{
	queue <pair<pair<int, int>, pair<int, int>>> constraints;
	
	for(int i=1; i<=9; ++i)
	{
		for(int j=1; j<=9; ++j)
		{
			pair <int, int> cur = {i, j};
			for(pair<int, int> each: grid[i][j].constraints)
				constraints.push({cur, each});
		}
	}
		
	while(!constraints.empty())
	{
		pair <int, int> L = constraints.front().first;
		pair <int, int> R = constraints.front().second;
		constraints.pop();
			
		if(grid[R.first][R.second].domain_size>1)
			continue;
		
		int val = grid[R.first][R.second].value;
		
		if(grid[L.first][L.second].domain[val]==0)
			continue;
		
		grid[L.first][L.second].domain[val] = 0;
		--grid[L.first][L.second].domain_size;
		
		for(pair<int, int> each: grid[L.first][L.second].constraints)
				constraints.push({each, L});
	}
}

bool backtrack()
{
	int lowest = 10;
	pair <int, int> cur;
	
	bool found = false;
	
	//choose most constrained cell
	for(int i=1; i<=9; ++i)
	{
		for(int j=1; j<=9; ++j)
		{
			if(grid[i][j].value==-1)
			{
				found = true;
				if(grid[i][j].domain_size < lowest)
				{
					lowest = grid[i][j].domain_size;
					cur = {i, j};
				}
			}
		}
	}
	
	if(!found)
		return true;
	
	vector <pair<int, int>> potential_value_list;
	
	//find least constraining value
	for(int potential_value = 1; potential_value<=9; ++potential_value)
	{
		if(grid[cur.first][cur.second].domain[potential_value]==0)
			continue;
		
		int LCV_cur_value = 9;
		
		for(pair <int, int> & each: grid[cur.first][cur.second].constraints)
		{
			if(grid[each.first][each.second].value!=-1)
				continue;
			
			if(grid[each.first][each.second].domain[potential_value]==1)
				LCV_cur_value = min(LCV_cur_value, grid[each.first][each.second].domain_size-1);
			else
				LCV_cur_value = min(LCV_cur_value, grid[each.first][each.second].domain_size);
		}
		
		if(LCV_cur_value!=0)
		{
			potential_value_list.push_back({potential_value, LCV_cur_value});
		}
	}
	
	sort(potential_value_list.begin(), potential_value_list.end(), [&](pair<int, int> & a, pair<int, int> & b){return a.second>b.second;});

	for(int i=0; i< (int) potential_value_list.size(); ++i)
	{
		int value = potential_value_list[i].first;
		
		grid[cur.first][cur.second].value = value;
		
		//perform forward checking
		vector <pair<int, int>> modified;
		
		for(pair <int, int> & each: grid[cur.first][cur.second].constraints)
		{
			if(grid[each.first][each.second].domain[value]==1)
			{
				grid[each.first][each.second].domain[value] = 0;
				--grid[each.first][each.second].domain_size;
				
				modified.push_back({each.first, each.second});
			}
		}
		
		if(!backtrack())
		{
			//reverse the changes made
			grid[cur.first][cur.second].value = -1;
			
			for(pair<int, int> & each: modified)
			{
				grid[each.first][each.second].domain[value] = 1;
				++grid[each.first][each.second].domain_size;
			}
		}
		
		else
			return true;
	}
	
	return false;
}

int main()
{
	for(int i=1; i<=9; ++i)
	{
		cout <<endl << "Enter row " << i << " of the sodoku with values separated by spaces. Put X if the value is to be found: " << endl;
		
		for(int j=1; j<=9; ++j)
		{
			char c;
			cin >> c;
			if(c=='X')
			{
				grid[i][j].domain_size = 9;
				fill(grid[i][j].domain+1, grid[i][j].domain+10, 1);
				grid[i][j].constraints = get_constraints(i, j);
				continue;
			}
			
			c -= '0';
			
			if(c>=1 && c<=9)
			{
				grid[i][j].constraints = get_constraints(i, j);
				grid[i][j].domain_size = 1;
				grid[i][j].value = c;
			}
			
			else
			{
				cout << endl << "Invalid input!" << endl;
				exit(1);
			}
		}
	}
	
	pruneDomain();
	
	if(backtrack())
	{
		cout << endl << "The solved sodoku is: " << endl;
		
		cout << endl;
		
		for(int i=1; i<=9; ++i)
		{
			for(int j=1; j<=9; ++j)
			{
				cout << grid[i][j].value << " ";
			}
			cout << endl;
		}
	}
	
	
	else
	{
		cout << endl << "Unable to solve. Check if input is correct." << endl;
	}
}