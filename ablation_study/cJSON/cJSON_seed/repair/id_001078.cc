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
//<ID> 1078
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON_AddStringToObject(root, "title", "cJSON API Sequence Test");

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, s1);
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, n1);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "subname", "child_object");
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemReferenceToObject(root, "child_ref", child);

    // step 3: Operate and Validate
    cJSON *parsed = cJSON_Parse("{\"parsed\": [true, false], \"value\": 3.14}");
    char *printed_parsed = cJSON_Print(parsed);
    cJSON_free(printed_parsed);
    char *printed_root = cJSON_Print(root);
    cJSON_free(printed_root);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}