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
//<ID> 481
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
    cJSON *num1 = cJSON_CreateNumber(10.5);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    cJSON *num3 = cJSON_CreateNumber(30.25);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, num3);
    cJSON *label = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddItemToObject(root, "arr", arr);

    // step 2: Configure
    cJSON *picked = cJSON_GetArrayItem(arr, 1);
    double picked_val = cJSON_GetNumberValue(picked);
    cJSON *computed_num = cJSON_CreateNumber(picked_val * 2.0);
    cJSON_AddItemToObject(root, "computed_num", computed_num);

    // step 3: Operate
    char buf[64];
    int n = snprintf(buf, sizeof(buf), "double_of_index1=%.2f", cJSON_GetNumberValue(cJSON_GetObjectItem(root, "computed_num")));
    (void)n;
    cJSON *computed_str = cJSON_CreateString(buf);
    cJSON_AddItemToObject(root, "computed_str", computed_str);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    cJSON *validate_num = cJSON_GetObjectItem(root, "computed_num");
    double final_val = cJSON_GetNumberValue(validate_num);
    (void)final_val;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}