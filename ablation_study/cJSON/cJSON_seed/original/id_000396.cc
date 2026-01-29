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
//<ID> 396
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num_x = cJSON_CreateNumber(10.5);
    cJSON *num_y = cJSON_CreateNumber(5.25);
    cJSON_AddItemToObject(child, "x", num_x);
    cJSON_AddItemToObject(child, "y", num_y);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);

    // step 2: Configure
    cJSON *x_item = cJSON_GetObjectItemCaseSensitive(child, "x");
    cJSON *y_item = cJSON_GetObjectItemCaseSensitive(child, "y");
    double x_val = cJSON_GetNumberValue(x_item);
    double y_val = cJSON_GetNumberValue(y_item);
    cJSON *sum_item = cJSON_CreateNumber(x_val + y_val);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 3: Operate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(child, "x");

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}