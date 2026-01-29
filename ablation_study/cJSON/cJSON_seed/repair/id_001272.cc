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
//<ID> 1272
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"items\":[true,false],\"meta\":{\"name\":\"orig\"}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *second_item = cJSON_GetArrayItem(items, 1);

    // step 2: Configure
    cJSON *forced_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "forced_false", forced_false);
    cJSON *note = cJSON_CreateString("added_false_flag");
    cJSON_AddItemToObject(root, "note", note);

    // step 3: Operate and Validate
    double was_false = (double)cJSON_IsFalse(second_item);
    cJSON *flag_num = cJSON_CreateNumber(was_false);
    cJSON_AddItemToObject(root, "second_was_false", flag_num);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON_free(out);
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}