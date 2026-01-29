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
//<ID> 971
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
    cJSON *r1 = cJSON_CreateNumber(10.0);
    cJSON *r2 = cJSON_CreateNumber(20.5);
    cJSON *name = cJSON_CreateString("alpha");

    // step 2: Configure
    cJSON_AddItemToArray(readings, r1);
    cJSON_AddItemToArray(readings, r2);
    cJSON_AddItemToObject(sensor, "readings", readings);
    cJSON_AddItemToObject(sensor, "name", name);
    cJSON_AddItemToObject(root, "sensor", sensor);

    // step 3: Operate & Validate
    cJSON *got_sensor = cJSON_GetObjectItem(root, "sensor");
    cJSON *got_readings = cJSON_GetObjectItem(got_sensor, "readings");
    cJSON *first = cJSON_GetArrayItem(got_readings, 0);
    cJSON_SetNumberHelper(first, 15.25);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *reported_first = cJSON_CreateNumber(first_val);
    cJSON_AddItemToObject(root, "reported_first", reported_first);
    const char *name_val = cJSON_GetStringValue(cJSON_GetObjectItem(got_sensor, "name"));
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}