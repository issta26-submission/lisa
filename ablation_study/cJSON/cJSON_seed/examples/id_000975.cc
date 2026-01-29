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
//<ID> 975
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *name = cJSON_CreateString("sensor-A");
    cJSON *num1 = cJSON_CreateNumber(5.0);
    cJSON *num2 = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);
    cJSON_AddItemToObject(info, "name", name);
    cJSON_AddItemToObject(info, "values", values);
    cJSON_AddItemToObject(root, "info", info);

    // step 3: Operate & Validate
    const char *current_name = cJSON_GetStringValue(name);
    cJSON *alias = cJSON_CreateString(current_name);
    cJSON_AddItemToObject(info, "alias", alias);
    double v1 = cJSON_GetNumberValue(num1);
    double set_result = cJSON_SetNumberHelper(num2, v1 * 2.0);
    double v2 = cJSON_GetNumberValue(num2);
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddItemToObject(summary, "original", cJSON_CreateNumber(v1));
    cJSON_AddItemToObject(summary, "doubled", cJSON_CreateNumber(v2));
    cJSON_AddItemToObject(root, "summary", summary);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}