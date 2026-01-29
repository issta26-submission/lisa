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
//<ID> 108
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *s3 = cJSON_CreateString("three");
    cJSON *meta = cJSON_CreateObject();
    cJSON *count_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *detached_count = (cJSON *)0;
    cJSON *got_arr = (cJSON *)0;
    int arr_size = 0;
    int detached_size = 0;
    double count_value = 0.0;
    cJSON_bool replace_result = 0;

    // step 2: Setup - build array and attach items to root
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, s3);
    cJSON_AddItemToObject(root, "letters", arr);
    cJSON_AddStringToObject(meta, "info", "data");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate - measure array, record count, replace meta, detach array
    arr_size = cJSON_GetArraySize(arr);
    count_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "count", count_item);
    got_arr = cJSON_GetObjectItem(root, "letters");
    replace_result = cJSON_ReplaceItemInObject(root, "meta", cJSON_CreateString("new-meta"));
    cJSON_AddBoolToObject(root, "meta_replaced", replace_result);
    detached = cJSON_DetachItemFromObject(root, "letters");
    detached_size = cJSON_GetArraySize(detached);
    detached_count = cJSON_CreateNumber((double)detached_size);
    cJSON_AddItemToObject(root, "detached_count", detached_count);

    // step 4: Validate - read back stored count value
    count_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    cJSON_AddNumberToObject(root, "validated_count", count_value);

    // step 5: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}