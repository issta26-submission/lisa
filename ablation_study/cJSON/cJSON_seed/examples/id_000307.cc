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
//<ID> 307
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
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON *null_item = cJSON_CreateNull();
    cJSON *raw_item = cJSON_CreateRaw("raw_value");
    char *dynamic = (char *)cJSON_malloc(4);
    dynamic[0] = 'h';
    dynamic[1] = 'i';
    dynamic[2] = '\0';
    cJSON *str_item = cJSON_CreateString(dynamic);

    // step 2: Configure
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, null_item);
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "note", str_item);

    // step 3: Operate & Validate
    cJSON *got_null = cJSON_GetArrayItem(arr, 1);
    cJSON_bool was_null = cJSON_IsNull(got_null);
    cJSON *was_null_flag = cJSON_CreateBool(was_null);
    cJSON_AddItemToObject(root, "null_was", was_null_flag);
    cJSON_bool has_data = cJSON_HasObjectItem(root, "data");
    cJSON *has_data_flag = cJSON_CreateBool(has_data);
    cJSON_AddItemToObject(root, "has_data", has_data_flag);
    char *printed = cJSON_PrintUnformatted(root);
    char scratch[128];
    memset(scratch, 0, sizeof(scratch));
    cJSON_AddItemToObject(root, "printed", cJSON_CreateString(printed));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(dynamic);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}