#include<cstdio>
bool BinarySearch(int *arr,int n,int key)
{
    int start=0;
    int end=n-1;
    int mid;
    while(start<=end)
    {
        mid=(start+end)/2;
        if(arr[mid]==key)
        {
            return true;
        }
        else if(arr[mid]<key)
        {
            start=mid+1;
        }
        else
        {
            end=mid-1;
        }
    }
    return false;
}
int main(void)
{
    int arr[10]={1,2,4,5,7,8,9,19,21,30};//정렬 되있을때만 사용가능
    printf("%d\n",BinarySearch(arr,10,22));//없으므로 0
    printf("%d\n",BinarySearch(arr,10,30));//있으므로 1
}
