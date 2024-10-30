import java.text.DecimalFormat;
import java.util.Locale;

public final class Main {
    public static void main(String[] args) {
        Locale.setDefault(Locale.US);
        double a = 1/3;
        DecimalFormat df = new DecimalFormat("#.###");
        System.out.println(df.format(a));
    }
}