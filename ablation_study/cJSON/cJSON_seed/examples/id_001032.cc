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
//<ID> 1032
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *objA = cJSON_CreateObject();
    cJSON *nameA = cJSON_CreateString("sensor");
    cJSON *numA = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(objA, "name", nameA);
    cJSON_AddItemToObject(objA, "value", numA);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(objA, "active", flag);
    cJSON_AddItemToObject(root, "deviceA", objA);
    cJSON *objB = cJSON_CreateObject();
    cJSON_AddItemToObject(objB, "name", cJSON_CreateString("sensor"));
    cJSON_AddItemToObject(objB, "value", cJSON_CreateNumber(42.0));
    cJSON_AddItemToObject(objB, "active", cJSON_CreateTrue());

    // step 2: Configure
    cJSON_bool equal_flag = cJSON_Compare(objA, objB, 1);
    cJSON_bool active_flag = cJSON_IsTrue(flag);
    cJSON_AddNumberToObject(root, "objects_equal", (double)equal_flag);
    cJSON_AddNumberToObject(root, "active_flag", (double)active_flag);

    // step 3: Operate & Validate
    cJSON *deviceA = cJSON_GetObjectItem(root, "deviceA");
    cJSON *device_name = cJSON_GetObjectItem(deviceA, "name");
    const char *name_str = cJSON_GetStringValue(device_name);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON *value_item = cJSON_GetObjectItem(deviceA, "value");
    double value = cJSON_GetNumberValue(value_item);
    cJSON *value_inc = cJSON_CreateNumber(value + 1.0);
    cJSON_AddItemToObject(root, "value_incremented", value_inc);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(objB);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}