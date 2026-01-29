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
//<ID> 733
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *ver = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON *name_ref = cJSON_CreateStringReference("example_name");
    cJSON *item_ref1 = cJSON_CreateStringReference("one");
    cJSON *item_ref2 = cJSON_CreateStringReference("two");

    // step 2: Configure
    cJSON_AddItemToArray(list, item_ref1);
    cJSON_AddItemToArray(list, item_ref2);
    cJSON_AddItemToObject(root, "list", list);
    cJSON_AddItemToObject(root, "name", name_ref);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snap_len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(snap_len + 1);
    memset(buffer, 0, snap_len + 1);
    memcpy(buffer, snapshot, snap_len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);

    // step 3: Operate & Validate
    cJSON *parsed_list = cJSON_GetObjectItem(parsed, "list");
    cJSON *first_elem = cJSON_GetArrayItem(parsed_list, 0);
    const char *first_val = cJSON_GetStringValue(first_elem);
    cJSON *parsed_name = cJSON_GetObjectItem(parsed, "name");
    const char *name_val = cJSON_GetStringValue(parsed_name);
    cJSON *dup_array = cJSON_CreateArray();
    cJSON *dup_ref = cJSON_CreateStringReference(first_val);
    cJSON_AddItemToArray(dup_array, dup_ref);
    cJSON_AddItemToObject(parsed, "dup", dup_array);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    (void)ver;
    (void)name_val;
    // API sequence test completed successfully
    return 66;
}