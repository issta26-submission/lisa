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
//<ID> 228
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *meta = cJSON_CreateObject();

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, s1);
    char *dyn = (char*)cJSON_malloc((size_t)8);
    dyn[0] = 'd';
    dyn[1] = 'y';
    dyn[2] = 'n';
    dyn[3] = '_';
    dyn[4] = 'v';
    dyn[5] = 'a';
    dyn[6] = 'l';
    dyn[7] = '\0';
    cJSON *sref = cJSON_CreateStringReference((const char*)dyn);
    cJSON_AddItemToArray(arr, sref);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "count", cJSON_CreateNumber(2.0));

    // step 3: Operate & Validate
    cJSON_bool has_list = cJSON_HasObjectItem(root, "list");
    cJSON_AddItemToObject(root, "has_list", cJSON_CreateNumber((double)has_list));
    cJSON_AddItemToObject(root, "list_size", cJSON_CreateNumber((double)cJSON_GetArraySize(arr)));
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(dyn);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}