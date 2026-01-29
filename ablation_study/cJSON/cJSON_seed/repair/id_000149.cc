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
//<ID> 149
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *entries = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "entries", entries);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *active_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "active", active_true);
    cJSON_AddStringToObject(meta, "version", cJSON_Version());

    // step 2: Configure
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(entries, s0);
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(entries, n0);
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(entries, s1);
    cJSON *n1 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(entries, n1);
    cJSON_AddStringToObject(root, "note", "constructed");

    // step 3: Operate and Validate
    cJSON *it0 = cJSON_GetArrayItem(entries, 0);
    cJSON *it1 = cJSON_GetArrayItem(entries, 1);
    cJSON *it2 = cJSON_GetArrayItem(entries, 2);
    cJSON *it3 = cJSON_GetArrayItem(entries, 3);
    double v1 = cJSON_GetNumberValue(it1);
    double v3 = cJSON_GetNumberValue(it3);
    double sum = v1 + v3;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, copy, 1);
    cJSON_AddBoolToObject(root, "equal_copy", equal);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(copy);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}