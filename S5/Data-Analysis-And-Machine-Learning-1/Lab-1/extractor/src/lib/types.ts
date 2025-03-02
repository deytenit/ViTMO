import { TagView } from "./views/tag-view.js";

export interface Album {
  title: string;
  author: string;
  coverUrl: string | null;
  listeners: number | null;
  released: Date | null;
  length: number | null;
  tagView: TagView;
};

export interface AlbumCSV {
  title: string;
  author: string;
  coverUrl: string | null;
  listeners: number | null;
  released: Date | null;
  length: number | null;
  tags: Tag[];
}

export type Tag = string;
