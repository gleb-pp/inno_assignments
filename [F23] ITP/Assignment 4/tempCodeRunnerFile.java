import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Scanner;
import java.util.List;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Iterator;

public final class Main {

    /**. main gameBoard */
    private static Board gameBoard;

    private Main() {
        // empty constructor just to avoid provlems with default contructor
    }

    private static void checkBoardSize(String line) throws InvalidBoardSizeException {

        // String is not empty
        if (line.isEmpty()) {
            throw new InvalidBoardSizeException();
        }

        // String is Integer
        try {
            Integer.parseInt(line);
        } catch (NumberFormatException e) {
            throw new InvalidBoardSizeException();
        }

        // D in interval
        final int lowerBound = 4;
        final int upperBound = 1000;
        int d = Integer.parseInt(line);
        if (d < lowerBound || d > upperBound) {
            throw new InvalidBoardSizeException();
        }

    }

    private static void checkNumberOfInsects(String line) throws InvalidNumberOfInsectsException {

        // String is not empty
        if (line.isEmpty()) {
            throw new InvalidNumberOfInsectsException();
        }

        // String is Integer
        try {
            Integer.parseInt(line);
        } catch (NumberFormatException e) {
            throw new InvalidNumberOfInsectsException();
        }

        // N in interval
        final int lowerBound = 1;
        final int upperBound = 16;
        int n = Integer.parseInt(line);
        if (n < lowerBound || n > upperBound) {
            throw new InvalidNumberOfInsectsException();
        }

    }

    private static void checkNumberOfFoodPoints(String line) throws InvalidNumberOfFoodPointsException {

        // String is not empty
        if (line.isEmpty()) {
            throw new InvalidNumberOfFoodPointsException();
        }

        // String is Integer
        try {
            Integer.parseInt(line);
        } catch (NumberFormatException e) {
            throw new InvalidNumberOfFoodPointsException();
        }

        // M in interval
        final int lowerBound = 1;
        final int upperBound = 200;
        int m = Integer.parseInt(line);
        if (m < lowerBound || m > upperBound) {
            throw new InvalidNumberOfFoodPointsException();
        }

    }

    private static void checkInsectColor(String line) throws InvalidInsectColorException {

        // String is not empty
        if (line.isEmpty()) {
            throw new InvalidInsectColorException();
        }

        line = line.toLowerCase();
        if (!line.equals("red") && !line.equals("green") && !line.equals("blue") && !line.equals("yellow")) {
            throw new InvalidInsectColorException();
        }

    }

    private static void checkInsectType(String line) throws InvalidInsectTypeException {

        // String is not empty
        if (line.isEmpty()) {
            throw new InvalidInsectTypeException();
        }

        line = line.toLowerCase();
        if (!line.equals("ant") && !line.equals("spider") && !line.equals("butterfly") && !line.equals("grasshopper")) {
            throw new InvalidInsectTypeException();
        }

    }

    private static void checkEntityPosition(String line1, String line2, int d) throws InvalidEntityPositionException {

        // String is not empty
        if (line1.isEmpty() || line2.isEmpty()) {
            throw new InvalidEntityPositionException();
        }

        // String is Integer
        try {
            Integer.parseInt(line1);
            Integer.parseInt(line2);
        } catch (NumberFormatException e) {
            throw new InvalidEntityPositionException();
        }

        // X and Y coordinates are within the scope of the field
        int x = Integer.parseInt(line1);
        int y = Integer.parseInt(line2);
        if (x > d || x < 1 || y > d || y < 1) {
            throw new InvalidEntityPositionException();
        }

    }

    private static void checkDuplicateInsects(InsectColor color, String type) throws DuplicateInsectsException {

        // error if there are two insects with the same color and type
        for (Insect insect : Insect.getInsects()) {
            if (insect.getInsectType().toLowerCase().equals(type) && insect.getColor().equals(color)) {
                throw new DuplicateInsectsException();
            }
        }
    }

    private static void checkTwoEntitiesOnSamePosition(int x, int y) throws TwoEntitiesOnSamePositionException {

        // error if there is already some object on (x, y)
        if (!(gameBoard.getEntity(new EntityPosition(x, y)) == (null))) {
            throw new TwoEntitiesOnSamePositionException();
        }
    }

