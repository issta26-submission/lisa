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
//<ID> 1748
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *retrieved_items = (cJSON *)0;
    cJSON *retrieved_first = (cJSON *)0;
    cJSON *retrieved_meta = (cJSON *)0;
    char *meta_str = (char *)0;
    int arr_size = 0;
    int validation = 0;
    cJSON_bool first_is_number = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(123.0);
    str_item = cJSON_CreateString("element-string");

    // step 3: Configure
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "meta", cJSON_CreateString("meta-info"));

    // step 4: Operate
    retrieved_items = cJSON_GetObjectItem(root, "items");
    retrieved_first = cJSON_GetArrayItem(retrieved_items, 0);
    first_is_number = cJSON_IsNumber(retrieved_first);
    retrieved_meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    meta_str = cJSON_GetStringValue(retrieved_meta);

    // step 5: Validate
    arr_size = cJSON_GetArraySize(retrieved_items);
    validation = (int)(first_is_number != 0) + (int)(arr_size == 2) + (int)(meta_str != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)num_item;
    (void)str_item;
    (void)retrieved_items;
    (void)retrieved_first;
    (void)retrieved_meta;
    (void)meta_str;
    (void)arr_size;
    (void)first_is_number;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}