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
	
	bool domain [10] = {0};
	bool temp_domain [10];
	
	int domain_size, temp_domain_size;
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
	
	//cout << i << " " << j << endl;
	
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
	
	cout << constraints.size() << endl;
	
	while(!constraints.empty())
	{
		pair <int, int> L = constraints.front().first;
		pair <int, int> R = constraints.front().second;
		constraints.pop();
		
	//	cout << L.first << " " << L.second << " " << R.first << " " << R.second << " " << grid[L.first][L.second].domain_size << " " << grid[R.first][R.second].domain_size << endl;
		
		if(grid[R.first][R.second].domain_size>1)
			continue;
		
		int val = grid[R.first][R.second].value;
		
		if(grid[L.first][L.second].domain[val]==0)
			continue;
		
		grid[L.first][L.second].domain[val] = 0;
		grid[L.first][L.second].temp_domain[val] = 0;
		--grid[L.first][L.second].domain_size;
		--grid[L.first][L.second].temp_domain_size;
		
		for(pair<int, int> each: grid[L.first][L.second].constraints)
				constraints.push({each, L});
	}
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
				grid[i][j].temp_domain_size = 9;
				fill(grid[i][j].domain+1, grid[i][j].domain+10, 1);
				fill(grid[i][j].temp_domain+1, grid[i][j].temp_domain+10, 1);
				grid[i][j].constraints = get_constraints(i, j);
				continue;
			}
			
			c -= '0';
			
			if(c>=1 && c<=9)
			{
				grid[i][j].constraints = get_constraints(i, j);
				grid[i][j].domain_size = 1;
				grid[i][j].temp_domain_size = 1;
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
	
	for(int i=1; i<=9; ++i)
	{
		for(int j=1; j<=9; ++j)
		{
			cout << i << " " << j << " " << grid[i][j].domain_size << endl;
		}
	}
	
	
}