    /**
     * function with main program.
     * @param args are main function's parametrs.
     */
    public static void main(String[] args) {
        File file = new File("input.txt"); // file with input data
        try {
            Scanner sc = new Scanner(file);
            FileWriter writer = new FileWriter("output.txt"); // file with output data
            String line; // the line being received

            line = sc.nextLine(); // input board size
            try {
                checkBoardSize(line);
            } catch (InvalidBoardSizeException e) {
                writer.write(e.getMessage()); // writing the error to a file
                writer.close(); System.exit(0); // close the file and terminate the program
            }
            int d = Integer.parseInt(line);
            gameBoard = new Board(d);

            line = sc.nextLine();
            try {
                checkNumberOfInsects(line);
            } catch (InvalidNumberOfInsectsException e) {
                writer.write(e.getMessage()); // writing the error to a file
                writer.close(); System.exit(0); // close the file and terminate the program
            }
            int n = Integer.parseInt(line);

            line = sc.nextLine();
            try {
                checkNumberOfFoodPoints(line);
            } catch (InvalidNumberOfFoodPointsException e) {
                writer.write(e.getMessage()); // writing the error to a file
                writer.close(); System.exit(0); // close the file and terminate the program
            }
            int m = Integer.parseInt(line);

            // input of insects
            for (int i = 0; i < n; i++) {
                line = sc.nextLine();
                String[] words = line.split(" ");
                final int colorInd = 0; // index of Insect Color in the input line
                final int typeInd = 1; // index of Insect Type in the input line
                final int xInd = 2; // index of Insect X coordinate in the input line
                final int yInd = 3; // index of Insect Y coordinate in the input line
                // checking color
                line = words[colorInd];
                try {
                    checkInsectColor(line);
                } catch (InvalidInsectColorException e) {
                    writer.write(e.getMessage()); // writing the error to a file
                    writer.close(); System.exit(0); // close the file and terminate the program
                }
                InsectColor color = InsectColor.toColor(line);
                // checking index
                line = words[typeInd];
                try {
                    checkInsectType(line);
                } catch (InvalidInsectTypeException e) {
                    writer.write(e.getMessage()); // writing the error to a file
                    writer.close(); System.exit(0); // close the file and terminate the program
                }
                // checking coordinates (within the boundaries of the board)
                try {
                    checkEntityPosition(words[xInd], words[yInd], d);
                } catch (InvalidEntityPositionException e) {
                    writer.write(e.getMessage()); // writing the error to a file
                    writer.close(); System.exit(0); // close the file and terminate the program
                }
                // checking if there is no the same insect
                try {
                    checkDuplicateInsects(color, words[1].toLowerCase());
                } catch (DuplicateInsectsException e) {
                    writer.write(e.getMessage()); // writing the error to a file
                    writer.close(); System.exit(0); // close the file and terminate the program
                }
                // checking coordinates (if there is already some entity on the same position)
                try {
                    checkTwoEntitiesOnSamePosition(Integer.parseInt(words[xInd]), Integer.parseInt(words[yInd]));
                } catch (TwoEntitiesOnSamePositionException e) {
                    writer.write(e.getMessage()); // writing the error to a file
                    writer.close(); System.exit(0); // close the file and terminate the program
                }

                // adding insect to board
                switch (words[1].toLowerCase()) {
                    case "ant":
                        gameBoard.addEntity(new Ant(new EntityPosition(Integer.parseInt(words[xInd]),
                        Integer.parseInt(words[yInd])), color));
                        break;
                    case "butterfly":
                        gameBoard.addEntity(new Butterfly(new EntityPosition(Integer.parseInt(words[xInd]),
                        Integer.parseInt(words[yInd])), color));
                        break;
                    case "spider":
                        gameBoard.addEntity(new Spider(new EntityPosition(Integer.parseInt(words[xInd]),
                        Integer.parseInt(words[yInd])), color));
                        break;
                    case "grasshopper":
                        gameBoard.addEntity(new Grasshopper(new EntityPosition(Integer.parseInt(words[xInd]),
                        Integer.parseInt(words[yInd])), color));
                        break;
                    default: // will never be used because we have already checked it earlier in checkInsectType
                        writer.write("Invalid insect type");
                        writer.close(); System.exit(0); // close the file and terminate the program
                }
            }

            // input food
            for (int i = 0; i < m; i++) {
                line = sc.nextLine();
                String[] words = line.split(" ");
                // checking coordinates (within the boundaries of the board)
                try {
                    checkEntityPosition(words[1], words[2], d);
                } catch (InvalidEntityPositionException e) {
                    writer.write(e.getMessage()); // writing the error to a file
                    writer.close(); System.exit(0); // close the file and terminate the program
                }
                // checking coordinates (if there is already some entity on the same position)
                try {
                    checkTwoEntitiesOnSamePosition(Integer.parseInt(words[1]), Integer.parseInt(words[2]));
                } catch (TwoEntitiesOnSamePositionException e) {
                    writer.write(e.getMessage()); // writing the error to a file
                    writer.close(); System.exit(0); // close the file and terminate the program
                }
                // adding food on the board
                gameBoard.addEntity(new FoodPoint(new EntityPosition(Integer.parseInt(words[1]),
                Integer.parseInt(words[2])), Integer.parseInt(words[0])));
            }

            // start of the game (taking the insects out one by one)
            for (int i = 0; i < n; i++) {
                Insect insect = Insect.getInsects().get(0);
                String dir = insect.getBestDirection(gameBoard.getboardData(), d).toString();
                int resFood = insect.travelDirection(insect.getBestDirection(gameBoard.getboardData(), d),
                gameBoard.getboardData(), d);
                writer.write(insect.getColor().toString() + " " + insect.getInsectType().toString()
                + " " + dir + " " + Integer.toString(resFood) + "\n");
            }
            writer.close(); System.exit(0); // close the file and terminate the program
        } catch (FileNotFoundException e) { // if input file did not found
            System.out.println("File not found");
        } catch (IOException e) { // if output file did not found
            System.out.println("File error");
        }
    }
}

class InvalidBoardSizeException extends Exception {
    @Override
    public String getMessage() {
        return "Invalid board size";
    }
}

class InvalidNumberOfInsectsException extends Exception {
    @Override
    public String getMessage() {
        return "Invalid number of insects";
    }
}

class InvalidNumberOfFoodPointsException extends Exception {
    @Override
    public String getMessage() {
        return "Invalid number of food points";
    }
}

class InvalidInsectColorException extends Exception {
    @Override
    public String getMessage() {
        return "Invalid insect color";
    }
}

class InvalidInsectTypeException extends Exception {
    @Override
    public String getMessage() {
        return "Invalid insect type";
    }
}

class InvalidEntityPositionException extends Exception {
    @Override
    public String getMessage() {
        return "Invalid entity position";
    }
}

class DuplicateInsectsException extends Exception {
    @Override
    public String getMessage() {
        return "Duplicate insects";
    }
}

class TwoEntitiesOnSamePositionException extends Exception {
    @Override
    public String getMessage() {
        return "Two entities in the same position";
    }
}


