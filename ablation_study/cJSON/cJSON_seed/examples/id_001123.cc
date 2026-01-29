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
//<ID> 1123
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *child_arr = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(3.14);
    cJSON *num1 = cJSON_CreateNumber(2.71);
    cJSON *label = cJSON_CreateString("original_label");
    const char *version_str = cJSON_Version();
    cJSON *version_ref = cJSON_CreateString(version_str);
    cJSON_AddItemToArray(child_arr, num0);
    cJSON_AddItemToArray(child_arr, num1);
    cJSON_AddItemToObject(root, "data", child_arr);
    cJSON_AddItemReferenceToObject(root, "alias", child_arr);
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddItemToObject(root, "version", version_ref);
    cJSON *replacement_label = cJSON_CreateString("replaced_label");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "label", replacement_label);
    int count = cJSON_GetArraySize(child_arr);
    cJSON *it0 = cJSON_GetArrayItem(child_arr, 0);
    cJSON *it1 = cJSON_GetArrayItem(child_arr, 1);
    double sum = cJSON_GetNumberValue(it0) + cJSON_GetNumberValue(it1) + (double)count;
    cJSON_AddNumberToObject(root, "sum", sum);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
}