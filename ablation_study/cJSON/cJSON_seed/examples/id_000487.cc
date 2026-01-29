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
//<ID> 487
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON *s2ref = cJSON_CreateStringReference("world");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2ref);
    cJSON_AddStringToObject(root, "title", "test-suite");

    // step 2: Configure
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    const char *first_val = cJSON_GetStringValue(first);
    const char *second_val = cJSON_GetStringValue(second);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "first=%s|second=%s", first_val ? first_val : "null", second_val ? second_val : "null");

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateString("world-updated");
    cJSON_ReplaceItemInArray(items, 1, replacement);
    cJSON *new_second = cJSON_GetArrayItem(items, 1);
    const char *new_second_val = cJSON_GetStringValue(new_second);
    char snapshot_note[128];
    memset(snapshot_note, 0, sizeof(snapshot_note));
    sprintf(snapshot_note, "after_replace=%s", new_second_val ? new_second_val : "null");
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    (void)equal;
    (void)status_buf;
    (void)snapshot_note;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}