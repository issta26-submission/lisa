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
//<ID> 1456
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *b_true = cJSON_CreateBool(1);
    cJSON *b_false = cJSON_CreateBool(0);
    cJSON *str_hello = cJSON_CreateString("hello");
    cJSON *str_world = cJSON_CreateString("world");

    // step 2: Configure
    cJSON_AddItemToArray(arr, b_true);
    cJSON_AddItemToArray(arr, b_false);
    cJSON_AddItemToArray(arr, str_hello);
    cJSON_AddItemToArray(arr, str_world);

    // step 3: Operate & Validate
    char *flat = cJSON_PrintUnformatted(root);
    cJSON *flat_repr = cJSON_CreateString(flat);
    cJSON_AddItemToObject(root, "json_unformatted", flat_repr);
    cJSON *str_hello_copy = cJSON_CreateString("hello");
    cJSON_bool same_compare = cJSON_Compare(str_hello, str_hello_copy, 1);
    cJSON_bool diff_compare = cJSON_Compare(str_hello, str_world, 1);
    cJSON *compare_result = cJSON_CreateBool(same_compare);
    cJSON_AddItemToArray(arr, compare_result);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_Delete(str_hello_copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}