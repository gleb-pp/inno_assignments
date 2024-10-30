import java.io.File;
import java.io.IOException;
import java.util.*;

public class Main {
    private static Scanner scanner;
    private static boolean letterChecks = true;

    public static void main(String[] args) throws IOException {
        try {

            var file = new File("./input.txt");
            scanner = new Scanner(file);
            var type = parseType();
            var fsa = new FSA();
            for (var state : parseStates()) {
                fsa.addState(state);
            }
            for (var letter : parseAlphabet()) {
                fsa.addLetter(letter);
            }
            fsa.setInitialState(parseInitialState());
            for (var finalState : parseFinalStates()) {
                fsa.addFinalState(finalState);
            }
            for (var transition : parseTransition()) {
                fsa.addTransition(transition[0], transition[2], transition[1]);
            }
            fsa.checkConnectness();
            if (type.equals("deterministic"))
                fsa.checkDeterminism();
            System.out.println(fsa.buildRegularExpression());
        } catch (IllegalArgumentException e) {
            System.out.println(e.getMessage());
        }
    }

    public static String parseType() {
        var str = scanner.nextLine();
        if (!str.matches("^type=\\[(deterministic|non-deterministic)\\]$"))
            throw new IllegalArgumentException("E1: Input file is malformed");
        return str.substring(6, str.length() - 1);
    }

    public static String[] parseStates() {
        var str = scanner.nextLine();
        if (!str.matches("^states=\\[[a-zA-Z0-9,\\s]+\\]$") || str.charAt(str.length() - 2) == ',')
            throw new IllegalArgumentException("E1: Input file is malformed");
        var states = str.substring(8, str.length() - 1).split(",");
        for (var state : states)
            if (state.isEmpty() || state.isBlank())
                throw new IllegalArgumentException("E1: Input file is malformed");
        return states;
    }

    public static String[] parseAlphabet() {
        var str = scanner.nextLine();
        if (!str.matches("^alphabet=\\[[a-zA-Z0-9,_\\s]+\\]$") || str.charAt(str.length() - 2) == ',')
            throw new IllegalArgumentException("E1: Input file is malformed");
        var alphabet = str.substring(10, str.length() - 1).split(",");
        for (var letter : alphabet)
            if (letterChecks && !isValidLetter(letter))
                throw new IllegalArgumentException("E1: Input file is malformed");
        return alphabet;
    }

    public static String parseInitialState() {
        var str = scanner.nextLine();
        if (!str.startsWith("initial=[") || !str.endsWith("]") || str.charAt(str.length() - 2) == ',')
            throw new IllegalArgumentException("E1: Input file is malformed");
        var initialState = str.substring(9, str.length() - 1);
        if (initialState.isEmpty() || initialState.isBlank())
            throw new IllegalArgumentException("E2: Initial state is not defined");
        return initialState;
    }

    public static String[] parseFinalStates() {
        var str = scanner.nextLine();
        if (!str.startsWith("accepting=[") || !str.endsWith("]") || str.charAt(str.length() - 2) == ',')
            throw new IllegalArgumentException("E1: Input file is malformed");
        var statesStr = str.substring(11, str.length() - 1);
        if (statesStr.isEmpty() || statesStr.isBlank())
            throw new IllegalArgumentException("E3: Set of accepting states is empty");
        var finalStates = statesStr.split(",");
        for (var state : finalStates)
            if (state.isEmpty() || state.isBlank())
                throw new IllegalArgumentException("E1: Input file is malformed");
        return finalStates;
    }

    public static ArrayList<String[]> parseTransition() {
        var str = scanner.nextLine();
        if (!str.startsWith("transitions=[") || !str.endsWith("]") || str.charAt(str.length() - 2) == ',')
            throw new IllegalArgumentException("E1: Input file is malformed");
        var transitions = new ArrayList<String[]>();
        var transitionSet = new HashSet<String>();
        for (var transition : str.substring(13, str.length() - 1).split(",")) {
            if(transitionSet.contains(transition))
                throw new IllegalArgumentException("E1: Input file is malformed");
            var parts = transition.split(">");
            if (parts[0].isEmpty() || parts[1].isEmpty() || parts[2].isEmpty())
                throw new IllegalArgumentException("E1: Input file is malformed");
            transitions.add(parts);
            transitionSet.add(transition);
        }
        return transitions;
    }

