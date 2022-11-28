# HamArc

## Features (modes)

-   create (default mode): create from file list provided. Name out archive by yourself or auto generate it.
-   extract: extract everything to archive-named folder or by providing list of **wildcards**.
-   append: expand existing archive with a new file(s).
-   delete: remove file(s) from archive using **wildcards**.
-   concat: concatenate one or more archives together.
-   list: print out content of an archive or filter it by **wildcard**. With file size provided.

## Options

-   word-size: modify internal encoder word size.
-   file: provide archive for manipulation.

## Notes

-   Operating on temporal files. Base file only being used while read and write.
-   Overwrite any existing files.
-   **MAGIC** in use for additional file format confirmation.
-   Block based writing (.tar for reference)