enum Direction {
    /**. North Direction */
    N("North"),
    /**. East Direction */
    E("East"),
    /**. South Direction */
    S("South"),
    /**. West Direction */
    W("West"),
    /**. North-East Direction */
    NE("North-East"),
    /**. South-East Direction */
    SE("South-East"),
    /**. South-West Direction */
    SW("South-West"),
    /**. North-West Direction */
    NW("North-West");

    /**. text representation of direction */
    private String textRepresentation;

    Direction(String text) {
        this.textRepresentation = text;
    }

    // returns Direction in String format
    public String toString() {
        return this.textRepresentation;
    }

}

class EntityPosition {

    /**. x coodinate */
    private int x;

    /**. y coodinate */
    private int y;

    EntityPosition(int x, int y) {
        this.x = x;
        this.y = y;
    }

    // returns X coordinate
    public int getX() {
        return this.x;
    }

    // returns Y coordinate
    public int getY() {
        return this.y;
    }

}

enum EntityType {
    /**. Insect Ant */
    Ant,
    /**. Insect Spider */
    Spider,
    /**. Insect Grasshopper */
    Grasshopper,
    /**. Insect Butterfly */
    Butterfly,
    /**. Food Point */
    FoodPoint;

    // returns Entity Type in String format
    public String toString() {
        switch (this) {
            case Ant:
                return "Ant";
            case Spider:
                return "Spider";
            case Grasshopper:
                return "Grasshopper";
            case Butterfly:
                return "Butterfly";
            case FoodPoint:
                return "Foodpoint";
            default:
                return "Foodpoint";
        }
    }

}

abstract class BoardEntity {

    /**. position on the board */
    protected EntityPosition entityPosition;

    /**. type of the entity */
    protected EntityType entityType;

    public EntityPosition getEntityPosition() {
        return this.entityPosition;
    }
}

class FoodPoint extends BoardEntity {

    /**. food value */
    protected int value;

    /**. list of all food on the board */
    private static List<FoodPoint> foodPoints = new ArrayList<FoodPoint>();

    FoodPoint(EntityPosition position, int value) {
        this.entityPosition = position;
        this.value = value;
        this.entityType = EntityType.FoodPoint;
        foodPoints.add(this);
    }

    // get the list of all food on the board
    public static List<FoodPoint> getfoodPoints() {
        return foodPoints;
    }

    // get the value of food
    public int getValue() {
        return this.value;
    }

    // delete food after eating
    public static void deleteObject(FoodPoint f) {
        foodPoints.remove(f);
    }

}


enum InsectColor {
    /**. red color */
    RED,
    /**. green color */
    GREEN,
    /**. blue color */
    BLUE,
    /**. yellow color */
    YELLOW;

    public static InsectColor toColor(String s) {
        switch (s.toLowerCase()) {
            case "red":
                return InsectColor.RED;
            case "green":
                return InsectColor.GREEN;
            case "blue":
                return InsectColor.BLUE;
            case "yellow":
                return InsectColor.YELLOW;
            default:
                return InsectColor.YELLOW;
        }
    }

    // returns Insect Color in String format
    public String toString() {
        switch (this) {
            case RED:
                return "Red";
            case GREEN:
                return "Green";
            case BLUE:
                return "Blue";
            case YELLOW:
                return "Yellow";
            default:
                return "Yellow";
        }
    }

}

abstract class Insect extends BoardEntity {

    /**. color of the insect */
    protected InsectColor color;

    /**. the list of all insects on the board */
    private static List<Insect> insects = new ArrayList<Insect>();

    Insect(EntityPosition position, InsectColor color) {
        this.color = color;
        this.entityPosition = position;
        insects.add(this);
    }

    public abstract Direction getBestDirection(List<BoardEntity> boardData, int boardSize);

    public abstract int travelDirection(Direction direction, List<BoardEntity> boardData, int boardSize);

    // returns the list of all insects on the board
    public static List<Insect> getInsects() {
        return insects;
    }

    // returns the type of the insect
    public String getInsectType() {
        return this.entityType.toString();
    }

    // returns the color of the insect
    public InsectColor getColor() {
        return this.color;
    }

    // delete the insect after turn
    public static void deleteObject(Insect i) {
        insects.remove(i);
    }
}


interface OrthogonalMoving {

    int getOrthogonalDirectionVisibleValue(Direction dir, EntityPosition entityPosition,
    List<BoardEntity> boardData, int boardSize);

    int travelOrthogonally(Direction dir, EntityPosition entityPosition, InsectColor color,
    List<BoardEntity> boardData, int boardSize);

}

interface DiagonalMoving {

    int getDiagonalDirectionVisibleValue(Direction dir, EntityPosition entityPosition,
    List<BoardEntity> boardData, int boardSize);

    int travelDiagonally(Direction dir, EntityPosition entityPosition, InsectColor color,
    List<BoardEntity> boardData, int boardSize);

}


class Butterfly extends Insect implements OrthogonalMoving {

    Butterfly(EntityPosition entityPosition, InsectColor color) {
        super(entityPosition, color);
        this.entityType = EntityType.Butterfly;
    }

