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
//<ID> 1239
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(items, num);
    cJSON *strref = cJSON_CreateStringReference("ref-string");
    cJSON_AddItemToArray(items, strref);
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, f);

    // step 2: Configure
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "dup_items", dup_items);
    cJSON *extra = cJSON_CreateObject();
    cJSON_AddItemToObject(dup_items, "extra", extra);
    cJSON_AddNumberToObject(extra, "addon", 7.0);

    // step 3: Operate and Validate
    cJSON *it0 = cJSON_GetArrayItem(items, 0);
    cJSON *it_bool = cJSON_GetArrayItem(items, 2);
    double v0 = cJSON_GetNumberValue(it0);
    cJSON_bool is_bool = cJSON_IsBool(it_bool);
    cJSON_bool is_false = cJSON_IsFalse(it_bool);
    double computed = v0 + (double)(is_bool) + (double)(is_false);
    cJSON *computed_num = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_num);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}