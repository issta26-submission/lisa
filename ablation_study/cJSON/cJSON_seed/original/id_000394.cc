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
//<ID> 394
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
    cJSON *child_value = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "answer", child_value);
    cJSON *child_copy = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_copy", child_copy);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    cJSON *pi = cJSON_CreateNumber(3.141592653589793);
    cJSON_AddItemToObject(root, "pi", pi);

    // step 2: Configure
    cJSON *pi_lookup = cJSON_GetObjectItemCaseSensitive(root, "pi");
    double pi_value = cJSON_GetNumberValue(pi_lookup);
    cJSON *pi_times_two = cJSON_CreateNumber(pi_value * 2.0);
    cJSON_AddItemToObject(root, "pi_times_two", pi_times_two);
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 3: Operate
    cJSON_DeleteItemFromObjectCaseSensitive(root, "pi");

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(child);
    // API sequence test completed successfully
    return 66;
}