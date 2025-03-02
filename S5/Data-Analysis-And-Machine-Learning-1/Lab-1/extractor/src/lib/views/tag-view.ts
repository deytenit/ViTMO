import * as cheerio from "cheerio";
import { Resource, View } from "./view.js";
import { Element } from "domhandler";
import { Tag } from "../types.js";

const SCRAP_GET_LIMIT = 5;

export class TagView extends View<string[]> {
  public static readonly VIEW = "+tags";

  public constructor(artist: string, album: string) {
    super("music", [artist, album]);
  }

  public override async scrap(): Promise<string[]> {
    let $: cheerio.CheerioAPI;
    let $content: cheerio.Cheerio<Element>;
    let attemps = 0;
    do {
      $ = await this.getPage();
      $content = $(".container.page-content");
    } while ($content.html() === null && attemps++ < SCRAP_GET_LIMIT);

    if ($content.html() === null) {
      console.error(this.getUnitPath());
    }

    const resources = $content
      .find(TagResourse.SELECTOR).toArray()
      .map((element) => new TagResourse($(element)));

    return resources.map((resource) => resource.make());
  }

  protected override getView(): string {
    return TagView.VIEW;
  }
}

class TagResourse extends Resource<Tag> {
  public static readonly SELECTOR = ".big-tags-item";

  public override getSelector(): string {
    return TagResourse.SELECTOR;
  }

  public override make(): string {
    return this.getTitle();
  }

  private getTitle(): string {
    return this.resource_
        .find(this.getFieldSelector("name"))
        .text()
        .trim();
  }
}


