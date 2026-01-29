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
//<ID> 1224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *str_item = cJSON_CreateString("greeting");
    cJSON_AddItemToArray(list, str_item);
    cJSON *num1 = cJSON_CreateNumber(12.34);
    cJSON *num2 = cJSON_CreateNumber(3.66);
    cJSON_AddItemToArray(list, num1);
    cJSON_AddItemToArray(list, num2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *base = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(meta, "base", base);

    // step 2: Configure
    cJSON_bool is_string_flag = cJSON_IsString(str_item);
    double v1 = cJSON_GetNumberValue(num1);
    double v2 = cJSON_GetNumberValue(num2);
    cJSON *dup_list = cJSON_Duplicate(list, 1);
    cJSON_AddItemToObject(root, "dup_list", dup_list);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    double base_val = cJSON_GetNumberValue(base);
    double accumulated = v1 + v2 + base_val + (double)is_string_flag + (double)(unsigned char)((char *)scratch)[0];
    cJSON *final_num = cJSON_CreateNumber(accumulated);
    cJSON_AddItemToObject(root, "final", final_num);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}