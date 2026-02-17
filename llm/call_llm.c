#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Replace with your actual API Key
#define API_KEY "YOUR_GEMINI_API_KEY"
#define URL "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=" API_KEY

void 
process_file(
    const char * const filename,
    const char * const url 
    )
{
  CURL *curl;
  CURLcode res;

  // 1. Read input file
  FILE *f = fopen(filename, "rb");
  if (!f) return;
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *content = malloc(fsize + 1);
  fread(content, 1, fsize, f);
  fclose(f);
  content[fsize] = 0;

  // 2. Prepare JSON Payload (Simplified for example)
  // In a real app, use cJSON to escape strings properly
  char payload[4096];
  snprintf(payload, sizeof(payload), 
      "{\"contents\": [{\"parts\":[{\"text\": \"Please summarize this: %s\"}]}]}", content);

  curl = curl_easy_init();
  if(curl) {
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Output file naming logic
    char out_name[256];
    snprintf(out_name, sizeof(out_name), "response_%s.json", filename);
    FILE *out_f = fopen(out_name, "w");

    curl_easy_setopt(curl, CURLOPT_URL, URL);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, out_f);

    res = curl_easy_perform(curl);

    if(res != CURLE_OK)
      fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(res));

    fclose(out_f);
    curl_easy_cleanup(curl);
  }
  free(content);
}

int 
main(
    int argc, 
    char *argv[]
    ) 
{
  const char *api_key = getenv("GEMINI_API_KEY");
  sprintf(url, "%s %s ", URL, api_key);
  if (argc < 2) {
    printf("Usage: %s file1.txt file2.txt ...\n", argv[0]);
    return 1;
  }

  curl_global_init(CURL_GLOBAL_ALL);
  for (int i = 1; i < argc; i++) {
    printf("Processing %s...\n", argv[i]);
    process_file(argv[i], url);
  }
  curl_global_cleanup();

  return 0;
}
