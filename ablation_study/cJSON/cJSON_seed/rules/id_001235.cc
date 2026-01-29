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
//<ID> 1235
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *arr = NULL;
    cJSON *num_item = NULL;
    cJSON *bool_item = NULL;
    cJSON *str_item = NULL;
    cJSON *retrieved = NULL;
    cJSON *first_arr_item = NULL;
    cJSON *val_obj = NULL;
    cJSON *val_item = NULL;
    char *printed = NULL;
    double set_result = 0.0;
    double numeric_value = 0.0;
    cJSON_bool is_false = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    data = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(0.0);
    bool_item = cJSON_CreateFalse();
    str_item = cJSON_CreateString("world");
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddItemToObject(data, "value", num_item);
    arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, bool_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemReferenceToArray(arr, num_item);
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Operate / Validate
    set_result = cJSON_SetNumberHelper(num_item, 42.5);
    retrieved = cJSON_GetObjectItem(root, "list");
    first_arr_item = cJSON_GetArrayItem(retrieved, 0);
    is_false = cJSON_IsFalse(first_arr_item);
    val_obj = cJSON_GetObjectItem(root, "data");
    val_item = cJSON_GetObjectItem(val_obj, "value");
    numeric_value = cJSON_GetNumberValue(val_item);
    printed = cJSON_PrintUnformatted(root);
    (void)set_result;
    (void)is_false;
    (void)numeric_value;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}