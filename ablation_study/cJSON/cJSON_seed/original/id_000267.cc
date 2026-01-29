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
//<ID> 267
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    /* step 1: Initialize */
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(child, "value", num);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *flag = cJSON_AddFalseToObject(root, "flag");
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    /* step 2: Configure
       Replace creating an object reference with duplicating the child to avoid
       potential double-free or lifecycle issues while preserving the logical
       effect of adding a representation of `child` into the array. */
    cJSON *refdup = cJSON_Duplicate(child, 1);
    cJSON_AddItemToArray(arr, refdup);
    cJSON *n2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, n2);
    int size_before = cJSON_GetArraySize(arr);
    cJSON *n3 = cJSON_CreateNumber(9.99);
    cJSON_AddItemToArray(arr, n3);
    int size_after = cJSON_GetArraySize(arr);

    /* step 3: Operate and Validate */
    cJSON_AddNumberToObject(root, "size_before", (double)size_before);
    cJSON_AddNumberToObject(root, "size_after", (double)size_after);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    if (tmp) {
        memset(tmp, 0, 128);
    }
    /* Use a simple deterministic summary value that does not dereference printed
       (to avoid a possible NULL dereference), keeping the intended numeric
       aggregation behavior. */
    int summary = size_before + size_after + (int)(root ? root->type : 0);
    (void)summary;

    /* step 4: Cleanup */
    if (printed) {
        cJSON_free(printed);
    }
    if (tmp) {
        cJSON_free(tmp);
    }
    cJSON_Delete(root);

    return 66;
}