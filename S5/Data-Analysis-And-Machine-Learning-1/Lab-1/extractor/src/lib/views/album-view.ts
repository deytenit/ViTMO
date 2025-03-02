import { Resource, View } from "./view.js";
import { TagView } from "./tag-view.js";
import { Album } from "../types.js";

export class AlbumView extends View<Album[]> {
  public static readonly VIEW = "albums";

  private page_: number;
  public set page(value: number) {
    this.page_ = value;
  }
  public get page(): number {
    return this.page_;
  }

  public constructor(tag: string) {
    super("tag", [tag]);

    this.page_ = 0;
  }

  public override async scrap(): Promise<Album[]> {
    const $ = await this.getPage();

    const $content = $(".container.page-content");

    const resources = $content
      .find(AlbumResource.SELECTOR).toArray()
      .map((element) => new AlbumResource($(element)));

    return resources.map((resource) => resource.make());
  }

  protected override getPageQueries(): Record<string, string> {
     return {
      page: String(this.page_),
    };
  }

  protected override getView(): string {
    return AlbumView.VIEW;
  }
}

class AlbumResource extends Resource<Album> {
  public static readonly SELECTOR = ".resource-list--release-list-item";

  private static readonly AUX_TRASH_CHARS = /·/g;

  public override make(): Album {
    return {
      title: this.getTitle(),
      author: this.getAuthor(),
      coverUrl: this.getCoverURL(),
      listeners: this.getListeners(),
      length: this.getLength(),
      released: this.getReleased(),
      tagView: this.getTagView(),
    };
  }

  public override getSelector(): string {
    return AlbumResource.SELECTOR;
  }

  private getTitle(): string {
    return this.resource_
        .find(this.getFieldSelector("name"))
        .text()
        .trim();
  }

  private getAuthor(): string {
    return this.resource_
        .find(this.getFieldSelector("artist"))
        .text()
        .trim();
  }

  private getCoverURL(): string | null {
    return this.resource_
        .find(`${this.getFieldSelector("image")} img`)
        .attr('src')
        ?.trim() ?? null;
  }

  private getAuxText(): string[] {
    return this.resource_
      .find(this.getFieldSelector("aux-text"))
      .text()
      .split(/\r?\n/)
      .map((str) => str.replace(AlbumResource.AUX_TRASH_CHARS, '').trim())
      .filter(Boolean);
  }

  private matchAuxText(regex: RegExp): string | null {
    const lines = this.getAuxText();

    for (const line of lines) {
      const match = line.match(regex);

      if (match) {
        return match[0];
      }
    }

    return null;
  }

  private getListeners(): number | null {
    const match = this.matchAuxText(/^[\d,]+(?= *listeners$)/g);

    return match ? parseInt(match.replace(/,/g, '')) : null;
  }

  private getLength(): number | null {
    const match = this.matchAuxText(/^[\d,]+(?= *tracks$)/g);

    return match ? parseInt(match.replace(/,/g, '')) : null;
  }

  private getReleased(): Date | null {
    const match = this.matchAuxText(/^(\d{1,2} \w{3,4} )?\d{4}$/g);

    return match ? new Date(match) : null;
  }

  private getTagView(): TagView {
    const href = this.resource_
        .find(`${this.getFieldSelector("name")} a`)
        .attr("href");
    
    if (!href) {
      throw new Error("Album does not have own page?");
    }

    const [_, artist, album] = href.split('/').filter(Boolean);
    return new TagView(artist, album);
  }
}

