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
//<ID> 116
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *n1 = cJSON_CreateNumber(5.0);
    cJSON_AddItemToArray(items, n1);
    cJSON *n2 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(items, n2);

    // step 2: Configure
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", dup_items);
    cJSON *i0 = cJSON_GetArrayItem(items, 0);
    cJSON *i1 = cJSON_GetArrayItem(items, 1);
    double sum_orig = cJSON_GetNumberValue(i0) + cJSON_GetNumberValue(i1);
    cJSON_AddNumberToObject(root, "sum_original", sum_orig);

    // step 3: Operate and Validate
    cJSON *got_dup = cJSON_GetObjectItem(root, "items_copy");
    cJSON *d0 = cJSON_GetArrayItem(got_dup, 0);
    cJSON *d1 = cJSON_GetArrayItem(got_dup, 1);
    double sum_dup = cJSON_GetNumberValue(d0) + cJSON_GetNumberValue(d1);
    cJSON_AddNumberToObject(root, "sum_duplicate", sum_dup);
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_name = cJSON_GetObjectItem(got_meta, "name");
    const char *s = cJSON_GetStringValue(got_name);
    double first_char_code = (double)(unsigned char)s[0];
    cJSON_AddNumberToObject(root, "first_char_code", first_char_code);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}