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
//<ID> 1551
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(30.0));
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddNumberToObject(root, "scale", 2.5);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *printed = cJSON_Print(root);
    char *printed_dup = cJSON_Print(dup);
    cJSON *values_ref = cJSON_GetObjectItem(root, "values");

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(values_ref);
    cJSON *second_item = cJSON_GetArrayItem(values_ref, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON *scale_item = cJSON_GetObjectItem(root, "scale");
    double scale_value = cJSON_GetNumberValue(scale_item);
    double scaled_value = second_value * scale_value;
    cJSON_AddItemToObject(root, "scaled_second", cJSON_CreateNumber(scaled_value));
    (void)count; (void)printed_dup; (void)printed; (void)scale_item;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(printed_dup);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}