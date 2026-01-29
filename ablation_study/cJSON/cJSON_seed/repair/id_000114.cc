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
//<ID> 114
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
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(nums, n1);
    cJSON_AddItemToArray(nums, n2);
    cJSON_AddItemToArray(nums, n3);
    cJSON *label = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(data, "label", label);

    // step 2: Configure
    cJSON *dup_data = cJSON_Duplicate(data, 1);
    cJSON_AddItemToObject(root, "backup", dup_data);
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 3: Operate and Validate
    cJSON *got_data = cJSON_GetObjectItem(root, "data");
    cJSON *got_nums = cJSON_GetObjectItem(got_data, "nums");
    cJSON *item0 = cJSON_GetArrayItem(got_nums, 0);
    cJSON *item1 = cJSON_GetArrayItem(got_nums, 1);
    cJSON *item2 = cJSON_GetArrayItem(got_nums, 2);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double v2 = cJSON_GetNumberValue(item2);
    double total = v0 + v1 + v2;
    cJSON_AddNumberToObject(root, "sum", total);
    cJSON *dup_label = cJSON_GetObjectItem(dup_data, "label");
    const char *lbl = cJSON_GetStringValue(dup_label);
    double first_char_code = (double)(unsigned char)lbl[0];
    cJSON_AddNumberToObject(root, "first_label_code", first_char_code);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}