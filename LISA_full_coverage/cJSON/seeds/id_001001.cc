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
//<ID> 1001
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"data\": [10, 20, false], \"meta\": {\"ok\": false}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(initial_json, &parse_end, 1);

    // step 2: Configure
    cJSON *data = cJSON_GetObjectItem(root, "data");
    cJSON *second_elem = cJSON_GetArrayItem(data, 1);
    cJSON_bool second_is_number = cJSON_IsNumber(second_elem);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *added_flag = cJSON_AddTrueToObject(meta, "processed");

    // step 3: Operate
    cJSON *temp_false = cJSON_CreateFalse();
    cJSON_bool temp_invalid = cJSON_IsInvalid(temp_false);
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(256);
    memset(print_buf, 0, 256);
    cJSON_PrintPreallocated(root, print_buf, 256, 0);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "data");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(temp_false);
    cJSON_Delete(root);
    (void)second_is_number;
    (void)temp_invalid;
    (void)added_flag;

    return 66;
    // API sequence test completed successfully
}