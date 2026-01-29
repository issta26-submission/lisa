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
//<ID> 489
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(10.0);
    cJSON *num1 = cJSON_CreateNumber(25.5);
    cJSON_AddItemToArray(array, num0);
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToObject(root, "numbers", array);
    cJSON *label = cJSON_CreateString("initial_label");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON *second_item = cJSON_GetArrayItem(array, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    char buf[64];
    int buf_len = snprintf(buf, sizeof(buf), "second=%.2f", second_value);
    (void)buf_len;
    cJSON *computed_string = cJSON_CreateString(buf);
    cJSON_AddItemToObject(root, "computed", computed_string);

    // step 3: Operate
    cJSON *doubled = cJSON_CreateNumber(second_value * 2.0);
    cJSON_AddItemToArray(array, doubled);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Validate and Cleanup
    cJSON *third_item = cJSON_GetArrayItem(array, 2);
    double third_value = cJSON_GetNumberValue(third_item);
    (void)third_value;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}