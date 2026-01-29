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
//<ID> 2318
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_AddArrayToObject(root, "values");
    cJSON_AddItemToObject(root, "flag", cJSON_CreateTrue());
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "type", "example");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddItemToArray(values, cJSON_CreateNumber(3.14));
    cJSON_AddItemToArray(values, cJSON_CreateString("data"));
    cJSON_AddItemToArray(values, cJSON_CreateFalse());

    // step 3: Operate
    cJSON *got_values = cJSON_GetObjectItem(root, "values");
    cJSON *second_item = cJSON_GetArrayItem(got_values, 1);
    const char *second_str = cJSON_GetStringValue(second_item);
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *type_item = cJSON_GetObjectItem(got_meta, "type");
    const char *type_str = cJSON_GetStringValue(type_item);

    // step 4: Validate and Cleanup
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (second_str && second_str[0]) ? second_str[0] : '\0';
    buffer[2] = (type_str && type_str[0]) ? type_str[0] : '\0';
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}