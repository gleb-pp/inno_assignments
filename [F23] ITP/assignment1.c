# include <stdio.h>
# include <string.h>

int main() {
    // open file with input data
    FILE *fp ;
    fp = fopen("input.txt", "r") ;

    // input N, check the conditions for it and finish program if not correct
    int N;
    fscanf (fp, "%d", &N);
    if (N < 2 || N > 50) {
        fclose(fp);
        fp = fopen("output.txt", "w");
        fprintf (fp, "Invalid inputs\n");
        return 0;
    }

    // input S and check the conditions for it
    char S[N];
    char symb[72] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890()*!@#$%&^";
    fscanf (fp, "%s", &S);
    for (int i = 0; i < N; i++) {
        int flag = 0;
        for (int j = 0; j < 72; j++) {
            if (S[i] == symb[j]) {
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            fclose(fp);
            fp = fopen("output.txt", "w");
            fprintf (fp, "Invalid inputs\n");
            return 0;
        }
    }

    // input M and check the conditions for it
    int M;
    fscanf (fp, "%d", &M);
    if (M < 1) {
        fclose(fp);
        fp = fopen("output.txt", "w");
        fprintf (fp, "Invalid inputs\n");
        return 0;
    } else if ((N - 1) <= 26 && (M > (N - 1))) {
        fclose(fp);
        fp = fopen("output.txt", "w");
        fprintf (fp, "Invalid inputs\n");
        return 0;
    } else if ((N - 1) > 26 && (M > 26)) {
        fclose(fp);
        fp = fopen("output.txt", "w");
        fprintf (fp, "Invalid inputs\n");
        return 0; 
    }

    char alph[26] = "abcdefghijklmnopqrstuvwxyz";
    int finished[M];

    for (int i = 0; i < M; i++) {
        int change;
        fscanf (fp, "%d", &change);

        // check the conditions for N
        if (change >= N) {
            fclose(fp);
            fp = fopen("output.txt", "w");
            fprintf (fp, "Invalid inputs\n");
            return 0; 
        } else if (change > 26 || change < 1){
            fclose(fp);
            fp = fopen("output.txt", "w");
            fprintf (fp, "Invalid inputs\n");
            return 0; 
        } else {
            for (int j = 0; j < i; j++) {
                if (finished[j] == change) {
                    fclose(fp);
                    fp = fopen("output.txt", "w");
                    fprintf (fp, "Invalid inputs\n");
                    return 0; 
                }
            }
        }

        // replace symbol for another letter from alphabet
        S[change] = alph[change-1];
        finished[i] = change;
    }

    // close input file, open output file, put new string
    fclose(fp);
    fp = fopen("output.txt", "w");
    fprintf (fp, "%s\n", S);

    return 0;
}