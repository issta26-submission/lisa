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
//<ID> 2319
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_AddArrayToObject(root, "data");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddItemToArray(data, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(data, cJSON_CreateNumber(3.14));
    cJSON_AddItemToArray(data, cJSON_CreateBool(1));
    cJSON_AddStringToObject(meta, "type", "dataset");
    cJSON_AddStringToObject(root, "owner", "tester");

    // step 3: Operate
    cJSON *meta_fetched = cJSON_GetObjectItem(root, "meta");
    cJSON *type_item = cJSON_GetObjectItem(meta_fetched, "type");
    const char *type_str = cJSON_GetStringValue(type_item);
    cJSON *data_fetched = cJSON_GetObjectItem(root, "data");
    cJSON *data_copy = cJSON_Duplicate(data_fetched, 1);
    cJSON_AddItemToObject(root, "data_copy", data_copy);

    // step 4: Validate and Cleanup
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (type_str && type_str[0]) ? type_str[0] : '\0';
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}