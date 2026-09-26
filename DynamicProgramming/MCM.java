class Solution {
  //Recursion with memoization
	private static int util(int i, int j, int arr[], int[][] dp) {
		if (i == j)return 0;
		
		if(dp[i][j]!=-1) return dp[i][j];
		
		int mini = Integer.MAX_VALUE;
		for (int k = i; k<j; k++) {
			mini = Math.min(mini, util(i, k, arr, dp) + util(k + 1, j, arr, dp) + arr[i - 1]*arr[k]*arr[j]);
		}
		return dp[i][j] = mini;
	}
	static int matrixMultiplication(int arr[]) {
		// code here
		int n = arr.length;
		int[][] dp = new int[n][n];
		for(int i=0;i<n;i++){
		    Arrays.fill(dp[i], -1);
		}
		return util(1, arr.length - 1, arr, dp);
	}
  //Bottom up DP

class Solution{
    static int matrixMultiplication(int n, int arr[])
    {
        // code here
        int[][] dp = new int[n][n];
        
        for(int i=1;i<n;i++){
            dp[i][i] = 0;
        }
        
        for(int l=2;l<=n;l++){
            for(int i=1;i<n;i++){
                int j=i+l-1;
                if(j>=n) continue;
                dp[i][j]=Integer.MAX_VALUE;
                for(int k=i;k<j;k++){
                  dp[i][j] = Math.min(dp[i][j], dp[i][k] + dp[k+1][j] + arr[i-1]*arr[k]*arr[j]);  
                }
            }
        }
        return dp[1][n-1];
    }
}
}
