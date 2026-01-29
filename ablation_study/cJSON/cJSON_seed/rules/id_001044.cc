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
//<ID> 1044
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *meta = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *got_item = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "device", "sensorX");
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "readings", arr);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_item);
    str_item = cJSON_CreateString("ok");
    cJSON_AddItemToArray(arr, str_item);
    meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "version", "1.2.3");
    cJSON_AddStringToObject(root, "status", "active");

    // step 3: Operate / Validate
    got_item = cJSON_GetArrayItem(arr, 1);
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}