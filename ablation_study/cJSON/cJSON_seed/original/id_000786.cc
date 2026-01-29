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
//<ID> 786
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = cJSON_CreateNumber(3.1415);
    const char raw_text[] = "{\"embedded\":1}";
    cJSON *raw_item = cJSON_CreateRaw(raw_text);

    // step 2: Configure
    cJSON_AddItemToObject(root, "flag", true_item);
    cJSON_AddItemToObject(root, "pi", num_item);
    cJSON_AddItemToObject(root, "raw_blob", raw_item);
    cJSON *array = cJSON_AddArrayToObject(root, "arr");
    cJSON *arr_num = cJSON_CreateNumber(10.0);
    cJSON *arr_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(array, arr_num);
    cJSON_AddItemToArray(array, arr_true);

    // step 3: Operate
    int arr_size = cJSON_GetArraySize(array);
    cJSON *size_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "arr_size", size_item);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}