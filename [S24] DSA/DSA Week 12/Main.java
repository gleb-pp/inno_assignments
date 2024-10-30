// Gleb Popov, ISE-01
// this code was written on Ali's DSAI-05 Lab (I visited this instead of ISE-01)
import java.util.LinkedList;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;
import java.util.ArrayList;

public class Main {

    static class Edge implements Comparable<Edge> {
        int src;
        int dist;
        int weight;

        public Edge (int src, int dist, int weight) {
            this.src = src;
            this.dist = dist;
            this.weight = weight;
        }

        public int compareTo(Edge other) {
            return Integer.compare(this.weight, other.weight);
        }
    }

    public static void kruskalMST(List<Edge> edges, int n) {
        DisjointIntSet disjoint = new DisjointIntSet() {
            int[] parent = new int[n + 1];

            @Override
            public int find(int i) {
                if (parent[i] == 0)
                    return i;
                parent[i] = find(parent[i]);
                return parent[i];
            }

            @Override
            public void union(int i, int j) {
                parent[find(i)] = find(j);
            }
        };

        List<Edge> mst = new LinkedList<Edge>();
        Collections.sort(edges);
        for (Edge e : edges) {
            if (disjoint.find(e.src) != disjoint.find(e.dist)) {
                disjoint.union(e.src, e.dist);
                mst.add(e);
            }
        }

        HashMap<Integer, List<Edge>> trees = new HashMap<>();
        HashMap<Integer, List<Integer>> vertices = new HashMap<>();
        boolean[] vert = new boolean[n + 1];
        for (Edge e : mst) {
            if (trees.get(disjoint.find(e.src)) == null) {
                trees.put(disjoint.find(e.src), new LinkedList<Edge>());
                vertices.put(disjoint.find(e.src), new LinkedList<Integer>());
            }
            trees.get(disjoint.find(e.src)).add(e);
            if (!vertices.get(disjoint.find(e.src)).contains(e.src)) {
                vertices.get(disjoint.find(e.src)).add(e.src);
            }
            if (!vertices.get(disjoint.find(e.src)).contains(e.dist)) {
                vertices.get(disjoint.find(e.src)).add(e.dist);
            }
            vert[e.src] = true;
            vert[e.dist] = true;
        }

        List<Integer> aloneVertices = new LinkedList<Integer>();
        for (int i = 1; i < n + 1; i++) {
            if (!(vert[i])) {
                aloneVertices.add(i);
            }
        }

        System.out.println(trees.size() + aloneVertices.size());
        for (var t : trees.entrySet()) {
            System.out.println(vertices.get(t.getKey()).size() + " " + t.getKey());
            for (Edge e : t.getValue()) {
                System.out.println(e.src + " " + e.dist + " " + e.weight);
            }
        }
        for (Integer v : aloneVertices) {
            System.out.println("1 " + v);
        }
    }

    public static void main(String[] args) {
        List<Edge> edges = new LinkedList<Edge>();
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt(), k = sc.nextInt();
        for (int i = 0; i < k; i++) {
            edges.add(new Edge(sc.nextInt(), sc.nextInt(), sc.nextInt()));
        }
        kruskalMST(edges, n);
    }
}

interface DisjointIntSet {
    int find(int i);
    void union(int i, int j);
}

interface Map<K, V> {
    int size();
    V get(K key);
    V put(K key, V value);
    List<KeyValuePair<K, V>> entrySet();
}
 
class KeyValuePair<K, V> {
    private K key = null;
    private V value = null;
 
    public KeyValuePair(K key, V value) {
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
    private int size = 4;
    private int numberOfElements = 0;
    private KeyValuePair<K, V>[] arr;
 
    public HashMap() {
        this.arr = new KeyValuePair[4];
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
 
        if ((numberOfElements / size) > 0.5) {
            this.resize();
        }
 
        int keyInt = key.hashCode();
        if (keyInt < 0) {
            keyInt = -keyInt;
        }
        keyInt %= size;
        int start = keyInt;
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