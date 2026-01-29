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
//<ID> 265
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"a\":1.5,\"b\":2}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(root, "doc", parsed);
    cJSON *doc = cJSON_GetObjectItem(root, "doc");
    cJSON *a_item = cJSON_GetObjectItem(doc, "a");

    // step 3: Operate
    double a_val = cJSON_GetNumberValue(a_item);
    cJSON *replacement_b = cJSON_CreateNumber(a_val * 2.0);
    cJSON_ReplaceItemInObjectCaseSensitive(doc, "b", replacement_b);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    /* API sequence test completed successfully */
    return 66;
}