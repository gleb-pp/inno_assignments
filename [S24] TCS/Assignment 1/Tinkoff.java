import java.util.Scanner;

public class Tinkoff {

    private static int search(int[] arr, int left, int right, int obj) {
        if (arr[(left + right) / 2] > obj) {
            if (right - left + 1 < 3) {
                return arr[(left + right) / 2];
            } else {
                return search(arr, left, (left + right) / 2, obj);
            }
        } else if (arr[(left + right) / 2] < obj) {
            if (right - left + 1 == 1) {
                return arr[(left + right) / 2];
            } else if (right - left + 1 == 2 && ((obj - arr[(left + right) / 2]) <= (arr[(left + right) / 2 + 1] - obj))) {
                return arr[(left + right) / 2];
            } else {
                return search(arr, (left + right) / 2, right, obj);
            }
        } else {
            return obj;
        }
    }
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        int k = sc.nextInt();
        int[] arr = new int[n];
        int[] res = new int[k];
        for (int i = 0; i < n; i++) {
            arr[i] = sc.nextInt();
        }
        for (int i = 0; i < k; i++) {
            res[i] = search(arr, 0, n - 1, sc.nextInt());
        }
        for (int i = 0; i < k; i++) {
            System.out.println(res[i]);
        }
    }
}
