class Solution {
    /*
    LIS calculated using binary search and printed using parent tracking
    */
    public ArrayList<Integer> getLIS(int arr[]) {
        // Code here
        int n = arr.length;
        
        int[] tail = new int[n];
        int[] parent = new int[n];
        
        Arrays.fill(parent, -1);
        int size = 0;
        
        for(int i=0;i<n;i++){
            int left = 0;
            int right = size;
            while(left<right){
                int mid = left + (right-left)/2;
                if(arr[tail[mid]]>=arr[i]){
                    right = mid;
                } else {
                    left = mid + 1;
                }
            }
            if(left>0){
                parent[i] = tail[left-1];
            }
            
            tail[left] = i;
            if(left==size){
                size++;
            }
        }
        
        ArrayList<Integer> res = new ArrayList<>();
        int curr = tail[size-1];
        while(curr!=-1){
            res.add(arr[curr]);
            curr = parent[curr];
        }
        Collections.reverse(res);
        return res;
    }
}