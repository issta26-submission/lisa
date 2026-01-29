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
//<ID> 1006
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"data\":[10,20,null,40],\"meta\":{\"flag\":false}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(initial_json, &parse_end, 1);

    // step 2: Configure
    cJSON *data = cJSON_GetObjectItem(root, "data");
    cJSON *elem1 = cJSON_GetArrayItem(data, 1);
    cJSON_bool elem1_is_num = cJSON_IsNumber(elem1);
    double elem1_val = cJSON_GetNumberValue(elem1);
    cJSON *elem2 = cJSON_GetArrayItem(data, 2);
    cJSON_bool elem2_invalid = cJSON_IsInvalid(elem2);
    cJSON_AddBoolToObject(root, "second_was_number", elem1_is_num);

    // step 3: Operate
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddNumberToObject(summary, "second_value", elem1_val);
    cJSON_AddTrueToObject(summary, "checked");
    cJSON_AddItemToObject(root, "summary", summary);
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(root, print_buf, 512, 0);
    (void)elem2_invalid;

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "meta");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}