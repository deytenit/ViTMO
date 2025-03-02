import * as fs from "node:fs";

import { FETCH_TIMEOUT_MS, LASTFM_BASENAME } from "./constants.js";
import { Readable } from "node:stream";
import { ReadableStream } from "node:stream/web";
import { dirname } from "node:path";
import { finished } from "node:stream/promises";
import { join } from "node:path/posix";
import { mkdir } from "node:fs/promises";


function throttleWithQueue<T extends (...args: Parameters<T>) => PromiseLike<Awaited<ReturnType<T>>>>(
  fn: T, delay: number
): (...args: Parameters<T>) => Promise<ReturnType<T>> {
  let isCooldown = false;  
  const queue: { args: Parameters<T>, resolve: (value: ReturnType<T>) => void, reject: (reason?: unknown) => void }[] = [];


  const processQueue = () => {
    if (queue.length > 0) {
      const { args, resolve, reject } = queue.shift()!; 
      fn(...args).then(resolve, reject);  
      isCooldown = true;  
      setTimeout(() => {
        isCooldown = false;  
        processQueue();  
      }, delay);
    }
  };


  return function throttled(...args: Parameters<T>): Promise<ReturnType<T>> {
    return new Promise((resolve, reject) => {
      if (isCooldown) {
        queue.push({ args, resolve, reject });
      } else {
        fn(...args).then(resolve, reject);  
        isCooldown = true;
        setTimeout(() => {
          isCooldown = false;
          processQueue();  
        }, delay);
      }
    });
  };
}

