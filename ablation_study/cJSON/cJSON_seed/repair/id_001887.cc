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
//<ID> 1887
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "flags", flags);
    cJSON *on_item = cJSON_AddTrueToObject(flags, "on");
    cJSON *off_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(flags, "off", off_item);
    double values_arr_vals[3] = {1.5, 2.5, 3.5};
    cJSON *values = cJSON_CreateDoubleArray(values_arr_vals, 3);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *got_on = cJSON_GetObjectItem(flags, "on");
    cJSON *got_off = cJSON_GetObjectItem(flags, "off");
    cJSON_bool got_on_is_bool = cJSON_IsBool(got_on);
    cJSON_bool got_off_is_bool = cJSON_IsBool(got_off);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (got_on_is_bool % 10));
    buffer[2] = (char)('0' + (got_off_is_bool % 10));
    buffer[3] = (char)('0' + (cJSON_GetArraySize(values) % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}