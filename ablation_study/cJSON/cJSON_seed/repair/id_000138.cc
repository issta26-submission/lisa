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
//<ID> 138
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *label = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "label", label);
    cJSON *num0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, num0);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "scale", 2.5);
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON_AddNumberToObject(settings, "threshold", 5.0);
    char *updated_label = cJSON_SetValuestring(label, "updated");

    // step 3: Operate and Validate
    cJSON *ins_num = cJSON_CreateNumber(7.0);
    cJSON_InsertItemInArray(arr, 0, ins_num);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *scale_item = cJSON_GetObjectItem(root, "scale");
    double scale_val = cJSON_GetNumberValue(scale_item);
    double scaled = first_val * scale_val;
    cJSON_AddNumberToObject(root, "scaled_first", scaled);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    (void)updated_label;
    return 66;
    // API sequence test completed successfully
}