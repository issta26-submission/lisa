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
//<ID> 976
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *sensor = cJSON_CreateObject();
    cJSON *readings = cJSON_CreateArray();
    cJSON *name = cJSON_CreateString("thermometer");
    cJSON *r1 = cJSON_CreateNumber(23.5);
    cJSON *r2 = cJSON_CreateNumber(24.0);

    // step 2: Configure
    cJSON_AddItemToArray(readings, r1);
    cJSON_AddItemToArray(readings, r2);
    cJSON_AddItemToObject(sensor, "name", name);
    cJSON_AddItemToObject(sensor, "readings", readings);
    cJSON_AddItemToObject(root, "sensor", sensor);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(readings, 0);
    double updated = cJSON_SetNumberHelper(first, 26.5);
    double second_val = cJSON_GetNumberValue(cJSON_GetArrayItem(readings, 1));
    double combined = updated + second_val;
    cJSON *sum_item = cJSON_CreateNumber(combined);
    cJSON_AddItemToArray(readings, sum_item);
    char *current_name = cJSON_GetStringValue(name);
    cJSON *alias = cJSON_CreateString(current_name);
    cJSON_AddItemToObject(sensor, "alias", alias);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}