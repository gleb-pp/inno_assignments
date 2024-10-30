import java.util.ArrayList;
import java.util.Arrays;
import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.Scanner;

public final class UniversityCourseManagementSystem {

    private UniversityCourseManagementSystem() {
        // empty constructor to avoid problems with public or default constructor (Checkstyle)
    }

    /**. map to storage courses, key = id, value = course */
    private static Map<Integer, Course> courses = new HashMap<>();

    /**. map to storage students, key = id, value = student */
    private static Map<Integer, Student> students = new HashMap<>();

    /**. map to storage courses, key = id, value = course */
    private static Map<Integer, Professor> professors = new HashMap<>();

    /**. map of banned names (command word can not be name) */
    private static List<String> ban = Arrays.asList("course", "student", "professor", "enroll",
"drop", "exempt", "teach"); // carry of a line to avoid problems with too long line (Checkstyle)

    private static void wrongInputs() {
        System.out.println("Wrong inputs");
        System.exit(0);
    }

    private static int checkId(String str) {
        if (str.isEmpty()) {
            System.exit(0);
        }
        try {
            Integer.parseInt(str);
        } catch (NumberFormatException e) {
            wrongInputs();
        }

        int memId = Integer.parseInt(str);
        return memId;
    }

    /**
     * function with main program.
     * @param args are main function's parametrs.
     */
    public static void main(String[] args) {
        fillInitialData();
        Scanner sc = new Scanner(System.in);
        String line;
        while (sc.hasNext()) {
            String command = sc.nextLine();
            if (command.equals("course")) {
                String name = sc.nextLine().toLowerCase();
                if (name.isEmpty()) {
                    System.exit(0);
                }
                if (name.matches("[a-z_]+") && !(name.charAt(0) == '_') && !(name.charAt(name.length() - 1) == '_')
                && !(name.contains("__")) && !ban.contains(name)) { // new line to avoid problems with too long line
                    // check if there is course with the same name
                    for (Course c : courses.values()) {
                        if (c.getcourseName().equals(name)) {
                            System.out.println("Course exists");
                            System.exit(0);
                        }
                    }
                    String lvl = sc.nextLine().toLowerCase();
                    if (lvl.isEmpty()) {
                        System.exit(0);
                    }
                    if (lvl.equals("master")) {
                        courses.put(Course.getnumberOfCourses() + 1, new Course(name, CourseLevel.MASTER));
                        System.out.println("Added successfully");
                    } else if (lvl.equals("bachelor")) {
                        courses.put(Course.getnumberOfCourses() + 1, new Course(name, CourseLevel.BACHELOR));
                        System.out.println("Added successfully");
                    } else {
                        wrongInputs();
                    }
                } else {
                    wrongInputs();
                }
            } else if (command.equals("student")) {
                String name = sc.nextLine().toLowerCase();
                if (name.isEmpty()) {
                    System.exit(0);
                }
                if (name.matches("[a-z]+") && !ban.contains(name)) {
                    students.put(UniversityMember.getnumberOfMembers() + 1, new Student(name));
                    System.out.println("Added successfully");
                } else {
                    wrongInputs();
                }
            } else if (command.equals("professor")) {
                String name = sc.nextLine().toLowerCase();
                if (name.isEmpty()) {
                    System.exit(0);
                }
                if (name.matches("[a-z]+") && !ban.contains(name)) {
                    professors.put(UniversityMember.getnumberOfMembers() + 1, new Professor(name));
                    System.out.println("Added successfully");
                } else {
                    wrongInputs();
                }
            } else if (command.equals("enroll")) {
                line = sc.nextLine();
                int memId = checkId(line);
                if (students.keySet().contains(memId)) {
                    line = sc.nextLine();
                    int courseId = checkId(line);
                    if (courses.keySet().contains(courseId)) {
                        if (students.get(memId).enroll(courses.get(courseId))) {
                            System.out.println("Enrolled successfully");
                        } else {
                            if (students.get(memId).getenrolledCourses().contains(courses.get(courseId))) {
                                System.out.println("Student is already enrolled in this course");
                                System.exit(0);
                            } else if (students.get(memId).getenrolledCourses().size() ==  Student.getMaxEnrollment()) {
                                System.out.println("Maximum enrollment is reached for the student");
                                System.exit(0);
                            } else {
                                System.out.println("Course is full");
                                System.exit(0);
                            }
                        }
                    } else {
                        wrongInputs();
                    }
                } else {
                    wrongInputs();
                }
            } else if (command.equals("drop")) {
                line = sc.nextLine();
                int memId = checkId(line);
                if (students.keySet().contains(memId)) {
                    line = sc.nextLine();
                    int courseId = checkId(line);
                    if (courses.keySet().contains(courseId)) {
                        if (students.get(memId).drop(courses.get(courseId))) {
                            System.out.println("Dropped successfully");
                        } else {
                            System.out.println("Student is not enrolled in this course");
                            System.exit(0);
                        }
                    } else {
                        wrongInputs();
                    }
                } else {
                    wrongInputs();
                }
            } else if (command.equals("exempt")) {
                line = sc.nextLine();
                int memId = checkId(line);
                if (professors.keySet().contains(memId)) {
                    line = sc.nextLine();
                    int courseId = checkId(line);
                    if (courses.keySet().contains(courseId)) {
                        if (professors.get(memId).exempt(courses.get(courseId))) {
                            System.out.println("Professor is exempted");
                        } else {
                            System.out.println("Professor is not teaching this course"); System.exit(0);
                        }
                    } else {
                        wrongInputs();
                    }
                } else {
                    wrongInputs();
                }
            } else if (command.equals("teach")) {
                line = sc.nextLine();
                int memId = checkId(line);
                if (professors.keySet().contains(memId)) {
                    line = sc.nextLine();
                    int courseId = checkId(line);
                    if (courses.keySet().contains(courseId)) {
                        if (professors.get(memId).teach(courses.get(courseId))) {
                            System.out.println("Professor is successfully assigned to teach this course");
                        } else {
                            if (professors.get(memId).getassignedCourses().size() ==  Professor.getMaxLoad()) {
                                System.out.println("Professor's load is complete"); System.exit(0);
                            } else {
                                System.out.println("Professor is already teaching this course"); System.exit(0);
                            }
                        }
                    } else {
                        wrongInputs();
                    }
                } else {
                    wrongInputs();
                }
            } else if (command.isEmpty()) {
                System.exit(0);
            } else {
                wrongInputs();
            }
        }
    }

