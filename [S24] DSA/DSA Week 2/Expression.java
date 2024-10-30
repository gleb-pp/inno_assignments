/* Gleb Popov */
import java.util.Scanner;

public final class Expression implements Stack<String> {

    private String[] array;
    int size;
    int top;

    Expression(int c) {
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
        if (isEmpty()) {
            return "";
        } else {
            size--;
            return array[top--];
        }
    }

    @Override
    public String peek() {
        if (isEmpty()) {
            return " ";
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

    static private boolean isNumber(String str) {
        if (str.length() == 0) {
            return false;
        } else {
            char a = str.charAt(0);
            if (a == '1' || a == '2' || a == '3' || a == '4' || a == '5' || a == '6' || a == '7' || a == '8' || a == '9' || a == '0' || a == '-') {
                for (int i = 1; i < str.length(); i++) {
                    a = str.charAt(i);
                    if (a == '1' || a == '2' || a == '3' || a == '4' || a == '5' || a == '6' || a == '7' || a == '8' || a == '9' || a == '0') {
                        continue;
                    } else {
                        return false;
                    }
                }
            } else {
                return false;
            }
        }
        return true;
    }

    static private String oper(int arg1, int arg2, String operation) {
        switch (operation) {
            case "* ":
                return Integer.toString(arg1 * arg2);
            case "/ ":
                return Integer.toString(arg1 / arg2);
            case "+ ":
                return Integer.toString(arg1 + arg2);
            case "- ":
                return Integer.toString(arg1 - arg2);
            case "max ":
                if (arg1 > arg2) {
                    return Integer.toString(arg1);
                } else {
                    return Integer.toString(arg2);
                }
            default:
                if (arg1 > arg2) {
                    return Integer.toString(arg2);
                } else {
                    return Integer.toString(arg1);
                }
        }
    }

    
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        String line = sc.nextLine().trim();
        Expression stackOper = new Expression(line.length());
        Expression stackNumbers = new Expression(line.length());
        boolean space = false;

        int i = 0;
        while (i < line.length()) {
            char symb = line.charAt(i);
            if (isNumber("" + symb)) {
                if (space) {
                    stackNumbers.push("" + symb);
                } else {
                    stackNumbers.push(stackNumbers.pop() + symb);
                }
                space = false;
                i++;
            } else if (symb == ' ') {
                space = true;
                i++;
            } else if (symb == '(') {
                stackOper.push(symb + " ");
                i++;
            } else if (symb == '*' || symb == '/') {
                while (stackOper.peek().equals("* ") || stackOper.peek().equals("/ ")) {
                    stackNumbers.push(stackOper.pop());
                }
                stackOper.push(symb + " ");
                i++;
            } else if (symb == '+' || symb == '-') {
                while (stackOper.peek().equals("* ") || stackOper.peek().equals("/ ") || stackOper.peek().equals("+ ") || stackOper.peek().equals("- ")) {
                    stackNumbers.push(stackOper.pop());
                }
                stackOper.push(symb + " ");
                i++;
            } else if (symb == 'm') {
                stackOper.push("" + symb + line.charAt(i+1) + line.charAt(i+2) + ' ');
                i += 3;
            } else if (symb == ',') {
                while (!stackOper.peek().equals("( ")) {
                    stackNumbers.push(stackOper.pop());
                }
                i++;
            } else if (symb == ')') {
                while (!stackOper.peek().equals("( ")) {
                    stackNumbers.push(stackOper.pop());
                }
                stackOper.pop();
                if (stackOper.peek().equals("min ") || stackOper.peek().equals("max ")) {
                    stackNumbers.push(stackOper.pop());
                }
                i++;
            }
        }

        while (!stackOper.isEmpty()) {
            stackNumbers.push(stackOper.pop());
        }

        while (!stackNumbers.isEmpty()) {
            System.out.print(stackNumbers.pop() + " ");
        }

        while (!stackNumbers.isEmpty()) {
            if (isNumber(stackNumbers.peek()) && isNumber(stackOper.peek())) {
                System.out.println(stackNumbers.peek() + stackOper.peek());
                String temp = oper(Integer.parseInt(stackNumbers.pop()), Integer.parseInt(stackOper.pop()), stackOper.pop());
                while (isNumber(stackOper.peek())) {
                    temp = oper(Integer.parseInt(temp), Integer.parseInt(stackOper.pop()), stackOper.pop());
                }
                stackOper.push(temp);
            } else {
                stackOper.push(stackNumbers.pop());
            }
        }
        System.out.println(stackOper.peek());
    }
}

interface Stack<T> {
    void push(T item);
    T pop();
    T peek();
    int size();
    boolean isEmpty();
}