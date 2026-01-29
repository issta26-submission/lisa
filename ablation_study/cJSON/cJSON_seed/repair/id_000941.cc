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
//<ID> 941
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "items", items);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON_InsertItemInArray(items, 0, s1);
    cJSON_InsertItemInArray(items, 1, s2);
    cJSON_InsertItemInArray(items, 1, s3);
    cJSON *id = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(meta, "id", id);
    cJSON_AddNumberToObject(root, "item_count", (double)cJSON_GetArraySize(items));

    // step 3: Operate and Validate
    cJSON_bool data_is_object = cJSON_IsObject(data);
    cJSON_bool meta_is_object = cJSON_IsObject(meta);
    cJSON_bool data_meta_equal = cJSON_Compare(data, meta, 0);
    cJSON_AddNumberToObject(root, "data_is_object", (double)data_is_object);
    cJSON_AddNumberToObject(root, "meta_is_object", (double)meta_is_object);
    cJSON_AddNumberToObject(root, "data_meta_equal", (double)data_meta_equal);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool dup_equal_cs = cJSON_Compare(root, dup, 1);
    cJSON_AddNumberToObject(root, "dup_equal_case_sensitive", (double)dup_equal_cs);
    int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}