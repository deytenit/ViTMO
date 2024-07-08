package info.kgeorgiy.ja.eremin.student;

import info.kgeorgiy.java.advanced.student.GroupName;
import info.kgeorgiy.java.advanced.student.Student;
import info.kgeorgiy.java.advanced.student.StudentQuery;
import java.util.*;
import java.util.function.BinaryOperator;
import java.util.function.Function;
import java.util.function.Predicate;
import java.util.stream.Collectors;

@SuppressWarnings("unused")
public class StudentDB implements StudentQuery {
  private static final Comparator<Student> NAME_COMPARATOR =
      Comparator.comparing(Student::getLastName)
          .thenComparing(Student::getFirstName)
          .thenComparing(Comparator.reverseOrder());

  private <T> List<T> map(final List<Student> students, Function<Student, T> mapFn) {
    return students.stream().map(mapFn).toList();
  }

  @Override
  public List<String> getFirstNames(final List<Student> students) {
    return map(students, Student::getFirstName);
  }

  @Override
  public List<String> getLastNames(final List<Student> students) {
    return map(students, Student::getLastName);
  }

  @Override
  public List<GroupName> getGroups(final List<Student> students) {
    return map(students, Student::getGroup);
  }

  @Override
  public List<String> getFullNames(final List<Student> students) {
    return map(students, x -> String.format("%s %s", x.getFirstName(), x.getLastName()));
  }

  @Override
  public Set<String> getDistinctFirstNames(final List<Student> students) {
    return new TreeSet<>(getFirstNames(students));
  }

  @Override
  public String getMaxStudentFirstName(final List<Student> students) {
    return students.stream().max(Comparator.naturalOrder()).map(Student::getFirstName).orElse("");
  }

  private List<Student> sorted(final Collection<Student> students, Comparator<Student> comparator) {
    return students.stream().sorted(comparator).toList();
  }

  @Override
  public List<Student> sortStudentsById(final Collection<Student> students) {
    return sorted(students, Comparator.comparingInt(Student::getId));
  }

  @Override
  public List<Student> sortStudentsByName(final Collection<Student> students) {
    return sorted(students, NAME_COMPARATOR);
  }

  private List<Student> filter(final Collection<Student> students, Predicate<Student> predicate) {
    return students.stream().filter(predicate).sorted(NAME_COMPARATOR).toList();
  }

  @Override
  public List<Student> findStudentsByFirstName(
      final Collection<Student> students, final String name) {
    return filter(students, x -> x.getFirstName().equals(name));
  }

  @Override
  public List<Student> findStudentsByLastName(
      final Collection<Student> students, final String name) {
    return filter(students, x -> x.getLastName().equals(name));
  }

  @Override
  public List<Student> findStudentsByGroup(
      final Collection<Student> students, final GroupName group) {
    return filter(students, x -> x.getGroup().equals(group));
  }

  @Override
  public Map<String, String> findStudentNamesByGroup(
      final Collection<Student> students, final GroupName group) {
    return findStudentsByGroup(students, group).stream()
        .collect(
            Collectors.toMap(
                Student::getLastName,
                Student::getFirstName,
                BinaryOperator.minBy(Comparator.naturalOrder())));
  }
}
