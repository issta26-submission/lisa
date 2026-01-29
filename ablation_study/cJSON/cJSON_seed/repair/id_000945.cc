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
//<ID> 945
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *id = cJSON_CreateNumber(1001.0);
    cJSON_AddItemToObject(root, "id", id);
    cJSON *name = cJSON_CreateString("fuzzer_node");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());

    // step 2: Configure
    cJSON_AddItemToArray(items, cJSON_CreateString("first"));
    cJSON_bool insert_ok = cJSON_InsertItemInArray(items, 0, cJSON_CreateNumber(7.0));
    (void)insert_ok;
    cJSON_AddItemToArray(items, cJSON_CreateRaw("{\"raw\":true}"));

    // step 3: Operate and Validate
    cJSON_bool is_obj = cJSON_IsObject(root);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    const char *name_str = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(meta, "name"));
    char *printed = cJSON_Print(root);
    int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)printed_ok;
    cJSON_free(printed);
    char *snapshot = cJSON_PrintUnformatted(dup);
    cJSON_free(snapshot);
    (void)is_obj;
    (void)equal;
    (void)name_str;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}