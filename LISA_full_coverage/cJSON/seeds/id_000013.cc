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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"flag\":true,\"count\":3}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *arr = cJSON_CreateArray();
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    cJSON *count_dup = cJSON_Duplicate(count_item, 0);
    cJSON_AddItemToArray(arr, count_dup);
    cJSON_AddItemToObject(dup, "counts", arr);
    cJSON_AddTrueToObject(dup, "addedTrue");
    cJSON_AddFalseToObject(dup, "addedFalse");

    // step 3: Operate and Validate
    char *sroot = cJSON_PrintUnformatted(root);
    char *sdup = cJSON_PrintUnformatted(dup);
    cJSON_Minify(sdup);
    cJSON_bool same = cJSON_Compare(root, dup, 0);
    double count_val = cJSON_GetNumberValue(count_item);
    (void)same;
    (void)count_val;

    // step 4: Cleanup
    cJSON_free(sroot);
    cJSON_free(sdup);
    cJSON_Delete(root);
    cJSON_Delete(dup);

    // API sequence test completed successfully
    return 66;
}