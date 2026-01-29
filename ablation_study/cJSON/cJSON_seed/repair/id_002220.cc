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
//<ID> 2220
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
    cJSON *nested = cJSON_CreateObject();
    cJSON *year = cJSON_CreateNumber(2026.0);
    cJSON *name = cJSON_CreateString("fuzzer");
    cJSON *remove_me = cJSON_CreateString("to_be_removed");
    cJSON *case_sensitive = cJSON_CreateString("CaseValue");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "year", year);
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(root, "remove_me", remove_me);
    cJSON_AddItemToObject(root, "CaseSensitive", case_sensitive);
    cJSON_AddItemToObject(root, "nested", nested);

    // step 2: Configure
    cJSON_bool meta_is_object = cJSON_IsObject(meta);
    cJSON_AddBoolToObject(root, "meta_is_object", meta_is_object);
    cJSON *nested_val = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(nested, "pi", nested_val);

    // step 3: Operate and Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_year = cJSON_GetObjectItem(got_meta, "year");
    double year_val = cJSON_GetNumberValue(got_year);
    (void)year_val;
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_DeleteItemFromObject(root, "remove_me");
    cJSON_DeleteItemFromObjectCaseSensitive(root, "CaseSensitive");

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}