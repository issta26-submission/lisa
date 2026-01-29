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
//<ID> 391
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *value = cJSON_CreateNumber(101.5);
    cJSON_AddItemToObject(root, "value", value);
    cJSON *child = cJSON_CreateObject();
    cJSON *sub = cJSON_CreateNumber(7.25);
    cJSON_AddItemToObject(child, "sub", sub);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi);
    cJSON *lookup_value = cJSON_GetObjectItem(root, "value");
    double v = cJSON_GetNumberValue(lookup_value);
    cJSON *lookup_child = cJSON_GetObjectItem(root, "child");
    cJSON *lookup_subnum = cJSON_GetObjectItem(lookup_child, "sub");
    double s = cJSON_GetNumberValue(lookup_subnum);

    // step 3: Operate
    cJSON *sum = cJSON_CreateNumber(v + s);
    cJSON_AddItemToObject(root, "sum", sum);
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate and Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "value");
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}