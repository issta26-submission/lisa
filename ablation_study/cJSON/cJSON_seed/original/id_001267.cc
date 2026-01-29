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
//<ID> 1267
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(data, n1);
    cJSON_AddItemToArray(data, n2);
    cJSON_AddItemToArray(data, n3);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(99.5);
    cJSON_ReplaceItemInArray(data, 1, replacement);

    // step 3: Operate
    cJSON *got_data = cJSON_GetObjectItem(root, "data");
    cJSON *first_item = cJSON_GetArrayItem(got_data, 0);
    cJSON_bool first_is_number = cJSON_IsNumber(first_item);
    (void)first_is_number;
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = (int)strlen(printed) + 1;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    memcpy(buf, printed, (size_t)buf_len);
    cJSON_Minify(buf);

    // step 4: Validate & Cleanup
    cJSON *second_item = cJSON_GetArrayItem(got_data, 1);
    cJSON_bool second_is_number = cJSON_IsNumber(second_item);
    double second_val = cJSON_GetNumberValue(second_item);
    (void)second_is_number;
    (void)second_val;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}