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
//<ID> 1412
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
    cJSON_AddNumberToObject(data, "version", 2.5);

    // step 2: Configure
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(30.5));
    cJSON_AddItemToObject(data, "values", values);

    // step 3: Operate and Validate
    char *rendered = cJSON_PrintUnformatted(root);
    cJSON *first_item = cJSON_GetArrayItem(values, 0);
    cJSON *second_item = cJSON_GetArrayItem(values, 1);
    double v0 = cJSON_GetNumberValue(first_item);
    double v1 = cJSON_GetNumberValue(second_item);
    double sum = v0 + v1;
    int buf_len = 128;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    buf[0] = rendered[0];
    buf[1] = (char)('0' + ((int)sum % 10));
    buf[2] = (char)('0' + (((int)sum / 10) % 10));

    // step 4: Cleanup
    cJSON_free(rendered);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}