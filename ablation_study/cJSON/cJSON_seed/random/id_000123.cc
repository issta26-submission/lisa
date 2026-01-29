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
//<ID> 123
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *desc = cJSON_CreateString("A float array of sensor readings");
    cJSON *values = (cJSON *)0;
    cJSON *old_count = (cJSON *)0;
    const float readings[3] = { 12.5f, 13.75f, 11.0f };
    int array_size = 0;
    double validated_count = 0.0;
    char *json_text = (char *)0;

    // step 2: Setup - attach descriptive string, category string, and float array to root; add initial count
    cJSON_AddItemToObject(root, "description", desc);
    cJSON_AddStringToObject(root, "category", "sensors");
    values = cJSON_CreateFloatArray(readings, 3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddNumberToObject(root, "count", 0.0);

    // step 3: Operate - measure array size, replace the placeholder count with the real size
    array_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "values"));
    old_count = cJSON_DetachItemFromObject(root, "count");
    cJSON_Delete(old_count);
    cJSON_AddNumberToObject(root, "count", (double)array_size);

    // step 4: Validate - read back count and produce JSON text then free it
    validated_count = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    json_text = cJSON_Print(root);
    cJSON_free(json_text);

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}