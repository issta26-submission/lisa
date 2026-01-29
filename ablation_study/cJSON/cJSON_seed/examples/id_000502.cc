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
//<ID> 502
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *item0 = cJSON_CreateString("alpha");
    cJSON *item1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(list, item0);
    cJSON_AddItemToArray(list, item1);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "note", cJSON_CreateString("initialized"));
    cJSON *list_copy = cJSON_Duplicate(list, 1);
    cJSON_AddItemToObject(root, "list_copy", list_copy);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    char buf[256];
    memset(buf, 0, sizeof(buf));
    cJSON *found_list = cJSON_GetObjectItem(root, "list");
    cJSON *first_item = cJSON_GetArrayItem(found_list, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    sprintf(buf, "first=%s;count=%d", first_str ? first_str : "null", cJSON_GetArraySize(found_list));
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    (void)equal;
    char *snapshot_parsed = cJSON_Print(parsed);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(snapshot_parsed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}