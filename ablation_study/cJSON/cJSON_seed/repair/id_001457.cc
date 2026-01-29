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
//<ID> 1457
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *parent = cJSON_AddObjectToObject(root, "parent");
    cJSON *items = cJSON_AddArrayToObject(parent, "items");
    cJSON *s = cJSON_CreateString("example-string");
    cJSON_AddItemToArray(items, s);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(7.0));

    // step 2: Configure
    cJSON_AddStringToObject(root, "note", "api-sequence-test");
    cJSON_AddNumberToObject(root, "version", 2.0);

    // step 3: Operate and Validate
    cJSON_bool has_parent = cJSON_HasObjectItem(root, "parent");
    cJSON *got_parent = cJSON_GetObjectItemCaseSensitive(root, "parent");
    cJSON *got_items = cJSON_GetObjectItemCaseSensitive(got_parent, "items");
    cJSON_bool items_are_array = cJSON_IsArray(got_items);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = out_len > 0 ? out[0] : ' ';
    scratch[1] = (char)('0' + (int)has_parent);
    scratch[2] = (char)('0' + (int)items_are_array);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}