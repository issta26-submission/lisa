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
//<ID> 479
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
    cJSON_AddStringToObject(root, "name", "fuzzer");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON *raw = cJSON_CreateRaw("{\"k\":42}");
    cJSON *t = cJSON_CreateTrue();
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, raw);
    cJSON_AddItemToArray(items, t);
    cJSON_AddItemToArray(items, f);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "status", "ok");

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    const char *first_str = cJSON_GetStringValue(first);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "ver=%s|first=%s", version ? version : "unknown", first_str ? first_str : "null");
    cJSON_DeleteItemFromArray(items, 1);
    cJSON *new_second = cJSON_GetArrayItem(items, 1);
    cJSON_bool third_is_true = cJSON_IsTrue(new_second);
    cJSON_bool fourth_is_false = cJSON_IsFalse(cJSON_GetArrayItem(items, 2));
    sprintf(status_buf + strlen(status_buf), "|t=%d|f=%d", (int)third_is_true, (int)fourth_is_false);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    (void)equal;
    (void)status_buf;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}