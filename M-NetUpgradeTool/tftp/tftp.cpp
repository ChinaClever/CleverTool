/*
 * tftp.cpp
 * tftp上传功能的实现
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "tftp.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <curl.h>

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
    curl_off_t nread;
    /* in real-world cases, this would probably get this data differently
     as this fread() stuff is exactly what the library already would do
     by default internally */
    size_t retcode = fread(ptr, size, nmemb, (FILE*)stream);

    nread = (curl_off_t)retcode;
//  fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
//          " bytes from file\n", nread);
    return retcode;
}

/**
 * @brief 实现文件上传
 * @param file 要上传的文件  如"E:/ippdu_temhum.bin"
 * @param url 如："tftp://192.168.1.52:69/uploading.bin"
 * @return 0 正常
 */
static int tftpfileUpload(char *file, char *url)
{
    CURL *curl;
    CURLcode res;
    FILE *hd_src;
    struct stat file_info;
    curl_off_t fsize;
    struct curl_slist *headerlist=NULL;
    /* get the file size of the local file */
    if(stat(file, &file_info)) {
        printf("Couldnt open '%s': %s\n", file, strerror(errno));
        return 1;
    }
    fsize = (curl_off_t)file_info.st_size;

//      printf("Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.\n", fsize);

    /* get a FILE * of the same file */
    hd_src = fopen(file, "rb");

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {

        /* we want to use our own read function */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

        /* enable uploading */
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        /* specify target */
        curl_easy_setopt(curl,CURLOPT_URL, url);

        /* pass in that last of FTP commands to run after the transfer */
        curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);

        /* now specify which file to upload */
        curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);

        /* Set the size of the file to upload (optional).  If you give a *_LARGE
           option you MUST make sure that the type of the passed-in argument is a
           curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
           make sure that to pass in a type 'long' argument. */
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                         (curl_off_t)fsize);

        /* Now run off and do what you've been told! */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* clean up the FTP commands list */
        curl_slist_free_all (headerlist);

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    fclose(hd_src); /* close the local file */

    curl_global_cleanup();
    return 0;
}

Tftp::Tftp(QObject *parent) : QObject(parent)
{

}

/**
 * @brief 文件上传
 * @param file 文件名，带路径 如："E:/ippdu_temhum.bin"
 * @param ip
 * @return true
 */
bool Tftp::upload(const QString &file, const QString &ip)
{
    QString url = "tftp://" + ip +":69/" + file.section("/", -1);

    memset(cFile,0, sizeof(cFile));
    for(int i=0; i<file.size(); ++i)
        cFile[i] = file.at(i).toLatin1();

     memset(cUrl,0, sizeof(cUrl));
     for(int i=0; i<url.size(); ++i)
         cUrl[i] = url.at(i).toLatin1();

    int ret = tftpfileUpload(cFile, cUrl);
    if(ret) {
        return false;
    }

    return true;
}

