// Gleb Popov, ISE-01
// this code was written on Ali's DSAI-05 Lab (I visited this instead of ISE-01)
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;

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
            int[] parent = new int[n];

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

        List<Edge> mst = new ArrayList<Edge>();
        Collections.sort(edges);
        for (Edge e : edges) {
            if (disjoint.find(e.src) != disjoint.find(e.dist)) {
                disjoint.union(e.src, e.dist);
                mst.add(e);
            }
        }

        List<Integer> sources = new ArrayList<Integer>();
        List<List<Integer>> vertic = new ArrayList<List<Integer>>();
        List<List<Edge>> trees = new ArrayList<List<Edge>>();
        boolean[] vert = new boolean[n];
        for (Edge e : mst) {
            if (!sources.contains(disjoint.find(e.src))) {
                sources.add(disjoint.find(e.src));
                trees.add(new ArrayList<Edge>());
                vertic.add(new ArrayList<Integer>());
            }
            trees.get(sources.indexOf(disjoint.find(e.src))).add(e);
            vert[e.src] = true;
            vert[e.dist] = true;
            if (!vertic.get(sources.indexOf(disjoint.find(e.src))).contains(e.src)) {
                vertic.get(sources.indexOf(disjoint.find(e.src))).add(e.src);
            }
            if (!vertic.get(sources.indexOf(disjoint.find(e.src))).contains(e.dist)) {
                vertic.get(sources.indexOf(disjoint.find(e.dist))).add(e.dist);
            }
        }

        List<Integer> empt = new ArrayList<Integer>();
        for (int i = 0; i < n; i++) {
            if (!(vert[i])) {
                empt.add(i + 1);
            }
        }

        System.out.println(sources.size() + empt.size());
        for (List<Edge> t : trees) {
            System.out.println(vertic.get(trees.indexOf(t)).size() + " " + (t.get(0).src + 1));
            for (Edge e : t) {
                System.out.println((e.src + 1) + " " + (e.dist + 1) + " " + e.weight);
            }
        }
        for (Integer v : empt) {
            System.out.println("1 " + v);
        }
    }

    public static void main(String[] args) {
        List<Edge> edges = new ArrayList<Edge>();
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt(), k = sc.nextInt();
        for (int i = 0; i < k; i++) {
            edges.add(new Edge(sc.nextInt() - 1, sc.nextInt() - 1, sc.nextInt()));
        }
        kruskalMST(edges, n);
    }
}

interface DisjointIntSet {
    int find(int i);
    void union(int i, int j);
}