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
//<ID> 122
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    const float readings[3] = {1.5f, 2.5f, 3.0f};
    cJSON *name_node = cJSON_CreateString("sensor-A");
    cJSON *desc_node = cJSON_CreateString("temperature sensor");
    cJSON *float_array = (cJSON *)0;
    cJSON *avg_node = (cJSON *)0;
    cJSON *status_node = (cJSON *)0;
    const char *status_text = (const char *)0;
    double average_value = 0.0;
    char *json_text = (char *)0;

    // step 2: Setup - attach strings and array and computed number to root
    cJSON_AddItemToObject(root, "name", name_node);
    cJSON_AddItemToObject(root, "description", desc_node);
    cJSON_AddStringToObject(root, "status", "ok");
    float_array = cJSON_CreateFloatArray(readings, 3);
    cJSON_AddItemToObject(root, "readings", float_array);
    cJSON_AddNumberToObject(root, "average", (double)((readings[0] + readings[1] + readings[2]) / 3.0f));

    // step 3: Operate - retrieve computed fields
    avg_node = cJSON_GetObjectItem(root, "average");
    average_value = cJSON_GetNumberValue(avg_node);
    status_node = cJSON_GetObjectItem(root, "status");
    status_text = cJSON_GetStringValue(status_node);

    // step 4: Validate - produce JSON text and minify then free it
    json_text = cJSON_Print(root);
    cJSON_Minify(json_text);
    cJSON_free(json_text);

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}