    // get amound of food being collected in specific orthogonal direction
    @Override
    public int getOrthogonalDirectionVisibleValue(Direction dir, EntityPosition entityPosition,
    List<BoardEntity> boardData, int boardSize) {
        int x = entityPosition.getX(); // X coordinate of the insect
        int y = entityPosition.getY(); // Y coordinate of the insect
        int res = 0; // amound of food collected

        if (dir.equals(Direction.N)) { // up
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                // if the food is on the way of the insect, add it to result
                if (food.getEntityPosition().getY() == y && food.getEntityPosition().getX() < x) {
                    res += food.getValue();
                }
            }

        } else if (dir.equals(Direction.W)) { // left
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                // if the food is on the way of the insect, add it to result
                if (food.getEntityPosition().getX() == x && food.getEntityPosition().getY() < y) {
                    res += food.getValue();
                }
            }

        } else if (dir.equals(Direction.E)) { // right
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                // if the food is on the way of the insect, add it to result
                if (food.getEntityPosition().getX() == x && food.getEntityPosition().getY() > y) {
                    res += food.getValue();
                }
            }

        } else { // down
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                // if the food is on the way of the insect, add it to result
                if (food.getEntityPosition().getY() == y && food.getEntityPosition().getX() > x) {
                    res += food.getValue();
                }
            }

        }
        return res; // return number of food collected on this direction
    }

    // get best direction for turn
    @Override
    public Direction getBestDirection(List<BoardEntity> boardData, int boardSize) {

        // collect data from all possible directions
        int resN = getOrthogonalDirectionVisibleValue(Direction.N, this.entityPosition, boardData, boardSize);
        int resE = getOrthogonalDirectionVisibleValue(Direction.E, this.entityPosition, boardData, boardSize);
        int resS = getOrthogonalDirectionVisibleValue(Direction.S, this.entityPosition, boardData, boardSize);
        int resW = getOrthogonalDirectionVisibleValue(Direction.W, this.entityPosition, boardData, boardSize);

        if (resN >= resE && resN >= resS && resN >= resW) {
            return Direction.N;
        } else if (resE >= resN && resE >= resS && resE >= resW) {
            return Direction.E;
        } else if (resS >= resE && resS >= resN && resS >= resW) {
            return Direction.S;
        } else {
            return Direction.W;
        }
    }

    // travel by specific direction
    @Override
    public int travelDirection(Direction dir, List<BoardEntity> boardData, int boardSize) {
        return travelOrthogonally(dir, this.entityPosition, this.color, boardData, boardSize);
    }

    // travel by specific orthogonal direction
    @Override
    public int travelOrthogonally(Direction dir, EntityPosition entityPosition, InsectColor color,
    List<BoardEntity> boardData, int boardSize) {

        int res = 0; // number of food collected
        int y = entityPosition.getY(); // Y coordinate of the insect
        int x = entityPosition.getX(); // X coordinate of the insect

        if (dir.equals(Direction.N)) { // up
            int maxX = 0;
            for (Insect insect : Insect.getInsects()) {

                // if the insect is on our way AND don't have the same color AND meets earlier
                if (insect.getEntityPosition().getY() == y && insect.getEntityPosition().getX() < x
                && !insect.getColor().equals(color) && insect.getEntityPosition().getX() > maxX) {
                    maxX = insect.getEntityPosition().getX();
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getY() == y
                && food.getEntityPosition().getX() < x
                && food.getEntityPosition().getX() > maxX) {
                    res += food.getValue();
                    iterator.remove();
                }
            }

        } else if (dir.equals(Direction.S)) {  // down
            final int maxxx = 1000000000;
            int minX = maxxx;
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getY() == y && insect.getEntityPosition().getX() > x
                && !insect.getColor().equals(color) && insect.getEntityPosition().getX() < minX) {
                    minX = insect.getEntityPosition().getX();
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getY() == y
                && food.getEntityPosition().getX() > x
                && food.getEntityPosition().getX() < minX) {
                    res += food.getValue();
                    iterator.remove();
                }
            }
        } else if (dir.equals(Direction.W)) { // left
            int maxY = 0;
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getX() == x && insect.getEntityPosition().getY() < y
                && !insect.getColor().equals(color) && insect.getEntityPosition().getY() > maxY) {
                    maxY = insect.getEntityPosition().getY();
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getX() == x
                && food.getEntityPosition().getY() < y
                && food.getEntityPosition().getY() > maxY) {
                    res += food.getValue();
                    iterator.remove();
                }
            }
        } else { // right
            final int maxxx = 1000000000;
            int minY = maxxx;
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getX() == x && insect.getEntityPosition().getY() > y
                && !insect.getColor().equals(color) && insect.getEntityPosition().getY() < minY) {
                    minY = insect.getEntityPosition().getY();
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getX() == x
                && food.getEntityPosition().getY() > y
                && food.getEntityPosition().getY() < minY) {
                    res += food.getValue();
                    iterator.remove();
                }
            }
        }
        Insect.deleteObject(this);
        return res;
    }

}

class Ant extends Insect implements OrthogonalMoving, DiagonalMoving {

    Ant(EntityPosition entityPosition, InsectColor color) {
        super(entityPosition, color);
        this.entityType = EntityType.Ant;
    }

    @Override
    public Direction getBestDirection(List<BoardEntity> boardData, int boardSize) {

        // collect data from all possible directions
        int resN = getOrthogonalDirectionVisibleValue(Direction.N, this.entityPosition, boardData, boardSize);
        int resE = getOrthogonalDirectionVisibleValue(Direction.E, this.entityPosition, boardData, boardSize);
        int resS = getOrthogonalDirectionVisibleValue(Direction.S, this.entityPosition, boardData, boardSize);
        int resW = getOrthogonalDirectionVisibleValue(Direction.W, this.entityPosition, boardData, boardSize);
        int resNE = getDiagonalDirectionVisibleValue(Direction.NE, this.entityPosition, boardData, boardSize);
        int resSE = getDiagonalDirectionVisibleValue(Direction.SE, this.entityPosition, boardData, boardSize);
        int resSW = getDiagonalDirectionVisibleValue(Direction.SW, this.entityPosition, boardData, boardSize);
        int resNW = getDiagonalDirectionVisibleValue(Direction.NW, this.entityPosition, boardData, boardSize);
        List<Integer> lst = Arrays.asList(resN, resE, resS, resW, resNE, resSE, resSW, resNW);
        if (Collections.max(lst) == resN) {
            return Direction.N;
        } else if (Collections.max(lst) == resE) {
            return Direction.E;
        } else if (Collections.max(lst) == resS) {
            return Direction.S;
        } else if (Collections.max(lst) == resW) {
            return Direction.W;
        } else if (Collections.max(lst) == resNE) {
            return Direction.NE;
        } else if (Collections.max(lst) == resSE) {
            return Direction.SE;
        } else if (Collections.max(lst) == resSW) {
            return Direction.SW;
        } else {
            return Direction.NW;
        }
    }

