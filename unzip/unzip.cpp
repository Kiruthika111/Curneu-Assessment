#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "../../zip.h"
#include "../../unzip.h"





ZRESULT RemoveFileFromZip(const TCHAR *zipfn, const TCHAR *zename);
ZRESULT AddFileToZip(const TCHAR *zipfn, const TCHAR *zename, const TCHAR *zefn);




int main()
{ printf("This program shows how to modify an existing zipfile -- add to it, remove from it.\n");
 
  // First we'll create some small files
  const char *s; FILE *f;
  
  HZIP hz = CreateZip("E:\\modify.zip",0);
  ZipAdd(hz,"zna.txt","E:\\a.txt");
  ZipAdd(hz,"znb.txt","E:\\b.txt");
  ZipAdd(hz,"znc.txt","E:\\c.txt");
  ZipAddFolder(hz,"znsub");
  CloseZip(hz);
  
 

  printf("Removing znb.txt from the zip file...\n");
  RemoveFileFromZip("E:\\modify.zip","znb.txt");


  return 0;
}



ZRESULT RemoveFileFromZip(const TCHAR *zipfn, const TCHAR *zename)
{ return AddFileToZip(zipfn,zename,0);
}
ZRESULT AddFileToZip(const TCHAR *zipfn, const TCHAR *zename, const TCHAR *zefn)
{ if (GetFileAttributes(zipfn)==0xFFFFFFFF || (zefn!=0 && GetFileAttributes(zefn)==0xFFFFFFFF)) return ZR_NOFILE;
  
  HANDLE hf=CreateFile(zipfn,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0); 
  if (hf==INVALID_HANDLE_VALUE)
   return ZR_NOFILE; DWORD size=GetFileSize(hf,0); 
   CloseHandle(hf);
  if (zefn!=0) {
  hf=CreateFile(zefn,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
   if (hf==INVALID_HANDLE_VALUE) 
   return ZR_NOFILE; size+=GetFileSize(hf,0); 
   CloseHandle(hf);
   }
  size*=2; 
  //
  HZIP hzsrc=OpenZip(zipfn,0); if (hzsrc==0) return ZR_READ;
  HZIP hzdst=CreateZip(0,size,0); if (hzdst==0) {CloseZip(hzsrc); return ZR_WRITE;}
  
  char *buf=0; unsigned int bufsize=0; /
  ZIPENTRY ze; ZRESULT zr=GetZipItem(hzsrc,-1,&ze); int numitems=ze.index; if (zr!=ZR_OK) {CloseZip(hzsrc); CloseZip(hzdst); return zr;}
  for (int i=0; i<numitems; i++)
  { zr=GetZipItem(hzsrc,i,&ze); 
  if (zr!=ZR_OK) {
  CloseZip(hzsrc); 
  CloseZip(hzdst); 
  return zr;}
    if (stricmp(ze.name,zename)==0) continue; 
    if (ze.attr&FILE_ATTRIBUTE_DIRECTORY) {
	zr=ZipAddFolder(hzdst,ze.name); 
	if (zr!=ZR_OK) {
	CloseZip(hzsrc); 
	CloseZip(hzdst); 
	return zr;} 
	continue;
	}
    if (ze.unc_size>(long)bufsize) {
	if (buf!=0) delete[] buf; 
	bufsize=ze.unc_size*2; 
	buf=new char[bufsize];
	}
    zr=UnzipItem(hzsrc,i,buf,bufsize); 
	if (zr!=ZR_OK) {
	CloseZip(hzsrc); 
	CloseZip(hzdst); 
	return zr;}
    zr=ZipAdd(hzdst,ze.name,buf,bufsize); 
	if (zr!=ZR_OK) {
	CloseZip(hzsrc); 
	CloseZip(hzdst); 
	return zr;
	}
  }
 
  return ZR_OK;
}

