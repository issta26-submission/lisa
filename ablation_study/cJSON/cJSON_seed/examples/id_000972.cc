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
//<ID> 972
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *readings = cJSON_CreateArray();
    cJSON *name_item = cJSON_CreateString("sensor-A");
    cJSON *value_item = cJSON_CreateNumber(12.5);
    cJSON *unit_item = cJSON_CreateString("Celsius");

    // step 2: Configure
    cJSON_AddItemToArray(readings, name_item);
    cJSON_AddItemToArray(readings, value_item);
    cJSON_AddItemToArray(readings, unit_item);
    cJSON_AddItemToObject(root, "readings", readings);
    cJSON_AddItemToObject(root, "meta", cJSON_CreateObject());
    cJSON_AddItemToObject(cJSON_GetObjectItem(root, "meta"), "source", cJSON_CreateString("probe-7"));

    // step 3: Operate & Validate
    char *extracted_name = cJSON_GetStringValue(cJSON_GetArrayItem(readings, 0));
    cJSON *alias = cJSON_CreateString(extracted_name);
    cJSON_AddItemToObject(root, "alias", alias);
    cJSON_SetNumberHelper(value_item, 99.9);
    double updated_value = cJSON_GetNumberValue(value_item);
    cJSON_AddItemToObject(root, "adjusted_value", cJSON_CreateNumber(updated_value));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}