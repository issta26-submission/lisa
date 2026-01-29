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
//<ID> 673
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
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_num0 = (cJSON *)0;
    cJSON *retrieved_label = (cJSON *)0;
    char *printed = (char *)0;
    char *label_str = (char *)0;
    double num_val = 0.0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added_arr_obj = 0;
    cJSON_bool added_label_obj = 0;
    int nonnull_count = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(42.0);
    num1 = cJSON_CreateNumber(3.14);
    label = cJSON_CreateString("example");

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, num0);
    added1 = cJSON_AddItemToArray(arr, num1);
    added_arr_obj = cJSON_AddItemToObject(root, "items", arr);
    added_label_obj = cJSON_AddItemToObject(root, "label", label);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    retrieved_num0 = cJSON_GetArrayItem(retrieved_arr, 0);
    num_val = cJSON_GetNumberValue(retrieved_num0);
    retrieved_label = cJSON_GetObjectItem(root, "label");
    label_str = cJSON_GetStringValue(retrieved_label);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    nonnull_count = (printed != (char *)0) + (label_str != (char *)0) + (num_val != 0.0);
    (void)added0;
    (void)added1;
    (void)added_arr_obj;
    (void)added_label_obj;
    (void)retrieved_arr;
    (void)retrieved_num0;
    (void)retrieved_label;
    (void)num_val;
    (void)label_str;
    (void)nonnull_count;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}