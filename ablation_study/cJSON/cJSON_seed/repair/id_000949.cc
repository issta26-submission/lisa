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
//<ID> 949
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

    // step 2: Configure
    cJSON *name = cJSON_CreateString("device_alpha");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddStringToObject(root, "version", "1.0");
    cJSON *id = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "id", id);
    cJSON_AddItemToArray(items, cJSON_CreateString("first"));
    cJSON *second = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(items, second);

    // step 3: Operate and Validate
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON_bool meta_equals_self = cJSON_Compare(meta, meta, 1);
    cJSON *inserted = cJSON_CreateString("inserted_at_1");
    cJSON_bool insert_result = cJSON_InsertItemInArray(items, 1, inserted);
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    char *snapshot = cJSON_PrintUnformatted(dup_root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(dup_root);
    int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)printed;
    cJSON_free(buffer);
    cJSON_Delete(root);
    (void)meta_is_obj;
    (void)meta_equals_self;
    (void)insert_result;
    // API sequence test completed successfully
    return 66;
}