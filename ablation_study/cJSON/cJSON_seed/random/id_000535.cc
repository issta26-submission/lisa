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
//<ID> 535
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
    cJSON *raw_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_raw = (cJSON *)0;
    const double numbers[] = {1.1, 2.2, 3.3};
    char *label_value = (char *)0;
    cJSON_bool add_ok_array = 0;
    cJSON_bool add_ok_object = 0;
    cJSON_bool is_raw = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateDoubleArray(numbers, 3);
    raw_item = cJSON_CreateRaw("rawcontent");
    str_item = cJSON_CreateString("example_label");

    // step 3: Configure
    add_ok_array = cJSON_AddItemToArray(arr, raw_item);
    add_ok_object = cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "label", str_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "data");
    retrieved_raw = cJSON_GetArrayItem(retrieved_arr, 3);
    is_raw = cJSON_IsRaw(retrieved_raw);
    label_value = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));

    // step 5: Validate
    (void)add_ok_array;
    (void)add_ok_object;
    (void)is_raw;
    (void)label_value;
    (void)retrieved_raw;
    (void)retrieved_arr;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}