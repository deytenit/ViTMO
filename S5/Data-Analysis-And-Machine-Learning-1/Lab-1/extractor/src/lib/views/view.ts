import * as cheerio from "cheerio";
import { Element } from "domhandler";

import { fetchLastFmHTML } from "../fetch.js";

export abstract class View<T> {
  private readonly unit_: string;
  private readonly path_: string[];

  public constructor(unit: string, path: string[]) {
    this.unit_ = unit;
    this.path_ = path;
  }

  public abstract scrap(): Promise<T>;

  protected async getPage(): Promise<cheerio.CheerioAPI> {
    const page = await fetchLastFmHTML(
      [this.unit_, ...this.path_, this.getView()],
      this.getPageQueries(),
    );

    return cheerio.load(page);
  }

  protected getPageQueries(): Record<string, string> {
    return {};
  }

  protected abstract getView(): string;

  public getUnitPath(): string {
    return this.unit_ + "/" + this.path_.join('/');
  }
}

export abstract class Resource<T> {
  protected readonly resource_: cheerio.Cheerio<Element>;

  public constructor(resource: cheerio.Cheerio<Element>) {
    this.resource_ = resource;
  }

  protected getFieldSelector(field: string): string {
    return `${this.getSelector()}-${field}`;
  }

  public abstract getSelector(): string;

  public abstract make(): T;
}