    public static boolean isValidLetter(String letter) {
        return letter.matches("[\\d\\w_]+");
    }
}

class FSA {
    private HashMap<Integer, ArrayList<Edge>> adjacencyMap;
    private int numberOfStates;
    private HashMap<String, Integer> statesMap;
    private int initialState;
    private HashSet<Integer> finalStates;
    private HashSet<String> alphabet;

    public FSA() {
        numberOfStates = 0;
        adjacencyMap = new HashMap<>();
        statesMap = new HashMap<>();
        finalStates = new HashSet<>();
        alphabet = new HashSet<>();
    }

    public void addState(String name) {
        if(statesMap.containsKey(name))
            return;
        statesMap.put(name, numberOfStates++);
        adjacencyMap.put(statesMap.get(name), new ArrayList<>());
    }

    public void addLetter(String letter) {
        alphabet.add(letter);
    }

    public void addTransition(String from, String to, String letter) {
        if (!statesMap.containsKey(from))
            throw new IllegalArgumentException("E4: A state '" + from + "' is not in the set of states");
        if (!statesMap.containsKey(to))
            throw new IllegalArgumentException("E4: A state '" + to + "' is not in the set of states");
        if (!alphabet.contains(letter))
            throw new IllegalArgumentException("E5: A transition '" + letter + "' is not represented in the alphabet");
        var fromState = statesMap.get(from);
        var toState = statesMap.get(to);
        adjacencyMap.get(fromState).add(new Edge(fromState, toState, letter));
    }

    public void setInitialState(String state) {
        if (!statesMap.containsKey(state))
            throw new IllegalArgumentException("E4: A state '" + state + "' is not in the set of states");
        this.initialState = statesMap.get(state);
    }

    public void addFinalState(String state) {
        if (!statesMap.containsKey(state))
            throw new IllegalArgumentException("E4: A state '" + state + "' is not in the set of states");
        finalStates.add(statesMap.get(state));
    }

    public void checkConnectness() {
        var visited = new HashSet<Integer>();
        var stack = new Stack<Integer>();
        stack.push(initialState);
        while (!stack.isEmpty()) {
            var state = stack.pop();
            if (visited.contains(state)) continue;
            visited.add(state);
            for (var neighbour : adjacencyMap.get(state)) {
                stack.push(neighbour.getTo());
            }
        }
        if (visited.size() != adjacencyMap.size()) throw new IllegalArgumentException("E6: Some states are disjoint");
    }

    public void checkDeterminism() {
        var letters = new HashSet<String>();
        for (var stateTransitions : adjacencyMap.values()) {
            for (var transition : stateTransitions) {
                if (letters.contains(transition.getLetter()))
                    throw new IllegalArgumentException("E7: FSA is non-deterministic");
                letters.add(transition.getLetter());
            }
            letters.clear();
        }
    }
    public String buildRegularExpression() {
        var expression = new StringBuilder();
        for (var finalState : finalStates) {
            expression.append("(").append(R(initialState, finalState, numberOfStates - 1)).append(")|");
        }
        expression.deleteCharAt(expression.length() - 1);
        return expression.toString();
    }

    private String R(int i, int j, int k) {
        if (k == -1) return RBaseCase(i, j);
        return "(" + R(i, k, k - 1)
                + ")(" + R(k, k, k - 1)
                + ")*(" + R(k, j, k - 1)
                + ")|(" + R(i, j, k - 1) + ")";
    }

    private String RBaseCase(int i, int j) {
        var result = new StringBuilder();
        for (var transition : adjacencyMap.get(i)) {
            if (transition.getTo() != j) continue;
            result.append(transition.getLetter()).append("|");
        }
        if (i == j) result.append("eps|");
        if (result.isEmpty()) return "{}";
        else result.deleteCharAt(result.length() - 1);

        return result.toString();

    }
}

class Edge {
    private int from;
    private int to;
    private String letter;

    Edge(int from, int to, String letter) {
        this.from = from;
        this.to = to;
        this.letter = letter;
    }

    public int getFrom() {
        return from;
    }

    public int getTo() {
        return to;
    }

    public String getLetter() {
        return letter;
    }
}