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
//<ID> 1058
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
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *bfalse = cJSON_CreateFalse();
    cJSON_bool rc1 = cJSON_AddItemToObject(root, "values", arr);
    cJSON_bool rc2 = cJSON_AddItemToObject(root, "flag", bfalse);
    cJSON *maybe = cJSON_AddNullToObject(root, "maybe");
    cJSON *label = cJSON_AddStringToObject(root, "label", "sample");

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(arr);
    cJSON *countnum = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "count", countnum);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *dup_arr = cJSON_GetObjectItem(dup, "values");
    int dup_count = cJSON_GetArraySize(dup_arr);
    cJSON *dup_countnum = cJSON_CreateNumber((double)dup_count);
    cJSON_AddItemToObject(dup, "count_copy", dup_countnum);
    char *out = cJSON_PrintUnformatted(dup);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}