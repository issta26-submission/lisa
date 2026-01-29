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
//<ID> 631
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(3.14159);
    cJSON *num2 = cJSON_CreateNumber(2.71828);
    cJSON *label = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddItemToObject(root, "label", label);
    cJSON *dup_root = cJSON_Duplicate(root, 1);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(dup_root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    cJSON *parsed_label = cJSON_GetObjectItem(parsed, "label");
    cJSON *first_item = cJSON_GetArrayItem(parsed_values, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON_DeleteItemFromObject(parsed, "label");
    char status[128];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;first=%.3f", version ? version : "null", first_val);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}