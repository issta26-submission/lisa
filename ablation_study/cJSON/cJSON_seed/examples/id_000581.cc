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
//<ID> 581
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddNumberToObject(child, "count", 123.0);
    cJSON_AddStringToObject(child, "label", "example_label");
    cJSON_AddItemToObjectCS(root, "child", child);

    cJSON *names = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateString("alice");
    cJSON *n2 = cJSON_CreateString("bob");
    cJSON *n3 = cJSON_CreateString("carol");
    cJSON_AddItemToArray(names, n1);
    cJSON_AddItemToArray(names, n2);
    cJSON_AddItemToArray(names, n3);
    cJSON_AddItemToObject(root, "names", names);

    // step 3: Operate & Validate
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON *dup_names = cJSON_GetObjectItem(dup_root, "names");
    int dup_names_count = cJSON_GetArraySize(dup_names);
    cJSON *dup_child = cJSON_GetObjectItem(dup_root, "child");
    cJSON *dup_count_item = cJSON_GetObjectItem(dup_child, "count");
    double dup_count = cJSON_GetNumberValue(dup_count_item);
    cJSON *first_name_item = cJSON_GetArrayItem(dup_names, 0);
    const char *first_name = cJSON_GetStringValue(first_name_item);
    char *snapshot = cJSON_PrintUnformatted(dup_root);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;names=%d;count=%.0f;first=%s",
            version ? version : "null",
            dup_names_count,
            dup_count,
            first_name ? first_name : "null");
    cJSON_free(snapshot);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}