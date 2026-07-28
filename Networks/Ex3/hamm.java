class Solution {
    public int hammingDistance(int x, int y) {
        int d = x ^ y;
        int c = 0;
        while (d != 0) {
            c += d & 1;
            d >>>= 1;
        }
        return c;
    }
}
