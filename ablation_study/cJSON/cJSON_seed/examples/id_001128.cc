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
//<ID> 1128
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.71);
    cJSON *ver_ref = cJSON_CreateStringReference(cJSON_Version());
    cJSON *meta = cJSON_CreateObject();
    cJSON *label_ref = cJSON_CreateStringReference("alpha");

    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemReferenceToObject(root, "values_ref", arr);
    cJSON_AddItemToObject(root, "version", ver_ref);
    cJSON_AddItemToObject(meta, "label", label_ref);
    cJSON_AddItemToObject(root, "meta", meta);

    cJSON *new_meta = cJSON_CreateObject();
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "meta", new_meta);
    cJSON_AddNumberToObject(root, "replaced", (double)replaced);
    cJSON *it0 = cJSON_GetArrayItem(arr, 0);
    cJSON *it1 = cJSON_GetArrayItem(arr, 1);
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double sum = v0 + v1;
    cJSON_AddNumberToObject(root, "sum", sum);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    cJSON_Delete(root);
    return 66;
}