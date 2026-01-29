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
//<ID> 473
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
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddStringToObject(root, "title", "cJSON API Test");
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToArray(items, raw);
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "ok", flag_true);
    cJSON_AddItemToObject(root, "failed", flag_false);

    // step 2: Configure
    int before_count = cJSON_GetArraySize(items);
    cJSON_DeleteItemFromArray(items, 1);
    int after_count = cJSON_GetArraySize(items);

    // step 3: Operate & Validate
    cJSON *ok_item = cJSON_GetObjectItem(root, "ok");
    cJSON *failed_item = cJSON_GetObjectItem(root, "failed");
    cJSON_bool ok_is_true = cJSON_IsTrue(ok_item);
    cJSON_bool failed_is_false = cJSON_IsFalse(failed_item);
    char info_buf[256];
    memset(info_buf, 0, sizeof(info_buf));
    sprintf(info_buf, "ver=%s|before=%d|after=%d|ok=%d|failed=%d", version ? version : "unknown", before_count, after_count, (int)ok_is_true, (int)failed_is_false);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}