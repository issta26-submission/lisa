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
//<ID> 1013
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *status_item = NULL;
    cJSON *flag_item = NULL;
    cJSON *dbl_array = NULL;
    cJSON *raw_item = NULL;
    cJSON *replacement = NULL;
    char *printed = NULL;
    double values[3] = {1.1, 2.2, 3.3};
    double second_value = 0.0;
    cJSON_bool replaced = 0;
    cJSON_bool flag_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    status_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "status", status_item);
    flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", flag_item);
    dbl_array = cJSON_CreateDoubleArray(values, 3);
    cJSON_AddItemToObject(root, "readings", dbl_array);
    raw_item = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw_item);

    // step 3: Operate / Validate
    replacement = cJSON_CreateString("updated"); 
    replaced = cJSON_ReplaceItemViaPointer(root, status_item, replacement);
    flag_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "active"));
    second_value = cJSON_GetNumberValue(cJSON_GetArrayItem(dbl_array, 1));
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}