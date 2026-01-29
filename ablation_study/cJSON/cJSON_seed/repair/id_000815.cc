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
//<ID> 815
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *threshold = cJSON_CreateNumber(7.5);
    cJSON_AddItemToObject(settings, "threshold", threshold);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);

    // step 2: Configure
    cJSON *inner = cJSON_AddObjectToObject(settings, "inner");
    cJSON_AddNumberToObject(inner, "scale", 2.0);
    char *snapshot_before = cJSON_PrintUnformatted(root);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(numbers);
    cJSON *threshold_item = cJSON_GetObjectItemCaseSensitive(settings, "threshold");
    double threshold_val = cJSON_GetNumberValue(threshold_item);
    cJSON *first_num = cJSON_GetArrayItem(numbers, 0);
    double first_val = cJSON_GetNumberValue(first_num);
    cJSON *detached_settings = cJSON_DetachItemFromObject(root, "settings");
    cJSON_AddItemToObject(root, "settings_moved", detached_settings);
    cJSON *moved = cJSON_GetObjectItemCaseSensitive(root, "settings_moved");
    cJSON *moved_threshold_item = cJSON_GetObjectItemCaseSensitive(moved, "threshold");
    double moved_threshold_val = cJSON_GetNumberValue(moved_threshold_item);
    char *snapshot_after = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot_before);
    cJSON_free(snapshot_after);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}