    @Override
    public int travelDirection(Direction dir, List<BoardEntity> boardData, int boardSize) {
        if (dir.equals(Direction.N) || dir.equals(Direction.E) || dir.equals(Direction.S) || dir.equals(Direction.W)) {
            return travelOrthogonally(dir, this.entityPosition, this.color, boardData, boardSize);
        } else {
            return travelDiagonally(dir, this.entityPosition, this.color, boardData, boardSize);
        }
    }

    @Override
    public int getDiagonalDirectionVisibleValue(Direction dir, EntityPosition entityPosition,
    List<BoardEntity> boardData, int boardSize) {
        int x = entityPosition.getX(); // X coordinate of the insect
        int y = entityPosition.getY(); // Y coordinate of the insect
        int res = 0;
        if (dir.equals(Direction.NE)) { // up and right
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                if (food.getEntityPosition().getY() > y && food.getEntityPosition().getX() < x
                && ((food.getEntityPosition().getY() - y) == (x - food.getEntityPosition().getX()))) {
                    res += food.getValue();
                }
            }
        } else if (dir.equals(Direction.SE)) { // down and right
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                if (food.getEntityPosition().getY() > y && food.getEntityPosition().getX() > x
                && ((food.getEntityPosition().getY() - y) == (food.getEntityPosition().getX() - x))) {
                    res += food.getValue();
                }
            }
        } else if (dir.equals(Direction.SW)) { // down and left
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                if (food.getEntityPosition().getY() < y && food.getEntityPosition().getX() > x
                && ((y - food.getEntityPosition().getY()) == (food.getEntityPosition().getX() - x))) {
                    res += food.getValue();
                }
            }
        } else { // up and left
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                if (food.getEntityPosition().getY() < y && food.getEntityPosition().getX() < x
                && ((y - food.getEntityPosition().getY()) == (x - food.getEntityPosition().getX()))) {
                    res += food.getValue();
                }
            }
        }
        return res;
    }

    @Override
    public int travelDiagonally(Direction dir, EntityPosition entityPosition,
    InsectColor color, List<BoardEntity> boardData, int boardSize) {
        int res = 0;
        int y = entityPosition.getY(); // Y coordinate of the insect
        int x = entityPosition.getX(); // X coordinate of the insect
        final int maxxx = 1000000000;
        int minStep = maxxx;
        if (dir.equals(Direction.NE)) { // up and right
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getY() > y && insect.getEntityPosition().getX() < x
                && !insect.getColor().equals(color)
                && (Math.abs(insect.getEntityPosition().getX() - x) == Math.abs(insect.getEntityPosition().getY() - y))
                && Math.abs(insect.getEntityPosition().getX() - x) < minStep) {
                    minStep = Math.abs(insect.getEntityPosition().getX() - x);
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getY() > y
                && food.getEntityPosition().getX() < x
                && (Math.abs(food.getEntityPosition().getX() - x)
                == Math.abs(food.getEntityPosition().getY() - y))
                && Math.abs(food.getEntityPosition().getY() - y) < minStep) {
                    res += food.getValue();
                    iterator.remove();
                }
            }
        } else if (dir.equals(Direction.SE)) {  // down and right
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getY() > y && insect.getEntityPosition().getX() > x
                && !insect.getColor().equals(color)
                && (Math.abs(insect.getEntityPosition().getX() - x)
                == Math.abs(insect.getEntityPosition().getY() - y))
                && Math.abs(insect.getEntityPosition().getX() - x) < minStep) {
                    minStep = Math.abs(insect.getEntityPosition().getX() - x);
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getY() > y
                && food.getEntityPosition().getX() > x
                && (Math.abs(food.getEntityPosition().getX() - x)
                == Math.abs(food.getEntityPosition().getY() - y))
                && Math.abs(food.getEntityPosition().getY() - y) < minStep) {
                    res += food.getValue();
                    iterator.remove();
                }
            }
        } else if (dir.equals(Direction.SW)) { // down and left
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getY() < y && insect.getEntityPosition().getX() > x
                && !insect.getColor().equals(color) && (Math.abs(insect.getEntityPosition().getX() - x)
                == Math.abs(insect.getEntityPosition().getY() - y))
                && Math.abs(insect.getEntityPosition().getX() - x) < minStep) {
                    minStep = Math.abs(insect.getEntityPosition().getX() - x);
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getY() < y
                && food.getEntityPosition().getX() > x
                && (Math.abs(food.getEntityPosition().getX() - x)
                == Math.abs(food.getEntityPosition().getY() - y))
                && Math.abs(food.getEntityPosition().getY() - y) < minStep) {
                    res += food.getValue();
                    iterator.remove();
                }
            }
        } else { // up and left
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getY() < y && insect.getEntityPosition().getX() < x
                && !insect.getColor().equals(color) && (Math.abs(insect.getEntityPosition().getX() - x)
                == Math.abs(insect.getEntityPosition().getY() - y))
                && Math.abs(insect.getEntityPosition().getX() - x) < minStep) {
                    minStep = Math.abs(insect.getEntityPosition().getX() - x);
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getY() < y
                && food.getEntityPosition().getX() < x
                && (Math.abs(food.getEntityPosition().getX() - x)
                == Math.abs(food.getEntityPosition().getY() - y))
                && Math.abs(food.getEntityPosition().getY() - y) < minStep) {
                    res += food.getValue();
                    iterator.remove();
                }
            }
        }
        Insect.deleteObject(this);
        return res;
    }

    @Override
    public int getOrthogonalDirectionVisibleValue(Direction dir,
    EntityPosition entityPosition, List<BoardEntity> boardData, int boardSize) {
        int x = entityPosition.getX(); // X coordinate of the insect
        int y = entityPosition.getY(); // Y coordinate of the insect
        int res = 0;
        if (dir.equals(Direction.N)) { // up
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                if (food.getEntityPosition().getY() == y && food.getEntityPosition().getX() < x) {
                    res += food.getValue();
                }
            }
        } else if (dir.equals(Direction.W)) { // left
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                if (food.getEntityPosition().getX() == x && food.getEntityPosition().getY() < y) {
                    res += food.getValue();
                }
            }
        } else if (dir.equals(Direction.E)) { // right
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                if (food.getEntityPosition().getX() == x && food.getEntityPosition().getY() > y) {
                    res += food.getValue();
                }
            }
        } else { // down
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                if (food.getEntityPosition().getY() == y && food.getEntityPosition().getX() > x) {
                    res += food.getValue();
                }
            }
        }
        return res;
    }

    @Override
    public int travelOrthogonally(Direction dir, EntityPosition entityPosition,
    InsectColor color, List<BoardEntity> boardData, int boardSize) {
        int res = 0;
        int y = entityPosition.getY(); // Y coordinate of the insect
        int x = entityPosition.getX(); // X coordinate of the insect
        if (dir.equals(Direction.N)) { // up
            int maxX = 0;
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getY() == y && insect.getEntityPosition().getX() < x
                && !insect.getColor().equals(color) && insect.getEntityPosition().getX() > maxX) {
                    maxX = insect.getEntityPosition().getX();
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getY() == y
                && food.getEntityPosition().getX() < x
                && food.getEntityPosition().getX() > maxX) {
                    res += food.getValue();
                    iterator.remove();
                }
            }

        } else if (dir.equals(Direction.S)) {  // down
            final int maxxx = 1000000000;
            int minX = maxxx;
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getY() == y && insect.getEntityPosition().getX() > x
                && !insect.getColor().equals(color) && insect.getEntityPosition().getX() < minX) {
                    minX = insect.getEntityPosition().getX();
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getY() == y
                && food.getEntityPosition().getX() > x
                && food.getEntityPosition().getX() < minX) {
                    res += food.getValue();
                    iterator.remove();
                }
            }
        } else if (dir.equals(Direction.W)) { // left
            int maxY = 0;
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getX() == x && insect.getEntityPosition().getY() < y
                && !insect.getColor().equals(color) && insect.getEntityPosition().getY() > maxY) {
                    maxY = insect.getEntityPosition().getY();
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getX() == x
                && food.getEntityPosition().getY() < y
                && food.getEntityPosition().getY() > maxY) {
                    res += food.getValue();
                    iterator.remove();
                }
            }
        } else { // right
            final int maxxx = 1000000000;
            int minY = maxxx;
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getX() == x && insect.getEntityPosition().getY() > y
                && !insect.getColor().equals(color) && insect.getEntityPosition().getY() < minY) {
                    minY = insect.getEntityPosition().getY();
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getX() == x
                && food.getEntityPosition().getY() > y
                && food.getEntityPosition().getY() < minY) {
                    res += food.getValue();
                    iterator.remove();
                }
            }
        }
        Insect.deleteObject(this);
        return res;
    }

}

