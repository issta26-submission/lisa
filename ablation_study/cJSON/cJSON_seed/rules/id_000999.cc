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
//<ID> 999
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *values = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *first = NULL;
    char *printed = NULL;
    double first_val = 0.0;
    cJSON_bool is_array = 0;
    int prebuf = 256;
    const char *device_name = "sensorA";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddItemToObject(root, "device", cJSON_CreateString(device_name));
    cJSON_AddItemToObject(root, "id", cJSON_CreateNumber(101.0));
    num1 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(values, num1);
    num2 = cJSON_CreateNumber(20.75);
    cJSON_InsertItemInArray(values, 1, num2);

    // step 3: Operate / Validate
    is_array = cJSON_IsArray(values);
    printed = cJSON_PrintBuffered(root, prebuf, is_array);
    first = cJSON_GetArrayItem(values, 0);
    first_val = cJSON_GetNumberValue(first);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}