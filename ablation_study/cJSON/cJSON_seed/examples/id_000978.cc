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
//<ID> 978
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *devices = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON *str1 = cJSON_CreateString("sensor-A");
    cJSON *str2 = cJSON_CreateString("sensor-B");

    // step 2: Configure
    cJSON_AddItemToArray(devices, str1);
    cJSON_AddItemToArray(devices, str2);
    cJSON_AddItemToArray(devices, num1);
    cJSON_AddItemToArray(devices, num2);
    cJSON_AddItemToObject(root, "devices", devices);

    // step 3: Operate & Validate
    char *primary_name = cJSON_GetStringValue(str1);
    cJSON *primary_alias = cJSON_CreateString(primary_name);
    cJSON_AddItemToObject(root, "primary_alias", primary_alias);
    double updated_num1 = cJSON_SetNumberHelper(num1, 6.28);
    double sum = updated_num1 + cJSON_GetNumberValue(num2);
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON_AddItemToArray(devices, cJSON_CreateString("sensor-C"));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}