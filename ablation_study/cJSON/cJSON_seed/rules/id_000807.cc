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
//<ID> 807
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *raw_json = "{\"meta\":true}";
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num = NULL;
    cJSON *meta_raw = NULL;
    cJSON *fetched = NULL;
    cJSON *new_root = NULL;
    cJSON *new_arr = NULL;
    cJSON *parsed_item = NULL;
    cJSON *size_num = NULL;
    char *printed = NULL;
    int arr_size = 0;
    cJSON_bool are_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(1.0); cJSON_AddItemToArray(arr, num);
    num = cJSON_CreateNumber(2.0); cJSON_AddItemToArray(arr, num);
    num = cJSON_CreateNumber(3.0); cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToObject(root, "numbers", arr);
    meta_raw = cJSON_CreateRaw(raw_json);
    cJSON_AddItemToObject(root, "meta_raw", meta_raw);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    fetched = cJSON_GetArrayItem(arr, 1);
    printed = cJSON_Print(root);
    new_root = cJSON_Parse(printed);
    new_arr = cJSON_GetObjectItem(new_root, "numbers");
    parsed_item = cJSON_GetArrayItem(new_arr, 1);
    are_equal = cJSON_Compare(fetched, parsed_item, 1);
    size_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "arr_size", size_num);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(new_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}