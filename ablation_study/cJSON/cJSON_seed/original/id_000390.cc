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
//<ID> 390
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
    cJSON *n1 = cJSON_CreateNumber(7.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(child, "n1", n1);
    cJSON_AddItemToObject(child, "n2", n2);
    cJSON_AddItemToObject(root, "note", cJSON_CreateString("reference test"));

    // step 2: Configure
    cJSON *lookup_n1 = cJSON_GetObjectItem(child, "n1");
    cJSON *lookup_n2 = cJSON_GetObjectItem(child, "n2");
    double v1 = cJSON_GetNumberValue(lookup_n1);
    double v2 = cJSON_GetNumberValue(lookup_n2);
    cJSON *sum = cJSON_CreateNumber(v1 + v2);
    cJSON_AddItemToObject(child, "sum", sum);

    // step 3: Operate
    cJSON *ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", ref);
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate and Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "child_ref");
    cJSON_free(printed);
    cJSON_Delete(child);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}