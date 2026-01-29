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
//<ID> 1554
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
    cJSON_AddItemToObject(root, "values", values);
    cJSON *num1 = cJSON_CreateNumber(1.5);
    cJSON *num2 = cJSON_CreateNumber(2.25);
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *printed_root = cJSON_Print(root);
    char *printed_dup = cJSON_Print(dup);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(values);
    cJSON *first_item = cJSON_GetArrayItem(values, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON_AddNumberToObject(dup, "first_value", first_value);
    (void)printed_root; (void)printed_dup; (void)first_value; (void)count;

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_dup);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}