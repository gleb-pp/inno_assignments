/* Gleb Popov */
import java.util.Scanner;

interface Stack<T> {
    void push(T item);
    T pop();
    T peek();
    int size();
    boolean isEmpty();
}

public final class Main implements Stack<String> {

    private String[] array;
    int size;
    int top;

    Main(int c) {
        array = new String[c];
        size = 0;
        top = -1;
    }

    @Override
    public void push(String item) {
        array[++top] = item;
        size++;
    }

    @Override
    public String pop() {
        size--;
        return array[top--];
    }

    @Override
    public String peek() {
        if (isEmpty()) {
            return "";
        } else {
            return array[top];
        }
    }

    @Override
    public int size() {
        return size;
    }

    @Override
    public boolean isEmpty() {
        return size == 0;
    }

    static private boolean isDigit(char a) {
        if (a == '1' || a == '2' || a == '3' || a == '4' || a == '5' || a == '6' || a == '7' || a == '8' || a == '9' || a == '0') {
            return true;
        } else {
            return false;
        }
    }

    
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        String line = sc.nextLine().trim();
        Main myStack = new Main(line.length() / 4);
        boolean space = false;

        int i = 0;
        while (i < line.length()) {
            char symb = line.charAt(i);
            if (isDigit(symb)) {
                System.out.print(symb);
                space = false;
                i++;
            } else if (symb == ' ' && !space) {
                System.out.print(' ');
                space = true;
                i++;
            } else if (symb == '(') {
                myStack.push(symb + " ");
                i += 2;
            } else if (symb == '*' || symb == '/') {
                while (myStack.peek().equals("* ") || myStack.peek().equals("/ ")) {
                    System.out.print(myStack.pop());
                }
                myStack.push(symb + " ");
                i += 2;
            } else if (symb == '+' || symb == '-') {
                while (myStack.peek().equals("* ") || myStack.peek().equals("/ ") || myStack.peek().equals("+ ") || myStack.peek().equals("- ")) {
                    System.out.print(myStack.pop());
                }
                myStack.push(symb + " ");
                i += 2;
            } else if (symb == 'm') {
                myStack.push("" + symb + line.charAt(i+1) + line.charAt(i+2) + ' ');
                i += 4;
            } else if (symb == ',') {
                while (!myStack.peek().equals("( ")) {
                    System.out.print(myStack.pop());
                }
                i += 2;
            } else if (symb == ')') {
                while (!myStack.peek().equals("( ")) {
                    System.out.print(myStack.pop());
                }
                myStack.pop();
                if (myStack.peek().equals("min ") || myStack.peek().equals("max ")) {
                    System.out.print(myStack.pop());
                }
                i += 2;
            }
        } 
        System.out.print(' ');
        while (!myStack.isEmpty()) {
            System.out.print(myStack.pop());
        }
    }
}