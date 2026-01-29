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
//<ID> 158
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(7.5);
    cJSON *child_obj = cJSON_CreateObject();
    cJSON *replacement = NULL;
    cJSON *num_item = NULL;
    cJSON *raw_item = NULL;
    double num_value = 0.0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "number", num);
    cJSON_AddItemToObject(root, "child", child_obj);
    raw_item = cJSON_AddRawToObject(root, "raw", "{\"flag\":true,\"n\":1}");

    // step 3: Operate / Validate
    num_item = cJSON_GetObjectItem(root, "number");
    num_value = cJSON_GetNumberValue(num_item);
    replacement = cJSON_CreateNumber(num_value + 2.25);
    replaced = cJSON_ReplaceItemViaPointer(root, num_item, replacement);
    (void)raw_item;
    (void)replaced;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}