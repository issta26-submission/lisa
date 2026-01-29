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
//<ID> 118
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *nums = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "nums", nums);
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(nums, n1);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(nums, n2);
    cJSON *label = cJSON_CreateString("sample");
    cJSON_AddItemToObject(data, "label", label);

    // step 2: Configure
    cJSON *got_data = cJSON_GetObjectItem(root, "data");
    cJSON *got_nums = cJSON_GetObjectItem(got_data, "nums");
    cJSON *first = cJSON_GetArrayItem(got_nums, 0);
    cJSON *dup_nums = cJSON_Duplicate(got_nums, 1);
    cJSON_AddItemToObject(root, "nums_backup", dup_nums);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(got_nums, 1);
    double v1 = cJSON_GetNumberValue(first);
    double v2 = cJSON_GetNumberValue(second);
    cJSON_AddNumberToObject(root, "sum", v1 + v2);
    const char *lbl = cJSON_GetStringValue(label);
    cJSON_AddNumberToObject(root, "first_char_code", (double)(unsigned char)lbl[0]);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}