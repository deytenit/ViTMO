import * as fs from "node:fs";

import { AlbumCSV } from "./lib/types.js";
import { AlbumView } from "./lib/views/album-view.js";
import { stringify } from "csv-stringify";


const TAG = "soundtrack";
const FROM = 1;
const TO = 501;
const STEP = 5;
const DEST = "../files/albums_plain";



for (let from = FROM; from < TO; from += STEP) {
  const albums = (await Promise.all(
    Array.from({ length: STEP }, (_, idx) => {
      const view = new AlbumView(TAG);
      view.page = from + idx;

      return view.scrap();
    })
  )).flat();
  
  const csvData = await Promise.all(
    albums.map(async (album): Promise<AlbumCSV> => ({
      title: album.title,
      author: album.author,
      coverUrl: album.coverUrl,
      length: album.length,
      listeners: album.listeners,
      released: album.released,
      tags: await album.tagView.scrap(),
    }))
  );

  const stream = fs.createWriteStream(`${DEST}[${from},${from + STEP - 1}].tsv`);

  await new Promise<void>((r) => stringify(csvData, {delimiter: '\t'}, (err, out) => {
    if (err) {
      console.error(err);
    }

    stream.write(out);
    r();
  }));
}
