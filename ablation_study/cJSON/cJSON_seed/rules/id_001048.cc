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
//<ID> 1048
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
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *got_item = NULL;
    char *printed = NULL;
    char *picked_val = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "id", "sensor-007");
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    item0 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, item0);
    item1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, item1);

    // step 3: Operate / Validate
    got_item = cJSON_GetArrayItem(arr, 1);
    picked_val = cJSON_GetStringValue(got_item);
    cJSON_AddStringToObject(root, "picked", picked_val);
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}