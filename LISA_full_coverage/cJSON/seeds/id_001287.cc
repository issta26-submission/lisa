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
//<ID> 1287
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"alpha\":true, \"beta\":42, \"nested\":{\"key\":\"value\"}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *shared_num = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(root, "shared_num", shared_num);
    cJSON *newObj = cJSON_CreateObject();
    cJSON *child = cJSON_CreateString("to_replace");
    cJSON_AddItemToObject(newObj, "target", child);
    cJSON_AddItemReferenceToObject(newObj, "ref_num", shared_num);

    // step 3: Operate
    cJSON_bool has_nested = cJSON_HasObjectItem(root, "nested");
    (void)has_nested;
    cJSON *replacement = cJSON_CreateFalse();
    cJSON_ReplaceItemViaPointer(newObj, child, replacement);
    char *printed = cJSON_PrintUnformatted(newObj);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(newObj);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}