async function fetchWithUserAgent_(url: string, options?: RequestInit): Promise<Response> {
  const res = await fetch(url, {
    "headers": {
      "accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7",
      "accept-language": "en-US,en;q=0.9",
      "cache-control": "no-cache",
      "pragma": "no-cache",
      "priority": "u=0, i",
      "sec-ch-ua": "\"Google Chrome\";v=\"129\", \"Not=A?Brand\";v=\"8\", \"Chromium\";v=\"129\"",
      "sec-ch-ua-mobile": "?0",
      "sec-ch-ua-platform": "\"Windows\"",
      "sec-fetch-dest": "document",
      "sec-fetch-mode": "navigate",
      "sec-fetch-site": "none",
      "sec-fetch-user": "?1",
      "upgrade-insecure-requests": "1",
      "cookie": "lfmjs=1; lfmjs=1; _lc2_fpi=13d633f43e5c--01j9ks2swpy277zt3manwnx953; s_ecid=MCMID%7C47973875862119734332237184461088100012; aam_uuid=48110279181218151052259568530431527892; _gid=GA1.2.933509441.1728557786; not_first_visit=1; lfmanon=1; X-UA-Device-Type=desktop; X-UA-Country-Code=RU; lpfrmo=0; s_vnum=1730906435658%26vn%3D3; s_invisit=true; s_lv_undefined_s=Less%20than%201%20day; AMCVS_10D31225525FF5790A490D4D%40AdobeOrg=1; AMCV_10D31225525FF5790A490D4D%40AdobeOrg=1585540135%7CMCIDTS%7C20007%7CMCMID%7C47973875862119734332237184461088100012%7CMCAAMLH-1729169470%7C6%7CMCAAMB-1729169470%7C6G1ynYcLPuiQxYZrsz_pkqfLG9yMXBpb2zX5dvJdYQJzPXImdj0y%7CMCOPTOUT-1728571870s%7CNONE%7CMCAID%7CNONE%7CvVersion%7C4.4.0; _BB.bs=f|1; _pubcid=fc1d488d-3397-423d-a1e2-572280eff7ee; _pubcid_cst=zix7LPQsHA%3D%3D; s_cc=true; _li_dcdm_c=.last.fm; _gat_gtag_UA_1966571_6=1; prevPageType=music_album_tags_overview; QSI_HistorySession=https%3A%2F%2Fwww.last.fm%2Fmusic%2FDan%2BSalvato%2FDoki%2BDoki%2BLiterature%2BClub!%2B(Original%2BSoundtrack)~1728564671482%7Chttps%3A%2F%2Fwww.last.fm%2Fmusic%2FDan%2BSalvato%2FDoki%2BDoki%2BLiterature%2BClub!%2B(Original%2BSoundtrack)%2F%2Btag~1728564673564%7Chttps%3A%2F%2Fwww.last.fm%2Fmusic%2FDan%2BSalvato%2FDoki%2BDoki%2BLiterature%2BClub!%2B(Original%2BSoundtrack)%2F%2Btags~1728564676400; __gads=ID=1ab75642f713596a:T=1728314431:RT=1728564685:S=ALNI_MaaD2a3nto4-zcI9Quo7QJKSCJ0uA; __gpi=UID=00000f046bd61889:T=1728314431:RT=1728564685:S=ALNI_MaXJVAD2vlhkopXeAqDS85pSesEEg; __eoi=ID=ea7fbc5c26480bd9:T=1728314431:RT=1728564685:S=AA-Afjbe20vB_S-qhuw6x3hAYr2c; OptanonConsent=isGpcEnabled=0&datestamp=Thu+Oct+10+2024+15%3A51%3A49+GMT%2B0300+(Moscow+Standard+Time)&version=202401.1.0&browserGpcFlag=0&isIABGlobal=false&hosts=&genVendors=&consentId=93dcc372-03f3-481c-8f29-1f400130a6c4&interactionCount=0&landingPath=NotLandingPage&groups=1%3A1%2C2%3A1%2C3%3A1%2C4%3A1%2C5%3A1&AwaitingReconsent=false&geolocation=RU%3BSPE; OptanonAlertBoxClosed=2024-10-10T12:51:49.205Z; _BB.d=0|||4; utag_main=v_id:01926791670000066590be0166df0506f002206700bd0$_sn:3$_ss:0$_st:1728566510045$vapi_domain:last.fm$_pn:4%3Bexp-session$ses_id:1728564670523%3Bexp-session; session_depth=www.last.fm%3D3%7C428642350%3D3; cto_bundle=qUFiIl8lMkZFUmlGc0ZkSm5oV0hJQmVhNkh0WUt2eGtoNGFTU2dERVZGTWw1dTdoaWdqUDNlZ0hZZEVyc1RMWTJBUCUyRlZqeE1jdUklMkZ2ejJ0VnhzbVNwc2cyd1FZRXhQcklkaEYlMkJkUkJLb3g5QUxpM1drJTNE; cto_bidid=kUCTHl8lMkJRZU9FY2tnelRsVmNCJTJGcERRQkVmT0hYQ21JTWpXUlJkTGJpNWdCUVhqZXhZaFJ6cnBOVXFkMUJLQ1RRJTJGODRtaiUyRlVpekFOUDdSUFphNGhpQ0E0dDNBJTNEJTNE; sessionid=eyJfYXV0aF91c2VyX2hhc2giOiJkZWZhdWx0Iiwic2Vzc2lvbl9pZCI6ImY5ZjA0M2JmLWYzY2MtNGM1Ni05YTA1LTE1OWViZjVlNzg5NSJ9:1sysdo:DARmWbze9VfZp21DW_zm0twe7PWLawKZFtABcn6vJ30; s_getNewRepeat=1728564710257-Repeat; s_lv_undefined=1728564710257; _awl=2.1728564701.5-c70f9858f1036df52692512fcff4170f-6763652d6575726f70652d7765737431-0; _ga_QVHSTYGXMZ=GS1.1.1728564671.3.1.1728564710.0.0.0; _ga=GA1.2.1122856784.1728314436"
    },
    "referrerPolicy": "strict-origin-when-cross-origin",
    "body": null,
    "method": "GET",
    ...options,
  });

  return res;
}

const fetchWithUserAgent = throttleWithQueue(fetchWithUserAgent_, FETCH_TIMEOUT_MS);

export const fetchLastFmHTML = async (
  path: string[],
  query: Record<string, string>,
  options?: RequestInit
): Promise<string> => {
  const url = new URL(join(...path), LASTFM_BASENAME);
  url.search = new URLSearchParams(query).toString();

  const res = await fetchWithUserAgent(url.toString(), options);
  return await res.text();
}

export const fetchFile = async (
  url: string,
  dest: string,
): Promise<void> => {
  if (fs.existsSync(dest)) {
    return;
  }

  const res = await fetchWithUserAgent(url);

  if (!res.body) {
    return;
  }
  
  const parent = dirname(dest);
  if (!fs.existsSync(parent)) {
    await mkdir(parent, { recursive: true });
  }

  const stream = fs.createWriteStream(dest);
  await finished(Readable.fromWeb(res.body as ReadableStream).pipe(stream));
}