    /**. loading the provided instances with the arguments from the abovementioned tables */
    public static void fillInitialData() {
        courses.put(Course.getnumberOfCourses() + 1, new Course("java_beginner", CourseLevel.BACHELOR));
        courses.put(Course.getnumberOfCourses() + 1, new Course("java_intermediate", CourseLevel.BACHELOR));
        courses.put(Course.getnumberOfCourses() + 1, new Course("python_basics", CourseLevel.BACHELOR));
        courses.put(Course.getnumberOfCourses() + 1, new Course("algorithms", CourseLevel.MASTER));
        courses.put(Course.getnumberOfCourses() + 1, new Course("advanced_programming", CourseLevel.MASTER));
        courses.put(Course.getnumberOfCourses() + 1, new Course("mathematical_analysis", CourseLevel.MASTER));
        courses.put(Course.getnumberOfCourses() + 1, new Course("computer_vision", CourseLevel.MASTER));

        // we could just enroll students and profs to courses one by one but Checkstyle requires to avoid magic numbers
        int newMemberId = 1;
        final List<Integer> aliceCourses = Arrays.asList(1, 2, 3);
        final List<Integer> bobCourses = Arrays.asList(1, 4);
        final List<Integer> alexCourses = Arrays.asList(5);
        final List<Integer> aliCourses = Arrays.asList(1, 2);
        final List<Integer> ahmedCourses = Arrays.asList(3, 5);
        final List<Integer> andreyCourses = Arrays.asList(6);

        students.put(newMemberId, new Student("Alice"));
        for (Integer course : aliceCourses) {
            students.get(newMemberId).enroll(courses.get(course));
        }

        newMemberId++;
        students.put(newMemberId, new Student("Bob"));
        for (Integer course : bobCourses) {
            students.get(newMemberId).enroll(courses.get(course));
        }

        newMemberId++;
        students.put(newMemberId, new Student("Alex"));
        for (Integer course : alexCourses) {
            students.get(newMemberId).enroll(courses.get(course));
        }

        newMemberId++;
        professors.put(newMemberId, new Professor("Ali"));
        for (Integer course : aliCourses) {
            professors.get(newMemberId).teach(courses.get(course));
        }

        newMemberId++;
        professors.put(newMemberId, new Professor("Ahmed"));
        for (Integer course : ahmedCourses) {
            professors.get(newMemberId).teach(courses.get(course));
        }

        newMemberId++;
        professors.put(newMemberId, new Professor("Andrey"));
        for (Integer course : andreyCourses) {
            professors.get(newMemberId).teach(courses.get(course));
        }
    }

}

