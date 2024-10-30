
// Gleb Popov
import java.util.Scanner;     // importing for input data
import java.util.List;        // importing just for implementing formal entrySet() function in HashMap
import java.util.ArrayList;   // importing just for implementing formal entrySet() function in HashMap

public final class Main {

    /** Utility classes should not have a public or default constructor (Checkstyle).*/
    private Main() { };

    /** The size of the small arrays.*/
    private static final int SMALL_ARRAY_SIZE = 5;

    private static int findMedian(int[] arr, int left, int right, int k) {

        // Sorting the array with respect to the pivot
        int[] leftArray = new int[right - left + 1];
        int[] rightArray = new int[right - left + 1];
        int leftCnt = 0;
        int rightCnt = 0;
        int pivot = arr[left];
        for (int i = left; i <= right; i++) {
            if (arr[i] < pivot) {
                leftArray[leftCnt++] = arr[i];
            } else if (arr[i] > pivot) {
                rightArray[rightCnt++] = arr[i];
            }
        }

        // Returing the median or continue searching
        if ((left + leftCnt) == k) {
            return pivot;
        } else if ((left + leftCnt) < k) {
            return findMedian(rightArray, 0, rightCnt - 1, k - leftCnt - 1 - left);
        } else {
            return findMedian(leftArray, 0, leftCnt - 1, k - left);
        }
    }

    private static int medOfMeds(int[] arr, int size, int k) {

        // Dividing the array into small arrays (<= 5 elements in each) and search its medians
        int arrCnt = (int) ((size + SMALL_ARRAY_SIZE - 1) / SMALL_ARRAY_SIZE); // devision with rounding up
        int[] smallArrays = new int[arrCnt];
        for (int i = 0; i < arrCnt; i++) {
            int start = i * SMALL_ARRAY_SIZE;
            int end = (i + 1) * SMALL_ARRAY_SIZE;
            if (end > size) {
                end = size;
            }
            smallArrays[i] = findMedian(arr, start, end - 1, (end + start) / 2);
        }

        // Finding the median of medians
        int pivot = findMedian(smallArrays, 0, arrCnt - 1, arrCnt / 2);

        // Sorting the array with respect to the pivot
        int[] leftArray = new int[size];
        int[] rightArray = new int[size];
        int leftCnt = 0;
        int rightCnt = 0;
        for (int i = 0; i < size; i++) {
            if (arr[i] < pivot) {
                leftArray[leftCnt++] = arr[i];
            } else if (arr[i] > pivot) {
                rightArray[rightCnt++] = arr[i];
            }
        }

        // Returing the median or continue searching
        if (leftCnt == k) {
            return pivot;
        } else if (leftCnt < k) {
            return medOfMeds(rightArray, rightCnt, k - leftCnt - 1);
        } else {
            return medOfMeds(leftArray, leftCnt, k);
        }
    }

    /** @param args command-line arguments.*/
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int size = sc.nextInt();
        int[] arr = new int[size];
        HashMap<Integer, String> names = new HashMap<>();
        for (int i = 0; i < size; i++) {
            int score = sc.nextInt();
            names.put(score, sc.next() + " " + sc.next());
            arr[i] = score;
        }
        System.out.println(names.get(medOfMeds(arr, size, size / 2)));
    }
}

interface Map<K, V> {
    int size();
    V get(K key);
    V put(K key, V value);
    List<KeyValuePair<K, V>> entrySet();
}

class KeyValuePair<K, V> {

    /** Key of the pair.*/
    private K key;

    /** Value of the pair.*/
    private V value;

    KeyValuePair(K key, V value) {
        this.key = key;
        this.value = value;
    }

    public K getKey() {
        return key;
    }

    public V getValue() {
        return value;
    }
}

class HashMap<K, V> implements Map<K, V> {

    /** Default size of the HashMap.*/
    private static final int DEFAULT_SIZE = 4;

    /** Maximal load factor (when we need to resize array).*/
    private static final double MAX_LOAD_FACTOR = 0.5;

    /** The current size of the HashMap.*/
    private int size = DEFAULT_SIZE;

    /** The current number of elements in the HashMap.*/
    private int numberOfElements = 0;

    /** Array of key-value pairs.*/
    private KeyValuePair<K, V>[] arr;

    HashMap() {
        this.arr = new KeyValuePair[size];
    }

    @Override
    public int size() {
        return size;
    }

    @Override
    public List<KeyValuePair<K, V>> entrySet() {
        List<KeyValuePair<K, V>> res = new ArrayList<>();
        for (int i = 0; i < size; i++) {
            if (arr[i] != null) {
                res.add(arr[i]);
            }
        }
        return res;
    }

    @Override
    public V get(K key) {

        int keyInt = key.hashCode();
        if (keyInt < 0) {
            keyInt = -keyInt;
        }
        keyInt %= size;
        int start = keyInt;

        while (arr[keyInt] != null) {
            if (arr[keyInt].getKey().equals(key)) {
                return arr[keyInt].getValue();
            }
            keyInt = (keyInt + 1) % size;
            if (keyInt == start) {
                return null;
            }
        }
        return null;
    }

    private void resize() {
        KeyValuePair<K, V>[] oldArr = arr;
        this.size *= 2;
        arr = new KeyValuePair[size];
        for (int i = 0; i < (size / 2); i++) {
            if (oldArr[i] != null) {
                put(oldArr[i].getKey(), oldArr[i].getValue());
            }
        }
    }

    @Override
    public V put(K key, V value) {

        if ((numberOfElements / size) > MAX_LOAD_FACTOR) {
            this.resize();
        }

        int keyInt = key.hashCode();
        if (keyInt < 0) {
            keyInt = -keyInt;
        }
        keyInt %= size;
        while (arr[keyInt] != null) {
            if (arr[keyInt].getKey().equals(key)) {
                return null;
            }
            keyInt = (keyInt + 1) % size;
        }
        arr[keyInt] = new KeyValuePair<>(key, value);
        numberOfElements++;
        return null;
    }
}