class Spider extends Insect implements DiagonalMoving {

    Spider(EntityPosition entityPosition, InsectColor color) {
        super(entityPosition, color);
        this.entityType = EntityType.Spider;
    }

    @Override
    public Direction getBestDirection(List<BoardEntity> boardData, int boardSize) {

        // collect data from all possible directions
        int resNE = getDiagonalDirectionVisibleValue(Direction.NE, this.entityPosition, boardData, boardSize);
        int resSE = getDiagonalDirectionVisibleValue(Direction.SE, this.entityPosition, boardData, boardSize);
        int resSW = getDiagonalDirectionVisibleValue(Direction.SW, this.entityPosition, boardData, boardSize);
        int resNW = getDiagonalDirectionVisibleValue(Direction.NW, this.entityPosition, boardData, boardSize);
        if (resNE >= resSE && resNE >= resSW && resNE >= resNW) {
            return Direction.NE;
        } else if (resSE >= resNE && resSE >= resSW && resSE >= resNW) {
            return Direction.SE;
        } else if (resSW >= resSE && resSW >= resNE && resSW >= resNW) {
            return Direction.SW;
        } else {
            return Direction.NW;
        }
    }

    @Override
    public int travelDirection(Direction dir, List<BoardEntity> boardData, int boardSize) {
        return travelDiagonally(dir, this.entityPosition, this.color, boardData, boardSize);
    }

