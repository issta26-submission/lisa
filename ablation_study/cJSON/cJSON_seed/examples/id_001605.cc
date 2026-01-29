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
//<ID> 1605
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data_arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data_arr);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(data_arr, n1);
    cJSON_AddItemToArray(data_arr, n2);
    cJSON_AddItemToArray(data_arr, n3);
    cJSON *raw_entry = cJSON_CreateRaw("{\"rawkey\":true}");
    cJSON_AddItemToObject(root, "raw", raw_entry);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "enabled", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *has_data_flag = cJSON_CreateNumber((double)cJSON_HasObjectItem(root, "data"));
    cJSON_AddItemToObject(root, "has_data", has_data_flag);
    cJSON *root_is_obj_flag = cJSON_CreateNumber((double)cJSON_IsObject(root));
    cJSON_AddItemToObject(root, "root_is_object", root_is_obj_flag);

    // step 3: Operate & Validate
    int arr_size = cJSON_GetArraySize(data_arr);
    cJSON *size_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "data_size", size_num);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffered = cJSON_PrintBuffered(root, 256, 1);
    cJSON_free(printed);
    cJSON_free(buffered);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}