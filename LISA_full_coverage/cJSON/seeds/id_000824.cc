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
//<ID> 824
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(3.1415);
    cJSON *str_first = cJSON_CreateString("first");
    cJSON *str_second = cJSON_CreateString("second");
    cJSON *bool_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddStringToObject(config, "version", cJSON_Version());
    cJSON_AddNumberToObject(config, "threshold", 42.0);
    cJSON_AddItemToArray(items, num_item);
    cJSON_InsertItemInArray(items, 0, str_first);
    cJSON_InsertItemInArray(items, 1, str_second);
    cJSON_AddItemToArray(items, bool_item);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_DeleteItemFromObjectCaseSensitive(config, "version");
    char *compact_after_delete = cJSON_PrintUnformatted(root);
    cJSON_free(compact_after_delete);

    // step 4: Validate & Cleanup
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    int count = cJSON_GetArraySize(got_items);
    cJSON *second_elem = cJSON_GetArrayItem(got_items, 1);
    char *second_value = cJSON_GetStringValue(second_elem);
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *thr_item = cJSON_GetObjectItem(got_config, "threshold");
    double threshold = cJSON_GetNumberValue(thr_item);
    (void)count;
    (void)second_value;
    (void)threshold;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}