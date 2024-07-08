package info.kgeorgiy.ja.eremin.crawler;

import info.kgeorgiy.ja.eremin.util.ArgumentsResolver;
import info.kgeorgiy.ja.eremin.util.SharedUtilities;
import info.kgeorgiy.java.advanced.crawler.*;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.*;

/**
 * Crawls websites by provided <a href="http://tools.ietf.org/html/rfc3986">URL</a>, with support of
 * ignoring URLs specified.
 *
 * @author Vladimir Eremin
 */
public class WebCrawler implements NewCrawler {
  private static final int DEFAULT_DEPTH = 5;
  private static final int DEFAULT_TIMESCALE = 1000;
  private static final int DEFAULT_DOWNLOADERS = 32;
  private static final int DEFAULT_EXTRACTORS = 32;

  private final Downloader downloader;

  private final ExecutorService downloaders;
  private final ExecutorService extractors;

  private final Phaser closer;
  private boolean closed;

  /**
   * Constructs this {@link WebCrawler}, defining configuration for {@link #download(String, int,
   * Set)} function and its derivatives.
   *
   * @param downloader {@link Downloader} instance for downloading webpages.
   * @param downloaders max amount of downloaders.
   * @param extractors max amount of extractors.
   * @param perhost should've been max amount of downloaders per host, see @apiNote.
   * @apiNote {@code perhost} param is ignored in this version of class.
   */
  public WebCrawler(
      final Downloader downloader, final int downloaders, final int extractors, final int perhost) {
    assert perhost >= downloaders;

    this.downloader = downloader;
    this.downloaders = Executors.newFixedThreadPool(downloaders);
    this.extractors = Executors.newFixedThreadPool(extractors);
    this.closer = new Phaser(1);
    this.closed = false;
  }

  /**
   * Implements command line interface for {@link WebCrawler}, using {@link CachingDownloader}.
   *
   * <p>Shall be used as following:
   *
   * <blockquote>
   *
   * <pre>
   *     WebCrawler url [depth [downloads [extractors [perHost]]]]
   * </pre>
   *
   * </blockquote>
   *
   * @param args command line arguments.
   */
  public static void main(final String[] args) {
    try {
      final ArgumentsResolver arguments = new ArgumentsResolver(args);

      try (final NewCrawler crawler =
          new WebCrawler(
              new CachingDownloader(DEFAULT_TIMESCALE),
              arguments.positional(2, Integer::parseInt, DEFAULT_DOWNLOADERS),
              arguments.positional(3, Integer::parseInt, DEFAULT_EXTRACTORS),
              arguments.positional(4, Integer::parseInt, DEFAULT_DOWNLOADERS))) {
        final Result result =
            crawler.download(
                arguments.positional(0, SharedUtilities::isValidURL),
                arguments.positional(1, Integer::parseInt, DEFAULT_DEPTH));

        System.out.println("URLs downloaded:");
        result.getDownloaded().forEach(System.out::println);

        System.out.println("Following errors occurred:");
        result.getErrors().forEach((ignored, e) -> System.out.println(e.toString()));
      } catch (final IOException e) {
        System.out.println("Error occurred while crawling website. " + e.getMessage());
      }
    } catch (final ArgumentsResolver.ArgumentsResolverException e) {
      System.out.println(e.getMessage());
      System.out.println("Usage: WebCrawler url [depth [downloads [extractors [perHost]]]]");
    }
  }

  /**
   * Crawls links from website of the provided {@code url} to the provided {@code depth}, ignoring
   * ones from {@code excludes}.
   *
   * <p>{@inheritDoc}
   *
   * @param url {@inheritDoc}
   * @param depth {@inheritDoc}
   * @param excludes {@inheritDoc}
   * @return {@inheritDoc}
   * @throws RejectedExecutionException if this download cannot be accepted for execution.
   */
  @Override
  public Result download(final String url, int depth, final Set<String> excludes) {
    if (closed) {
      throw new IllegalStateException("WebCrawler is closed. No new downloads are accepted.");
    }

    closer.register();

    final Map<String, IOException> errors = new ConcurrentHashMap<>();
    final Set<String> visited = ConcurrentHashMap.newKeySet();
    final Set<String> nextLayer = ConcurrentHashMap.newKeySet();
    final Phaser phaser = new Phaser(1);

    nextLayer.add(url);

    while (depth-- > 0) {
      final List<String> layer =
          nextLayer.stream()
              .filter(
                  u ->
                      !errors.containsKey(u)
                          && excludes.stream().noneMatch(u::contains)
                          && visited.add(u))
              .toList();
      nextLayer.clear();

      final int finalDepth = depth;
      layer.forEach(
          u -> {
            phaser.register();
            downloaders.execute(
                () -> {
                  try {
                    final Document doc = downloader.download(u);
                    if (finalDepth != 0) {
                      phaser.register();
                      extractors.execute(
                          () -> {
                            try {
                              nextLayer.addAll(doc.extractLinks());
                            } catch (final IOException e) {
                              errors.put(u, e);
                            } finally {
                              phaser.arriveAndDeregister();
                            }
                          });
                    }
                  } catch (final IOException e) {
                    visited.remove(u);
                    errors.put(u, e);
                  } finally {
                    phaser.arriveAndDeregister();
                  }
                });
          });

      phaser.arriveAndAwaitAdvance();
    }

    closer.arriveAndDeregister();
    return new Result(new ArrayList<>(visited), errors);
  }

  /**
   * Initiates an orderly shutdown in which previously submitted downloads are executed, but no new
   * ones will be accepted.
   *
   * <p>{@inheritDoc}
   */
  @Override
  public void close() {
    closed = true;
    try {
      closer.awaitAdvanceInterruptibly(
          closer.arrive(), SharedUtilities.WATERFALL_TIMEOUT, TimeUnit.MILLISECONDS);
    } catch (final InterruptedException ignored) {
      Thread.currentThread().interrupt();
    } catch (final TimeoutException ignored) {
    } finally {
      extractors.close();
      downloaders.close();
    }
  }
}
