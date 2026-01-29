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
//<ID> 932
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"meta\":\"example\",\"items\":[{\"label\":\"a\"}],\"numbers\":[1.0,2.0]}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    float extra_vals[] = {4.5f, 5.5f, 6.5f};
    cJSON *float_arr = cJSON_CreateFloatArray(extra_vals, 3);
    cJSON *tag = cJSON_CreateString("added_tag");
    cJSON_bool add_ok1 = cJSON_AddItemToObjectCS(root, "extra_numbers", float_arr);
    cJSON_bool add_ok2 = cJSON_AddItemToObjectCS(root, "tag", tag);

    // step 3: Operate
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *items_ref = cJSON_CreateArrayReference(items);
    cJSON_bool add_ok3 = cJSON_AddItemToObjectCS(root, "items_ref", items_ref);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    void *buf = cJSON_malloc(256);
    memset(buf, 0, 256);
    char *prebuf = (char *)buf;
    cJSON_PrintPreallocated(root, prebuf, 256, 1);
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}