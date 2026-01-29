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
//<ID> 1237
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *nested = NULL;
    cJSON *arr = NULL;
    cJSON *orig_num = NULL;
    cJSON *value_num = NULL;
    cJSON *flag_bool = NULL;
    cJSON *retrieved = NULL;
    char *printed = NULL;
    double set_result = 0.0;
    int arr_size = 0;
    cJSON_bool add_ref_result = 0;
    cJSON_bool is_false_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    nested = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    value_num = cJSON_CreateNumber(1.0);
    flag_bool = cJSON_CreateFalse();
    orig_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(nested, "value", value_num);
    cJSON_AddItemToObject(nested, "flag", flag_bool);
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddItemToObject(root, "original", orig_num);
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "nested");
    set_result = cJSON_SetNumberHelper(cJSON_GetObjectItem(retrieved, "value"), 3.14159);
    is_false_result = cJSON_IsFalse(cJSON_GetObjectItem(retrieved, "flag"));
    add_ref_result = cJSON_AddItemReferenceToArray(arr, cJSON_GetObjectItem(root, "original"));
    arr_size = cJSON_GetArraySize(arr);
    printed = cJSON_PrintUnformatted(root);
    (void)set_result;
    (void)is_false_result;
    (void)add_ref_result;
    (void)arr_size;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}