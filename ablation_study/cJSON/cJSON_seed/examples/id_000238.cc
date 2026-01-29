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
//<ID> 238
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *raw = cJSON_CreateRaw("{\"raw\":true}");
    cJSON *sub = cJSON_CreateObject();

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *str1 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "meta", sub);
    cJSON_AddItemToObject(root, "rawdata", raw);
    cJSON *subflag = cJSON_CreateString("subvalue");
    cJSON_AddItemToObject(sub, "name", subflag);

    // step 3: Operate & Validate
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    int asize = cJSON_GetArraySize(got_items);
    cJSON *got_raw = cJSON_GetObjectItem(root, "rawdata");
    cJSON_bool israw = cJSON_IsRaw(got_raw);
    cJSON *dup_meta = cJSON_Duplicate(sub, 1);
    cJSON_AddItemToObject(root, "meta_copy", dup_meta);
    cJSON *asize_num = cJSON_CreateNumber((double)asize);
    cJSON *israw_num = cJSON_CreateNumber((double)israw);
    cJSON_AddItemToObject(root, "items_count", asize_num);
    cJSON_AddItemToObject(root, "raw_flag", israw_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}