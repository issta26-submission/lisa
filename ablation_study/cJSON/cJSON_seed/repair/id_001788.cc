#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1788
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddNumberToObject(config, "version", 1.2);
    cJSON_AddStringToObject(config, "env", "production");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "author", "tester");
    cJSON_AddNumberToObject(root, "threshold", 100.0);

    // step 2: Configure
    cJSON_AddStringToObject(root, "notes", "transient");
    cJSON_AddNumberToObject(root, "retries", 3.0);
    cJSON_DeleteItemFromObject(root, "notes");

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *cfg = cJSON_GetObjectItem(root, "config");
    cJSON *env_item = cJSON_GetObjectItem(cfg, "env");
    const char *env_str = cJSON_GetStringValue(env_item);
    cJSON *author_item = cJSON_GetObjectItem(cJSON_GetObjectItem(root, "meta"), "author");
    const char *author_str = cJSON_GetStringValue(author_item);
    cJSON *thr_item = cJSON_GetObjectItem(root, "threshold");
    double thr_val = cJSON_GetNumberValue(thr_item);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = env_str ? env_str[0] : 'Z';
    buffer[2] = author_str ? author_str[0] : 'Y';
    buffer[3] = (char)('0' + ((int)thr_val % 10));
    (void)env_str;
    (void)author_str;
    (void)thr_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}