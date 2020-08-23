#include<iostream>
#include<vector>
using namespace std;
vector<int> arr{ 12,1,11,2,10,3,9,4,8,5,7,6 };
vector<int> tree(48, 0);
int init(vector<int> & arr, vector<int> &tree, int node, int start, int end)
{
	if (start == end)
		return tree[node] = arr[start];
	int mid = (start + end) / 2;
	return tree[node] = init(arr, tree, node * 2, start, mid) + init(arr, tree, node * 2 + 1, mid + 1, end);
}
void update(vector<int> &arr, vector<int> &tree, int node, int start, int end, int index, int diff)
{
	if ((start <= index && index <= end) == 0)
		return;
	tree[node] = tree[node] + diff;
	if (start != end)
	{
		int mid = (start + end) / 2;
		update(arr, tree, node * 2, start, mid, index, diff);
		update(arr, tree, node * 2 + 1, mid + 1, end, index, diff);
	}
}
int sum(vector<int> &arr, vector<int> &tree, int node, int start, int end, int left, int right)
{
	if (start > right || end < left)
		return 0;
	if (left <= start && end <= right)
		return tree[node];
	int mid = (start + end) / 2;
	return sum(arr, tree, node * 2, start, mid, left, right) + sum(arr, tree, node * 2 + 1, mid + 1, end, left, right);
}
int main(void)
{
	init(arr, tree, 1, 0, 11);
	for (int i = 0; i < 48; i++)
	{
		cout << tree[i] << " ";
	}
	cout << '\n';
	arr[5] = arr[5] + 10;
	update(arr, tree, 1, 0, 11, 5, 10);
	for (int i = 0; i < 48; i++)
	{
		cout << tree[i] << " ";
	}
	cout << '\n';
	cout<<sum(arr, tree, 1, 0, 11, 8, 11);
}
