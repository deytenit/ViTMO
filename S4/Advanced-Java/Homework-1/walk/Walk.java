package info.kgeorgiy.ja.eremin.walk;

import info.kgeorgiy.ja.eremin.util.ArgumentsResolver;
import info.kgeorgiy.ja.eremin.util.SharedUtilities;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.InvalidPathException;
import java.nio.file.Path;
import java.util.function.UnaryOperator;

public class Walk {
  private static final int BUFFER_SIZE = 1024;
  private static final String INVALID_HASH = "0".repeat(8);

  private static String generateFileHash(String path_) {
    Path path;
    try {
      path = Path.of(path_);
    } catch (InvalidPathException err) {
      System.err.println("Invalid path was provided in input file. " + err.getMessage());
      return INVALID_HASH;
    }

    int hash = 0;

    try (InputStream stream = Files.newInputStream(path)) {
      int bufferSpan;
      byte[] buffer = new byte[BUFFER_SIZE];

      while ((bufferSpan = stream.read(buffer)) != -1) {
        for (int i = 0; i < bufferSpan; ++i) {
          hash += buffer[i] & 0xff;
          hash += hash << 10;
          hash ^= hash >>> 6;
        }
      }
    } catch (final IOException err) {
      System.err.println("Cannot calculate checksum for file. " + err.getMessage());
      return INVALID_HASH;
    }

    hash += hash << 3;
    hash ^= hash >>> 11;
    hash += hash << 15;

    return String.format("%08x", hash);
  }

  public static void main(String[] args) {
    try {
      final ArgumentsResolver arguments = new ArgumentsResolver(args);

      Path inputFilePath;
      try {
        inputFilePath = Path.of(arguments.positional(0, UnaryOperator.identity()));
      } catch (final InvalidPathException err) {
        System.err.println("Invalid input path was provided. " + err.getMessage());
        return;
      }

      Path outputFilePath;
      try {
        outputFilePath = Path.of(arguments.positional(1, UnaryOperator.identity()));
      } catch (final InvalidPathException err) {
        System.err.println("Invalid output path was provided. " + err.getMessage());
        return;
      }

      try {
        SharedUtilities.createDirectories(outputFilePath);
      } catch (final IOException err) {
        System.out.println("Cannot create directories for output file. " + err.getMessage());
        return;
      }

      if (outputFilePath.normalize().getParent() != null) {
        try {
          Files.createDirectories(outputFilePath.normalize().getParent());
        } catch (final IOException err) {
          System.err.println(
              "Cannot create parent directories for output file. " + err.getMessage());
          return;
        }
      }

      try (BufferedReader reader = Files.newBufferedReader(inputFilePath, StandardCharsets.UTF_8)) {
        try (BufferedWriter writer =
            Files.newBufferedWriter(outputFilePath, StandardCharsets.UTF_8)) {
          String line;
          try {
            while ((line = reader.readLine()) != null) {
              try {
                writer.write(generateFileHash(line) + " " + line);
                writer.newLine();
              } catch (final IOException err) {
                System.err.println("Error while writing to output file. " + err.getMessage());
              }
            }
          } catch (final IOException err) {
            System.err.println("Error while reading from input file. " + err.getMessage());
          }
        } catch (final IOException err) {
          System.err.println("Cannot create/access output file to write. " + err.getMessage());
        }
      } catch (final FileNotFoundException err) {
        System.err.println("Cannot find input file. " + err.getMessage());
      } catch (final IOException err) {
        System.err.println("Cannot access input file to read. " + err.getMessage());
      }
    } catch (final ArgumentsResolver.ArgumentsResolverException e) {
      System.out.println(e.getMessage());
    }
  }
}
