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
//<ID> 588
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"user\": \"alice\", \"id\": 42, \"active\": true }";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);

    // step 2: Configure
    cJSON *user_item = cJSON_GetObjectItem(root, "user");
    char *user_name = cJSON_GetStringValue(user_item);
    cJSON *id_item = cJSON_GetObjectItem(root, "id");
    double id_value = cJSON_GetNumberValue(id_item);
    cJSON *history = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "history", history);
    cJSON_AddItemToArray(history, cJSON_CreateString("login"));
    cJSON_AddItemToArray(history, cJSON_CreateString("update"));

    // step 3: Operate
    char *buffered = cJSON_PrintBuffered(root, 256, 1);
    int prealloc_len = 512;
    char *prealloc = (char *)cJSON_malloc(prealloc_len);
    memset(prealloc, 0, prealloc_len);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, prealloc, prealloc_len, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "id");
    cJSON_AddStringToObject(root, "id_status", "removed");

    // step 4: Validate & Cleanup
    cJSON *first_hist = cJSON_GetArrayItem(history, 0);
    char *first_hist_str = cJSON_GetStringValue(first_hist);
    size_t buffered_len = buffered ? strlen(buffered) : 0;
    size_t prealloc_content_len = prealloc_ok ? strlen(prealloc) : 0;
    (void)user_name;
    (void)id_value;
    (void)first_hist_str;
    (void)buffered_len;
    (void)prealloc_content_len;
    cJSON_free(buffered);
    cJSON_free(prealloc);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}