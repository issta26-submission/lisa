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
//<ID> 632
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON_AddNumberToObject(child, "value", 42.0);
    cJSON_AddStringToObject(child, "label", "example");
    cJSON *raw_added = cJSON_AddRawToObject(root, "raw", "{\"flag\":false, \"count\":3}");

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(child, 1);
    cJSON_bool are_equal = cJSON_Compare(child, dup, 1);
    cJSON_bool child_is_object = cJSON_IsObject(child);
    const char *label = cJSON_GetStringValue(cJSON_GetObjectItem(child, "label"));
    double value = cJSON_GetNumberValue(cJSON_GetObjectItem(child, "value"));

    // step 3: Operate
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    char *dup_unformatted = cJSON_PrintUnformatted(dup);
    double scaled = value * 2.0;
    cJSON_AddNumberToObject(root, "scaled", scaled);

    // step 4: Validate & Cleanup
    (void)label;
    (void)raw_added;
    (void)are_equal;
    (void)child_is_object;
    cJSON_Delete(dup);
    cJSON_free(dup_unformatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}