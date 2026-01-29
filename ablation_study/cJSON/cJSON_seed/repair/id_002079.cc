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
//<ID> 2079
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *count_item = cJSON_CreateNumber(314.0);
    cJSON_AddItemToObject(root, "count", count_item);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "version", "1.0");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    cJSON *got_count = cJSON_GetObjectItem(root, "count");
    const char *name_str = cJSON_GetStringValue(got_name);
    double count_val = cJSON_GetNumberValue(got_count);
    cJSON *root_small = cJSON_CreateObject();
    cJSON_AddItemToObject(root_small, "name", cJSON_CreateString(name_str));
    cJSON_AddItemToObject(root_small, "count", cJSON_CreateNumber(count_val));
    cJSON *mirror = cJSON_CreateObject();
    cJSON_AddItemToObject(mirror, "name", cJSON_CreateString(name_str));
    cJSON_AddItemToObject(mirror, "count", cJSON_CreateNumber(count_val));
    cJSON_bool cmp_result = cJSON_Compare(root_small, mirror, 1);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    const char *first_name = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = first_name ? first_name[0] : '\0';
    buffer[2] = (char)('0' + ((int)count_val % 10));
    buffer[3] = (char)('0' + (int)cmp_result);
    buffer[4] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root_small);
    cJSON_Delete(mirror);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}