enum CourseLevel {
    /**. for bachelors */
    BACHELOR,
    /**. for masters */
    MASTER
}

interface Enrollable {
    boolean drop(Course course);
    boolean enroll(Course course);
}

class Course {

    /**. course capacity (how many student can study) */
    private static final int CAPACITY = 3;

    /**. number of all courses  */
    private static int numberOfCourses;

    /**. id of the course */
    private int courseId;

    /**. name of the course */
    private String courseName;

    /**. list of the students studying */
    private List<Student> enrolledStudents;

    /**. for which students the course is (bachelors, master) */
    private CourseLevel courseLevel;

    Course(String courseName, CourseLevel courseLevel) {
        this.courseName = courseName;
        this.courseLevel = courseLevel;
        this.enrolledStudents = new ArrayList<Student>();
        numberOfCourses++;
        this.courseId = numberOfCourses;
    }

    public boolean isFull() {
        return this.enrolledStudents.size() == CAPACITY;
    }

    public void setenrolledStudents(Student student) {
        if (this.enrolledStudents.contains(student)) {
            this.enrolledStudents.remove(student);
        } else {
            this.enrolledStudents.add(student);
        }
    }

    public String getcourseName() {
        return this.courseName;
    }

    public static int getnumberOfCourses() {
        return numberOfCourses;
    }
}

abstract class UniversityMember {
    /**. number of all members  */
    private static int numberOfMembers;

    /**. id of the member  */
    private int memberId;

    /**. name of the member  */
    private String memberName;

    UniversityMember(int memberId, String memberName) {
        this.memberId = memberId;
        this.memberName = memberName;
        numberOfMembers++;
    }

    public static int getnumberOfMembers() {
        return numberOfMembers;
    }
}

class Student extends UniversityMember implements Enrollable {

    /**. how many courses can student study simultaneously  */
    private static final int MAX_ENROLLMENT = 3;

    /**. the list of courses studing  */
    private List<Course> enrolledCourses;

    Student(String memberName) {
        super(UniversityMember.getnumberOfMembers() + 1, memberName);
        this.enrolledCourses = new ArrayList<Course>();
    }

    public boolean enroll(Course course) {
        if (this.enrolledCourses.size() < MAX_ENROLLMENT && !this.enrolledCourses.contains(course)) {
            this.enrolledCourses.add(course);
            course.setenrolledStudents(this);
            return true;
        }
        return false;
    }

    public boolean drop(Course course) {
        if (this.enrolledCourses.contains(course)) {
            this.enrolledCourses.remove(course);
            course.setenrolledStudents(this);
            return true;
        }
        return false;
    }

    public List<Course> getenrolledCourses() {
        return this.enrolledCourses;
    }

    public static int getMaxEnrollment() {
        return MAX_ENROLLMENT;
    }

}

class Professor extends UniversityMember {

    /**. how many courses can professor teach simultaneously  */
    private static final int MAX_LOAD = 2;

    /**. the list of courses teaching */
    private List<Course> assignedCourses;

    Professor(String memberName) {
        super(UniversityMember.getnumberOfMembers() + 1, memberName);
        this.assignedCourses = new ArrayList<Course>();
    }

    public boolean teach(Course course) {
        if (this.assignedCourses.size() < MAX_LOAD && !this.assignedCourses.contains(course)) {
            this.assignedCourses.add(course);
            return true;
        }
        return false;
    }

    public boolean exempt(Course course) {
        if (this.assignedCourses.contains(course)) {
            this.assignedCourses.remove(course);
            return true;
        }
        return false;
    }

    public List<Course> getassignedCourses() {
        return this.assignedCourses;
    }

    public static int getMaxLoad() {
        return MAX_LOAD;
    }
}
