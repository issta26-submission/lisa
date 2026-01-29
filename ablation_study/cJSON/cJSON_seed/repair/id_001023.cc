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
//<ID> 1023
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_AddObjectToObject(root, "data");
    cJSON *values = cJSON_AddArrayToObject(data, "values");
    cJSON_AddNumberToObject(root, "pi", 3.1415926535);

    // step 2: Configure
    double arr_vals[3] = { 1.0, 2.0, 3.0 };
    cJSON_AddItemToArray(values, cJSON_CreateNumber(arr_vals[0]));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(arr_vals[1]));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(arr_vals[2]));
    size_t buf_len = 32;
    char *buffer = (char*)cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    buffer[0] = 't'; buffer[1] = 'e'; buffer[2] = 's'; buffer[3] = 't';
    cJSON_AddStringToObject(data, "note", buffer);

    // step 3: Operate and Validate
    cJSON_bool is_arr = cJSON_IsArray(values);
    cJSON_AddNumberToObject(root, "is_array_flag", (double)is_arr);
    cJSON *got_values = cJSON_GetObjectItem(data, "values");
    cJSON *got_pi = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(got_pi);
    cJSON_AddNumberToObject(root, "pi_plus_one", pi_val + 1.0);
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}