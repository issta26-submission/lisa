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
//<ID> 1120
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num_answer = cJSON_CreateNumber(42.0);
    cJSON *label_ref = cJSON_CreateStringReference("initial");
    cJSON *version_ref = cJSON_CreateStringReference(cJSON_Version());
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "answer", num_answer);
    cJSON_AddItemToObject(root, "label", label_ref);
    cJSON_AddItemToObject(root, "version", version_ref);
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON *arr_ref = cJSON_CreateArrayReference(arr_dup);
    cJSON_AddItemToObject(root, "data_ref", arr_ref);
    cJSON *new_label_ref = cJSON_CreateStringReference("updated");
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "label", new_label_ref);
    cJSON_AddNumberToObject(root, "replace_ok", (double)replaced);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
}