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
//<ID> 306
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
    cJSON *raw = cJSON_CreateRaw("{\"raw_key\":123}");
    cJSON *label = cJSON_CreateString("example");
    cJSON *number = cJSON_CreateNumber(42.0);
    cJSON *first = cJSON_CreateString("first");
    cJSON *second = cJSON_CreateNull();
    cJSON *third = cJSON_CreateString("third");

    // step 2: Configure
    cJSON_AddItemToArray(arr, first);
    cJSON_AddItemToArray(arr, second);
    cJSON_AddItemToArray(arr, third);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "raw", raw);
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddItemToObject(root, "answer", number);

    // step 3: Operate & Validate
    cJSON_bool has_data = cJSON_HasObjectItem(root, "data");
    cJSON *has_data_item = cJSON_CreateBool(has_data);
    cJSON_AddItemToObject(root, "has_data", has_data_item);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_null = cJSON_IsNull(second_item);
    cJSON *second_null_flag = cJSON_CreateBool(second_is_null);
    cJSON_AddItemToObject(root, "second_is_null", second_null_flag);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, (cJSON_bool)1);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_AddItemToObject(root, "printed", cJSON_CreateString(printed));
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}