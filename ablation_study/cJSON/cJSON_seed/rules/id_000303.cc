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
//<ID> 303
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *btrue = cJSON_CreateTrue();
    cJSON *bfalse = cJSON_CreateFalse();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *str = cJSON_CreateString("value");
    char *printed = NULL;
    cJSON *retrieved_nested = NULL;
    cJSON *flag_item = NULL;
    cJSON_bool flag_is_bool = 0;
    cJSON *str_item = NULL;
    char *text_value = NULL;
    int arr_size = 0;
    cJSON *first = NULL;
    double first_elem = 0.0;
    double set_result_rootnum = 0.0;
    double set_result_first = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "config", nested);
    cJSON_AddItemToObject(nested, "flag", btrue);
    cJSON_AddItemToObject(root, "disabled", bfalse);
    cJSON_AddItemToObject(root, "answer", num);
    cJSON_AddItemToObject(root, "text", str);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));

    // step 3: Operate / Validate
    set_result_rootnum = cJSON_SetNumberHelper(num, 84.0);
    retrieved_nested = cJSON_GetObjectItem(root, "config");
    flag_item = cJSON_GetObjectItem(retrieved_nested, "flag");
    flag_is_bool = cJSON_IsBool(flag_item);
    str_item = cJSON_GetObjectItem(root, "text");
    text_value = cJSON_GetStringValue(str_item);
    printed = cJSON_Print(root);
    cJSON_Minify(printed);
    arr_size = cJSON_GetArraySize(arr);
    first = cJSON_GetArrayItem(arr, 0);
    first_elem = cJSON_GetNumberValue(first);
    set_result_first = cJSON_SetNumberHelper(first, first_elem + 0.5);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)set_result_rootnum;
    (void)flag_is_bool;
    (void)text_value;
    (void)arr_size;
    (void)set_result_first;
    return 66;
}