    @Override
    public int getDiagonalDirectionVisibleValue(Direction dir,
    EntityPosition entityPosition, List<BoardEntity> boardData, int boardSize) {
        int x = entityPosition.getX(); // X coordinate of the insect
        int y = entityPosition.getY(); // Y coordinate of the insect
        int res = 0;
        if (dir.equals(Direction.NE)) { // up and right
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                if (food.getEntityPosition().getY() > y && food.getEntityPosition().getX() < x
                && ((food.getEntityPosition().getY() - y) == (x - food.getEntityPosition().getX()))) {
                    res += food.getValue();
                }
            }
        } else if (dir.equals(Direction.SE)) { // down and right
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                if (food.getEntityPosition().getY() > y && food.getEntityPosition().getX() > x
                && ((food.getEntityPosition().getY() - y) == (food.getEntityPosition().getX() - x))) {
                    res += food.getValue();
                }
            }
        } else if (dir.equals(Direction.SW)) { // down and left
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                if (food.getEntityPosition().getY() < y && food.getEntityPosition().getX() > x
                && ((y - food.getEntityPosition().getY()) == (food.getEntityPosition().getX() - x))) {
                    res += food.getValue();
                }
            }
        } else { // up and left
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                if (food.getEntityPosition().getY() < y && food.getEntityPosition().getX() < x
                && ((y - food.getEntityPosition().getY()) == (x - food.getEntityPosition().getX()))) {
                    res += food.getValue();
                }
            }
        }
        return res;
    }

    @Override
    public int travelDiagonally(Direction dir, EntityPosition entityPosition,
    InsectColor color, List<BoardEntity> boardData, int boardSize) {
        int res = 0;
        int y = entityPosition.getY(); // Y coordinate of the insect
        int x = entityPosition.getX(); // X coordinate of the insect
        final int maxxx = 1000000000;
        int minStep = maxxx;
        if (dir.equals(Direction.NE)) { // up and right
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getY() > y && insect.getEntityPosition().getX() < x
                && !insect.getColor().equals(color) && (Math.abs(insect.getEntityPosition().getX() - x)
                == Math.abs(insect.getEntityPosition().getY() - y))
                && Math.abs(insect.getEntityPosition().getX() - x) < minStep) {
                    minStep = Math.abs(insect.getEntityPosition().getX() - x);
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getY() > y
                && food.getEntityPosition().getX() < x
                && (Math.abs(food.getEntityPosition().getX() - x)
                == Math.abs(food.getEntityPosition().getY() - y))
                && Math.abs(food.getEntityPosition().getY() - y) < minStep) {
                    res += food.getValue();
                    iterator.remove();
                }
            }
        } else if (dir.equals(Direction.SE)) {  // down and right
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getY() > y && insect.getEntityPosition().getX() > x
                && !insect.getColor().equals(color) && (Math.abs(insect.getEntityPosition().getX() - x)
                == Math.abs(insect.getEntityPosition().getY() - y))
                && Math.abs(insect.getEntityPosition().getX() - x) < minStep) {
                    minStep = Math.abs(insect.getEntityPosition().getX() - x);
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getY() > y
                && food.getEntityPosition().getX() > x
                && (Math.abs(food.getEntityPosition().getX() - x)
                == Math.abs(food.getEntityPosition().getY() - y))
                && Math.abs(food.getEntityPosition().getY() - y) < minStep) {
                    res += food.getValue();
                    iterator.remove();
                }
            }
        } else if (dir.equals(Direction.SW)) { // down and left
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getY() < y && insect.getEntityPosition().getX() > x
                && !insect.getColor().equals(color) && (Math.abs(insect.getEntityPosition().getX() - x)
                == Math.abs(insect.getEntityPosition().getY() - y))
                && Math.abs(insect.getEntityPosition().getX() - x) < minStep) {
                    minStep = Math.abs(insect.getEntityPosition().getX() - x);
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getY() < y
                && food.getEntityPosition().getX() > x
                && (Math.abs(food.getEntityPosition().getX() - x)
                == Math.abs(food.getEntityPosition().getY() - y))
                && Math.abs(food.getEntityPosition().getY() - y) < minStep) {
                    res += food.getValue();
                    iterator.remove();
                }
            }
        } else { // up and left
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getY() < y && insect.getEntityPosition().getX() < x
                && !insect.getColor().equals(color) && (Math.abs(insect.getEntityPosition().getX() - x)
                == Math.abs(insect.getEntityPosition().getY() - y))
                && Math.abs(insect.getEntityPosition().getX() - x) < minStep) {
                    minStep = Math.abs(insect.getEntityPosition().getX() - x);
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getY() < y
                && food.getEntityPosition().getX() < x
                && (Math.abs(food.getEntityPosition().getX() - x)
                == Math.abs(food.getEntityPosition().getY() - y))
                && Math.abs(food.getEntityPosition().getY() - y) < minStep) {
                    res += food.getValue();
                    iterator.remove();
                }
            }
        }
        Insect.deleteObject(this);
        return res;
    }

}

class Grasshopper extends Insect {

    Grasshopper(EntityPosition entityPosition, InsectColor color) {
        super(entityPosition, color);
        this.entityType = EntityType.Grasshopper;
    }

    @Override
    public Direction getBestDirection(List<BoardEntity> boardData, int boardSize) {

        // collect data from all possible directions
        int resN = getOrthogonalDirectionVisibleValue(Direction.N, this.entityPosition, boardData, boardSize);
        int resE = getOrthogonalDirectionVisibleValue(Direction.E, this.entityPosition, boardData, boardSize);
        int resS = getOrthogonalDirectionVisibleValue(Direction.S, this.entityPosition, boardData, boardSize);
        int resW = getOrthogonalDirectionVisibleValue(Direction.W, this.entityPosition, boardData, boardSize);
        if (resN >= resE && resN >= resS && resN >= resW) {
            return Direction.N;
        } else if (resE >= resN && resE >= resS && resE >= resW) {
            return Direction.E;
        } else if (resS >= resE && resS >= resN && resS >= resW) {
            return Direction.S;
        } else {
            return Direction.W;
        }
    }

