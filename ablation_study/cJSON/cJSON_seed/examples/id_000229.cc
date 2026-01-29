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
//<ID> 229
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
    double *dynamic_value = (double *)cJSON_malloc(sizeof(double));
    *dynamic_value = 2.718281828;

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(*dynamic_value);
    cJSON_bool added_num = cJSON_AddItemToArray(arr, num);
    cJSON *str = cJSON_CreateString("example_text");
    cJSON_bool added_str = cJSON_AddItemToArray(arr, str);
    cJSON_bool added_array_to_root = cJSON_AddItemToObject(root, "items", arr);
    cJSON *flag_added_num = cJSON_CreateNumber((double)added_num);
    cJSON_AddItemToObject(root, "added_num_flag", flag_added_num);
    cJSON *flag_added_str = cJSON_CreateNumber((double)added_str);
    cJSON_AddItemToObject(root, "added_str_flag", flag_added_str);
    cJSON *flag_added_array = cJSON_CreateNumber((double)added_array_to_root);
    cJSON_AddItemToObject(root, "added_array_flag", flag_added_array);

    // step 3: Operate & Validate
    cJSON_bool has_items = cJSON_HasObjectItem(root, "items");
    cJSON *has_items_flag = cJSON_CreateNumber((double)has_items);
    cJSON_AddItemToObject(root, "has_items_flag", has_items_flag);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(dynamic_value);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}