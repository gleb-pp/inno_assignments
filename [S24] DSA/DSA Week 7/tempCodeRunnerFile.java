// Gleb Popov
import java.util.Scanner;
import java.util.ArrayList;

public final class Main {
    public static void main(String[] args) {
        RedBlackTree tree = new RedBlackTree();
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        for (int i = 0; i < n; i++) {
            tree.addElement(sc.nextInt());
        }
        sc.close();
        tree.print();
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
    public int value;
    public Colors color;

    Node(Node parent, Node left, Node right, int value, Colors color) {
        this.parent = parent;
        this.left = left;
        this.right = right;
        this.value = value;
        this.color = color;
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
}

class RedBlackTree {
    private ArrayList<Node> tree;
    private Node root;

    RedBlackTree() {
        tree = new ArrayList<Node>();
        root = null;
    }

    public void addElement(int element) {
        if (tree.size() == 0) {
            root = new Node(null, null, null, element, Colors.BLACK);
            tree.add(root);
        } else {
            Node node = root;
            while (true) {
                if (element >= node.value) {
                    if (node.right != null) {
                        node = node.right;
                    } else {
                        Node newNode = new Node(node, null, null, element, Colors.RED);
                        tree.add(newNode);
                        node.setRight(newNode);
                        checkColors(newNode);
                        return;
                    }
                } else {
                    if (node.left != null) {
                        node = node.left;
                    } else {
                        Node newNode = new Node(node, null, null, element, Colors.RED);
                        tree.add(newNode);
                        node.setLeft(newNode);
                        checkColors(newNode);
                        return;
                    }
                }
            }
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

    public void print() {
        System.out.println(tree.size());
        for (int i = 0; i < tree.size(); i++) {
            int l, r;
            if (tree.get(i).left == null) {
                l = -1;
            } else {
                l = tree.indexOf(tree.get(i).left) + 1;
            }
            if (tree.get(i).right == null) {
                r = -1;
            } else {
                r = tree.indexOf(tree.get(i).right) + 1;
            }
            System.out.println(tree.get(i).value + " " + l + " " + r);
        }
        System.out.println(tree.indexOf(root) + 1);
    }
}