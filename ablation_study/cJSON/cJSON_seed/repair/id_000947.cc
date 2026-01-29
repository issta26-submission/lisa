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
//<ID> 947
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "items", items);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "meta", meta);
    cJSON_AddStringToObject(meta, "author", "fuzzer");
    cJSON_AddTrueToObject(meta, "enabled");
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *n1 = cJSON_CreateNumber(7.0);
    cJSON_InsertItemInArray(items, 0, s0);
    cJSON_InsertItemInArray(items, 1, n1);
    cJSON *id = cJSON_CreateNumber(1234.0);
    cJSON_AddItemToObject(config, "id", id);

    // step 3: Operate and Validate
    cJSON_bool is_obj = cJSON_IsObject(config);
    cJSON *dup = cJSON_Duplicate(config, 1);
    cJSON_bool equal_cfg = cJSON_Compare(config, dup, 1);
    int arr_size = cJSON_GetArraySize(items);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    const char *first_str = cJSON_GetStringValue(first);
    double second_num = cJSON_GetNumberValue(second);
    (void)first_str;
    (void)second_num;
    (void)is_obj;
    (void)arr_size;
    (void)equal_cfg;
    int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)printed;
    char *snapshot = cJSON_PrintUnformatted(dup);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}