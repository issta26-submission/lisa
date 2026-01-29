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
//<ID> 1003
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"root\":{\"data\":[3.14,10],\"meta\":{\"valid\":true}},\"value\":7}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *root_obj = cJSON_GetObjectItem(root, "root");
    cJSON *data_arr = cJSON_GetObjectItem(root_obj, "data");
    cJSON *first_item = cJSON_GetArrayItem(data_arr, 0);
    cJSON_bool first_is_number = cJSON_IsNumber(first_item);
    cJSON *added_true = cJSON_AddTrueToObject(root_obj, "injected_true");
    cJSON *second_item = cJSON_GetArrayItem(data_arr, 1);
    cJSON_bool second_is_invalid = cJSON_IsInvalid(second_item);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(root, print_buf, 512, 0);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root_obj, "data");
    cJSON_DeleteItemFromObject(root_obj, "injected_true");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)first_is_number;
    (void)second_is_invalid;
    (void)added_true;
    return 66;
}