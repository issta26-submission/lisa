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
//<ID> 1283
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"existing\": 1, \"nested\": {\"a\": true}}";
    const char *endptr = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &endptr, 1);

    // step 2: Configure
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemReferenceToObject(root, "flag", false_item);
    cJSON *existing_item = cJSON_GetObjectItem(root, "existing");
    cJSON *replacement_number = cJSON_CreateNumber(42.0);

    // step 3: Operate
    cJSON_bool has_flag = cJSON_HasObjectItem(root, "flag");
    (void)has_flag;
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, existing_item, replacement_number);
    (void)replaced;
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}