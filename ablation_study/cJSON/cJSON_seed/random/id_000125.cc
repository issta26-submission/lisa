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
//<ID> 125
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_str = (cJSON *)0;
    cJSON *desc_str = (cJSON *)0;
    cJSON *float_arr = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    double n0 = 0.0;
    double n1 = 0.0;
    double n2 = 0.0;
    double avg = 0.0;
    char *json_text = (char *)0;
    float nums[3] = { 21.5f, 22.0f, 23.25f };

    // step 2: Initialize
    root = cJSON_CreateObject();
    name_str = cJSON_CreateString("sensorA");
    desc_str = cJSON_CreateString("temperature readings");
    float_arr = cJSON_CreateFloatArray(nums, 3);

    // step 3: Configure
    cJSON_AddItemToObject(root, "readings", float_arr);
    cJSON_AddItemToObject(root, "name", name_str);
    cJSON_AddItemToObject(root, "description", desc_str);
    cJSON_AddStringToObject(root, "unit", "Celsius");
    cJSON_AddNumberToObject(root, "placeholder_count", 0.0);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "readings");
    item0 = cJSON_GetArrayItem(retrieved_arr, 0);
    item1 = cJSON_GetArrayItem(retrieved_arr, 1);
    item2 = cJSON_GetArrayItem(retrieved_arr, 2);
    n0 = cJSON_GetNumberValue(item0);
    n1 = cJSON_GetNumberValue(item1);
    n2 = cJSON_GetNumberValue(item2);
    avg = (n0 + n1 + n2) / 3.0;
    cJSON_AddNumberToObject(root, "average", avg);

    // step 5: Validate
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}