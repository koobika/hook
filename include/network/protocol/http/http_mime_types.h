// /////////////////////////////////////////////////////////////////////////////
//
//       ╓▄▓▓▓▓▓▓▓▄╖      ╓▄▓▓▓▓▓▓▓▄╖
//    ╓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓╖╓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓w
//  ,▓▓▓▓▓▓▓▓▀▀▀▀▓▓▓▓▓▓▓▓▓▓▓▓▓▀▀▀▀▓▓▓▓▓▓▓,
//  ▓▓▓▓▓▓`       `▓▓▓▓▓▓▓▓`        ▓▓▓▓▓▓
// ╫▓▓▓▓▓           ▓▓▓▓▓▓           ▓▓▓▓▓▓
// ▓▓▓▓▓▓           ▓▓▓▓▓▓           ╟▓▓▓▓▓
// ╙▓▓▓▓▓▄         ╓▓▓▓▓▓╛          ╓▓▓▓▓▓▌
//  ▀▓▓▓▓▓▓æ,   ,g▓▓▓▓▓▓▀   ,,,  ,g▓▓▓▓▓▓▌
//   '▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓`  ╒▓▓▓▓▓▓▓▓▓▓▓▓▓'
//      ▀▓▓▓▓▓▓▓▓▓▓▓▀`     ▓▓▓▓▓▓▓▓▓▓▀`
//          `"""`            `"""`
// -----------------------------------------------------------------------------
// network/protocol/http/http_mime_types.h
// -----------------------------------------------------------------------------
// Copyright (c) 2021 koobika corporation. All rights reserved.
// Author: Marcos Rojas (mrojas@koobika.org).
// -----------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

#ifndef koobika_hook_network_protocol_http_httpmimetypes_h
#define koobika_hook_network_protocol_http_httpmimetypes_h

