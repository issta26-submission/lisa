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
//<ID> 683
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *t = cJSON_CreateTrue();
    cJSON *n = cJSON_CreateNumber(7.0);
    cJSON *s = cJSON_CreateString("sample");
    cJSON_AddItemToArray(items, t);
    cJSON_AddItemToArray(items, n);
    cJSON_AddItemToArray(items, s);
    cJSON_AddItemToObject(root, "items", items);
    (void)version;

    // step 2: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);

    // step 3: Validate
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    int parsed_count = cJSON_GetArraySize(parsed_items);
    cJSON *first_item = cJSON_GetArrayItem(parsed_items, 0);
    cJSON_bool equal = cJSON_Compare(first_item, t, 1);
    cJSON *status = cJSON_CreateBool(equal);
    cJSON_AddItemToObject(parsed, "first_is_true", status);
    char *final_snapshot = cJSON_PrintUnformatted(parsed);
    (void)parsed_count;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(final_snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}