    public int getOrthogonalDirectionVisibleValue(Direction dir,
    EntityPosition entityPosition, List<BoardEntity> boardData, int boardSize) {
        int x = entityPosition.getX(); // X coordinate of the insect
        int y = entityPosition.getY(); // Y coordinate of the insect
        int res = 0;
        if (dir.equals(Direction.N)) { // up
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                if (food.getEntityPosition().getY() == y && food.getEntityPosition().getX() < x
                && ((food.getEntityPosition().getX() % 2) ==  (x % 2))) {
                    res += food.getValue();
                }
            }
        } else if (dir.equals(Direction.W)) { // left
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                if (food.getEntityPosition().getX() == x && food.getEntityPosition().getY() < y
                && ((food.getEntityPosition().getY() % 2) ==  (y % 2))) {
                    res += food.getValue();
                }
            }
        } else if (dir.equals(Direction.E)) { // right
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                if (food.getEntityPosition().getX() == x && food.getEntityPosition().getY() > y
                && ((food.getEntityPosition().getY() % 2) ==  (y % 2))) {
                    res += food.getValue();
                }
            }
        } else { // down
            for (FoodPoint food : FoodPoint.getfoodPoints()) {
                if (food.getEntityPosition().getY() == y && food.getEntityPosition().getX() > x
                && ((food.getEntityPosition().getX() % 2) ==  (x % 2))) {
                    res += food.getValue();
                }
            }
        }
        return res;
    }

    @Override
    public int travelDirection(Direction dir, List<BoardEntity> boardData, int boardSize) {
        int res = 0;
        int y = entityPosition.getY(); // Y coordinate of the insect
        int x = entityPosition.getX(); // X coordinate of the insect
        if (dir.equals(Direction.N)) { // up
            int maxX = 0;
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getY() == y && insect.getEntityPosition().getX() < x
                && ((insect.getEntityPosition().getX() % 2) == (x % 2)) && !insect.getColor().equals(color)
                && insect.getEntityPosition().getX() > maxX) {
                    maxX = insect.getEntityPosition().getX();
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getY() == y && food.getEntityPosition().getX() < x
                && ((food.getEntityPosition().getX() % 2) == (x % 2))
                && food.getEntityPosition().getX() > maxX) {
                    res += food.getValue();
                    iterator.remove();
                }
            }

        } else if (dir.equals(Direction.S)) {  // down
            final int maxxx = 1000000000;
            int minX = maxxx;
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getY() == y && insect.getEntityPosition().getX() > x
                && ((insect.getEntityPosition().getX() % 2) == (x % 2)) && !insect.getColor().equals(color)
                && insect.getEntityPosition().getX() < minX) {
                    minX = insect.getEntityPosition().getX();
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getY() == y
                && food.getEntityPosition().getX() > x
                && ((food.getEntityPosition().getX() % 2) == (x % 2))
                && food.getEntityPosition().getX() < minX) {
                    res += food.getValue();
                    iterator.remove();
                }
            }
        } else if (dir.equals(Direction.W)) { // left
            int maxY = 0;
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getX() == x && insect.getEntityPosition().getY() < y
                && ((insect.getEntityPosition().getY() % 2) == (y % 2)) && !insect.getColor().equals(color)
                && insect.getEntityPosition().getY() > maxY) {
                    maxY = insect.getEntityPosition().getY();
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getX() == x
                && food.getEntityPosition().getY() < y
                && ((food.getEntityPosition().getY() % 2) == (y % 2))
                && food.getEntityPosition().getY() > maxY) {
                    res += food.getValue();
                    iterator.remove();
                }
            }
        } else { // right
            final int maxxx = 1000000000;
            int minY = maxxx;
            for (Insect insect : Insect.getInsects()) {
                if (insect.getEntityPosition().getX() == x && insect.getEntityPosition().getY() > y
                && ((insect.getEntityPosition().getY() % 2) == (y % 2)) && !insect.getColor().equals(color)
                && insect.getEntityPosition().getY() < minY) {
                    minY = insect.getEntityPosition().getY();
                }
            }

            Iterator<FoodPoint> iterator = FoodPoint.getfoodPoints().iterator();
            while (iterator.hasNext()) {
                FoodPoint food = iterator.next();
                if (food.getEntityPosition().getX() == x && food.getEntityPosition().getY() > y
                && ((food.getEntityPosition().getY() % 2) == (y % 2)) && food.getEntityPosition().getY() < minY) {
                    res += food.getValue();
                    iterator.remove();
                }
            }
        }
        Insect.deleteObject(this);
        return res;
    }

}


class Board {

    /**. list of all board entities */
    private List<BoardEntity> boardData = new ArrayList<BoardEntity>();

    /**. board size */
    private int size;

    /**. list of all created boards */
    private static List<Board> boards = new ArrayList<Board>();

    Board(int boardSize) {
        this.size = boardSize;
        boards.add(this);
    }

    // add new entity to the board
    public void addEntity(BoardEntity entity) {
        boardData.add(entity);
    }

    // get entity on given position or null in case of empty cell
    public BoardEntity getEntity(EntityPosition position) {
        for (BoardEntity entity : this.boardData) {
            if ((entity.getEntityPosition().getX()) == position.getX()
            && (entity.getEntityPosition().getY()) == position.getY()) {
                return entity;
            }
        }
        return null;
    }

    // get the board information
    public List<BoardEntity> getboardData() {
        return this.boardData;
    }

}
