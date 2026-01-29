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
//<ID> 1332
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"flag\":false,\"items\":[10,20],\"info\":\"demo\"}";
    const char *parse_end = 0;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *extras = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "extras", extras);
    cJSON *items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    cJSON *copied = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "copied", copied);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *ref_first = cJSON_CreateObjectReference(first_item);
    cJSON_AddItemReferenceToArray(copied, ref_first);

    // step 3: Operate
    cJSON_bool flag_is_false = cJSON_IsFalse(cJSON_GetObjectItemCaseSensitive(parsed, "flag"));
    int buf_len = 1024;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = 0;
    printed_len = strlen(printed);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    char *scratch = (char *)cJSON_malloc(16);
    memset(scratch, 0, 16);
    cJSON_free(scratch);
    cJSON_Delete(root);
    (void)flag_is_false;
    // API sequence test completed successfully
    return 66;
}