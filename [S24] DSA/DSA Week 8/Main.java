// Gleb Popov
import java.util.Scanner;
import java.util.LinkedList;
import java.util.ArrayList;

public final class Main {
    public static void main(String[] args) {
        RedBlackTree start = new RedBlackTree(); // stoing start and fin points
        RedBlackTree fin = new RedBlackTree();   // stoing fin and start points
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        for (int i = 0; i < n; i++) {
            int x1 = sc.nextInt();
            int y1 = sc.nextInt();
            int x2 = sc.nextInt();
            int y2 = sc.nextInt();
            if (y2 >= y1) {
                start.insert(x1, y1, x2, y2, i, false);
                fin.insert(x2, y2, x1, y1, i, false);
            } else {
                start.insert(x2, y2, x1, y1, i, true);
                fin.insert(x1, y1, x2, y2, i, true);
            }
        }
        sc.close();
        HashMap activeLines = new HashMap(n);
        boolean s = true;
        boolean f = true;
        boolean sEmpty = false;
        boolean fEmpty = false;
        Node startt = start.getMinimum();
        Node finn = fin.getMinimum();
        for (int i = 0; i < (n * 2 - 1); i++) {

            if (!s) {
                startt = start.getMinimum();
                s = true;
                sEmpty = (startt == null);
            }
            
            if (!f) {
                finn = fin.getMinimum();
                f = true;
                fEmpty = (finn == null);    
            }

            if (fEmpty) {
                Node left = activeLines.getLeft(startt);
                Node right = activeLines.getRight(startt);
                if (left != null) {
                    if (check(left, startt)) {
                        return;
                    }
                }
                if (right != null) {
                    if (check(right, startt)) {
                        return;
                    }
                }
                s = false;
                activeLines.put(startt.index, startt);
            } else if (!sEmpty) {
                if ((finn.y1) >= (startt.y1)) {
                    Node left = activeLines.getLeft(startt);
                    Node right = activeLines.getRight(startt);
                    if (left != null) {
                        if (check(left, startt)) {
                            return;
                        }
                    }
                    if (right != null) {
                        if (check(right, startt)) {
                            return;
                        }
                    }
                    s = false;
                    activeLines.put(startt.index, startt);
                }
            }

            if (sEmpty) {
                Node left = activeLines.getLeft(finn);
                Node right = activeLines.getRight(finn);
                if (left != null && right != null) {
                    if (check(left, right)) {
                        return;
                    }
                }
                activeLines.remove(finn.index);
                f = false;
            } else if (!fEmpty) {
                if ((finn.y1) < (startt.y1)) {
                    Node left = activeLines.getLeft(finn);
                    Node right = activeLines.getRight(finn);
                    if (left != null && right != null) {
                        if (check(left, right)) {
                            return;
                        }
                    }
                    activeLines.remove(finn.index);
                    f = false;
                }
            }
        }
        System.out.println("NO INTERSECTIONS");
    }

    public static void intersection(Node one, Node another) {
        System.out.println("INTERSECTION");
        if (one.reversed) {
            System.out.println(one.x2 + " " + one.y2 + " " + one.x1 + " " + one.y1);
        } else {
            System.out.println(one.x1 + " " + one.y1 + " " + one.x2 + " " + one.y2);
        }
        if (another.reversed) {
            System.out.println(another.x2 + " " + another.y2 + " " + another.x1 + " " + another.y1);
        } else {
            System.out.println(another.x1 + " " + another.y1 + " " + another.x2 + " " + another.y2);
        }
    }