namespace koobika::hook::network::protocol::http {
// =============================================================================
// HttpMimeTypes                                                       ( class )
// -----------------------------------------------------------------------------
// This specification holds for http method writer interface
// =============================================================================
class HttpMimeTypes {
 public:
  // ___________________________________________________________________________
  // CONSTANTs                                                        ( public )
  // 
  // Aac: AAC audio | audio/aac.
  static constexpr char kAAC[] = "audio/aac";
  // Abw: AbiWord document | application/x-abiword.
  static constexpr char kABW[] = "application/x-abiword";
  // Arc: Archive document (multiple files embedded) | application/x-freearc.
  static constexpr char kARC[] = "application/x-freearc";
  // Avi: AVI: Audio Video Interleave | video/x-msvideo.
  static constexpr char kAVI[] = "video/x-msvideo";
  // Azw: Amazon Kindle eBook format | application/vnd.amazon.ebook.
  static constexpr char kAZW[] = "application/vnd.amazon.ebook";
  // Bin: Any kind of binary data | application/octet-stream.
  static constexpr char kBIN[] = "application/octet-stream";
  // Bmp: Windows OS/2 Bitmap Graphics | image/bmp.
  static constexpr char kBMP[] = "image/bmp";
  // Bz: BZip archive | application/x-bzip.
  static constexpr char kBZ[] = "application/x-bzip";
  // Bz2: BZip2 archive | application/x-bzip2.
  static constexpr char kBZ2[] = "application/x-bzip2";
  // Csh: C-Shell script | application/x-csh.
  static constexpr char kCSH[] = "application/x-csh";
  // Css: Cascading Style Sheets (CSS) | text/css.
  static constexpr char kCSS[] = "text/css";
  // Csv: Comma-separated values (CSV) | text/csv.
  static constexpr char kCSV[] = "text/csv";
  // Doc: Microsoft Word | application/msword.
  static constexpr char kDOC[] = "application/msword";
  // Docx: Microsoft Word (OpenXML)
  //  application/vnd.openxmlformats-officedocument.wordprocessingml.document.
  static constexpr char kDOCX[] =
      "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
  // Eot: MS Embedded OpenType fonts | application/vnd.ms-fontobject.
  static constexpr char kEOT[] = "application/vnd.ms-fontobject";
  // Epub: Electronic publication (EPUB) | application/epub+zip.
  static constexpr char kEPUB[] = "application/epub+zip";
  // Gz: GZip Compressed Archive | application/gzip.
  static constexpr char kGZ[] = "application/gzip";
  // Gif: Graphics Interchange Format (GIF) | image/gif.
  static constexpr char kGIF[] = "image/gif";
  // Htm: HyperText Markup Language (HTML) | text/html.
  static constexpr char kHTM[] = "text/html";
  // Html: HyperText Markup Language (HTML) | text/html.
  static constexpr char kHTML[] = "text/html";
  // Ico: Icon format | image/vnd.microsoft.icon.
  static constexpr char kICO[] = "image/vnd.microsoft.icon";
  // Ics: iCalendar format | text/calendar.
  static constexpr char kICS[] = "text/calendar";
  // Jar: Java Archive (JAR) | application/java-archive.
  static constexpr char kJAR[] = "application/java-archive";
  // Jpg: JPEG images | image/jpeg.
  static constexpr char kJPG[] = "image/jpeg";
  // Jpeg: JPEG images | image/jpeg.
  static constexpr char kJPEG[] = "image/jpeg";
  // Js: JavaScript | text/javascript, per the following specifications:
  //  https://html.spec.whatwg.org/multipage/#scriptingLanguages
  //  https://html.spec.whatwg.org/multipage/#dependencies:willful-violation
  //  https://datatracker.ietf.org/doc/draft-ietf-dispatch-javascript-mjs/
  static constexpr char kJS[] = "text/javascript";
  // Json: JSON format | application/json.
  static constexpr char kJSON[] = "application/json";
  // Jsonld: JSON-LD format | application/ld+json.
  static constexpr char kJSONLD[] = "application/ld+json";
  // Midi: Musical Instrument Digital Interface (MIDI) | audio/midi.
  static constexpr char kMIDI[] = "audio/midi";
  static constexpr char kMID[] = "audio/midi";
  // Xmidi: Musical Instrument Digital Interface (MIDI) | audio/x-midi.
  static constexpr char kXMIDI[] = "audio/x-midi";
  static constexpr char kXMID[] = "audio/x-midi";
  // Mjs: JavaScript module | text/javascript.
  static constexpr char kMJS[] = "text/javascript";
  // Mp3: MP3 audio | audio/mpeg.
  static constexpr char kMP3[] = "audio/mpeg";
  // Mpeg	MPEG Video	video/mpeg.
  static constexpr char kMPEG[] = "video/mpeg";
  // Mpkg: Apple Installer Package | application/vnd.apple.installer+xml.
  static constexpr char kMPKG[] = "application/vnd.apple.installer+xml";
  // Odp: OpenDocument presentation document |
  //  application/vnd.oasis.opendocument.presentation.
  static constexpr char kODP[] =
      "application/vnd.oasis.opendocument.presentation";
  // Ods: OpenDocument spreadsheet document |
  //  application/vnd.oasis.opendocument.spreadsheet.
  static constexpr char kODS[] =
      "application/vnd.oasis.opendocument.spreadsheet";
  // Odt: OpenDocument text document |
  //  application/vnd.oasis.opendocument.text.
  static constexpr char kODT[] = "application/vnd.oasis.opendocument.text";
  // Oga: OGG audio | audio/ogg.
  static constexpr char kOGA[] = "audio/ogg";
  // Ogv: OGG video | video/ogg.
  static constexpr char kOGV[] = "video/ogg";
  // Ogx: OGG | application/ogg.
  static constexpr char kOGX[] = "application/ogg";
  // Opus: Opus audio | audio/opus.
  static constexpr char kOPUS[] = "audio/opus";
  // Otf: OpenType font | font/otf.
  static constexpr char kOTF[] = "font/otf";
  // Png: Portable Network Graphics | image/png.
  static constexpr char kPNG[] = "image/png";
  // Pdf: Adobe Portable Document Format (PDF) | application/pdf.
  static constexpr char kPDF[] = "application/pdf";
  // Php: Hypertext Preprocessor (Personal Home Page) | application/x-httpd-php.
  static constexpr char kPHP[] = "application/x-httpd-php";
  // Ppt: Microsoft PowerPoint | application/vnd.ms-powerpoint.
  static constexpr char kPPT[] = "application/vnd.ms-powerpoint";
  // Pptx: Microsoft PowerPoint (OpenXML) |
  //  application/vnd.openxmlformats-officedocument.presentationml.presentation.
  static constexpr char kPPTX[] =
      "application/"
      "vnd.openxmlformats-officedocument.presentationml.presentation";
  // Rar: RAR archive | application/vnd.rar.
  static constexpr char kRAR[] = "application/vnd.rar";
  // Rtf: Rich Text Format (RTF) | application/rtf.
  static constexpr char kRTF[] = "application/rtf";
  // Sh: Bourne shell script | application/x-sh.
  static constexpr char kSH[] = "application/x-sh";
  // Svg: Scalable Vector Graphics (SVG) | image/svg+xml.
  static constexpr char kSVG[] = "image/svg+xml";
  // Swf: Small web format (SWF) or Adobe Flash document |
  //  application/x-shockwave-flash.
  static constexpr char kSWF[] = "application/x-shockwave-flash";
  // Tar: Tape Archive (TAR) | application/x-tar.
  static constexpr char kTAR[] = "application/x-tar";
  // Tiff: Tagged Image File Format (TIFF) | image/tiff.
  static constexpr char kTIF[] = "image/tiff";
  static constexpr char kTIFF[] = "image/tiff";
  // Ts: MPEG transport stream | video/mp2t.
  static constexpr char kTS[] = "video/mp2t";
  // Ttf: TrueType Font | font/ttf.
  static constexpr char kTTF[] = "font/ttf";
  // Txt: Text, (generally ASCII or ISO 8859-n) | text/plain.
  static constexpr char kTXT[] = "text/plain";
  // Vsd: Microsoft Visio | application/vnd.visio.
  static constexpr char kVSD[] = "application/vnd.visio";
  // Wav: Waveform Audio Format | audio/wav.
  static constexpr char kWAV[] = "audio/wav";
  // Weba: WEBM audio | audio/webm.
  static constexpr char kWEBA[] = "audio/webm";
  // Webm: WEBM video | video/webm.
  static constexpr char kWEBM[] = "video/webm";
  // Webp: WEBP image | image/webp.
  static constexpr char kWEBP[] = "image/webp";
  // Woff: Web Open Font Format (WOFF) | font/woff.
  static constexpr char kWOFF[] = "font/woff";
  // Woff2: Web Open Font Format (WOFF) | font/woff2.
  static constexpr char kWOFF2[] = "font/woff2";
  // Xhtml: XHTML | application/xhtml+xml.
  static constexpr char kXHTML[] = "application/xhtml+xml";
  // Xls: Microsoft Excel | application/vnd.ms-excel.
  static constexpr char kXLS[] = "application/vnd.ms-excel";
  // Xlsx: Microsoft Excel (OpenXML) |
  //  application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.
  static constexpr char kXLSX[] =
      "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
  // Xml: XML | application/xml
  //  if not readable from casual users (RFC 3023, section 3) text/xml
  //  if readable from casual users (RFC 3023, section 3).
  static constexpr char kXMLNONREADABLE[] = "application/xml";
  static constexpr char kXMLREADABLE[] = "text/xml";
  // Xul: XUL | application/vnd.mozilla.xul+xml.
  static constexpr char kXUL[] = "application/vnd.mozilla.xul+xml";
  // Zip: ZIP archive | application/zip.
  static constexpr char kZIP[] = "application/zip";
  // 3gp: 3GPP audio/video container | video/3gpp audio/3gpp
  //  if it doesn't contain video.
  static constexpr char k3GP[] = "video/3gpp";
  static constexpr char k3GPNOVIDEO[] = "audio/3gpp";
  // 3g2: 3GPP2 audio/video container | video/3gpp2 audio/3gpp2
  //  if it doesn't contain video.
  static constexpr char k3G2[] = "video/3gpp2";
  static constexpr char k3G2NOVIDEOS[] = "audio/3gpp2";
  // 7z: 7-zip archive | application/x-7z-compressed.
  static constexpr char k7Z[] = "application/x-7z-compressed";
};
}  // namespace koobika::hook::network::protocol::http

#endif