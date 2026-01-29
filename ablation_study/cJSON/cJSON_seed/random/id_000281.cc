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
//<ID> 281
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
    cJSON *item_ref = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *sum_item = (cJSON *)0;
    cJSON *parsed = (cJSON *)0;
    cJSON *parsed_flag = (cJSON *)0;
    cJSON_bool add_ref_ok = (cJSON_bool)0;
    cJSON_bool add_num_ok = (cJSON_bool)0;
    cJSON_bool add_arr_ok = (cJSON_bool)0;
    cJSON_bool add_sum_ok = (cJSON_bool)0;
    cJSON_bool is_false_created = (cJSON_bool)0;
    cJSON_bool is_false_parsed = (cJSON_bool)0;
    int arr_size = 0;
    double num_val = 0.0;
    double sum = 0.0;
    char json_buf[] = "  { \"key\" : false, \"arr\" : [ false ] }  ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    item_ref = cJSON_CreateFalse();
    num_item = cJSON_CreateNumber(3.5);
    arr = cJSON_CreateArray();

    // step 3: Configure
    add_num_ok = cJSON_AddItemToObject(root, "value", num_item);
    add_arr_ok = cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "flag", item_ref);
    add_ref_ok = cJSON_AddItemReferenceToArray(arr, item_ref);

    // step 4: Operate
    cJSON_Minify(json_buf);
    parsed = cJSON_Parse(json_buf);
    parsed_flag = cJSON_GetObjectItem(parsed, "key");
    is_false_created = cJSON_IsFalse(item_ref);
    is_false_parsed = cJSON_IsFalse(parsed_flag);

    // step 5: Validate
    arr_size = cJSON_GetArraySize(arr);
    num_val = cJSON_GetNumberValue(num_item);
    sum = num_val + (double)arr_size;
    sum_item = cJSON_CreateNumber(sum);
    add_sum_ok = cJSON_AddItemToObject(root, "sum", sum_item);

    // step 6: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}