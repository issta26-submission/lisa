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
//<ID> 1415
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *stats = cJSON_AddObjectToObject(root, "stats");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON_AddNumberToObject(stats, "count", 3.0);
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(30.0));

    // step 3: Operate and Validate
    char *json = cJSON_PrintUnformatted(root);
    cJSON *count_item = cJSON_GetObjectItemCaseSensitive(stats, "count");
    double count = cJSON_GetNumberValue(count_item);
    int buf_len = 128;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    buf[0] = json[0];
    buf[1] = (char)('0' + ((int)count % 10));
    cJSON *first_val = cJSON_GetArrayItem(values, 0);
    double first_num = cJSON_GetNumberValue(first_val);
    buf[2] = (char)('0' + ((int)first_num % 10));

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}