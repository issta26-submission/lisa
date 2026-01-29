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
//<ID> 978
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_buf[] = " { \"meta\": { \"id\": 42 }, \"items\": [ null, false ] } ";
    cJSON_Minify(json_buf);
    cJSON *root = cJSON_Parse(json_buf);

    // step 2: Configure
    cJSON *added_true = cJSON_AddTrueToObject(root, "enabled");
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "explicit_null", null_item);
    cJSON *child_obj = cJSON_AddObjectToObject(root, "child");
    cJSON_AddTrueToObject(child_obj, "flag");

    // step 3: Operate
    char *out_unformatted = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON_PrintPreallocated(root, buffer, 512, 1);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);

    // step 4: Validate & Cleanup
    (void)added_true;
    (void)equal;
    cJSON_free(out_unformatted);
    cJSON_free(buffer);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}