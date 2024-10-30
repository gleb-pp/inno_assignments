private String kleeneAlgorithm(FsaInfo fsaInfo) {
        String[] states = fsaInfo.getStates();
        String[] alphabet = fsaInfo.getAlphabet();
        String[] transitions = fsaInfo.getTransitions();

        // Initialize the triple map for R
        int numStates = states.length;
        Map<Integer, String[][]> R = new HashMap<>();

        // Initialize R for k = -1
        String[][] R_minus_1 = new String[numStates][numStates];
        initializeRMinus1(R_minus_1, states, transitions);
        R.put(-1, R_minus_1);

        // Compute R for k = 0 to numStates - 1
        for (int k = 0; k < numStates; k++) {
            String[][] R_k = new String[numStates][numStates];
            for (int i = 0; i < numStates; i++) {
                for (int j = 0; j < numStates; j++) {
                    R_k[i][j] = "(" + R.get(k-1)[i][k] + ")(" + R.get(k-1)[k][k] + ")*(" + R.get(k-1)[k][j] + ")|(" + R.get(k-1)[i][j] + ")";
                }
            }
            R.put(k, R_k);
        }
        return R.get(numStates-1)[0][1];
    }

    private void initializeRMinus1(String[][] R_minus_1, String[] states, String[] transitions) {
        for (String stateI : states) {
            int i = getIndex(states, stateI);
            for (String stateJ : states) {
                int j = getIndex(states, stateJ);
                StringBuilder regex = new StringBuilder();
                boolean isCon = false;
                for (String transition : transitions) {
                    String[] parts = transition.split(">");
                    int sourceIndex = getIndex(states, parts[0]);
                    int targetIndex = getIndex(states, parts[2]);
                    String action = parts[1];
                    if (sourceIndex == i && targetIndex == j && stateI.equals(states[sourceIndex])) {
                        if (action != null) {
                            if (regex.length() > 0 && !regex.toString().equals("{}")) {
                                regex.append("|");
                            }
                            regex.append(action);
                            isCon = true;
                        }
                    }
                }
                // If there are no connections, use {}
                if (i == j && !regex.toString().contains("eps")) {
                    if (isCon) {
                        regex.append("|eps");
                    } else {
                        regex.append("eps");
                    }
                } else if (!isCon) {
                    regex.append("{}");
                }
                R_minus_1[i][j] = regex.toString();
            }
        }
    }
