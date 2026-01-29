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
//<ID> 1033
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"arr\": [\"alpha\", \"beta\"], \"obj\": {\"k\": \"alpha\"}, \"name\": \"root\"}";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *obj = (cJSON *)0;
    cJSON *child_k = (cJSON *)0;
    cJSON *dup_item0 = (cJSON *)0;
    char *printed_item0 = (char *)0;
    char *printed_root = (char *)0;
    const char *str0 = (const char *)0;
    int arr_size = 0;
    cJSON_bool cmp_equal = (cJSON_bool)0;
    cJSON_bool cmp_diff = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - parse initial JSON into root
    root = cJSON_Parse(json_text);

    // step 3: Configure - fetch object members and array items
    arr = cJSON_GetObjectItem(root, "arr");
    obj = cJSON_GetObjectItem(root, "obj");
    child_k = cJSON_GetObjectItem(obj, "k");
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);

    // step 4: Operate - inspect values, produce printable forms, duplicate and compare
    arr_size = cJSON_GetArraySize(arr);
    str0 = cJSON_GetStringValue(item0);
    printed_item0 = cJSON_PrintUnformatted(item0);
    printed_root = cJSON_PrintUnformatted(root);
    dup_item0 = cJSON_Duplicate(item0, (cJSON_bool)1);
    cmp_equal = cJSON_Compare(item0, dup_item0, (cJSON_bool)1);
    cmp_diff = cJSON_Compare(item1, child_k, (cJSON_bool)1);

    // step 5: Validate - derive a small validation score exercising the used APIs
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(item0 != (cJSON *)0) + (int)(item1 != (cJSON *)0) + (int)(arr_size == 2) + (int)(str0 != (const char *)0) + (int)(cmp_equal) + (int)(cmp_diff == 0);
    (void)validation_score;

    // step 6: Cleanup - free printed buffers, delete duplicates and root
    cJSON_free((void *)printed_item0);
    cJSON_free((void *)printed_root);
    cJSON_Delete(dup_item0);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}