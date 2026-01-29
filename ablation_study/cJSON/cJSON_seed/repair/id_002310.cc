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
//<ID> 2310
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *values = cJSON_AddArrayToObject(root, "values");

    // step 2: Configure
    cJSON_AddItemToArray(values, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(7.0));
    cJSON_AddItemToArray(values, cJSON_CreateFalse());
    cJSON_AddStringToObject(meta, "title", "dataset");
    cJSON_AddNumberToObject(meta, "count", 3.0);

    // step 3: Operate
    cJSON *values_lookup = cJSON_GetObjectItem(root, "values");
    cJSON *meta_lookup = cJSON_GetObjectItem(root, "meta");
    cJSON *second_item = cJSON_GetArrayItem(values_lookup, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    char *first_str = cJSON_GetStringValue(cJSON_GetArrayItem(values_lookup, 0));
    char *json = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (first_str && first_str[0]) ? first_str[0] : (second_val != 0.0 ? '1' : '0');
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}