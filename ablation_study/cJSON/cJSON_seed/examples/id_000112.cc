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
//<ID> 112
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
    cJSON_AddItemToObject(root, "arr", arr);

    // step 2: Configure
    size_t buf_size = 64;
    char *json_buf = (char *)cJSON_malloc(buf_size);
    memset(json_buf, 0, buf_size);
    strcpy(json_buf, "  [ null , { \"k\" : \"v\" } ]  ");
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *first_parsed_item = cJSON_GetArrayItem(parsed, 0);
    cJSON_bool first_was_null = cJSON_IsNull(first_parsed_item);
    cJSON *raw_item = cJSON_CreateRaw("\"raw_string\"");
    cJSON_AddItemToArray(arr, raw_item);
    cJSON *null_flag_num = cJSON_CreateNumber((double)first_was_null);
    cJSON_AddItemToObject(root, "first_was_null", null_flag_num);

    // step 3: Operate & Validate
    cJSON *arr_first = cJSON_GetArrayItem(arr, 0);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(json_buf);
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}