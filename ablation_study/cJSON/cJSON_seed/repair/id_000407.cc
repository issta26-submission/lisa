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
//<ID> 407
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
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *s1 = cJSON_CreateString("three");
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "version", 1.0);

    // step 2: Configure
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    int size = cJSON_GetArraySize(got_items);
    cJSON *second = cJSON_GetArrayItem(got_items, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *dup_items = cJSON_Duplicate(got_items, 1);
    cJSON_AddItemToObject(root, "items_copy", dup_items);

    // step 3: Operate
    cJSON *new_arr = cJSON_CreateArray();
    cJSON_AddItemToArray(new_arr, cJSON_CreateString("new_entry"));
    cJSON_AddItemToArray(new_arr, cJSON_CreateNumber(second_val + (double)size));
    cJSON_AddItemToObject(root, "new_arr", new_arr);
    cJSON *meta_get = cJSON_GetObjectItem(root, "meta");
    double ver = cJSON_GetNumberValue(cJSON_GetObjectItem(meta_get, "version"));

    // step 4: Validate and Cleanup
    char *out = cJSON_PrintUnformatted(root);
    (void)ver;
    (void)second_val;
    (void)size;
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}