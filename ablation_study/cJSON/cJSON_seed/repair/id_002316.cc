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
//<ID> 2316
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_AddArrayToObject(root, "data");
    cJSON *meta = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToArray(array, cJSON_CreateString("first"));
    cJSON_AddItemToArray(array, cJSON_CreateNumber(42.0));
    cJSON_AddItemToObject(meta, "label", cJSON_CreateString("sequence"));
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    cJSON *got_array = cJSON_GetObjectItem(root, "data");
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *first = cJSON_GetArrayItem(got_array, 0);
    const char *first_str = cJSON_GetStringValue(first);
    cJSON *second = cJSON_GetArrayItem(got_array, 1);
    double second_num = cJSON_GetNumberValue(second);
    char *json = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (first_str && first_str[0]) ? first_str[0] : '\0';
    buffer[2] = (second_num != 0.0) ? '1' : '0';
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}