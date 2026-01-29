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
//<ID> 1451
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"data\":{\"values\":[1,2,3]},\"status\":\"ok\"}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *data = cJSON_GetObjectItem(root, "data");
    cJSON *values = cJSON_GetObjectItem(data, "values");

    // step 2: Configure
    cJSON *new_number = cJSON_CreateNumber(99.5);
    cJSON_AddItemToArray(values, new_number);
    cJSON *values_ref = cJSON_CreateArrayReference(values);
    cJSON_AddItemToObject(root, "values_ref", values_ref);

    // step 3: Operate & Validate
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    cJSON_bool has_ref = cJSON_HasObjectItem(root, "values_ref");
    cJSON *first_item = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    double added_val = cJSON_GetNumberValue(new_number);
    (void)printed_ok;
    (void)has_ref;
    (void)first_val;
    (void)added_val;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}