    public static boolean check(Node one, Node another) {
        System.out.println("check");
        if (one.x1 == one.x2) {
            if (another.x1 == another.x2) {
                if (another.x1 == one.x1 && (max(one.x1, one.x2) >= min(another.x1, another.x2) || max(another.x1, another.x2) >= min(one.x1, one.x2))) {
                    intersection(one, another);
                    return true;
                } else {
                    return false;
                }
            } else {
                double k2 = (double)(another.y2 - another.y1) / (double)(another.x2 - another.x1);
                double b2 = (double)(another.y1) - k2 * (double)(another.x1);
                if (another.x1 <= another.x2 && another.x1 <= one.x1 && one.x1 <= another.x2) {
                    double y = k2 * (double)(one.x1) + b2;
                    if (y >= another.y1 && y <= another.y2) {
                        intersection(one, another);
                        return true;
                    } else {
                        return false;
                    }
                } else if (another.x1 >= another.x2 && another.x2 <= one.x1 && one.x1 <= another.x1) {
                    double y = k2 * (double)(one.x1) + b2;
                    if (y >= another.y1 && y <= another.y2) {
                        intersection(one, another);
                        return true;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            }
        }

        if (another.x1 == another.x2) {
            if (one.x1 == one.x2) {
                if (one.x1 == another.x1 && (max(another.x1, another.x2) >= min(one.x1, one.x2) || max(one.x1, one.x2) >= min(another.x1, another.x2))) {
                    intersection(another, one);
                    return true;
                } else {
                    return false;
                }
            } else {
                double k2 = (double)(one.y2 - one.y1) / (double)(one.x2 - one.x1);
                double b2 = (double)(one.y1) - k2 * (double)(one.x1);
                if (one.x1 <= one.x2 && one.x1 <= another.x1 && another.x1 <= one.x2) {
                    double y = k2 * (double)(another.x1) + b2;
                    if (y >= one.y1 && y <= one.y2) {
                        intersection(another, one);
                        return true;
                    } else {
                        return false;
                    }
                } else if (one.x1 >= one.x2 && one.x2 <= another.x1 && another.x1 <= one.x1) {
                    double y = k2 * (double)(another.x1) + b2;
                    if (y >= one.y1 && y <= one.y2) {
                        intersection(another, one);
                        return true;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            }
        }
        
        double k1 = (double)(one.y2 - one.y1) / (double)(one.x2 - one.x1);
        double b1 = (double)(one.y1) - k1 * (double)(one.x1);
        double k2 = (double)(another.y2 - another.y1) / (double)(another.x2 - another.x1);
        double b2 = (double)(another.y1) - k2 * (double)(another.x1);
        if (k2 == k1 && b1 == b2) {
            if (max(one.x1, one.x2) >= min(another.x1, another.x2) || max(another.x1, another.x2) >= min(one.x1, one.x2)) {
                intersection(one, another);
                return true;
            } else {
                return false;
            }
        } else if (k2 == k1 && b1 != b2) {
            return false;
        }
        double xPer = (b1 - b2) / (k2 - k1);
        double yPer = k1 * xPer + b1;
        if (xPer > min(one.x1, one.x2) && xPer < max(one.x1, one.x2) && yPer > one.y1 && yPer < one.y2) {
            if (xPer > min(another.x1, another.x2) && xPer < max(another.x1, another.x2) && yPer > another.y1 && yPer < another.y2) {
                intersection(one, another);
                return true;
            }
        }
        return false;
    }

    public static int min(int x, int y) {
        if (x >= y) {
            return y;
        } else {
            return x;
        }
    }

    public static int max(int x, int y) {
        if (x <= y) {
            return y;
        } else {
            return x;
        }
    }
}

enum Colors {
    RED,
    BLACK
}

class Node {
    public Node parent;
    public Node left;
    public Node right;
    public int x1, y1, x2, y2;
    public Colors color;
    public int index = -1;
    public boolean reversed;

    Node(Node parent, Node left, Node right, int x1, int y1, int x2, int y2, Colors color, int index, boolean rev) {
        this.parent = parent;
        this.left = left;
        this.right = right;
        this.x1 = x1;
        this.y1 = y1;
        this.x2 = x2;
        this.y2 = y2;
        this.color = color;
        this.index = index;
        this.reversed = rev;
    }

    public void setParent(Node parent) {
        this.parent = parent;
    }

    public void setLeft(Node left) {
        this.left = left;
    }

    public void setRight(Node right) {
        this.right = right;
    }

    public void setIndex(int ind) {
        this.index = ind;
    }
}

class RedBlackTree {
    private ArrayList<Node> tree;
    private Node root;

    RedBlackTree() {
        tree = new ArrayList<Node>();
        root = null;
    }

    public void insert(int x1, int y1, int x2, int y2, int ind, boolean rev) {
        if (tree.size() == 0) {
            root = new Node(null, null, null, x1, y1, x2, y2, Colors.BLACK, ind, rev);
            tree.add(root);
        } else {
            Node node = root;
            while (true) {
                if (y1 >= node.y1) {
                    if (node.right != null) {
                        node = node.right;
                    } else {
                        Node newNode = new Node(node, null, null, x1, y1, x2, y2, Colors.RED, ind, rev);
                        tree.add(newNode);
                        node.setRight(newNode);
                        checkColors(newNode);
                        return;
                    }
                } else {
                    if (node.left != null) {
                        node = node.left;
                    } else {
                        Node newNode = new Node(node, null, null, x1, y1, x2, y2, Colors.RED, ind, rev);
                        tree.add(newNode);
                        node.setLeft(newNode);
                        checkColors(newNode);
                        return;
                    }
                }
            }
        }
    }

    public Node remove(Node node, boolean del) {
        if (node.color == Colors.RED && node.left == null && node.right == null) {
            if (del) {
                tree.remove(node);
                if (node.parent.left == node) {
                    node.parent.setLeft(null);
                } else {
                    node.parent.setRight(null);
                }
            }
            return node;
        } else if (node.right != null && node.left != null) {
            Node newNode = node.right;
            while (newNode.left != null) {
                newNode = newNode.left;
            }
            int x11 = newNode.x1;
            int y11 = newNode.y1;
            int x21 = newNode.x2;
            int y21 = newNode.y2;
            int ind1 = newNode.index;
            boolean rev1 = newNode.reversed;
            newNode.x1 = node.x1;
            newNode.y1 = node.y1;
            newNode.x2 = node.x2;
            newNode.y2 = node.y2;
            newNode.reversed = node.reversed;
            newNode.setIndex(node.index);
            node.x1 = x11;
            node.x2 = x21;
            node.y1 = y11;
            node.y2 = y21;
            node.reversed = rev1;
            node.setIndex(ind1);
            return remove(newNode, del);
        } else if (node.left == null && node.right != null) {
            Node newNode = node.right;
            int x11 = newNode.x1;
            int y11 = newNode.y1;
            int x21 = newNode.x2;
            int y21 = newNode.y2;
            int ind1 = newNode.index;
            boolean rev1 = newNode.reversed;
            newNode.x1 = node.x1;
            newNode.y1 = node.y1;
            newNode.x2 = node.x2;
            newNode.y2 = node.y2;
            newNode.reversed = node.reversed;
            newNode.setIndex(node.index);
            node.x1 = x11;
            node.x2 = x21;
            node.y1 = y11;
            node.y2 = y21;
            node.reversed = rev1;
            node.setIndex(ind1);
            return remove(newNode, del);
        } else if (node.left != null && node.right == null) {
            Node newNode = node.left;
            int x11 = newNode.x1;
            int y11 = newNode.y1;
            int x21 = newNode.x2;
            int y21 = newNode.y2;
            int ind1 = newNode.index;
            boolean rev1 = newNode.reversed;
            newNode.x1 = node.x1;
            newNode.y1 = node.y1;
            newNode.x2 = node.x2;
            newNode.y2 = node.y2;
            newNode.reversed = node.reversed;
            newNode.setIndex(node.index);
            node.x1 = x11;
            node.x2 = x21;
            node.y1 = y11;
            node.y2 = y21;
            node.reversed = rev1;
            node.setIndex(ind1);
            return remove(newNode, del);
        } else if (node != root && node.parent.left == node) {
            Node brother = node.parent.right;
            if (brother == null) {
                brother.color = Colors.RED;
                if (node.parent.color == Colors.RED) {
                    node.parent.color = Colors.BLACK;
                } else {
                    node.parent.color = Colors.BLACK;
                    remove(node.parent, false);
                }
                return node;
            } else if (brother.color == Colors.BLACK) {
                if (brother.right != null && brother.right.color == Colors.RED) {
                    brother.color = node.parent.color;
                    node.parent.color = Colors.BLACK;
                    brother.right.color = Colors.BLACK;
                    Node B = node.parent;
                    Node A = brother;
                    if (B != root) {
                        if (B.parent.left == B) {
                            B.parent.setLeft(A);
                        } else {
                            B.parent.setRight(A);
                        }
                    } else {
                        root = A;
                    }
                    A.setParent(B.parent);
                    B.setParent(A);
                    B.setRight(A.left);
                    if (A.left != null) {
                        A.left.setParent(B);
                    }
                    A.setLeft(B);
                    if (del) {
                        tree.remove(node);
                        B.setLeft(null);
                    }
                    return node;
                } else if (brother.left != null && brother.left.color == Colors.RED) {
                    brother.color = Colors.RED;
                    brother.left.color = Colors.BLACK;
                    node.parent.setRight(brother.left);
                    brother.left.setParent(node.parent);
                    brother.setParent(brother.left);
                    brother.setLeft(brother.left.right);
                    if (brother.left != null) {
                        brother.left.setParent(brother);
                    }
                    node.parent.right.setRight(brother);
                    return remove(node, del);
                } else {
                    brother.color = Colors.RED;
                    if (node.parent.color == Colors.RED) {
                        node.parent.color = Colors.BLACK;
                        if (del) {
                            tree.remove(node);
                            node.parent.setLeft(null);
                        }
                    } else {
                        node.parent.color = Colors.BLACK;
                        if (del) {
                            tree.remove(node);
                            node.parent.setLeft(null);
                        }
                        remove(node.parent, false);
                    }
                    return node;
                }
            } else {
                node.parent.color = Colors.RED;
                brother.color = Colors.BLACK;
                Node B = node.parent;
                Node A = brother;
                if (B != root) {
                    if (B.parent.left == B) {
                        B.parent.setLeft(A);
                    } else {
                        B.parent.setRight(A);
                    }
                } else {
                    root = A;
                }
                A.setParent(B.parent);
                B.setParent(A);
                B.setRight(A.left);
                if (A.left != null) {
                    A.left.setParent(B);
                }
                A.setLeft(B);
                return remove(node, del);
            }
        } else if (node != root && node.parent.right == node) {
            Node brother = node.parent.left;
            if (brother == null) {
                brother.color = Colors.RED;
                if (node.parent.color == Colors.RED) {
                    node.parent.color = Colors.BLACK;
                } else {
                    node.parent.color = Colors.BLACK;
                    remove(node.parent, false);
                }
                return node;
            } else if (brother.color == Colors.BLACK) {
                if (brother.left != null && brother.left.color == Colors.RED) {
                    brother.color = node.parent.color;
                    node.parent.color = Colors.BLACK;
                    brother.left.color = Colors.BLACK;
                    Node B = node.parent;
                    Node A = brother;
                    if (B != root) {
                        if (B.parent.right == B) {
                            B.parent.setRight(A);
                        } else {
                            B.parent.setLeft(A);
                        }
                    } else {
                        root = A;
                    }
                    A.setParent(B.parent);
                    B.setParent(A);
                    B.setLeft(A.right);
                    if (A.right != null) {
                        A.right.setParent(B);
                    }
                    A.setRight(B);
                    if (del) {
                        node.parent.setRight(null);
                        tree.remove(node);
                    }
                    return node;
                } else if (brother.right != null && brother.right.color == Colors.RED) {
                    brother.color = Colors.RED;
                    brother.right.color = Colors.BLACK;
                    node.parent.setLeft(brother.right);
                    brother.right.setParent(node.parent);
                    brother.setParent(brother.right);
                    brother.setRight(brother.right.left);
                    if (brother.right != null) {
                        brother.right.setParent(brother);
                    }
                    node.parent.left.setRight(brother);
                    return remove(node, del);
                } else {
                    brother.color = Colors.RED;
                    if (del) {
                        tree.remove(node);
                        node.parent.setRight(null);
                    }
                    if (node.parent.color == Colors.RED) {
                        node.parent.color = Colors.BLACK;
                    } else {
                        node.parent.color = Colors.BLACK;
                        remove(node.parent, false);
                    }
                    return node;
                }
            } else {
                node.parent.color = Colors.RED;
                brother.color = Colors.BLACK;
                Node B = node.parent;
                Node A = brother;
                if (B != root) {
                    if (B.parent.right == B) {
                        B.parent.setRight(A);
                    } else {
                        B.parent.setLeft(A);
                    }
                } else {
                    root = A;
                }
                A.setParent(B.parent);
                B.setParent(A);
                B.setLeft(A.right);
                if (A.right != null) {
                    A.right.setParent(B);
                }
                A.setRight(B);
                return remove(node, del);
            }
        } else {
            tree.remove(node);
            return node;
        }
    }

    public Node getMinimum() {
        if (tree.size() == 0) {
            return null;
        } else {
            Node node = root;
            while (node.left != null) {
                node = node.left;
            }
            return remove(node, true);
        }
    }

    private void checkColors(Node node) {

        if (node == root) {
            node.color = Colors.BLACK;
        } else if (node.parent.color == Colors.RED) {

            Node uncle;
            if (node.parent.parent.left == node.parent) {
                uncle = node.parent.parent.right;
            } else {
                uncle = node.parent.parent.left;
            }

            if (uncle != null && uncle.color == Colors.RED) {
                uncle.color = Colors.BLACK;
                node.parent.color = Colors.BLACK;
                node.parent.parent.color = Colors.RED;
                checkColors(node.parent.parent);
            } else {
                if (node.parent.left == node && node.parent.parent.right == node.parent) {
                    Node parent = node.parent;
                    node.parent.parent.setRight(node);
                    node.setParent(node.parent.parent);
                    parent.setParent(node);
                    parent.setLeft(node.right);
                    node.setRight(parent);
                    checkColors(parent);
                } else if (node.parent.right == node && node.parent.parent.left == node.parent) {
                    Node parent = node.parent;
                    node.parent.parent.setLeft(node);
                    node.setParent(node.parent.parent);
                    parent.setParent(node);
                    parent.setRight(node.left);
                    node.setLeft(parent);
                    checkColors(parent);
                } else if (node.parent.right == node) {
                    Node B = node.parent.parent;
                    Node A = node.parent;
                    if (B != root) {
                        if (B.parent.left == B) {
                            B.parent.setLeft(A);
                        } else {
                            B.parent.setRight(A);
                        }
                    } else {
                        root = A;
                    }
                    A.setParent(B.parent);
                    B.setParent(A);
                    B.setRight(A.left);
                    if (A.left != null) {
                        A.left.setParent(B);
                    }
                    A.setLeft(B);

                    B.color = Colors.RED;
                    A.color = Colors.BLACK;
                } else {
                    Node B = node.parent.parent;
                    Node A = node.parent;
                    if (B != root) {
                        if (B.parent.left == B) {
                            B.parent.setLeft(A);
                        } else {
                            B.parent.setRight(A);
                        }
                    } else {
                        root = A;
                    }
                    A.setParent(B.parent);
                    B.setParent(A);
                    B.setLeft(A.right);
                    if (A.right != null) {
                        A.right.setParent(B);
                    }
                    A.setRight(B);

                    B.color = Colors.RED;
                    A.color = Colors.BLACK;
                }
            }
        }
    }
}

class KeyValuePair {
    private int key;
    private Node value;

    public KeyValuePair(int key, Node value) {
        this.key = key;
        this.value = value;
    }

    public int getKey() {
        return key;
    }

    public Node getValue() {
        return value;
    }

    public void setValue(Node val) {
        this.value = val;
    }
}

class HashMap {
    private int size;
    private ArrayList<LinkedList<KeyValuePair>> arr;

    public HashMap(int size) {
        this.size = size;
        this.arr = new ArrayList<>(size);
        for (int i = 0; i < size; i++) {
            arr.add(new LinkedList<>());
        }
    }

    public Node get(int key) {
        key %= size;
        LinkedList<KeyValuePair> list = arr.get(key);
        for (KeyValuePair pair : list) {
            if (pair.getKey() == key) {
                return pair.getValue();
            }
        }
        return null;
    }

    public void put(int key, Node value) {
        key %= size;
        LinkedList<KeyValuePair> list = arr.get(key);
        if (list.size() == 0)
        for (KeyValuePair pair : list) {
            if (pair.getKey() == key) {
                pair.setValue(value);
                return;
            }
        }
        list.add(new KeyValuePair(key, value));
        return;
    }

    public void remove(int key) {
        key %= size;
        LinkedList<KeyValuePair> list = arr.get(key);
        for (KeyValuePair pair : list) {
            if (pair.getKey() == key) {
                list.remove(pair);
                return;
            }
        }
        return;
    }

    public Node getLeft(Node node) {
        Node res = null;
        double maxx = -2147483648;
        for (LinkedList<KeyValuePair> list : arr) {
            for (KeyValuePair pair : list) {
                Node one = pair.getValue();
                double coord;
                if (one.x1 == one.x2) {
                    coord = one.x1;
                } else {
                    double k = (double)(one.y2 - one.y1) / (double)(one.x2 - one.x1);
                    double b = (double)(one.y1) - k * (double)(one.x1);
                    if (k == 0) {
                        coord = b;
                    } else {
                        coord = (node.y1 - b) / k;
                    }
                }
                if (coord < node.x1 && coord > maxx) {
                    maxx = coord;
                    res = one;
                }
            }
        }
        return res;
    }

    public Node getRight(Node node) {
        Node res = null;
        double minn = 2147483647;
        for (LinkedList<KeyValuePair> list : arr) {
            for (KeyValuePair pair : list) {
                Node one = pair.getValue();
                double coord;
                if (one.x1 == one.x2) {
                    coord = one.x1;
                } else {
                    double k = (double)(one.y2 - one.y1) / (double)(one.x2 - one.x1);
                    double b = (double)(one.y1) - k * (double)(one.x1);
                    if (k == 0) {
                        coord = b;
                    } else {
                        coord = (node.y1 - b) / k;
                    }
                }
                if (coord > node.x1 && coord < minn) {
                    minn = coord;
                    res = one;
                }
            }
        }
        return res;
    }
}