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
//<ID> 138
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON *replacement_str = cJSON_CreateString("gamma");
    cJSON *num_initial = NULL;
    cJSON *num_replacement = NULL;
    cJSON *found_arr_item = NULL;
    cJSON *found_num_item = NULL;
    char *printed = NULL;
    int arr_size = 0;
    cJSON_bool replaced_array = 0;
    cJSON_bool replaced_number = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToObject(root, "items", arr);
    num_initial = cJSON_AddNumberToObject(root, "count", 0.0);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    found_arr_item = cJSON_GetArrayItem(arr, 1);
    replaced_array = cJSON_ReplaceItemViaPointer(arr, found_arr_item, replacement_str);
    num_replacement = cJSON_CreateNumber((double)arr_size);
    found_num_item = cJSON_GetObjectItem(root, "count");
    replaced_number = cJSON_ReplaceItemViaPointer(root, found_num_item, num_replacement);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}