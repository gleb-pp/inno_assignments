// Gleb Popov
import java.util.Scanner;
import java.util.List;
import java.util.LinkedList;
import java.util.HashMap;

public final class Main {

    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);

        int size = sc.nextInt();
        String[] arr = new String[size];
        for (int i = 0; i < size; i++) {
            arr[i] = sc.next();
        }

        int M = sc.nextInt(), K = sc.nextInt(), L = sc.nextInt();
        StringBuilder res2 = new StringBuilder("\n");
        StringBuilder res3 = new StringBuilder("\n");

        // case 1 — always contains empty string
        System.out.print("_ ");
        M--;

        if (size == 1) {
            res2.append(arr[0] + " ");  // case 2 — if size = 1, L2 contains 1 letter
            res3.append("_");       // case 3 — if size = 1, L3 contatins only empty string
            K--; L--;
        }

        List<String> result = new LinkedList<>();
        HashMap<Character, Integer> counting;
        for (int i = 0; i < size; i++) {
            result.add(arr[i]);
            if (size == 2 && L > 0) {
                res3.append(arr[i] + " ");
                L--;
            }
        }
        
        int ignor = 0;
        while(M > 0 || K > 0 || L > 0) {
            
            int k = result.size();
            for (int combInd = ignor; combInd < k; combInd++) {
                for (int letterInd = 0; letterInd < size; letterInd++) {
                    String comba = result.get(combInd) + arr[letterInd];
                    result.add(comba);
                    counting = new HashMap<Character, Integer>();
                    int flag1 = 0;
                    for (int oo = 0; oo < comba.length(); oo++) {
                        counting.put(comba.charAt(oo), counting.getOrDefault(comba.charAt(oo), 0) + 1);
                        if (counting.get(comba.charAt(oo)) % 2 == 1) {
                            flag1 += 1;
                        } else {
                            flag1 -= 1;
                        }
                    }
    
                    if (M > 0 && flag1 == 0) {
                        M--;
                        System.out.print(comba + " ");
                    }
                    if (K > 0 && counting.keySet().size() == size) {
                        K--;
                        res2.append(comba);
                        res2.append(" ");
                    }
                    if (L > 0 && counting.keySet().size() == size - 1) {
                        L--;
                        res3.append(comba);
                        res3.append(" ");
                    }
                }
            }
            ignor = k;
        }
        System.out.println(res2 + "" + res3);
    }
}